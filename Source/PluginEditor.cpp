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
    : AudioProcessorEditor (&p), processor (p), videoTable("Video list", nullptr)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    videoTable.setHeader(new VideoTableHeader());
    
    videoTableModel = new VideoTableContents(processor);
    videoTable.setModel(videoTableModel);
    addAndMakeVisible(videoTable);
    
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
}

VispiControllerVstAudioProcessorEditor::VideoTableContents::VideoTableContents(VispiControllerVstAudioProcessor& p) : processor(p)
{}

int VispiControllerVstAudioProcessorEditor::VideoTableContents::getNumRows() {
    return processor.getNumFiles();
}

void VispiControllerVstAudioProcessorEditor::VideoTableContents::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    if(rowNumber >= getNumRows()) return;
    g.setColour(Colours::black);

    g.setFont(height * 0.7f);
    // ScopedLock...?
    String content;
    switch(columnId) {
        case INDEX_COLUMN:
            content = String(rowNumber);
            break;
        case NAME_COLUMN:
            content = processor.getFileName(rowNumber);
            break;
        default:
            break;
    }
    g.drawText(content, 5, 0, width, height, Justification::centredLeft, true);
}

void VispiControllerVstAudioProcessorEditor::VideoTableContents::paintRowBackground(juce::Graphics & g, int rowNumber, int width, int height, bool rowIsSelected)
{
    //if(rowNumber >= getNumRows()) return;
    if(rowIsSelected) {
        g.fillAll(Colours::cornflowerblue);
    } else {
        g.fillAll(Colours::cornsilk);
    }

}

void VispiControllerVstAudioProcessorEditor::VideoTableContents::cellClicked(int rowNumber, int columnId, const MouseEvent& e)
{
    if(rowNumber != -1) {
        processor.processVideoSelection(rowNumber);
    }
}

void VispiControllerVstAudioProcessorEditor::VideoTableHeader::columnClicked(int columnId, const ModifierKeys& mods)
{
    std::cout << "columnClicked" << std::endl;
    switch(columnId) {
        case(INDEX_COLUMN):
            // set flag to toggle note name / int display
            noteStyleDisplay = !noteStyleDisplay;
            std::cout << "noteStyleDisplay: " << noteStyleDisplay << std::endl;
            //break;
            /*
        case(NAME_COLUMN):
            // don't sort
            break;
             */
        default:
            // defer to base
            TableHeaderComponent::columnClicked(columnId, mods);
            break;
    }
}

VispiControllerVstAudioProcessorEditor::VideoTableHeader::VideoTableHeader()
{
    // gross - think more about where to set these things correctly
    // resized if possible
    addColumn("Index", INDEX_COLUMN, 50, 50, -1, defaultFlags);
    addColumn("Name", NAME_COLUMN, 120, 50, -1, defaultFlags);
}
