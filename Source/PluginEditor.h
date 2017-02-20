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

    VispiControllerVstAudioProcessor& processor;

    std::vector<String> fileNames;
    
    class VideoListboxContents : public ListBoxModel
    {
    public:
        VideoListboxContents(const std::vector<String> *names);
        int getNumRows() override;
        
        void paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected) override;
    private:
        const std::vector<String> *fileNames;
    };
    
    ScopedPointer<VideoListboxContents> videoListboxModel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VispiControllerVstAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
