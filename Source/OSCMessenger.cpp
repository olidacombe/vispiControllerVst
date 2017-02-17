/*
  ==============================================================================

    OSCMessenger.cpp
    Created: 17 Feb 2017 11:19:57pm
    Author:  Oli

  ==============================================================================
*/

#include "OSCMessenger.h"

OSCMessenger::OSCMessenger(String targetHost, int targetPort) :
   Thread("Message Dispatcher (OSC)"),  host(targetHost), port(targetPort)
{
    if (! sender.connect (host, port))
        showConnectionErrorMessage ("Error: could not connect to UDP port");
}