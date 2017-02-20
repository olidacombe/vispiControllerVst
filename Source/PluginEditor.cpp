/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
VispiControllerVstAudioProcessorEditor::VispiControllerVstAudioProcessorEditor (VispiControllerVstAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), videoListBox("Video list", nullptr)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    videoListBox.setModel(&videoListboxModel);
    addAndMakeVisible(videoListBox);
}

VispiControllerVstAudioProcessorEditor::~VispiControllerVstAudioProcessorEditor()
{
}

//==============================================================================
void VispiControllerVstAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::white);

    g.setColour (Colours::black);
    g.setFont (15.0f);
    //g.drawFittedText (String(processor.getVideoSelection()), getLocalBounds(), Justification::centred, 1);
}

void VispiControllerVstAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    Rectangle<int> r (getLocalBounds().reduced(8));
    videoListBox.setBounds(r.withSize(250, 180));
    //videoListBox.setBounds(r.removeFromTop(30).removeFromLeft(30));
}

VispiControllerVstAudioProcessorEditor::VideoListboxContents::VideoListboxContents()
{
    
}

int VispiControllerVstAudioProcessorEditor::VideoListboxContents::getNumRows() {
    return 20;
}

void VispiControllerVstAudioProcessorEditor::VideoListboxContents::paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected)
{
    g.setColour(Colours::black);
    g.setFont(height * 0.7f);
    g.drawText("Thingaaay " + String(rowNumber + 1), 5, 0, width, height, Justification::centredLeft, true);
}
