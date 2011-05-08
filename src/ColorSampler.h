#ifndef COLORSAMPLER_H
#define COLORSAMPLER_H

#include <ofImage.h>

#include <vector>

class ColorSampler : public ofImage
{
    public:
        ColorSampler(string _filename);
        virtual ~ColorSampler();
        ofColor getSampleColor();
    protected:
    private:
    vector<ofColor*> goodColors;
    ofColor defaultColor;
    unsigned char* pPixels;

};

#endif // COLORSAMPLER_H
