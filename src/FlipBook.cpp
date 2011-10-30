#include "FlipBook.h"
#include "ofMain.h"

FlipBook::FlipBook(std::string _basefilename, unsigned int repeatCount):
    GlyphSampler(_basefilename),
    m_frameRepeatMax(repeatCount),
    m_frameRepeatCount(0),
    m_Position(0,0),
    m_whichFrame(0),
    paused(true)
{
    //ctor
    printf("loaded %d  files\n", goodGlyphs.size());
}

FlipBook::~FlipBook()
{
    //dtor
}

void FlipBook::draw()
{
    // only increment the frame and draw if we aren't paused
    if (!paused) {
        m_frameRepeatCount++;
        if (m_frameRepeatCount >= m_frameRepeatMax) {
            // step to the next frame if the repeate count has been reached
            m_frameRepeatCount = 0;
            m_whichFrame++;
//            printf("frame++ %d", m_whichFrame);
            if (m_whichFrame >= goodGlyphs.size()) {
                // loop the sequence
//                printf("loop\n");
                m_whichFrame = 0;
            }
        }
        glColor4f(1,1,1, 1);
        ofRect(m_Position.x,m_Position.y,150,150);
        goodGlyphs[(int) m_whichFrame]->draw(m_Position.x, m_Position.y);
    }
}


void FlipBook::pause()
{
    paused = true;
}

void FlipBook::play()
{
    paused = false;
}
