#ifndef FLIPBOOK_H
#define FLIPBOOK_H

#include "GlyphSampler.h"


class FlipBook : public GlyphSampler
{
    public:
        FlipBook(std::string _basefilename, unsigned int repeatCount);
        virtual ~FlipBook();
        unsigned int getframeRepeatMax() { return m_frameRepeatMax; }
        void setframeRepeatMax(unsigned int val) { m_frameRepeatMax = val; }
        ofPoint getPosition() { return m_Position; }
        void setPosition(ofPoint val) { m_Position = val; }
        void setDimension(ofPoint val) { m_Dimension = val; }
        void draw();
        void pause();
        void play();
        void setWhichFrame(unsigned int val) { m_whichFrame = val; }
    protected:
    private:
        unsigned int m_frameRepeatMax;
        unsigned int m_frameRepeatCount;
        ofPoint m_Position;
        ofPoint m_Dimension;
        unsigned int m_whichFrame;
        bool paused;
};

#endif // FLIPBOOK_H
