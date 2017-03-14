/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
VispiControllerVstAudioProcessor::VispiControllerVstAudioProcessor() :
#ifndef JucePlugin_PreferredChannelConfigurations
     AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),

#endif
    messenger("vispi", 12345),
    playlistFilename("~/live_videos.xspf"),
    videoSelectionCC(99),
    parameters(*this, nullptr)
{
    reloadPlaylist();
    parameters.createAndAddParameter ("loop", "Loop", String(),
                                      NormalisableRange<float> (0.0f, 1.0f, 1.0f), 0.0f,
                                      [](float value)
                                      {
                                          // value to text function
                                          return value < 0.5 ? "Once" : "Loop";
                                      },
                                      [](const String& text)
                                      {
                                          // text to value function
                                          if (text == "Once")    return 0.0f;
                                          if (text == "Loop")  return 1.0f;
                                          return 0.0f;
                                      });
    
    parameters.state = ValueTree(Identifier("vispiController"));
}

VispiControllerVstAudioProcessor::~VispiControllerVstAudioProcessor()
{
}

//==============================================================================
const String VispiControllerVstAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool VispiControllerVstAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool VispiControllerVstAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool VispiControllerVstAudioProcessor::isMidiEffect() const
{
    #if JucePlugin_IsMidiEffect
     return true;
    #else
     return false;
    #endif
}

double VispiControllerVstAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int VispiControllerVstAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int VispiControllerVstAudioProcessor::getCurrentProgram()
{
    return 0;
}

void VispiControllerVstAudioProcessor::setCurrentProgram (int index)
{
}

const String VispiControllerVstAudioProcessor::getProgramName (int index)
{
    return String();
}

void VispiControllerVstAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void VispiControllerVstAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void VispiControllerVstAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VispiControllerVstAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void VispiControllerVstAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    /*
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
    */
    buffer.clear();
    
    MidiBuffer processedMidi;
    int time;
    MidiMessage m;
    
    for (MidiBuffer::Iterator i (midiMessages); i.getNextEvent (m, time);)
    {
        if (m.isController() && m.getControllerNumber() == videoSelectionCC)
        {
            //videoSelection = m.getControllerValue(); // % playlist length...
            processVideoSelection(m.getControllerValue());
        } else if(m.isNoteOn()) {
            processVideoSelection(m.getNoteNumber());
        }
        
        
        processedMidi.addEvent (m, time);
    }
    
    midiMessages.swapWith (processedMidi);
}

void VispiControllerVstAudioProcessor::processVideoSelection(const int n) {
    if(n<getNumFiles()) {
        selectedVideoIndex=n;
        messenger.pushVideoFile(getFileName(n));
    }
}


bool VispiControllerVstAudioProcessor::loadPlaylist(const String& path) {
    
    File file(path);
    if(!file.exists()) return false;
    playlistData = XmlDocument::parse(file);
    if(playlistData == nullptr) return false;
    
    ScopedLock fileNamesLock(fileNamesMutex);
    fileNames.clear();
    // now populate our fileNames vector...
    
    XmlElement * tracklist = playlistData->getChildByName("trackList");
    if(tracklist == nullptr) return false;
    XmlElement * track = tracklist->getChildByName("track");
    
    while(track != nullptr)
    {
        
        
        XmlElement * location = track->getChildByName("location");
        if(location != nullptr) {
            fileNames.push_back(xspfUriToString(location->getAllSubText()));
        }
        
        track = track->getNextElementWithTagName("track");
    }
    
    
    return true;
}

const String VispiControllerVstAudioProcessor::xspfUriToString(const String& uri)
{
    return URL::removeEscapeChars(uri.fromLastOccurrenceOf("/", false, false));
    
}



//==============================================================================
bool VispiControllerVstAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* VispiControllerVstAudioProcessor::createEditor()
{
    return new VispiControllerVstAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void VispiControllerVstAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    ScopedPointer<XmlElement> xml (parameters.state.createXml());
    copyXmlToBinary (*xml, destData);
}

void VispiControllerVstAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.state = ValueTree::fromXml (*xmlState);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VispiControllerVstAudioProcessor();
}
