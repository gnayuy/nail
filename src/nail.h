// nail.h
// neural activity imaging library
// developed by Yang Yu (gnayuy@gmail.com)

#ifndef __NAIL_H__
#define __NAIL_H__

#include <sys/time.h>
#include <gflags/gflags.h>

#include "biomedicaldataio.h"
#include "image.h"
#include "image.hxx"

typedef Vec1D<int> IVec1D;

//
class IntensityRange
{
public:
    IntensityRange();
    IntensityRange(long min, long max);
    ~IntensityRange();

public:
    long min();
    long max();

    void setMin(long v);
    void setMax(long v);

public:
    long _min, _max;
};

class ImageProcess
{
public:
    ImageProcess();
    ~ImageProcess();

public:
    void setImage(BioMedicalData *image);
    BioMedicalData * getImage();
    void thresholding();
    void adjustIntensity(unsigned short *&p, IntensityRange ori, IntensityRange dst);
    void gammaCorrection(double gamma, CodeType ct);
    void getImageFromLabels(IVec1D *v, bool exclude);

public:
    BioMedicalData *m_image;
};

//
class Nail
{
public:
    Nail();
    ~Nail();

public:
    int load(string filename);
    int save(string filename);

    int adjustIntensity(string in, string out);
    int imageReadWrite(string in, string out);
    int gammaFilter(string in, string out, double gamma, CodeType ct);
    int genMaskImageFromLabels(string in, string out, string s, bool exclude);

public:
    ImageProcess process;
};

#endif // __NAIL_H__
