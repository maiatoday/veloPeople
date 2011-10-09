#include "TextSampler.h"
#include <ofMath.h>

using namespace std;
#define MAX_TEXT_SAMPLES (20)
#define MAX_TEXT_LENGTH (20)
TextSampler::TextSampler(string _filename)
{
    FILE* fp;
    printf("opening file %s\n", _filename.c_str());
    fp = fopen(_filename.c_str(), "r");
    if (fp != NULL) {
        while (!feof(fp)) {
            char tempChars[MAX_TEXT_LENGTH];
            fgets(tempChars, MAX_TEXT_LENGTH-1, fp);
            string* pStr = new string(tempChars);
            goodStrings.push_back(pStr);
        }
        fclose(fp);
    } else {
        defaultString = "Hello World!";
    }
}

TextSampler::~TextSampler()
{

    // bleargh vector of pointers so must delete objects
    for (int i = 0;i<goodStrings.size(); i++) delete goodStrings[i];
    goodStrings.clear();
}

string TextSampler::getSampleText()
{
    if (goodStrings.size() > 0) {
        float rr = ofRandom(0, goodStrings.size()-1);
        return *goodStrings[(int) rr];
    }
    return defaultString;
}
