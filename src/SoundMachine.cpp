#include "SoundMachine.h"

SoundMachine::SoundMachine() : ofxTCPClient()
{
    //ctor
    m_port = 31863;
    m_ip = "127.0.0.1";
    setup(m_ip, m_port, false);
}

SoundMachine::~SoundMachine()
{
    //dtor
}


void SoundMachine::reportEvent(enum eSoundEventIds _id)
{
    string sendString = "";
    switch (_id) {
    case SOUND_EVENT_START:
        sendString = "start";
        break;
    case SOUND_EVENT_SOMEONE_THERE:
        sendString = "there";
        break;
    case SOUND_EVENT_NOONE_THERE:
        sendString = "gone";
        break;
    case SOUND_EVENT_STOP:
        sendString = "stop";
        break;
    }
    if (isConnected()) {
        send(sendString);
    } else {
        setup(m_ip, m_port, false);
        printf("sound event %s \n", sendString.c_str());
    }
}
