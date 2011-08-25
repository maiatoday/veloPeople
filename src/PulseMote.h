#ifndef PULSEMOTE_H
#define PULSEMOTE_H

#include <DataMote.h>


class PulseMote : public DataMote
{
public:
    PulseMote();
    virtual ~PulseMote();
    unsigned int getPulseCounter() {
        return m_PulseCounter;
    }
    void setPulseCounter(unsigned int val) {
        m_PulseCounter = val;
    }
    virtual void	draw();

protected:
    virtual void drawInside();
private:
    unsigned int m_PulseCounter;
};

#endif // PULSEMOTE_H
