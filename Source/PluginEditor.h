/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class VispiControllerVstAudioProcessorEditor  : public AudioProcessorEditor,
                                                public ChangeListener,
                                                public ButtonListener
{
public:
    const enum {
        INDEX_COLUMN = 1,
        NOTE_COLUMN = 2,
        NAME_COLUMN = 3
        //INT_MIDI_FORMAT = 3
    };
    
    VispiControllerVstAudioProcessorEditor (VispiControllerVstAudioProcessor&);
    ~VispiControllerVstAudioProcessorEditor();

    void changeListenerCallback(ChangeBroadcaster *source) override;
    void buttonClicked (Button* button) override;
    
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    TableListBox videoTable;
    TextButton reloadButton;

    VispiControllerVstAudioProcessor& processor;
    OSCMessenger* messenger;
    
    void reloadPlaylist();
    
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
    
    ScopedPointer<VideoTableContents> videoTableModel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VispiControllerVstAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
