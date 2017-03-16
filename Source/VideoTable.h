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
    
    void filesDropped();
    void itemDropped();
    
    void setDragHoverIndex(const int i) { dragHoverIndex = i; }
    void setDragSourceIndex(const int i)
    {
        dragSourceIndex = i;
        displayList = processor.getFileNames();
        displayList.erase(displayList.begin() + i);
    }
    void clearHoveringFiles() { hoveringFiles.clear(); }
    void addHoveringFile(const String& name) { hoveringFiles.add(name); }
    const int getNumHoveringFiles() { return hoveringFiles.size(); }

private:
    VispiControllerVstAudioProcessor& processor;
    int dragHoverIndex, dragSourceIndex;
    StringArray hoveringFiles;
    std::vector<String> displayList;

};



class VideoTable : public TableListBox,
                public DragAndDropTarget,
                public FileDragAndDropTarget
{
public:
    VideoTable(const String &componentName=String(), TableListBoxModel *model=nullptr)
        : TableListBox(componentName, model) { model = getModel(); }
    
    
    VideoTableContents* getModel() { return static_cast<VideoTableContents*>(TableListBox::getModel()); }
    
    /*
    int getNumRows() override {
        static int prevNumRows = 0;
        int newNumRows = 0;
        VideoTableContents* model = getModel();
        if(model != nullptr)
            newNumRows = model->getNumRows();
        return newNumRows;
    }
    */
        
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
        getModel()->setDragSourceIndex(dragSourceDetails.description);
        //itemIsBeingDraggedOver = true;
        repaint();
    }

    void itemDragMove (const SourceDetails& dragSourceDetails) override
    {
        getModel()->setDragHoverIndex(getInsertionIndexForPosition(
            dragSourceDetails.localPosition.getX(), dragSourceDetails.localPosition.getY()
        ));
        repaint();
    }

    void itemDragExit (const SourceDetails& /*dragSourceDetails*/) override
    {
        //itemIsBeingDraggedOver = false;
        getModel()->setDragHoverIndex(-1);
        getModel()->setDragSourceIndex(-1);
        repaint();
    }

    void itemDropped (const SourceDetails& dragSourceDetails) override
    {
        //DBG("Item " + dragSourceDetails.description.toString() + " dropped at " + String(dragHoverIndex));
        // handle the drop
        getModel()->itemDropped();
        //itemIsBeingDraggedOver = false;
        //model->setDragHoverIndex(-1);
        //odel->setDragSourceIndex(-1);
        repaint();
    }

    bool hasInterestingFileExtension(const String& fileName) {
        return (
                fileName.matchesWildcard("*.mov", true)
            ||  fileName.matchesWildcard("*.mp4", true)
        );
    }
    
    //==============================================================================
    // These methods implement the FileDragAndDropTarget interface, and allow our component
    // to accept drag-and-drop of files..

    bool isInterestedInFileDrag (const StringArray& files) override
    {
        // filter out by extension:
        for(String& fileName : files) {
            if(hasInterestingFileExtension(fileName)) return true;
        }
        
        return false;
    }

    void fileDragEnter (const StringArray& files, int /*x*/, int /*y*/) override
    {
        //getModel()->clearHoveringFiles();
        for(String& fileName : files) {
            if(hasInterestingFileExtension(fileName)) getModel()->addHoveringFile(fileName);
        }
        //fileIsBeingDraggedOver = true;
        updateContent();
        //repaint();
    }

    void fileDragMove (const StringArray& /*files*/, int x, int y) override
    {
        getModel()->setDragHoverIndex(getInsertionIndexForPosition(x, y));
        repaint();
    }

    void fileDragExit (const StringArray& /*files*/) override
    {
        //fileIsBeingDraggedOver = false;
        getModel()->clearHoveringFiles();
        
        repaint();
    }

    void filesDropped (const StringArray& /*files*/, int x, int y) override
    {
        //DBG("Files dropped: " + hoveringFiles.joinIntoString("\n") + "\nat row " + String(dragHoverIndex));
        //fileIsBeingDraggedOver = false;
        //hoveringFiles.clear();
        //model->clearHoveringFiles();
        getModel()->filesDropped();
        updateContent();
        //repaint();
    }
private:


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
