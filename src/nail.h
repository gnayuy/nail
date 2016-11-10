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
    double getEclipseTime() const;

private:
    struct timeval m_startTime, m_endTime;
};

//
class Image
{
public:
    Image();
    ~Image();
    
public:
    void getOffset(string octreepath, double ox, double oy, double oz, double vx, double vy, double vz, long dimx, long dimy, long dimz, double &offx, double &offy, double &offz);
    void setOrigin(double x, double y, double z);
    void setDimension(long x, long  y, long z);
    void setResolution(double x, double y, double z);

public:
    unsigned char *p;
    double ox,oy,oz; // origin
    double vx,vy,vz; // voxelsize
    long dimx,dimy,dimz; // dimension
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
