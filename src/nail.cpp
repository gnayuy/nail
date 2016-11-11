// nail.cpp
// neural activity imaging library
// developed by Yang Yu (gnayuy@gmail.com)

#include "nail.h"

// class Timer
Timer::Timer()
{
}

Timer::~Timer()
{
}

void Timer::start()
{
    gettimeofday(&m_startTime, NULL);
}

double Timer::getEclipseTime()
{
    gettimeofday(&m_endTime, NULL);
    double diff = m_endTime.tv_sec - m_startTime.tv_sec + double (m_endTime.tv_usec - m_startTime.tv_usec) / 1000000;

    return diff;
}

// class Image
Image::Image()
{
    p = NULL;

    dt = UNKNOWNDATATYPE;
}

Image::Image(unsigned char *data, long x, long y, long z, long c, long t, float vsx, float vsy, float vsz, DataType type)
{
    setData(data);

    setDimension(x,y,z,c,t);
    setResolution(vsx,vsy,vsz);
    setDataType(type);
}

Image::~Image()
{
    del1dp<unsigned char>(p);
}

void Image::setOrigin(float x, float y, float z)
{
    ox = x;
    oy = y;
    oz = z;
}

void Image::setDimension(long x, long  y, long z, long c, long t)
{
    sx = x;
    sy = y;
    sz = z;
    sc = c;
    st = t;
}

void Image::setResolution(float x, float y, float z)
{
    vx = x;
    vy = y;
    vz = z;
}

void Image::setData(unsigned char *data)
{
    p = data;
}
void Image::setDataType(DataType type)
{
    dt = type;
}

long Image::dimX()
{
    return sx;
}

long Image::dimY()
{
    return sy;
}

long Image::dimZ()
{
    return sz;
}

long Image::dimC()
{
    return sc;
}

long Image::dimT()
{
    return st;
}

float Image::voxelSizeX()
{
    return vx;
}

float Image::voxelSizeY()
{
    return vy;
}

float Image::voxelSizeZ()
{
    return vz;
}

unsigned char * Image::data()
{
    return p;
}

DataType Image::dataType()
{
    return dt;
}

void Image::adjustIntensity(unsigned short *&p, IntensityRange ori, IntensityRange dst)
{
    //
    long i,j,k, offz, offy, idx;

    //
    long lengthOri = ori.max - ori.min;
    long lengthDst = dst.max - dst.min;

    double *lut = NULL;
    new1dp<double, long>(lut, lengthOri);

    for(i=ori.min; i<ori.max; i++)
    {
        idx = i - ori.min;

        lut[idx] = double(lengthDst * idx) / double(lengthOri) + double(dst.min);
    }

    //
    for(k=0; k<sz; k++)
    {
        offz = k*sx*sy;
        for(j=0; j<sy; j++)
        {
            offy = offz + j*sx;
            for(i=0; i<sx; i++)
            {
                idx = offy + i;

                p[idx] = lut[ p[idx] ];
            }
        }
    }
}

// class Nail
Nail::Nail()
{
}

Nail::~Nail()
{
}

int Nail::load(string filename)
{
    //
    TiffIO tiff;

    if(tiff.canReadFile(const_cast<char*>(filename.c_str())))
    {
        tiff.read();
    }
    else
    {
        std::cout<<"Fail to read tiff image "<<filename<<"."<<std::endl;
        return -1;
    }

    long x,y,z;
    long sx = tiff.getDimx();
    long sy = tiff.getDimy();
    long sz = tiff.getDimz();
    int datatype = tiff.getDataType();

    //
    if(datatype==USHORT)
    {
        //
        unsigned short *pTiff = (unsigned short*)(tiff.getData());

    }
    else
    {
        // other data type
    }

    //
    return 0;
}

int Nail::save(string filename)
{
    //
    if(!m_image.p)
    {
        cout<<"Empty image!"<<endl;
        return -1;
    }

    cout<<"save image as "<<filename<<endl;

    //
    TiffIO tif;

    //
    tif.setResX(m_image.vx);
    tif.setResY(m_image.vy);
    tif.setResZ(m_image.vz);

    tif.setDataType(m_image.dt);

    tif.setDimx(m_image.sx);
    tif.setDimy(m_image.sy);
    tif.setDimz(m_image.sz);
    tif.setDimc(m_image.sc);
    tif.setDimt(m_image.st);

    //
    tif.setData((void*)(m_image.p));
    tif.setFileName(const_cast<char*>(filename.c_str()));

    //
    tif.write();

    //
    return 0;
}

// CLI
DEFINE_string(input, "", "input TIFF file name (.tif)");
DEFINE_string(output, "", "output TIFF file name (.tif)");
DEFINE_uint64(sx, 1, "size (voxels) in x axis");
DEFINE_uint64(sy, 1, "size (voxels) in y axis");
DEFINE_uint64(sz, 1, "size (voxels) in z axis");
DEFINE_uint64(sc, 1, "the number of color channels");
DEFINE_uint64(st, 1, "the number of time frames");
DEFINE_uint64(x, 1, "offset (voxels) in x axis");
DEFINE_uint64(y, 1, "offset (voxels) in y axis");
DEFINE_uint64(z, 1, "offset (voxels) in z axis");
DEFINE_double(vx, 1.0, "voxel size in x axis");
DEFINE_double(vy, 1.0, "voxel size in y axis");
DEFINE_double(vz, 1.0, "voxel size in z axis");

//
DEFINE_bool(test, false, "test");
DEFINE_uint64(testOption, 0, "test option");

// main
int main(int argc, char *argv[])
{
    //
    gflags::SetUsageMessage("nail -i <input> -o <output>");
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    //
    if(FLAGS_test)
    {
        if(FLAGS_testOption==0)
        {
        }
        else if(FLAGS_testOption==1)
        {
        }
        else
        {

        }

        //
        return 0;
    }

    //
    if(FLAGS_input.substr(FLAGS_input.find_last_of(".") + 1) != "tif")
    {
        cout<<"Your input \""<<FLAGS_input<<"\" is not a TIFF image!"<<endl;
        return -1;
    }

    //
    return 0;
}
