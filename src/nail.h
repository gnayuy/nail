// nail.h
// neural activity imaging library
// developed by Yang Yu (gnayuy@gmail.com)


// todo list
// 1. mls affine
// 2. warp
//

#ifndef __NAIL_H__
#define __NAIL_H__

//
#include <sys/time.h>
#include <sys/stat.h>
#include <gflags/gflags.h>

#include "image.h"
#include "image.hxx"

//
typedef Vec1D<int> IVec1D;
typedef Vec1D<long> LVec1D;
typedef Vec1D<string> SVec1D;

//
bool doubleVarCompare(double x, double y);
bool floatVarCompare(float x, float y);

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

class ColorRGBLUT
{
public:
    ColorRGBLUT();
    ~ColorRGBLUT();
public:
    vector<Tuplet<unsigned char> > colors;
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
    void im2bw(double threshold, double v);
    void add(BioMedicalData *image);
    void multiply(double v);
    void createLabelImage(BioMedicalData *image, double threshold, double label);
    LVec1D * countVoxels(IVec1D *labels);
    LVec1D * countVoxels(BioMedicalData *mask, int nLabels);
    double imageCompare(BioMedicalData *reference, SimilarityType similarity);
    void sampling(BioMedicalData *image, double srx, double sry, double srz, int interptype);

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
    // I/O interface
    int load(string filename);
    int save(string filename);

    // image processing interface
    int adjustIntensity(string in, string out);
    int imageReadWrite(string in, string out, double vx, double vy, double vz);
    int gammaFilter(string in, string out, double gamma, CodeType ct);
    int genMaskImageFromLabels(string in, string out, string s, bool exclude);
    int genLabelImage(string in, string out);
    int countVoxels(string in, string out, string s, bool withMask=false, int nLabels=1);
    int binarize(string in, string out);
    int imageCompare(string in, string out, string ref, SimilarityType similarity);
    int constructStack(string in, string out, long z, double k, double b);
    int findOptimalPath(string in, string out, long sx, long sy, double minY0, double maxY0, double minY, double maxY);
    int recenterImage(string in, string out, long x, long y, long z, long c);
    int seq2stack(string in, string out, long zpos, long zstep, long z);
    int convert2byte(string in, string out);
    int imageSampling(string in, string out, double srx, double sry, double srz, int interptype);
    int imageTranslate(string in, string out, long x, long y, long z, long sx, long sy, long sz);
    int convertIndex2Color(string in, string out);
    int intensityRescale(string in, string out, double min, double max);

public:
    ImageProcess process;
};

#endif // __NAIL_H__
