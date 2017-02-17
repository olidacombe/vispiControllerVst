/*
  ==============================================================================

    OSCMessenger.h
    Created: 17 Feb 2017 11:19:57pm
    Author:  Oli

  ==============================================================================
*/

#ifndef MESSAGEDISPATCHER_H_INCLUDED
#define MESSAGEDISPATCHER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class OSCMessenger : public Thread
{
public:
    OSCMessenger(String targetHost, int targetPort);
    void run() override
    {
        while (! threadShouldExit())
        {
            //checkForBuffersToFree();
            wait (500);
        }   
    }
private:
    String host;
    int port;
    OSCSender sender;
    
    void showConnectionErrorMessage (const String& messageText)
    {
        AlertWindow::showMessageBoxAsync (
            AlertWindow::WarningIcon,
            "Connection error",
            messageText,
            "OK");
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCMessenger)
};



#endif  // MESSAGEDISPATCHER_H_INCLUDED
