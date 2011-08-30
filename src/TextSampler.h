#ifndef TEXTSAMPLER_H
#define TEXTSAMPLER_H

#include <string>
#include <vector>

class TextSampler
{
    public:
        TextSampler(std::string _filename);
        virtual ~TextSampler();
        std::string getSampleText();
    protected:
    private:
    std::vector<std::string*> goodStrings;
    std::string defaultString;
};

#endif // TEXTSAMPLER_H
