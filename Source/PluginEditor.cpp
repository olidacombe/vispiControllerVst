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
    
    videoListboxModel = new VideoListboxContents(processor);
    videoListBox.setModel(videoListboxModel);
    addAndMakeVisible(videoListBox);
    
    reloadButton.setButtonText("Reload Playlist");
    addAndMakeVisible(reloadButton);
    reloadButton.addListener(this);
    
    messenger = processor.getMessenger();
    messenger->addChangeListener(this);
}

VispiControllerVstAudioProcessorEditor::~VispiControllerVstAudioProcessorEditor()
{
    messenger->removeChangeListener(this);
    reloadButton.removeListener(this);
    // delete this:
    videoListboxModel = nullptr;
    // before the list it references
}

void VispiControllerVstAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster *source) {
    if(source==messenger) {
        //std::cout << "word from the messenger" << std::endl;
        videoListBox.selectRow(processor.getSelectedVideoIndex());
    }
}

void VispiControllerVstAudioProcessorEditor::buttonClicked(Button* button)
{
    if(button == &reloadButton) {
        reloadPlaylist();
    }
    
}

void VispiControllerVstAudioProcessorEditor::reloadPlaylist()
{
    processor.reloadPlaylist();
    videoListBox.updateContent();
    videoListBox.deselectAllRows();
    
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
    //videoListBox.setBounds(r.withSize(250, 180));
    videoListBox.setBounds(r.removeFromLeft(220));
    reloadButton.setBounds(r.removeFromTop(40).reduced(10));
}

VispiControllerVstAudioProcessorEditor::VideoListboxContents::VideoListboxContents(VispiControllerVstAudioProcessor& p) : processor(p)
{}

int VispiControllerVstAudioProcessorEditor::VideoListboxContents::getNumRows() {
    return processor.getNumFiles();
}

void VispiControllerVstAudioProcessorEditor::VideoListboxContents::paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected)
{
    if(rowNumber >= getNumRows()) return;
    g.setColour(Colours::black);
    if(rowIsSelected) {
        g.fillAll(Colours::cornsilk);
    }
    g.setFont(height * 0.7f);
    // ScopedLock...?
    g.drawText(processor.getFileName(rowNumber), 5, 0, width, height, Justification::centredLeft, true);
}

void VispiControllerVstAudioProcessorEditor::VideoListboxContents::listBoxItemClicked(int row, const MouseEvent& e)
{
    if(row != -1) {
        processor.processVideoSelection(row);
    }
}
