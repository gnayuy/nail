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
    clock_gettime(CLOCK_MONOTONIC, &m_startTime);
}

double Timer::getEclipseTime()
{
    clock_gettime(CLOCK_MONOTONIC, &m_endTime);
    double diff = m_endTime.tv_sec - m_startTime.tv_sec + double (m_endTime.tv_nsec - m_startTime.tv_nsec) / 1000000000;

    return diff;
}

// class Image
Image::Image()
{
    p = NULL;
}

Image::~Image()
{
    if(p)
    {
        delete []p;
        p = NULL;
    }
}

void Image::setOrigin(double x, double y, double z)
{
    ox = x;
    oy = y;
    oz = z;
}

void Image::setDimension(long x, long  y, long z)
{
    sx = x;
    sy = y;
    sz = z;
}

void Image::setResolution(double x, double y, double z)
{
    vx = x;
    vy = y;
    vz = z;
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
    m_Data = NULL;
}

Nail::~Nail()
{
    clearData();
}

void Nail::clearData()
{
    del1dp<unsigned char>(m_Data);
}

int Nail::load(unsigned char *&p, string filename)
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
        unsigned short *pData = (unsigned short*)p;
        unsigned short *pTiff = (unsigned short*)(tiff.getData());

    }
    else
    {
        // other data type
    }

    //
    return 0;
}

int Nail::save(string filename, long sx, long sy, long sz, long sc, float vsx, float vsy, float vsz, int dataType)
{
    //
    if(!m_Data)
    {
        cout<<"NULL pointer!"<<endl;
        return -1;
    }

    cout<<"save image as "<<filename<<endl;
    cout<<"image size: "<<sx<<" "<<sy<<" "<<sz<<" "<<sc<<endl;
    cout<<"voxel size: "<<vsx<<" "<<vsy<<" "<<vsz<<endl;

    //
    TiffIO tif;

    //
    tif.setResX(vsx);
    tif.setResY(vsy);
    tif.setResZ(vsz);

    tif.setDataType(dataType);

    tif.setDimx(sx);
    tif.setDimy(sy);
    tif.setDimz(sz);
    tif.setDimc(sc);
    tif.setDimt(1);

    //
    tif.setData((void*)m_Data);
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
DEFINE_uint64(x, 1, "offset (voxels) in x axis");
DEFINE_uint64(y, 1, "offset (voxels) in y axis");
DEFINE_uint64(z, 1, "offset (voxels) in z axis");
DEFINE_double(vsx, 1.0, "voxel size in x axis");
DEFINE_double(vsy, 1.0, "voxel size in y axis");
DEFINE_double(vsz, 1.0, "voxel size in z axis");

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
        cout<<"Your input "<<FLAGS_input<<" is not a TIFF image!"<<endl;
        return -1;
    }

    //
    return 0;
}
