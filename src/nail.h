// nail.h
// neural activity imaging library
// developed by Yang Yu (gnayuy@gmail.com)

#ifndef __NAIL_H__
#define __NAIL_H__

//
#include <sys/time.h>
#include <gflags/gflags.h>

#include "biomedicaldataio.h"
#include "image.h"
#include "image.hxx"

//
typedef Vec1D<int> IVec1D;
typedef Vec1D<long> LVec1D;
typedef Vec1D<string> SVec1D;

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
    void im2bw(double threshold, double v);
    void add(BioMedicalData *image);
    void multiply(double v);
    void createLabelImage(BioMedicalData *image, double threshold, double label);
    LVec1D * countVoxels(IVec1D *labels);
    LVec1D * countVoxels(BioMedicalData *mask, int nLabels);
    double imageCompare(BioMedicalData *reference, SimilarityType similarity);

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
    int imageReadWrite(string in, string out);
    int gammaFilter(string in, string out, double gamma, CodeType ct);
    int genMaskImageFromLabels(string in, string out, string s, bool exclude);
    int genLabelImage(string in, string out);
    int countVoxels(string in, string out, string s, bool withMask=false, int nLabels=1);
    int binarize(string in, string out);
    int imageCompare(string in, string out, string ref, SimilarityType similarity);

public:
    ImageProcess process;
};

#endif // __NAIL_H__
