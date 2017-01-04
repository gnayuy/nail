// image.hpp
// define image related template functions
// developed by Yang Yu (gnayuy@gmail.com)

#ifndef IMAGE_HPP
#define IMAGE_HPP
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

// histogram
template<class Tdata, class Tidx>
class HistogramLUT
{
public:
    HistogramLUT() { bins=0; index=0; lut=NULL;}
    ~HistogramLUT(){}

public:
    void initLUT(Tdata *p, Tidx sz, Tidx nbins);
    Tidx getIndex(Tdata val);

public:
    Tidx bins, index;
    Tdata *lut; // look up table
    float minv, maxv;
};

template<class Tdata, class Tidx>
void HistogramLUT<Tdata, Tidx> :: initLUT(Tdata *p, Tidx sz, Tidx nbins)
{
    //
    if(!p || nbins<=0)
    {
        cout<<"Invalid inputs"<<endl;
        return;
    }

    //
    bins = nbins;
    new1dp<Tdata, Tidx>(lut, bins);

    // histogram bin #i [minv+i*stepv, minv+(i+1)*stepv)
    minv=INF;
    maxv=-INF;
    float stepv=0;
    for(Tidx i=0; i<sz; i++)
    {
        if(minv>p[i]) minv=p[i];
        if(maxv<p[i]) maxv=p[i];
    }
    stepv = (maxv - minv)/(float)bins;

    for(Tidx i=0; i<bins; i++)
    {
        lut[i] = minv + i*stepv; // only left values
    }

    return;
}

template<class Tdata, class Tidx>
Tidx HistogramLUT<Tdata, Tidx> :: getIndex(Tdata val)
{
    //
    Tidx min=0;
    Tidx max=bins-1;

    // binary search
    bool found = false;
    index=0;
    while(min<max && !found)
    {
        Tidx mid=(min+max)/2;

        if(val == lut[mid] || (val > lut[mid] && y_abs<Tdata>(val - lut[mid])<EPS) ) // =
        {
            found = true;
            index = mid;
        }
        else if(val < lut[mid]) // <
        {
            max = mid - 1;
        }
        else // >
        {
            if(mid+1>=max)
            {
                found = true;
                index = mid;
            }
            else
            {
                min = mid + 1;
            }
        }
    }

    //
    return index;
}



#endif // IMAGE_HPP
