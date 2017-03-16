/*
  ==============================================================================

    VideoTable.cpp
    Created: 16 Mar 2017 12:17:25am
    Author:  Oli

  ==============================================================================
*/

#include "VideoTable.h"




VideoTableContents::VideoTableContents(VispiControllerVstAudioProcessor& p) :
    processor(p), dragSourceIndex(-1), dragHoverIndex(-1)
{}

int VideoTableContents::getNumRows() {
    int total = processor.getNumFiles();
    if(dragHoverIndex != -1) {
        //total += jmax(hoveringFiles.size(), 1);
        total += hoveringFiles.size();
    }
    return total;
}

void VideoTableContents::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    if(rowNumber >= getNumRows()) return;
    g.setColour(Colours::black);

    g.setFont(height * 0.7f);
    // ScopedLock...?
    String content;
    
    const int numHoveringFiles = hoveringFiles.size();
    
    switch(columnId) {
        case INDEX_COLUMN:
            content = String(rowNumber);
            break;
        case NOTE_COLUMN:
            content = MidiMessage::getMidiNoteName(rowNumber, true, true, 3);
            break;
        case NAME_COLUMN:
            if(dragSourceIndex != -1) { // we're rearranging items
                
                if(rowNumber < dragHoverIndex)
                    content = processor.getFileName(rowNumber + ((rowNumber<dragSourceIndex)?0:1));
                else if(rowNumber == dragHoverIndex)
                    content = "";
                else
                    content = processor.getFileName(rowNumber - 1 + ((rowNumber<dragSourceIndex)?0:1));
                
            } else if(numHoveringFiles>0) { // we're dragging files in
                
                if(rowNumber < dragHoverIndex)
                    content = processor.getFileName(rowNumber);
                else if(rowNumber < dragHoverIndex + numHoveringFiles)
                    content = "";
                else
                    content = processor.getFileName(rowNumber - numHoveringFiles);
                
            } else {  // we're not up to anything special, just show the list
                content = processor.getFileName(rowNumber);
            }
            break;
        default:
            break;
    }
    g.drawText(content, 5, 0, width, height, Justification::centredLeft, true);
}

void VideoTableContents::paintRowBackground(juce::Graphics & g, int rowNumber, int width, int height, bool rowIsSelected)
{
    //if(rowNumber >= getNumRows()) return;
    if(rowIsSelected) {
        g.fillAll(Colours::cornflowerblue);
    } else {
        g.fillAll(Colours::cornsilk);
    }

}

void VideoTableContents::cellClicked(int rowNumber, int columnId, const MouseEvent& e)
{
    if(rowNumber != -1) {
        processor.processVideoSelection(rowNumber);
    }
}

var VideoTableContents::getDragSourceDescription (const SparseSet<int>& selectedRows)
{
    // for our drag description, we'll just make a comma-separated list of the selected row
    // numbers - this will be picked up by the drag target and displayed in its box.
    /*
    StringArray rows;

    for (int i = 0; i < selectedRows.size(); ++i)
        rows.add (String (selectedRows[i] + 1));

    return rows.joinIntoString (", ");
    */
    // we only allow one selected row
    if(selectedRows.size()>0) return selectedRows[0];
    return var();
}

void VideoTableContents::filesDropped()
{
    DBG("Files dropped:\n" + hoveringFiles.joinIntoString("\n") + "\nat row " + String(dragHoverIndex));
    
    // handle the drop
    processor.insertFiles(hoveringFiles, dragHoverIndex);
    
    // clean up
    dragHoverIndex = -1;
    clearHoveringFiles();
}

void VideoTableContents::itemDropped()
{
    DBG("Item " + String(dragSourceIndex) + " dropped at " + String(dragHoverIndex));
    
    // handle the drop
    processor.swapFiles(dragSourceIndex, dragHoverIndex);
    
    // clean up
    dragHoverIndex = -1;
    dragSourceIndex = -1;
}



VideoTableHeader::VideoTableHeader()
{
    // gross - think more about where to set these things correctly
    // resized if possible
    addColumn("Index", INDEX_COLUMN, 50, 50, -1, defaultFlags);
    addColumn("Note", NOTE_COLUMN, 50, 50, -1, defaultFlags);
    addColumn("Name", NAME_COLUMN, 120, 50, -1, defaultFlags);
}


