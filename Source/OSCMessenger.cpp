/*
  ==============================================================================

    OSCMessenger.cpp
    Created: 17 Feb 2017 11:19:57pm
    Author:  Oli

  ==============================================================================
*/

// see Poco::Contidion section of http://openframeworks.cc/ofBook/chapters/threads.html
// and replace Poco stuff with Juce Thread class functionality

#include "OSCMessenger.h"

OSCMessenger::OSCMessenger(String targetHost, int targetPort) :
   Thread("Message Dispatcher (OSC)"),  host(targetHost), port(targetPort)
{
    if (sender.connect (host, port)) {
        startThread(); // something going wrong here
    }
    else {
        showConnectionErrorMessage ("Error: could not connect to UDP port");
    }
}

void OSCMessenger::pushVideoFile(const String& name) {
    
    const ScopedLock mutex(videoSelectionMutex);
    videoSelections.push_back(name);
    notify();

}

void OSCMessenger::run()
{
    while (! threadShouldExit())
    {

        videoSelectionMutex.enter();
        
        if(!videoSelections.empty()) {
            String videoSelection = videoSelections.front();
            videoSelections.pop_front();
            
            videoSelectionMutex.exit();
            
            sendVideoSelection(videoSelection);
        } else {
            wait(-1);
        }
            
    }   
}

bool OSCMessenger::sendVideoSelection(const String& name) {
    if(!sender.send("/video/playfile", name)) {
        showConnectionErrorMessage("Error: could not send Video Selection");
        return false;
    }
    return true;
}