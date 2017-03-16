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
#include "VideoTable.h"


//==============================================================================
/**
*/
class VispiControllerVstAudioProcessorEditor  : public AudioProcessorEditor,
                                                public ChangeListener,
                                                public ButtonListener,
                                                public DragAndDropContainer
{
public:
    //typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
    
    VispiControllerVstAudioProcessorEditor (VispiControllerVstAudioProcessor&, AudioProcessorValueTreeState&);
    ~VispiControllerVstAudioProcessorEditor();

    void changeListenerCallback(ChangeBroadcaster *source) override;
    void buttonClicked (Button* button) override;
    
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    class MomentaryTextButton : public TextButton
    {
        void buttonStateChanged() override
        {
            setToggleState(isDown(), NotificationType::sendNotification);
        }
    };
    
private:
    
    ToggleButton loopButton;
    ScopedPointer<ButtonAttachment> loopButtonAttachment;
    MomentaryTextButton stopButton;
    ScopedPointer<ButtonAttachment> stopButtonAttachment;
    
    TableListBox videoTable;
    TextButton reloadButton;

    AudioProcessorValueTreeState& valueTreeState;
    VispiControllerVstAudioProcessor& processor;
    OSCMessenger* messenger;
    
    void reloadPlaylist();
    
    ScopedPointer<VideoTableContents> videoTableModel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VispiControllerVstAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
