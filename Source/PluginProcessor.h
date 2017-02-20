/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "OSCMessenger.h"


//==============================================================================
/**
*/
class VispiControllerVstAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    VispiControllerVstAudioProcessor();
    ~VispiControllerVstAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //int getVideoSelection() { return videoSelection; }
    const String getFileName(const int n) {
        const ScopedLock fileNamesLock(fileNamesMutex);
        if(n < fileNames.size())
            return fileNames.at(n);
        return "";
    }
    const int getNumFiles() {
        //const ScopedLock fileNamesLock(fileNamesMutex); // probably unnecessary
        return fileNames.size();
    }
    
    OSCMessenger* getMessenger() { return &messenger; }
    const int getSelectedVideoIndex() { return selectedVideoIndex; }
    
    static const String xspfUriToString(const String& uri);

    
private:
    int videoSelectionCC;
    int selectedVideoIndex;
    String playlistFilename;
    ScopedPointer<XmlElement> playlistData;
    
    void processVideoSelection(const int n);
    bool loadPlaylist(const String& path);
    
    std::vector<String> fileNames;
    CriticalSection fileNamesMutex;
    OSCMessenger messenger;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VispiControllerVstAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
