#ifndef GLYPHSAMPLER_H
#define GLYPHSAMPLER_H

#include <ofImage.h>

#include <vector>

class GlyphSampler
{
    public:
        GlyphSampler(std::string _basefilename);
        virtual ~GlyphSampler();
        ofImage* getSampleGlyph();
    protected:
    bool isImageFile(std::string _filename);
    std::vector<ofImage*> goodGlyphs;
    ofImage* defaultGlyph;
    unsigned char* pPixels;
};

#endif // GLYPHSAMPLER_H
