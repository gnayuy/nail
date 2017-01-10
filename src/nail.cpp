// nail.cpp
// neural activity imaging library
// developed by Yang Yu (gnayuy@gmail.com)

#include "nail.h"

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

// class ImageProcess
ImageProcess::ImageProcess()
{
    m_image = new BioMedicalData();
}

ImageProcess::~ImageProcess()
{
    //del1dp<BioMedicalData>(m_image);
}

void ImageProcess::setImage(BioMedicalData *image)
{
    m_image = image;
}

BioMedicalData * ImageProcess::getImage()
{
    return m_image;
}

void ImageProcess::thresholding()
{
    //
    // threshold segmentation using k-means
    //

    //
    if(!m_image->data())
    {
        cout<<"Invalid inputs for thresholding function"<<endl;
        return;
    }

    //
    long pagesz;
    pagesz=m_image->size.size();

    //
    if(m_image->dataType()==USHORT)
    {
        //
        unsigned short *data = (unsigned short *)(m_image->data());

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
            h[hlut.getIndex(data[i])] ++;
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

                float cb = y_abs<float>(float(hlut.lut[i])-mub);
                float cf = y_abs<float>(float(hlut.lut[i])-muf);

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

            if(y_abs<float>(mub - oldmub)<1 && y_abs<float>(muf - oldmuf)<1)  break;
        }

        //
        unsigned short threshold = (mub+muf)/2;
        for(long i=0; i<pagesz; i++)
        {
            if(data[i]>threshold)
                data[i] = hlut.maxv;
            else
                data[i] = hlut.minv;
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

void ImageProcess::adjustIntensity(unsigned short *&p, IntensityRange ori, IntensityRange dst)
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
    for(k=0; k<m_image->size.getZ(); k++)
    {
        offz = k*m_image->size.getX()*m_image->size.getY();
        for(j=0; j<m_image->size.getY(); j++)
        {
            offy = offz + j*m_image->size.getX();
            for(i=0; i<m_image->size.getX(); i++)
            {
                idx = offy + i;

                p[idx] = lut[ p[idx] ];
            }
        }
    }
}

void ImageProcess::gammaCorrection(double gamma, CodeType ct)
{
    if(gamma<=0)
    {
        cout<<"gamma is less than 0\n";
        return;
    }

    if(m_image->data())
    {
        switch (ct) {
        case ENCODE:

            if(m_image->dataType()==UCHAR)
            {
                unsigned char *p = (unsigned char *) (m_image->data());

                long imgsz = m_image->size.size();

                for(long i=0; i<imgsz; i++)
                {
                    p[i] = pow(double(p[i])/255.0, 1/gamma) * 255.0;
                }
            }
            else
            {

            }

            break;

        case DECODE:

            if(m_image->dataType()==UCHAR)
            {
                unsigned char *p = (unsigned char *) (m_image->data());

                long imgsz = m_image->size.size();

                for(long i=0; i<imgsz; i++)
                {
                    p[i] = pow(double(p[i])/255.0, gamma) * 255.0;
                }
            }
            else
            {

            }

            break;

        default:
            break;
        }
    }
    else
    {
        cout<<"Null image\n";
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
    BioMedicalDataIO bmdata;

    if(bmdata.readData(filename)!=0)
    {
        cout<<"Fail to read data!"<<endl;
        return -1;
    }

    //
    process.setImage(bmdata.data());

    //
    return 0;
}

int Nail::save(string filename)
{
    //
    if(!process.getImage()->data())
    {
        cout<<"Empty image!"<<endl;
        return -1;
    }

    //
    BioMedicalDataIO bmdata;

    //
    bmdata.setData(process.getImage());

    //
    return bmdata.writeData(filename);
}

int Nail::adjustIntensity(string in, string out)
{
    //
    load(in);

    //
    IntensityRange ori, dst;

    unsigned short *p = (unsigned short*)(process.getImage()->data());

    process.adjustIntensity(p, ori, dst);

    //
    save(out);

    //
    return 0;
}

int Nail::imageReadWrite(string in, string out)
{
    //
    load(in);

    //
    save(out);

    //
    return 0;
}

int Nail::gammaFilter(string in, string out, double gamma, CodeType ct)
{
    //
    load(in);

    //
    process.gammaCorrection(gamma, ct);

    //
    save(out);

    //
    return 0;
}

