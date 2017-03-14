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

    OSCMessage newVideoSelection("/video/playfile", name);
    pushRawOscMsg(newVideoSelection);

}

void OSCMessenger::pushRawOscMsg(const OSCMessage& m) {
    const ScopedLock mutex(oscMessagesMutex);
    oscMessages.push_back(m);
    notify();
}

void OSCMessenger::run()
{
    while (! threadShouldExit())
    {
        oscMessagesMutex.enter();
        
        if(!oscMessages.empty()) {
            OSCMessage message = oscMessages.front();
            oscMessages.pop_front();
            
            oscMessagesMutex.exit();
            
            sendOscMsg(message);
            sendChangeMessage();
        } else {
            oscMessagesMutex.exit();
            wait(-1);
        }
            
    }   
}


bool OSCMessenger::sendOscMsg(const OSCMessage& m) {
    if(!sender.send(m)) {
        showConnectionErrorMessage("Error: could not send OSC message");
        return false;
    }
    return true;
}