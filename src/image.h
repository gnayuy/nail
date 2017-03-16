// image.h
// define image related template functions
// developed by Yang Yu (gnayuy@gmail.com)

#ifndef __IMAGE_H__
#define __IMAGE_H__
//
#include <deque>
#include <queue>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <functional>
#include <vector>
#include <cmath>
#include <ctime>
#include <limits>
#include <complex>
#include <float.h>
//#include <codecvt>
using namespace std;

//#define EIGEN_DONT_ALIGN 1
#define EIGEN_DONT_VECTORIZE 1
#include <Eigen/Eigen>
using namespace Eigen;

#include "biomedicaldataio.h"

#ifndef EPSILON
#define EPSILON 1E-10
#endif

typedef enum { UNKNOWNDIMTYPE, D1D, D2D, D3D, D4D, D5D, D6D, D7D, D8D, D9D, DND } DimType;
typedef enum { UNKNOWNTT, TRANSLATION, RIGID, AFFINE, NONRIGID } TransformationType;
typedef enum { UNKNOWNIT, NearestNeighbor, Linear, BSpline, ThinPlateSpline } InterpolationType;
typedef enum { UNKNOWNOT, GradientDescent, GaussNewton, LevenbergMarquardt, PowellDogLeg, DownhillSimplex, MRF } OptimizationType;
typedef enum { UNKNOWNST, SSD, CC, MI } SimilarityType;
typedef enum { UNKNOWNTD, FORWARD, INVERSE } TransformDirectionType;
typedef enum { UNKNOWNCT, ENCODE, DECODE } CodeType;

// histogram
template<class Tdata, class Tidx>
class HistogramLUT
{
public:
    HistogramLUT();
    ~HistogramLUT();

public:
    void initLUT(Tdata *p, Tidx sz, Tidx nbins);
    Tidx getIndex(Tdata val);

public:
    Tidx bins, index;
    Tdata *lut; // look up table
    double minv, maxv;
};

//
// 1D vector
//

template <class T>
class Vec1D
{
public:
    Vec1D();
    ~Vec1D();
public:
    bool isNumber(const string& s);
    int str2num(string s);
    int getFileList(string fn);
    void zeros(long size);

public:
    vector<T> items;
};

//
/// 2d line
//

template <class T>
class Line2D
{
public:
    Line2D();
    Line2D(T slope, T intercept);
    ~Line2D();

public:
    void slope(T slope);
    void intercept(T intercept);
    void score(T score);

public:
    T k, b, s;
};

//
/// functions
//

/// similarity measures

// normalized mutual information
template <class Tsrc, class Tref, class Tidx>
int computeNMI(Tsrc *src, Tref *ref, Tidx sz, Tidx nbin, double& nmi);

// normalized cross correlation
template <class Tsrc, class Tref, class Tidx>
int computeNCC(Tsrc *src, Tref *ref, Tidx sz, double& ncc);

/// dynamic programming

//
template <class Tdata, class Tidx>
int getOptimalPath(Tdata *p, Tidx x, Tdata minY0, Tdata maxY0, Tdata minY, Tdata maxY, Tdata &k, Tdata &b);

/// 3D stack reconstruction from 2D slices

//
template <class Tdata, class Tidx>
int reconstructStack(Tdata *slices, Tidx n, Tdata *&stack, Tidx x, Tidx y, Tidx z, double k, double b);

/// basic image processing

//
template <class Tdata, class Tidx>
void recenter(Tdata* &pOut, LQuintuplet sizeOut, Tdata* pIn, LQuintuplet sizeIn);
template<class Tdata, class Tidx>
int isampler(Tdata *&dst, Tdata *src, Tidx dx, Tidx dy, Tidx dz, Tidx sx, Tidx sy, Tidx sz, double rx, double ry, double rz, InterpolationType interptype);

#endif // __IMAGE_H__
