#ifndef PULSEMOTE_H
#define PULSEMOTE_H

#include <DataMote.h>


class PulseMote : public DataMote
{
public:
    PulseMote();
    virtual ~PulseMote();
    virtual void init();
    unsigned int getPulseCounter() {
        return pulseCounter;
    }
    void setPulseCounter(unsigned int val) {
        pulseCounter = val;
    }
    virtual void	draw();

protected:
    virtual void drawInside();
private:
    unsigned int pulseCounter;
    int inc;

};

#endif // PULSEMOTE_H
