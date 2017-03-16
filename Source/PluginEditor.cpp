/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
VispiControllerVstAudioProcessorEditor::VispiControllerVstAudioProcessorEditor (VispiControllerVstAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor(&p), processor(p), valueTreeState(vts), videoTable("Video list", nullptr)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    //videoTableHeader = new VideoTableHeader();
    videoTable.setHeader(new VideoTableHeader());
    
    videoTableModel = new VideoTableContents(processor);
    videoTable.setModel(videoTableModel);
    addAndMakeVisible(videoTable);
    
    reloadButton.setButtonText("Reload Playlist");
    addAndMakeVisible(reloadButton);
    reloadButton.addListener(this);
    
    loopButton.setButtonText("Loop");
    addAndMakeVisible(loopButton);
    loopButtonAttachment = new ButtonAttachment (valueTreeState, "loop", loopButton);
    
    stopButton.setButtonText("Stop");
    addAndMakeVisible(stopButton);
    stopButtonAttachment = new ButtonAttachment (valueTreeState, "stop", stopButton);
    
    messenger = processor.getMessenger();
    messenger->addChangeListener(this);
}

VispiControllerVstAudioProcessorEditor::~VispiControllerVstAudioProcessorEditor()
{
    messenger->removeChangeListener(this);
    reloadButton.removeListener(this);
    
    loopButtonAttachment = nullptr;
    
    //videoTableHeader = nullptr;
    // delete this:
    videoTableModel = nullptr;
    // before the list it references
}

void VispiControllerVstAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster *source) {
    if(source==messenger) {
        //std::cout << "word from the messenger" << std::endl;
        videoTable.selectRow(processor.getSelectedVideoIndex());
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
    videoTable.updateContent();
    videoTable.deselectAllRows();
    videoTable.autoSizeAllColumns();
    
    OSCMessage reloadMsg("/playlist/reload", true);
    messenger->pushRawOscMsg(reloadMsg);
    
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
    videoTable.setBounds(r.removeFromLeft(220));
    videoTable.autoSizeAllColumns();
    reloadButton.setBounds(r.removeFromTop(40).reduced(10));
    loopButton.setBounds(r.removeFromTop(40).reduced(10));
    stopButton.setBounds(r.removeFromTop(40).reduced(10));
}


