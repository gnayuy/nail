// nail.h
// neural activity imaging library
// developed by Yang Yu (gnayuy@gmail.com)

#ifndef __NAIL_H__
#define __NAIL_H__

#include <sys/time.h>
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
    struct timeval m_startTime, m_endTime;
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
    Image(unsigned char *data, long x, long y, long z, long c, long t, float vsx, float vsy, float vsz, DataType type);
    ~Image();
    
public:
    // image
    void setOrigin(float x, float y, float z);
    void setDimension(long x, long  y, long z, long c, long t);
    void setResolution(float x, float y, float z);
    void setData(unsigned char *data);
    void setDataType(DataType type);

    long dimX();
    long dimY();
    long dimZ();
    long dimC();
    long dimT();

    float voxelSizeX();
    float voxelSizeY();
    float voxelSizeZ();

    unsigned char * data();
    DataType dataType();

    // process
    void adjustIntensity(unsigned short *&p, IntensityRange ori, IntensityRange dst);

public:
    unsigned char *p;
    float ox,oy,oz; // origin (offset)
    float vx,vy,vz; // voxelsize
    long sx,sy,sz,sc,st; // dimension XYZCT
    DataType dt;
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

public:
    Image m_image;
};

#endif // __NAIL_H__
