// nail.cpp
// neural activity imaging library
// developed by Yang Yu (gnayuy@gmail.com)

#include "nail.h"

//
bool doubleVarCompare(double x, double y)
{
    if(y_abs<double>(x-y)<EPS)
        return true;
    else
        return false;
}

bool floatVarCompare(float x, float y)
{
    if(y_abs<float>(x-y)<EPS)
        return true;
    else
        return false;
}

// class ColorLUT
ColorRGBLUT::ColorRGBLUT()
{
    // init
    colors.push_back(Tuplet<unsigned char>(129,251,84));
    colors.push_back(Tuplet<unsigned char>(223,225,191));
    colors.push_back(Tuplet<unsigned char>(86,71,135));
    colors.push_back(Tuplet<unsigned char>(136,234,123));
    colors.push_back(Tuplet<unsigned char>(161,84,55));
    colors.push_back(Tuplet<unsigned char>(91,121,227));
    colors.push_back(Tuplet<unsigned char>(142,153,63));
    colors.push_back(Tuplet<unsigned char>(67,13,166));
    colors.push_back(Tuplet<unsigned char>(134,41,255));
    colors.push_back(Tuplet<unsigned char>(112,143,5));
    colors.push_back(Tuplet<unsigned char>(9,205,72));
    colors.push_back(Tuplet<unsigned char>(211,6,181));
    colors.push_back(Tuplet<unsigned char>(19,178,172));
    colors.push_back(Tuplet<unsigned char>(49,70,54));
    colors.push_back(Tuplet<unsigned char>(246,22,116));
    colors.push_back(Tuplet<unsigned char>(124,65,113));
    colors.push_back(Tuplet<unsigned char>(112,1,51));
    colors.push_back(Tuplet<unsigned char>(192,13,51));
    colors.push_back(Tuplet<unsigned char>(94,230,166));
    colors.push_back(Tuplet<unsigned char>(195,168,219));
    colors.push_back(Tuplet<unsigned char>(96,191,103));
    colors.push_back(Tuplet<unsigned char>(37,49,98));
    colors.push_back(Tuplet<unsigned char>(79,9,159));
    colors.push_back(Tuplet<unsigned char>(169,11,128));
    colors.push_back(Tuplet<unsigned char>(71,165,172));
    colors.push_back(Tuplet<unsigned char>(235,36,146));
    colors.push_back(Tuplet<unsigned char>(197,133,140));
    colors.push_back(Tuplet<unsigned char>(171,82,156));
    colors.push_back(Tuplet<unsigned char>(249,186,93));
    colors.push_back(Tuplet<unsigned char>(126,140,113));
    colors.push_back(Tuplet<unsigned char>(94,51,81));
    colors.push_back(Tuplet<unsigned char>(123,100,239));
    colors.push_back(Tuplet<unsigned char>(66,81,247));
    colors.push_back(Tuplet<unsigned char>(69,13,204));
    colors.push_back(Tuplet<unsigned char>(187,28,148));
    colors.push_back(Tuplet<unsigned char>(125,212,97));
    colors.push_back(Tuplet<unsigned char>(56,191,13));
    colors.push_back(Tuplet<unsigned char>(128,213,136));
    colors.push_back(Tuplet<unsigned char>(182,147,139));
    colors.push_back(Tuplet<unsigned char>(2,80,86));
    colors.push_back(Tuplet<unsigned char>(66,221,233));
    colors.push_back(Tuplet<unsigned char>(225,234,97));
    colors.push_back(Tuplet<unsigned char>(72,20,133));
    colors.push_back(Tuplet<unsigned char>(70,200,209));
    colors.push_back(Tuplet<unsigned char>(229,67,3));
    colors.push_back(Tuplet<unsigned char>(235,53,102));
    colors.push_back(Tuplet<unsigned char>(54,225,56));
    colors.push_back(Tuplet<unsigned char>(41,94,73));
    colors.push_back(Tuplet<unsigned char>(83,63,37));
    colors.push_back(Tuplet<unsigned char>(13,76,50));
    colors.push_back(Tuplet<unsigned char>(127,87,206));
    colors.push_back(Tuplet<unsigned char>(174,19,76));
    colors.push_back(Tuplet<unsigned char>(179,153,90));
    colors.push_back(Tuplet<unsigned char>(124,1,234));
    colors.push_back(Tuplet<unsigned char>(136,20,176));
    colors.push_back(Tuplet<unsigned char>(53,45,215));
    colors.push_back(Tuplet<unsigned char>(48,8,153));
    colors.push_back(Tuplet<unsigned char>(210,173,248));
    colors.push_back(Tuplet<unsigned char>(3,201,15));
    colors.push_back(Tuplet<unsigned char>(122,104,114));
    colors.push_back(Tuplet<unsigned char>(194,114,52));
    colors.push_back(Tuplet<unsigned char>(105,58,139));
    colors.push_back(Tuplet<unsigned char>(162,221,53));
    colors.push_back(Tuplet<unsigned char>(75,227,13));
    colors.push_back(Tuplet<unsigned char>(190,229,255));
    colors.push_back(Tuplet<unsigned char>(118,244,204));
    colors.push_back(Tuplet<unsigned char>(126,200,143));
    colors.push_back(Tuplet<unsigned char>(11,198,156));
}

ColorRGBLUT::~ColorRGBLUT()
{

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
    if(m_image->dataType()==UCHAR)
    {
        //
        unsigned char *data = (unsigned char *)(m_image->data());

        //
        long BINS = 256; // histogram bins

        long *h=NULL, *hc=NULL;
        new1dp<long, long>(h, BINS);
        new1dp<long, long>(hc, BINS);

        memset(h, 0, sizeof(long)*BINS);

        // histogram
        HistogramLUT<unsigned char, long> hlut;
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
    else if(m_image->dataType()==USHORT)
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
        cout<<"Unsupported data type in thresholding"<<endl;
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

void ImageProcess::getImageFromLabels(IVec1D *v, bool exclude)
{
    if(m_image->data())
    {
        if(v->items.size()>0)
        {
            if(m_image->dataType()==UCHAR)
            {
                unsigned char *p = (unsigned char *) (m_image->data());

                long imgsz = m_image->size.size();

                if(exclude==true)
                {
                    for(long i=0; i<imgsz; i++)
                    {
                        bool found = false;
                        for(size_t j=0; j<v->items.size(); j++)
                        {
                            if(p[i]==v->items[j])
                            {
                                found = true;
                                break;
                            }
                        }

                        if(found==true)
                        {
                            p[i] = 0;
                        }
                        else
                        {
                            if(p[i] > 0)
                                p[i] = 1;
                        }
                    }
                }
                else
                {
                    for(long i=0; i<imgsz; i++)
                    {
                        bool found = false;
                        for(size_t j=0; j<v->items.size(); j++)
                        {
                            if(p[i]==v->items[j])
                            {
                                found = true;
                                break;
                            }
                        }

                        if(found==true)
                        {
                            p[i] = 1;
                        }
                        else
                        {
                            p[i] = 0;
                        }
                    }
                }
            }
            else
            {

            }
        }
    }
}

void ImageProcess::im2bw(double threshold, double v)
{
    if(m_image->data())
    {
        if(m_image->dataType()==UCHAR)
        {
            unsigned char *p = (unsigned char *) (m_image->data());

            long imgsz = m_image->size.size();

            for(long i=0; i<imgsz; i++)
            {
                if(double(p[i])>=threshold)
                    p[i] = (unsigned char)v;
                else
                    p[i] = 0;
            }
        }
        else
        {
            cout<<"unsupported data type\n";
            return;
        }
    }
    else
    {
        cout<<"Null image\n";
        return;
    }
}

void ImageProcess::add(BioMedicalData *image)
{

}

void ImageProcess::multiply(double v)
{

}

void ImageProcess::createLabelImage(BioMedicalData *image, double threshold, double label)
{
    if(m_image->data() && image->data())
    {
        if(m_image->dataType()==UCHAR && image->dataType()==UCHAR)
        {
            unsigned char *p1 = (unsigned char *) (m_image->data());
            unsigned char *p2 = (unsigned char *) (image->data());

            long size1 = m_image->size.size();
            long size2 = image->size.size();

            if(size1!=size2)
            {
                cout<<"image size does not match\n";
                return;
            }

            // assume m_image is binary image
            // im2bw image and assign label to m_image
            for(long i=0; i<size1; i++)
            {
                if(double(p2[i])>=threshold)
                {
                    if(p1[i]>0)
                    {
                        p1[i] = 1;
                    }
                    else
                    {
                        p1[i] = (unsigned char)label;
                    }
                }
            }
        }
        else
        {
            cout<<"unsupported data type\n";
            return;
        }

    }
    else
    {
        cout<<"Null image\n";
        return;
    }
}

LVec1D * ImageProcess::countVoxels(IVec1D *labels)
{
    //
    LVec1D *voxels = new LVec1D();

    //
    long szLabels = labels->items.size();

    //
    if(szLabels>0)
    {
        // init
        voxels->zeros(szLabels);

        //
        if(m_image->data())
        {
            //
            if(m_image->dataType()==UCHAR)
            {
                unsigned char *p = (unsigned char *) (m_image->data());

                long size = m_image->size.size();

                //
                for(long i=0; i<size; i++)
                {
                    for(long j=0; j<szLabels; j++)
                    {
                        if(double(p[i]) == labels->items[j])
                        {
                            voxels->items[j]++;
                        }
                    }
                }
            }
            else
            {
                cout<<"unsupported data type\n";
            }
        }
        else
        {
            cout<<"Null image\n";
        }
    }

    //
    return voxels;
}

LVec1D * ImageProcess::countVoxels(BioMedicalData *mask, int nLabels)
{
    //
    LVec1D *voxels = new LVec1D();

    //
    if(nLabels>0)
    {
        // init
        voxels->zeros(nLabels);

        //
        if(m_image->data() && mask->data())
        {
            //
            if(m_image->dataType()==UCHAR && mask->dataType()==UCHAR)
            {
                unsigned char *p = (unsigned char *) (m_image->data());
                unsigned char *pMask = (unsigned char *) (mask->data());

                long size = m_image->size.size();

                //
                for(long i=0; i<size; i++)
                {
                    for(long j=0; j<nLabels; j++)
                    {
                        if(pMask[i]==j+1 && p[i]>0)
                        {
                            voxels->items[j]++;
                        }
                    }
                }
            }
            else
            {
                cout<<"unsupported data type\n";
            }
        }
        else
        {
            cout<<"Null data\n";
        }
    }

    //
    return voxels;
}

double ImageProcess::imageCompare(BioMedicalData *reference, SimilarityType similarity)
{
    double metric = -1;

    if(m_image->data() && reference->data())
    {
        //
        if(m_image->dataType()==UCHAR && reference->dataType()==UCHAR)
        {
            unsigned char *p = (unsigned char *)(m_image->data());
            unsigned char *pRef = (unsigned char *)(reference->data());

            long size = m_image->size.size();
            long sizeRef = reference->size.size();

            if(size!=sizeRef)
            {
                cout<<"Sizes do not match.\n";
                return metric;
            }

            //
            if(similarity==CC)
            {
                computeNCC<unsigned char, unsigned char, long>(p, pRef, size, metric);
            }
            else if(similarity==MI)
            {
                computeNMI<unsigned char, unsigned char, long>(p, pRef, size, 256, metric);
            }
            else
            {
                cout<<"Unsupported similarity.\n";
            }
        }
        else
        {
            cout<<"Unsupported data type.\n";
        }
    }
    else
    {
        cout<<"Null data.\n";
    }

    //
    return metric;
}

void ImageProcess::sampling(BioMedicalData *image, double srx, double sry, double srz, int interptype)
{
    //
    long sx = image->size.getX();
    long sy = image->size.getY();
    long sz = image->size.getZ();

    long dx = sx*srx;
    long dy = sy*sry;
    long dz = sz*srz;

    long dc = image->size.getC();
    long dt = image->size.getT();

    //
    if(image->data())
    {
        if(image->dataType()==UCHAR)
        {
            m_image->size.setXYZCT(dx, dy, dz, dc, dt);
            m_image->setDataType(UCHAR);
            m_image->zeros();

            unsigned char *p1 = (unsigned char *) (m_image->data());
            unsigned char *p2 = (unsigned char *) (image->data());

            isampler<unsigned char, long>(p1, p2, dx, dy, dz, sx, sy, sz, srx, sry, srz, (InterpolationType)interptype);
        }
        else
        {
            // other datatype
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

int Nail::imageReadWrite(string in, string out, double vx, double vy, double vz)
{
    //
    load(in);

    if(vx>0 || vy>0 || vz>0)
    {
        process.getImage()->setSpaceUnit(UnitUM); // in the microscope context

        if(vx>0)
        {
            process.getImage()->spacing.setX(vx);
        }

        if(vy>0)
        {
            process.getImage()->spacing.setY(vy);
        }

        if(vz>0)
        {
            process.getImage()->spacing.setZ(vz);
        }
    }

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

int Nail::genMaskImageFromLabels(string in, string out, string s, bool exclude)
{
    //
    load(in);

    //
    IVec1D *v = new IVec1D();
    v->str2num(s);

    process.getImageFromLabels(v, exclude);

    //
    save(out);

    //
    return 0;
}

int Nail::genLabelImage(string in, string out)
{
    //
    SVec1D *v = new SVec1D();
    v->getFileList(in);

    //
    long size = v->items.size();

    //
    for(long i=0; i<size; i++)
    {
        string fn = v->items[i];

        cout<<"processing image "<<i<<" "<<fn<<endl;

        //
        BioMedicalDataIO bmdata;

        if(bmdata.readData(fn)!=0)
        {
            cout<<"Fail to read data!"<<endl;
            return -1;
        }

        cout<<"test ... "<<bmdata.data()->size.getX()<<" "<<bmdata.data()->size.getY()<<" "<<bmdata.data()->size.getZ()<<endl;

        if(i==0)
        {
            process.setImage(bmdata.data());

            cout<<"test ... 1 "<<process.m_image->size.getX()<<" "<<process.m_image->size.getY()<<" "<<process.m_image->size.getZ()<<endl;

            process.im2bw(1, 2);

            cout<<"test ... 2 "<<process.m_image->size.getX()<<" "<<process.m_image->size.getY()<<" "<<process.m_image->size.getZ()<<endl;
        }
        else
        {
            process.createLabelImage(bmdata.data(), 1, i+2);
        }
    }

    //
    save(out);

    //
    return 0;
}

int Nail::countVoxels(string in, string out, string s, bool withMask, int nLabels)
{
    //
    load(in);

    //
    LVec1D *voxels;

    if(withMask==true)
    {
        //
        BioMedicalDataIO bmdata;

        if(bmdata.readData(s)!=0)
        {
            cout<<"Fail to read data!"<<endl;
            return -1;
        }

        //
        voxels = process.countVoxels(bmdata.data(), nLabels);
    }
    else
    {
        //
        IVec1D *v = new IVec1D();
        v->str2num(s);

        //
        voxels = process.countVoxels(v);
    }

    //
    ofstream fout(const_cast<char*>(out.c_str()));
    if(fout.is_open())
    {
        //
        for(long i=0; i<voxels->items.size(); i++)
        {
            fout << i+1 << ": " << voxels->items[i] << endl;
        }
    }
    else
    {
        cout << "Fail to open the output file." << endl;
        return -1;
    }
    fout.close();

    //
    return 0;
}

int Nail::binarize(string in, string out)
{
    //
    load(in);

    //
    process.thresholding();

    //
    save(out);

    //
    return 0;
}

int Nail::imageCompare(string in, string out, string ref, SimilarityType similarity)
{
    //
    load(in);

    //
    BioMedicalDataIO bmdata;

    if(bmdata.readData(ref)!=0)
    {
        cout<<"Fail to read data!"<<endl;
        return -1;
    }

    //
    double metricVal = process.imageCompare(bmdata.data(), similarity);

    //
    ofstream fout(const_cast<char*>(out.c_str()));
    if(fout.is_open())
    {
        //
        fout << metricVal << endl;
    }
    else
    {
        cout << "Fail to open the output file." << endl;
        return -1;
    }
    fout.close();

    //
    return 0;
}

int Nail::constructStack(string in, string out, long z, double k, double b)
{
    // load slices
    BioMedicalDataIO slices;

    if(slices.readData(in)!=0)
    {
        cout<<"Fail to read data!"<<endl;
        return -1;
    }

    //
    long x = slices.data()->size.getX();
    long y = slices.data()->size.getY();
    long n = slices.data()->size.getZ();

    //
    BioMedicalData *stack = new BioMedicalData();
    stack->setDataType(slices.data()->dataType());
    stack->size.setXYZCT(x,y,z,1,1);
    stack->zeros();

    cout<<"datatype ... "<<UCHAR<<endl;

    //
    if(slices.data()->dataType()==UCHAR)
    {
        unsigned char *p = (unsigned char *)(slices.data()->data());
        unsigned char *pStack = (unsigned char *)(stack->data());

        reconstructStack<unsigned char, long>(p, n, pStack, x, y, z, k, b);
    }
    else if(slices.data()->dataType()==CHAR)
    {
        char *p = (char *)(slices.data()->data());
        char *pStack = (char *)(stack->data());

        reconstructStack<char, long>(p, n, pStack, x, y, z, k, b);
    }
    else if(slices.data()->dataType()==USHORT)
    {
        unsigned short *p = (unsigned short *)(slices.data()->data());
        unsigned short *pStack = (unsigned short *)(stack->data());

        reconstructStack<unsigned short, long>(p, n, pStack, x, y, z, k, b);
    }
    else if(slices.data()->dataType()==SHORT)
    {
        short *p = (short *)(slices.data()->data());
        short *pStack = (short *)(stack->data());

        reconstructStack<short, long>(p, n, pStack, x, y, z, k, b);
    }
    else
    {
        cout<<"Unsupported datatype "<<slices.data()->dataType()<<endl;
        return -1;
    }

    //
    process.setImage(stack);

    //
    save(out);

    //
    return 0;
}

int Nail::findOptimalPath(string in, string out, long sx, long sy, double minY0, double maxY0, double minY, double maxY)
{
    // load raw data (2D array)
    BioMedicalDataIO dpm;
    dpm.data()->size.setX(sx);
    dpm.data()->size.setY(sy);
    dpm.data()->setDataType(FLOAT);

    if(dpm.readData(in)!=0)
    {
        cout<<"Fail to read data!"<<endl;
        return -1;
    }

    // calculate the slope and intercept of the line equation (optimal path of dp matrix)
    float k, b;
    getOptimalPath<float, long>((float*)(dpm.data()), sx, minY0, maxY0, minY, maxY, k, b);

    //
    cout<<"k "<<k<<" \nb "<<b<<endl;

    for(long i=0; i<sx; i++)
    {
        cout<<i<<" "<<k*i+b<<endl;
    }

    //
    return 0;
}

int Nail::recenterImage(string in, string out, long x, long y, long z, long c)
{
    //
    load(in);

    //
    BioMedicalDataIO dpm;
    dpm.data()->size.setXYZCT(x,y,z,c,1);
    dpm.data()->setDataType(process.getImage()->dataType());
    dpm.data()->zeros();

    //
    cout<<process.getImage()->size.getC()<<endl;

    //
    if(process.getImage()->dataType()==UCHAR)
    {
        unsigned char *pIn = (unsigned char *)(process.getImage()->data());
        unsigned char *pOut = (unsigned char *)(dpm.data()->data());

        recenter<unsigned char, long>(pOut, dpm.data()->size, pIn, process.getImage()->size);
    }
    else
    {
        // other data types
    }

    //
    process.setImage(dpm.data());

    //
    save(out);

    //
    return 0;
}

int Nail::seq2stack(string in, string out, long zpos, long zstep, long z)
{
    //  in: a text file of image paths
    // out: a image stack

    // read file names
    string fn;
    vector<string> imList;
    ifstream fin(const_cast<char*>(in.c_str()));

    struct stat buffer;
    long nFiles = 0;
    if(fin.is_open())
    {
        while( getline(fin, fn) )
        {
            if(stat (fn.c_str(), &buffer) == 0)
            {
                imList.push_back(fn);
                nFiles++;
            }
            cout<<fn<<" "<<nFiles<<endl;
        }
        fin.close();
    }
    else
    {
        cout << "Fail to open the input image list." << endl;
        return -1;
    }

    //
    if(z<nFiles)
    {
        z = nFiles;
    }
    else
    {
        if(zpos + (nFiles - 1)*zstep > z)
        {
            cout << "z dimension is invalid." << endl;
            return -1;
        }
    }

    //
    long x, y, offsets, zoffsets;
    BioMedicalData *stack = new BioMedicalData();

    //
    for(int i=0; i<imList.size(); i++)
    {
        fn = imList[i];

        BioMedicalDataIO bmd;
        if(bmd.readData(fn)!=0)
        {
            cout<<"Fail to read data!"<<endl;
            return -1;
        }

        if(i==0)
        {
            x = bmd.data()->size.getX();
            y = bmd.data()->size.getY();
            offsets = x*y*bmd.data()->size.getZ(); // pick the first slice in green channel, hard coded

            stack->setDataType(bmd.data()->dataType());
            stack->size.setXYZCT(x,y,z,1,1);
            stack->zeros();
        }

        zoffsets = (i*zstep + zpos)*x*y;

        // copy data
        if(bmd.data()->dataType()==UCHAR)
        {
            //unsigned char *p = (unsigned char *)(bmd.data()->data());
            //unsigned char *pStack = (unsigned char *)(stack->data());

            //pStack[zoffsets + idx] = p[offsets + idx];
        }
        else if(bmd.data()->dataType()==CHAR)
        {
            //char *p = (char *)(bmd.data()->data());
            //char *pStack = (char *)(stack->data());

            //pStack[zoffsets + idx] = p[offsets + idx];
        }
        else if(bmd.data()->dataType()==USHORT)
        {
            //unsigned short *p = (unsigned short *)(bmd.data()->data());
            //unsigned short *pStack = (unsigned short *)(stack->data());

            //pStack[zoffsets + idx] = p[offsets + idx];
        }
        else if(bmd.data()->dataType()==SHORT)
        {
            short *p = (short *)(bmd.data()->data());
            short *pStack = (short *)(stack->data());

            for(int m=0; m<y; m++)
            {
                long yoff = m*x;
                for(int n=0; n<x; n++)
                {
                    long idx = yoff + n;

                    pStack[zoffsets + idx] = p[offsets + idx];
                }
            }
        }
        else
        {
            cout<<"Unsupported datatype "<<bmd.data()->dataType()<<endl;
            return -1;
        }
    }

    //
    process.setImage(stack);

    //
    save(out);

    //
    return 0;
}

int Nail::convert2byte(string in, string out)
{
    //
    load(in);

    //
    BioMedicalDataIO dpm;
    dpm.data()->size.setSize(process.getImage()->size);
    dpm.data()->setDataType(UCHAR);
    dpm.data()->zeros();

    cout<<in<<endl;
    cout<<"process.getImage()->dataType() "<<process.getImage()->dataType()<<" "<<SHORT<<endl;

    //
    if(process.getImage()->dataType()==SHORT)
    {
        short *pIn = (short *)(process.getImage()->data());
        unsigned char *pOut = (unsigned char *)(dpm.data()->data());

        long sz = process.getImage()->size.getZ();
        long sy = process.getImage()->size.getY();
        long sx = process.getImage()->size.getX();

        long size = process.getImage()->size.size();

        short maxVal=0;
        short minVal=65535;
        for(int i=0; i<size; i++)
        {
            short val = pIn[i];

            if(maxVal<val)
                maxVal = val;
            if(minVal>val)
                minVal = val;
        }

        short dist = maxVal - minVal;

        if(dist==0)
        {
            cout<<"NULL image."<<endl;
            return -1;
        }

        double factor = 255.0/(double)(dist);

        cout<<"test ... "<<sx<<" "<<sy<<" "<<sz<<" "<<dist<<" "<<factor<<endl;

        for(int z=0; z<sz; z++)
        {
            long ofz = z*sx*sy;
            for(int y=0; y<sy; y++)
            {
                long ofy = ofz + y*sx;
                for(int x=0; x<sx; x++)
                {
                    long idx = ofy + x;

                    pOut[idx] = factor*(pIn[idx] - minVal);
                }
            }
        }
    }
    else if(process.getImage()->dataType()==FLOAT)
    {
        float *pIn = (float *)(process.getImage()->data());
        unsigned char *pOut = (unsigned char *)(dpm.data()->data());

        for(int i=0; i<process.getImage()->size.size(); i++)
        {
            pOut[i] = pIn[i];
        }
    }
    else
    {
        // other data types
    }

    //
    process.setImage(dpm.data());

    //
    save(out);

    //
    return 0;
}

int Nail::imageSampling(string in, string out, double srx, double sry, double srz, int interptype)
{
    //
    BioMedicalDataIO bmdata;

    if(bmdata.readData(in)!=0)
    {
        cout<<"Fail to read data!"<<endl;
        return -1;
    }

    //
    process.sampling(bmdata.data(), srx, sry, srz, interptype);

    //
    save(out);

    //
    return 0;
}

int Nail::imageTranslate(string in, string out, long x, long y, long z, long sx, long sy, long sz)
{
    // input image
    BioMedicalDataIO bmdata;

    if(bmdata.readData(in)!=0)
    {
        cout<<"Fail to read data!"<<endl;
        return -1;
    }

    long osx = bmdata.data()->size.getX();
    long osy = bmdata.data()->size.getY();
    long osz = bmdata.data()->size.getZ();
    long osc = bmdata.data()->size.getC();

    // translated image
    BioMedicalDataIO translated;
    translated.data()->size.setSize(process.getImage()->size);
    translated.data()->setDataType(UCHAR);
    translated.data()->zeros();

    //


    //
    save(out);

    //
    return 0;
}

int Nail::convertIndex2Color(string in, string out)
{
    // input indexed image
    load(in);

    long sx = process.getImage()->size.getX();
    long sy = process.getImage()->size.getY();
    long sz = process.getImage()->size.getZ();
    long sc = 3;

    // RGB image
    BioMedicalDataIO colorImage;
    colorImage.data()->size.setXYZCT(sx, sy, sz, sc, 1);
    colorImage.data()->setDataType(UCHAR);
    colorImage.data()->zeros();

    //
    ColorRGBLUT colorlut;

    long pagesz = sx*sy*sz;
    long pagesz2 = 2*pagesz;

    if(process.getImage()->dataType()==UCHAR)
    {
        unsigned char *pIn = (unsigned char *)(process.getImage()->data());
        unsigned char *pOut = (unsigned char *)(colorImage.data()->data());

        for(long i=0; i<pagesz; i++)
        {
            if(pIn[i]>1)
            {
                long idx = pIn[i];

                pOut[i] = colorlut.colors[idx].getX();
                pOut[i+pagesz] = colorlut.colors[idx].getY();
                pOut[i+pagesz2] = colorlut.colors[idx].getZ();

            }
        }
    }
    else
    {
        // other data types
    }

    //
    process.setImage(colorImage.data());
    save(out);

    //
    return 0;
}

int Nail::intensityRescale(string in, string out, double min, double max)
{
    //
    if(max<=min)
    {
        cout<<"Invalid inputs for intensityRescale"<<endl;
        return -1;
    }

    // input indexed image
    load(in);

    long sx = process.getImage()->size.getX();
    long sy = process.getImage()->size.getY();
    long sz = process.getImage()->size.getZ();

    long volumesz = sx*sy*sz;
    double dist = max - min;

    //
    if(process.getImage()->dataType()==UCHAR)
    {
        unsigned char *p = (unsigned char *)(process.getImage()->data());

        for(long i=0; i<volumesz; i++)
        {
            if(p[i]>max)
            {
                p[i] = 255;
            }
            else
            {
                p[i] = 255*(p[i] - min) / dist;
            }
        }
    }
    else
    {
        // other data types
    }

    //
    save(out);

    //
    return 0;
}
