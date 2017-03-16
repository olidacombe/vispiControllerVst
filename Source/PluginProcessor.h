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
class VispiControllerVstAudioProcessor  :
        public AudioProcessor,
        public AudioProcessorValueTreeState::Listener
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
        if(n >=0 && n < fileNames.size())
            return basename(fileNames.at(n));
        return "";
    }
    
    std::vector<String> getFileNames() { return fileNames; }
    
    const int getNumFiles() {
        //const ScopedLock fileNamesLock(fileNamesMutex); // probably unnecessary
        return fileNames.size();
    }
    
    OSCMessenger* getMessenger() { return &messenger; }
    const int getSelectedVideoIndex() { return selectedVideoIndex; }
    
    static const String xspfUriToString(const String& uri);
    static const String basename(const String& path);

    void reloadPlaylist() {
        String playlistLoadSuccess = loadPlaylist(playlistFilename) ? "playlist ok" : "playlist fail";
        DBG(playlistLoadSuccess);
    }
    
    void processVideoSelection(const int n);
    void swapFiles(const int i, const int j)
    {
        const ScopedLock fileNamesLock(fileNamesMutex);
        
        const int fileNamesSizeAsInt = static_cast<int>(fileNames.size());
        const int ni = jmax(0, jmin(i, fileNamesSizeAsInt-1));
        const int nj = jmax(0, jmin(j, fileNamesSizeAsInt-1));


        std::iter_swap(fileNames.begin() + ni, fileNames.begin() + nj);
    }
    
    void insertFiles(const StringArray& paths, const int index) {
        const ScopedLock fileNamesLock(fileNamesMutex);
        if(index>=0 && index<=fileNames.size())
            fileNames.insert(fileNames.begin()+index, paths.begin(), paths.end());
        
        DBG("insertFiles result: ");
        for(String& f : fileNames) {
            DBG(f);
        }
    }
    
    void parameterChanged(const String &parameterID, float newValue) override;
    
private:
    AudioProcessorValueTreeState parameters;
    
    int videoSelectionCC;
    int selectedVideoIndex;
    String playlistFilename;
    ScopedPointer<XmlElement> playlistData;
    
    bool loadPlaylist(const String& path);
    
    std::vector<String> fileNames;
    CriticalSection fileNamesMutex;
    OSCMessenger messenger;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VispiControllerVstAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
