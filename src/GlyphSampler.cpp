#include "GlyphSampler.h"
#include <sys/types.h>
#include <dirent.h>

using namespace std;
#define MAX_GLYPH_SAMPLES (20)

// use this constructor something like this
//    pGlyphSampler = new GlyphSampler("data/images/glyphs");
// it uses data/images/defaultGlyph.png as the default image
// then it will load all .png files in the directory specified in the constructor

GlyphSampler::GlyphSampler(string _dirname)
{
    //ctor
    defaultGlyph = new ofImage();
    defaultGlyph->loadImage("images/defaultGlyph.png");

    DIR *dir; //the directory
    struct dirent *dp;
    //open the directory
    try {
        dir  = opendir(_dirname.c_str());
        if (dir != NULL) {
            while ((dp = readdir(dir)) != NULL) {
                if (dp->d_type == DT_REG) {
                    // images are loaded relative to the data directory so we chop this part off first
                    string filename(_dirname.substr(strlen("data/"),_dirname.length()));
                    filename.append("/");
                    filename.append(dp->d_name);

                    if (isImageFile(filename)) {
                        ofImage* newGlyph = new ofImage();
                        if (newGlyph->loadImage(filename)) {
                            goodGlyphs.push_back(newGlyph);
                        } else {
                            delete newGlyph;
                        }
                    }
                }
            }
            closedir(dir);
        } else {
            cout << "can't open directory" << _dirname << endl;
        }
    } catch (std::exception& e) {
        cout << e.what() <<" opening/reading " << _dirname << endl;
    }
}


GlyphSampler::~GlyphSampler()
{
    //dtor
    // bleargh vector of pointers so must delete objects
    for (int i = 0;i<goodGlyphs.size(); i++) delete goodGlyphs[i];
    goodGlyphs.clear();
}

ofImage* GlyphSampler::getSampleGlyph()
{
    if (goodGlyphs.size() > 0) {
        float rr = ofRandom(0, goodGlyphs.size()-1);
        return goodGlyphs[(int) rr];
    }
    return defaultGlyph;
}

bool GlyphSampler::isImageFile(std::string _filename)
{
    bool fileOk = false;
    string extension = _filename.substr(_filename.length() - 4, _filename.length()-1);
    // only allow png files because they have transparency
    if (extension == ".png")
        fileOk = true;
    return fileOk;
}
