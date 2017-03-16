/*
  ==============================================================================

    VideoTable.h
    Created: 16 Mar 2017 12:17:25am
    Author:  Oli

  ==============================================================================
*/

#ifndef VIDEOTABLE_H_INCLUDED
#define VIDEOTABLE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
  
enum {
    INDEX_COLUMN = 1,
    NOTE_COLUMN = 2,
    NAME_COLUMN = 3
    //INT_MIDI_FORMAT = 3
};

class VideoTable : public TableListBox,
                public DragAndDropTarget,
                public FileDragAndDropTarget
{
public:
    VideoTable(const String &componentName=String(), TableListBoxModel *model=nullptr) : TableListBox(componentName, model) {}
    //==============================================================================
    // These methods implement the DragAndDropTarget interface, and allow our component
    // to accept drag-and-drop of objects from other Juce components..

    bool isInterestedInDragSource (const SourceDetails& /*dragSourceDetails*/) override
    {
        // normally you'd check the sourceDescription value to see if it's the
        // sort of object that you're interested in before returning true, but for
        // the demo, we'll say yes to anything..
        return true;
    }

    void itemDragEnter (const SourceDetails& /*dragSourceDetails*/) override
    {
        somethingIsBeingDraggedOver = true;
        repaint();
    }

    void itemDragMove (const SourceDetails& /*dragSourceDetails*/) override
    {
    }

    void itemDragExit (const SourceDetails& /*dragSourceDetails*/) override
    {
        somethingIsBeingDraggedOver = false;
        repaint();
    }

    void itemDropped (const SourceDetails& dragSourceDetails) override
    {
        int row = getInsertionIndexForPosition(dragSourceDetails.localPosition.getX(), dragSourceDetails.localPosition.getY());
        DBG("Item " + dragSourceDetails.description.toString() + " dropped at " + String(row));

        somethingIsBeingDraggedOver = false;
        repaint();
    }

    //==============================================================================
    // These methods implement the FileDragAndDropTarget interface, and allow our component
    // to accept drag-and-drop of files..

    bool isInterestedInFileDrag (const StringArray& /*files*/) override
    {
        // normally you'd check these files to see if they're something that you're
        // interested in before returning true, but for the demo, we'll say yes to anything..
        return true;
    }

    void fileDragEnter (const StringArray& /*files*/, int /*x*/, int /*y*/) override
    {
        somethingIsBeingDraggedOver = true;
        repaint();
    }

    void fileDragMove (const StringArray& /*files*/, int /*x*/, int /*y*/) override
    {
    }

    void fileDragExit (const StringArray& /*files*/) override
    {
        somethingIsBeingDraggedOver = false;
        repaint();
    }

    void filesDropped (const StringArray& files, int x, int y) override
    {
        int row = getInsertionIndexForPosition(x,y);
        DBG("Files dropped: " + files.joinIntoString("\n") + "\nat row " + String(row));

        somethingIsBeingDraggedOver = false;
        repaint();
    }
private:
    bool somethingIsBeingDraggedOver;
};
    
class VideoTableContents : public TableListBoxModel
{
public:
    VideoTableContents(VispiControllerVstAudioProcessor& p);
    int getNumRows() override;
        
    void paintRowBackground (Graphics&,
                             int rowNumber,
                             int width, int height,
                             bool rowIsSelected) override;
    void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
        
    void cellClicked(int rowNumber, int columnId, const MouseEvent& e) override;
    
    var getDragSourceDescription (const SparseSet<int>& selectedRows) override;

private:
    VispiControllerVstAudioProcessor& processor;
};
    
class VideoTableHeader : public TableHeaderComponent
{
public:
    VideoTableHeader();
    bool isNoteStyle() { return noteStyleDisplay; }
private:
    bool noteStyleDisplay;
};



#endif  // VIDEOTABLE_H_INCLUDED
