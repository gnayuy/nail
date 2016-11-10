// dataio.h
// read/write .tif, .json, ...
// developed by Yang Yu (gnayuy@gmail.com)

#ifndef __DATAIO_H__
#define __DATAIO_H__

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

//
#include <tiff.h>
#include <tiffio.h>

//
#include <png.h>

//
#ifdef Use_OpenMP
#include <omp.h>
#endif

//
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

#include <cpprest/http_listener.h>              // HTTP server
#include <cpprest/json.h>                       // JSON library
#include <cpprest/uri.h>                        // URI library
#include <cpprest/ws_client.h>                  // WebSocket client
#include <cpprest/containerstream.h>            // Async streams backed by STL containers
#include <cpprest/interopstream.h>              // Bridges for integrating Async streams with STL and WinRT streams
#include <cpprest/rawptrstream.h>               // Async streams backed by raw pointer to memory
#include <cpprest/producerconsumerstream.h>     // Async streams for producer consumer scenarios

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

//using namespace web::http::experimental::listener;          // HTTP server
//using namespace web::experimental::web_sockets::client;     // WebSockets client
//using namespace web::json;                                  // JSON library

// types
typedef enum { UNKNOWNDATATYPE, UCHAR, CHAR, USHORT, SHORT, UINT, INT, ULONG, LONG, FLOAT, DOUBLE } DataType;

typedef enum { UNKNOWNPIXELTYPE, SCALAR, RGB, RGBA, OFFSET, VECTOR,
    POINT, COVARIANTVECTOR, SYMMETRICSECONDRANKTENSOR,
    DIFFUSIONTENSOR3D, COMPLEX, FIXEDARRAY, MATRIX }  IOPixelType;

typedef enum { UNKNOWNFILEFORMAT, TIFFIMAGE, VDBIMAGE, RLEIMAGE } IOFileFormat;

typedef enum { NoCompression, PackBits, JPEG, Deflate, LZW } CompressionType;

// abs
template <class T>
T dm_abs(T x)
{
    return (x<(T)0)?-x:x;
}

// delete 1d pointer
template <class T>
void del1dp(T *&p)
{
    if(p) {delete []p; p=NULL;}
    return;
}

// new 1d pointer
template<class T, class Tidx>
void new1dp(T *&p, Tidx n)
{
    //
    del1dp<T>(p);
    
    //
    try
    {
        p = new T [n];
    }
    catch(...)
    {
        cout<<"Attempt to allocate memory failed!"<<endl;
        del1dp<T>(p);
        return;
    }
    return;
}

//
template <class Tdata, class Tidx>
int convertImageOrder(Tdata *&p, Tidx dimx, Tidx dimy, Tidx dimz, Tidx dimc, bool cxyz2xyzc)
{
    //
    /// convert image order [CXYZ] <-> [XYZC]
    //
    
    //
    if(!p)
    {
        cout<<"Invalid pointer for convertImageOrder."<<endl;
        return -1;
    }
    
    //
    Tidx offset_tif_x = dimc;
    Tidx offset_tif_y = offset_tif_x*dimx;
    Tidx offset_tif_z = offset_tif_y*dimy;
    
    Tidx offset_ilia_y = dimx;
    Tidx offset_ilia_z = offset_ilia_y*dimy;
    Tidx offset_ilia_c = offset_ilia_z*dimz;
    
    Tidx imagesz = dimc*offset_ilia_c;
    
    //
    Tdata *pTmp = NULL;
    new1dp<Tdata, Tidx>(pTmp, imagesz);
    memcpy(pTmp, p, imagesz * sizeof(Tdata) );
    // std::copy(&p[0], &p[imagesz-1], pTmp);
    
    //
    for (Tidx c=0; c<dimc; c++)
    {
        Tidx offset_out_c = c*offset_ilia_c;
        for (Tidx z=0; z<dimz; z++)
        {
            Tidx offset_in_z = z*offset_tif_z;
            Tidx offset_out_z = z*offset_ilia_z + offset_out_c;
            for (Tidx y=0; y<dimy; y++)
            {
                Tidx offset_in_y = y*offset_tif_y + offset_in_z;
                Tidx offset_out_y = y*offset_ilia_y + offset_out_z;
                for (Tidx x=0; x<dimx; x++)
                {
                    Tidx idx_in = offset_in_y + x*offset_tif_x + c;
                    Tidx idx_out = offset_out_y + x;
                    
                    if(cxyz2xyzc)
                    {
                        // CXYZ -> XYZC
                        p[idx_out] = pTmp[idx_in];
                    }
                    else
                    {
                        // XYZC -> CXYZ
                        p[idx_in] = pTmp[idx_out];
                    }
                    
                }// x
            }// y
        }// z
    }// c
    
    // de-alloc
    del1dp<Tdata>(pTmp);
    
    //
    return 0;
}

template <class T>
bool checkFileExtension(T* filename, const T* extension)
{
    if(filename == NULL || extension == NULL)
        return false;
    
    if(strlen(filename) == 0 || strlen(extension) == 0)
        return false;
    
    if(strchr(filename, '.') == NULL || strchr(extension, '.') == NULL)
        return false;
    
    for(unsigned int i = 0; i < strlen(filename); i++)
    {
        if(tolower(filename[strlen(filename) - i - 1]) == tolower(extension[strlen(extension) - i - 1]))
        {
            if(i == strlen(extension) - 1)
                return true;
        } else
            break;
    }
    
    return false;
}

// classes

// metainfo
class MetaInfo
{
public:
    MetaInfo();
    ~MetaInfo();
    
public:
    string name; // name of the specimen
    string unit; // "micrometer"/"nanometer"
    double voxelsize_x, voxelsize_y, voxelsize_z;
    double origin_x, origin_y, origin_z;
    double size_x, size_y, size_z;
    unsigned int dim_x, dim_y, dim_z;
    unsigned int bits_per_voxel;
    unsigned int number_of_channels;
};

// tile
class Tile
{
public:
    Tile();
    ~Tile();
    
public:
    string uuid;
    string octreepath;
    string ch1, ch2, ch3;
    long offTileX, offTileY, offTileZ;
    bool visited;
};

typedef std::vector<Tile> tileListType;

// tiff image
class TiffIO
{
public:
    TiffIO();
    ~TiffIO();
    
public:
    
    // reading
    bool canReadFile(char *fileNameToRead);
    int read();
    
    // writing
    bool canWriteFile(char *fileNameToWrite);
    int write();
    
    //
    void close();
    
    //
    void changeImageOrder(bool io);
    
    // filename
    char* getFileName();
    void setFileName(char* fileName);
    
    // dimensions
    long getDimx();
    long getDimy();
    long getDimz();
    long getDimc();
    long getDimt();
    
    void setDimx(long x);
    void setDimy(long y);
    void setDimz(long z);
    void setDimc(long c);
    void setDimt(long t);
    
    // resolutions
    void setResX(float resolution_x);
    void setResY(float resolution_y);
    void setResZ(float resolution_z);
    
    // datatype
    int getDataType();
    void setDataType(int dt);
    
    // data
    void* getData();
    void setData(void *p);
    
public:
    string inputFileName,outputFileName;
    
    IOFileFormat m_FileFormat;
    DataType m_DataType;
    IOPixelType m_PixelType;
    
    long dimx, dimy, dimz, dimc, dimt; // dimensions
    float resx, resy, resz, resc, rest; // spacing
    
    void *m_Data;
    char* m_FileName;
    
private:
    uint16 config;
    uint16 bitspersample, samplesperpixel;
    uint16 compression, photometric;
    uint32 width, length;
    uint16 orientation;
    uint32 tilewidth, tilelength, rowsperstrip;
    uint16 transferR, transferG, transferB, transferA;
    uint16 colormapR, colormapG, colormapB, colormapA;
    
    bool bICC;
    uint32 lenICC;
    void** dataICC;
    
    bool bINKS, bInkName;
    uint16 ninks;
    const char* inknames;
    int inknameslen;
    
    bool bPG;
    unsigned short m_CurrentPage, m_NumberOfPages;
    
    bool bTiled;
    uint16 fillorder;
    
    uint32 m_NumberOfTiles;
    
private:
    uint32 m_SUBFILETYPE;
    uint16 m_THRESHHOLDING;
    char* m_DOCUMENTNAME;
    char* m_IMAGEDESCRIPTION;
    char* m_MAKE;
    char* m_MODEL;
    uint16 m_MINSAMPLEVALUE;
    uint16 m_MAXSAMPLEVALUE;
    float m_XRESOLUTION;
    float m_YRESOLUTION;
    char* m_PAGENAME;
    float m_XPOSITION;
    float m_YPOSITION;
    uint16 m_RESOLUTIONUNIT;
    char* m_SOFTWARE;
    char* m_DATETIME;
    char* m_ARTIST;
    char* m_HOSTCOMPUTER;
    float* m_WHITEPOINT;
    float* m_PRIMARYCHROMATICITIES;
    uint16 m_HALFTONEHINTS1, m_HALFTONEHINTS2;
    uint16 m_INKSET;
    uint16 m_DOTRANGE1, m_DOTRANGE2;
    char* m_TARGETPRINTER;
    uint16 m_SAMPLEFORMAT;
    float* m_YCBCRCOEFFICIENTS;
    uint16 m_YCBCRSUBSAMPLING1, m_YCBCRSUBSAMPLING2;
    uint16 m_YCBCRPOSITIONING;
    float* m_REFERENCEBLACKWHITE;
    uint16 m_EXTRASAMPLES1;
    uint16* m_EXTRASAMPLES2;
    double m_SMINSAMPLEVALUE;
    double m_SMAXSAMPLEVALUE;
    double m_STONITS;
    
    unsigned int   m_SubFiles;
    unsigned int   m_IgnoredSubFiles;
    
    //
    TIFF* m_TiffImage;
};

// class Timer
class Timer
{
public:
    Timer();
    void start();
    double getEclipseTime() const;

private:
    struct timeval m_startTime;
};

#endif // __DATAIO_H__
