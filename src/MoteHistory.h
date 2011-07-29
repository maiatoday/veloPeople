#ifndef MOTESATELLITE_H
#define MOTESATELLITE_H
#include "ofTypes.h"


class MoteHistory
{
    public:
        MoteHistory();
        virtual ~MoteHistory();
        void update(int x, int y, int radius, bool active);
        void draw();
        int getX(){return x;};
        int getY(){return y;};
        void setColor(ofColor _color){color = _color;};
        void setSkipCount(int _skipCount) {skipCount = _skipCount;};
    protected:
    private:
    bool active;
    int x;
    int y;
    ofColor color;
    int radius;
    int skipCount;

};

#endif // MOTESATELLITE_H
