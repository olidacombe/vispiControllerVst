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
    if (! sender.connect (host, port))
        showConnectionErrorMessage ("Error: could not connect to UDP port");
}

void OSCMessenger::pushVideoFile(const String& name) {
    // mutex.lock();
    videoSelections.push_back(name);
    notify();
    // mutex.unlock();
}

void OSCMessenger::run()
{
    while (! threadShouldExit())
    {
        //mutex.lock();
        if(videoSelections.empty()) {
            wait(-1);  // suspend exectution on condition of addition to queue
        }
        
        String videoSelection = videoSelections.front();
        videoSelections.pop_front();
        
        //mutex.unlock();
        
        sendVideoSelection(videoSelection);
            
    }   
}

bool OSCMessenger::sendVideoSelection(const String& name) {
    if(!sender.send("/video/playfile", name)) {
        showConnectionErrorMessage("Error: could not send Video Selection");
        return false;
    }
    return true;
}