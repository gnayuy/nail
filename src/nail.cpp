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

// class IntensityRange
IntensityRange::IntensityRange()
{

}

IntensityRange::IntensityRange(long min, long max)
{
    setMax(max);
    setMin(min);
}

IntensityRange::~IntensityRange()
{

}

long IntensityRange::min()
{
    return _min;
}

long IntensityRange::max()
{
    return _max;
}

void IntensityRange::setMin(long v)
{
    _min = v;
}

void IntensityRange::setMax(long v)
{
    _max = v;
}

// class Image
Image::Image()
{
    p = NULL;

    dt = UNKNOWNDATATYPE;

    ox = 0;
    oy = 0;
    oz = 0;

    vx = 1.0;
    vy = 1.0;
    vz = 1.0;

    sx = 1;
    sy = 1;
    sz = 1;
    sc = 1;
    st = 1;
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

void Image::thresholding()
{
    //
    // threshold segmentation using k-means
    //

    //
    if(!p)
    {
        cout<<"Invalid inputs for thresholding function"<<endl;
        return;
    }

    //
    long pagesz;
    pagesz=sx*sy*sz;

    //
    if(dt==USHORT)
    {
        //
        unsigned short *data = (unsigned short *)p;

        //
        long BINS = 4096; // histogram bins

        long *h=NULL, *hc=NULL;
        new1dp<long, long>(h, BINS);
        new1dp<long, long>(hc, BINS);

        memset(h, 0, sizeof(long)*BINS);

        // histogram
        HistogramLUT<unsigned short, long> hlut;
        hlut.initLUT(data, pagesz, BINS);
        foreach(pagesz, i)
        {
            h[hlut.getIndex(p[i])] ++;
        }

        // heuristic init center
        float mub=0.05*(hlut.maxv - hlut.minv) + hlut.minv;
        float muf=0.30*(hlut.maxv - hlut.minv) + hlut.minv;

        //
        while (true)
        {
            float oldmub=mub, oldmuf=muf;

            for(long i=0; i<BINS; i++)
            {
                if(h[i]==0)
                    continue;

                float cb = nail_abs<float>(float(hlut.lut[i])-mub);
                float cf = nail_abs<float>(float(hlut.lut[i])-muf);

                hc[i] = (cb<=cf)?1:2; // class 1 and class 2
            }

            // update centers
            float sum_b=0, sum_bw=0, sum_f=0, sum_fw=0;

            for(long i=0; i<BINS; i++)
            {
                if(h[i]==0)
                    continue;

                if(hc[i]==1)
                {
                    sum_bw += (i+1)*h[i];
                    sum_b += h[i];
                }
                else if(hc[i]==2)
                {
                    sum_fw += (i+1)*h[i];
                    sum_f += h[i];
                }
            }

            mub = hlut.lut[ long(sum_bw/sum_b) ];
            muf = hlut.lut[ long(sum_fw/sum_f) ];

            if(nail_abs<float>(mub - oldmub)<1 && nail_abs<float>(muf - oldmuf)<1)  break;
        }

        //
        unsigned short threshold = (mub+muf)/2;
        for(long i=0; i<pagesz; i++)
        {
            if(p[i]>threshold)
                p[i] = hlut.maxv;
            else
                p[i] = hlut.minv;
        }

        //
        del1dp<long>(h);
        del1dp<long>(hc);
    }
    else
    {

    }

    //
    return;
}

void Image::adjustIntensity(unsigned short *&p, IntensityRange ori, IntensityRange dst)
{
    //
    long i,j,k, offz, offy, idx;

    //
    long lengthOri = ori.max() - ori.min();
    long lengthDst = dst.max() - dst.min();

    double *lut = NULL;
    new1dp<double, long>(lut, lengthOri);

    for(i=ori.min(); i<ori.max(); i++)
    {
        idx = i - ori.min();

        lut[idx] = double(lengthDst * idx) / double(lengthOri) + double(dst.min());
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

    //
    m_image.setData((unsigned char*)(tiff.getData()));
    m_image.setDimension(tiff.getDimX(),tiff.getDimY(),tiff.getDimZ(),tiff.getDimC(),tiff.getDimT());
    m_image.setResolution(tiff.getResX(),tiff.getResY(),tiff.getResZ());
    m_image.setDataType((DataType)(tiff.getDataType()));

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

    tif.setDimX(m_image.sx);
    tif.setDimY(m_image.sy);
    tif.setDimZ(m_image.sz);
    tif.setDimC(m_image.sc);
    tif.setDimT(m_image.st);

    //
    tif.setData((void*)(m_image.p));
    tif.setFileName(const_cast<char*>(filename.c_str()));

    //
    tif.write();

    //
    return 0;
}

int Nail::adjustIntensity(string in, string out)
{
    // test
    nDebug<long>(m_image.sc);

    //
    load(in);

    //
    IntensityRange ori, dst;

    unsigned short *p = (unsigned short*)(m_image.data());

    m_image.adjustIntensity(p, ori, dst);

    //
    save(out);

    //
    return 0;
}

// CLI
DEFINE_string(f, "", "specify which function to call");
DEFINE_string(i, "", "input TIFF file name (.tif)");
DEFINE_string(o, "", "output TIFF file name (.tif)");
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
    gflags::SetUsageMessage("nail -f <func> -i <input> -o <output>");
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    //
    if(FLAGS_i!=""  && FLAGS_i.substr(FLAGS_i.find_last_of(".") + 1) != "tif")
    {
        cout<<"Your input \""<<FLAGS_i<<"\" is not a TIFF image!"<<endl;
        return -1;
    }

    if(FLAGS_o!="" && FLAGS_o.substr(FLAGS_o.find_last_of(".") + 1) != "tif")
    {
        cout<<"Your output \""<<FLAGS_o<<"\" is not a TIFF image!"<<endl;
        return -1;
    }

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
    }
    else
    {
        //
        if(FLAGS_f == "adjustIntensity")
        {
            // src/nail -f adjustIntensity -i ../data/gcampchannel_mip.tif -o ../data/test.tif
            Nail nail;
            nail.adjustIntensity(FLAGS_i, FLAGS_o);
        }
        else
        {

        }
    }

    //
    return 0;
}
