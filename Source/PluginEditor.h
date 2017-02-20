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
class VispiControllerVstAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    VispiControllerVstAudioProcessorEditor (VispiControllerVstAudioProcessor&);
    ~VispiControllerVstAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    ListBox videoListBox;
    //TextEditor midiMessagesDebugBox;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    VispiControllerVstAudioProcessor& processor;

    
    struct VideoListboxContents : public ListBoxModel
    {
        int getNumRows() override
        {
            return 20;
        }
        
        void paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected) override
        {
            g.setColour(Colours::black);
            g.setFont(height * 0.7f);
            g.drawText("Thingaaay " + String(rowNumber + 1), 5, 0, width, height, Justification::centredLeft, true);
        }
    };
    
    VideoListboxContents videoListboxModel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VispiControllerVstAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
