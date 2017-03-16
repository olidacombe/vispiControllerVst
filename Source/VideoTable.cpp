/*
  ==============================================================================

    VideoTable.cpp
    Created: 16 Mar 2017 12:17:25am
    Author:  Oli

  ==============================================================================
*/

#include "VideoTable.h"




VideoTableContents::VideoTableContents(VispiControllerVstAudioProcessor& p) : processor(p)
{}

int VideoTableContents::getNumRows() {
    int total = processor.getNumFiles();
    // how do we get at VideoTable bits from here?
    return total;
}

void VideoTableContents::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    if(rowNumber >= getNumRows()) return;
    g.setColour(Colours::black);

    g.setFont(height * 0.7f);
    // ScopedLock...?
    String content;
    switch(columnId) {
        case INDEX_COLUMN:
            content = String(rowNumber);
            break;
        case NOTE_COLUMN:
            content = MidiMessage::getMidiNoteName(rowNumber, true, true, 3);
            break;
        case NAME_COLUMN:
            content = processor.getFileName(rowNumber);
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
    DBG("Files dropped: " + hoveringFiles.joinIntoString("\n") + "\nat row " + String(dragHoverIndex));
    
    // handle the drop
    
    
    // clean up
    dragHoverIndex = -1;
    clearHoveringFiles();
}

void VideoTableContents::itemDropped()
{
    DBG("Item " + String(dragSourceIndex) + " dropped at " + String(dragHoverIndex));
    
    // handle the drop
    
    
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


