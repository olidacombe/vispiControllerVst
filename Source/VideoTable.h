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
    VideoTable(const String &componentName=String(), TableListBoxModel *model=nullptr)
        : TableListBox(componentName, model) {}
    
    
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

    void itemDragEnter (const SourceDetails& dragSourceDetails) override
    {
        dragSourceIndex = dragSourceDetails.description;
        itemIsBeingDraggedOver = true;
        repaint();
    }

    void itemDragMove (const SourceDetails& dragSourceDetails) override
    {
        dragHoverIndex = getInsertionIndexForPosition(
            dragSourceDetails.localPosition.getX(), dragSourceDetails.localPosition.getY()
        );
        DBG("Item " + dragSourceDetails.description.toString() + " hovering at " + String(dragHoverIndex));
        // looky here: http://stackoverflow.com/questions/6224830/c-trying-to-swap-values-in-a-vector
        // want to swap values within processor.fileNames - but not get messed-up as we drag over a second.. element
        // also, that vector is hidden away inside processor.  Might just pass some stuff from here and call 
        // repaint.
    }

    void itemDragExit (const SourceDetails& /*dragSourceDetails*/) override
    {
        itemIsBeingDraggedOver = false;
        repaint();
    }

    void itemDropped (const SourceDetails& dragSourceDetails) override
    {
        DBG("Item " + dragSourceDetails.description.toString() + " dropped at " + String(dragHoverIndex));

        itemIsBeingDraggedOver = false;
        repaint();
    }

    //==============================================================================
    // These methods implement the FileDragAndDropTarget interface, and allow our component
    // to accept drag-and-drop of files..

    bool isInterestedInFileDrag (const StringArray& files) override
    {
        // normally you'd check these files to see if they're something that you're
        // interested in before returning true, but for the demo, we'll say yes to anything..
        return true;
    }

    void fileDragEnter (const StringArray& files, int /*x*/, int /*y*/) override
    {
        // filter out by extension:
        for(String fileName : files) {
            if(
                fileName.matchesWildcard("*.mov", true)
            ||  fileName.matchesWildcard("*.mp4", true)
            ) hoveringFiles.add(fileName);
        }
        fileIsBeingDraggedOver = true;
        repaint();
    }

    void fileDragMove (const StringArray& /*files*/, int x, int y) override
    {
        dragHoverIndex = getInsertionIndexForPosition(x, y);
    }

    void fileDragExit (const StringArray& /*files*/) override
    {
        fileIsBeingDraggedOver = false;
        hoveringFiles.clear();
        repaint();
    }

    void filesDropped (const StringArray& files, int x, int y) override
    {
        DBG("Files dropped: " + files.joinIntoString("\n") + "\nat row " + String(dragHoverIndex));

        fileIsBeingDraggedOver = false;
        repaint();
    }
private:
    bool itemIsBeingDraggedOver, fileIsBeingDraggedOver;
    int dragHoverIndex, dragSourceIndex;
    StringArray hoveringFiles;

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
