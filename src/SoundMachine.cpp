#include "SoundMachine.h"

SoundMachine::SoundMachine() : ofxTCPClient()
{
    //ctor
    m_port = SOUND_PORT;
    m_ip = "127.0.0.1";
    setVerbose(true);
    setup(m_ip, m_port, false);

}

SoundMachine::~SoundMachine()
{
    //dtor
}


void SoundMachine::sendEvent(enum eSoundEventIds _id, string param)
{
    string sendString = "";
    switch (_id) {
    case SOUND_EVENT_START:
        sendString = "someone start ";
        break;
    case SOUND_EVENT_SOMEONE_THERE:
        sendString = "someone there ";
        break;
    case SOUND_EVENT_SOMEONE_THERESNIFF:
        sendString = "someone theresniff ";
        break;
    case SOUND_EVENT_NOONE_THERE:
        sendString = "someone gone ";
        break;
    case SOUND_EVENT_STOP:
        sendString = "someone stop ";
        break;
    }
    sendString.append(param);
    sendString.append("\n");
    if (isConnected()) {
        sendRaw(sendString);
        printf("sound event %s \n", sendString.c_str());
    } else {
        setup(m_ip, m_port, false);
        printf("Couldn't send sound event %s \n", sendString.c_str());
    }

}
