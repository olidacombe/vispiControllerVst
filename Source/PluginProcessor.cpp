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
    parameters.addParameterListener("loop", this);
    parameters.createAndAddParameter ("stop", "Stop", String(),
                                      NormalisableRange<float> (0.0f, 1.0f, 1.0f), 0.0f,
                                      [](float value)
                                      {
                                          // value to text function
                                          return value < 0.5 ? "Stop" : "Stopped";
                                      },
                                      [](const String& text)
                                      {
                                          // text to value function
                                          if (text == "Stop")    return 0.0f;
                                          if (text == "Stopped")  return 1.0f;
                                          return 0.0f;
                                      });
    parameters.addParameterListener("stop", this);
    
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

void VispiControllerVstAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue) {
    if(parameterID == "loop") {
        OSCMessage loopMessage("/loop", (newValue > 0.5)?true:false);
        messenger.pushRawOscMsg(loopMessage);
    } else if(parameterID == "stop" && newValue > 0.5) {
        OSCMessage stopMessage("/stop", true);
        messenger.pushRawOscMsg(stopMessage);
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
    return URL::removeEscapeChars(basename(uri));
}

const String VispiControllerVstAudioProcessor::basename(const String& path) {
    return path.fromLastOccurrenceOf("/",false, false);
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
    ScopedPointer<XmlElement> xml = new XmlElement("state");
    //ScopedPointer<XmlElement> parametersXml (parameters.state.createXml());
    //ScopedPointer<XmlElement> playlist = new XmlElement("playlist");
    
    ValueTree playlistValueTree("playlist");
    for(int i=0; i<fileNames.size(); i++) {
        ValueTree track("track");
        track.setProperty("path", fileNames[i], nullptr);
        playlistValueTree.addChild(track, i, nullptr);
    }
    
    xml->addChildElement(parameters.state.createXml());
    xml->addChildElement(playlistValueTree.createXml());
    copyXmlToBinary (*xml, destData);
}

void VispiControllerVstAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState != nullptr) {
        if(xmlState->hasTagName("state")) {
            XmlElement* parametersXml = xmlState->getChildByName (parameters.state.getType());
            if(parametersXml != nullptr)
                parameters.state = ValueTree::fromXml (*parametersXml);
            XmlElement* playlistXml = xmlState->getChildByName("playlist");
            if(playlistXml != nullptr) {
                ValueTree playlistValueTree = ValueTree::fromXml(*playlistXml);
                for(const ValueTree& child : playlistValueTree) {
                    if(child.hasType("track")) {
                        DBG("loading track name: " + child.getProperty("path", "not found").toString());
                        const ScopedLock fileNamesLock(fileNamesMutex);
                        fileNames.push_back(child.getProperty("path", "not found").toString());
                    }
                }
            }
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VispiControllerVstAudioProcessor();
}
