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
    return processor.getNumFiles();
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


VideoTableHeader::VideoTableHeader()
{
    // gross - think more about where to set these things correctly
    // resized if possible
    addColumn("Index", INDEX_COLUMN, 50, 50, -1, defaultFlags);
    addColumn("Note", NOTE_COLUMN, 50, 50, -1, defaultFlags);
    addColumn("Name", NAME_COLUMN, 120, 50, -1, defaultFlags);
}


