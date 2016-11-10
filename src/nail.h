// nail.h
// neural activity imaging library
// developed by Yang Yu (gnayuy@gmail.com)

#ifndef __NAIL_H__
#define __NAIL_H__

#include <time.h>
#include "dataio.h"
#include <gflags/gflags.h>

//
class Timer
{
public:
    Timer();
    ~Timer();

public:
    void start();
    double getEclipseTime();

private:
    struct timespec m_startTime, m_endTime;
};

//
struct IntensityRange
{
    long min, max;
};

//
class Image
{
public:
    Image();
    ~Image();
    
public:
    void setOrigin(double x, double y, double z);
    void setDimension(long x, long  y, long z);
    void setResolution(double x, double y, double z);

    long dimX();
    long dimY();
    long dimZ();

    void adjustIntensity(unsigned short *&p, IntensityRange ori, IntensityRange dst);

public:
    unsigned char *p;
    double ox,oy,oz; // origin
    double vx,vy,vz; // voxelsize
    long sx,sy,sz; // dimension
};

//
class Nail
{
public:
    Nail();
    ~Nail();

public:
    void clearData();

    int load(unsigned char *&p, string filename);
    int save(string filename, long sx, long sy, long sz, long sc, float vsx, float vsy, float vsz, int dataType);

public:
    unsigned char *m_Data;
};

#endif // __NAIL_H__
