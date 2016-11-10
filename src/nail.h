// datamanagement.h
// manage mouse light data with dvid
// developed by Yang Yu (gnayuy@gmail.com)

#ifndef __DATAMANAGEMENT_H__
#define __DATAMANAGEMENT_H__

#include "dataio.h"
#include <gflags/gflags.h>
#include "png/lodepng.h"

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
class IndexBuffer
{
public:
    IndexBuffer(long otx, long oty, long otz, long ocx, long ocy, long ocz);
    ~IndexBuffer();

public:
    long offTileX, offTileY, offTileZ;
    long offChunkX, offChunkY, offChunkZ;
};

typedef std::vector<IndexBuffer> IndexBufferType;

//
class Block
{
public:
    Block();
    ~Block();

public:
    long sx, sy, sz; // block size
    long ox, oy, oz; // block offset in database
    long box, boy, boz; // block offset in buffer
};

typedef std::vector<Block> BlockList;

//
class DataManager
{
public:
    DataManager();
    ~DataManager();

public:
    int upstreaming(http_client client, uri_builder builder, unsigned char *buffer, long sx, long sy, long sz, long bufSizeX, long bufSizeY, long bufSizeZ);
    int loadTile(unsigned char *&p, string ch1, string ch2, long bufSizeX, long bufSizeY, long bufSizeZ);
    int findNode(tileListType tiles, long xoff, long yoff, long zoff);
    void computeOffset(tileListType &tiles);
    int saveTile(string outFileName, long sx, long sy, long sz, long sc, float vsx, float vsy, float vsz, int dataType);

    pplx::task<void> httpPostAsync(http_client client, uri_builder builder, concurrency::streams::istream isbuf, utility::size64_t size);
    int putData(tileListType tiles, utility::string_t server, utility::string_t uuid, utility::string_t dataName);
    int putBufferData(tileListType tiles, utility::string_t server, utility::string_t uuid, utility::string_t dataName, long bufNumber);

    pplx::task<size_t> httpGetAsync(http_client client, uri_builder builder, rawptr_buffer<unsigned char> rawBuf);
    int getData(utility::string_t server, utility::string_t uuid, utility::string_t dataName, long xoff, long yoff, long zoff, long sx, long sy, long sz, long sc, float vsx, float vsy, float vsz, string outFileName);

    void clearData();

public:
    void setBufferLUT(long tilesX, long tilesY, long tilesZ, long blocksX, long blocksY, long blocksZ, long chunksX, long chunksY, long chunksZ, int branch);

public:
    unsigned char *m_Data;
    IndexBufferType bufLUT;
};

//
// octree:
// z=0 1 2  z=1  5 6
//     3 4       7 8
//
//

#endif // __DATAMANAGEMENT_H__
