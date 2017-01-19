// image.hpp
// define image related template functions
// developed by Yang Yu (gnayuy@gmail.com)

#ifndef __IMAGE_HXX__
#define __IMAGE_HXX__

#include "image.h"

// histogram
template<class Tdata, class Tidx>
HistogramLUT<Tdata, Tidx> :: HistogramLUT()
{
    bins=0; index=0; lut=NULL;
}

template<class Tdata, class Tidx>
HistogramLUT<Tdata, Tidx> :: ~HistogramLUT()
{
}

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
    double stepv=0;
    for(Tidx i=0; i<sz; i++)
    {
        if(minv>p[i]) minv=p[i];
        if(maxv<p[i]) maxv=p[i];
    }
    stepv = (maxv - minv)/(double)bins;

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

/// similarity measures

// normalized mutual information
template <class Tsrc, class Tref, class Tidx>
int computeNMI(Tsrc *src, Tref *ref, Tidx sz, Tidx nbin, double& nmi)
{
    //
    // compute the normalized mutual information [0,1]
    //

    if( !src || !ref || sz<1 || nbin<1)
    {
        cout<<"Invalid inputs for computeNMI func."<<endl;
        return -1;
    }

    // assuming inputs with the same size and their intensities range [0,1]
    Tidx szHist = nbin*nbin, i, j;
    Tidx *hist = NULL;
    new1dp<Tidx, Tidx>(hist, szHist);
    memset(hist, 0, sizeof(Tidx) * szHist);

    // Marginal and joint entropies
    double srHist = 0, sHist = 0, rHist = 0, ps, pr, p, maxvsrc = 0, maxvref = 0;
    Tidx idx;

#pragma acc region copyin(src[0:sz-1],ref[0:sz-1]), copy(hist[0:szHist-1])
    {
        for (i = 0; i < sz; i++)
        {
            if(src[i]>maxvsrc) maxvsrc = src[i];
            if(ref[i]>maxvref) maxvref = ref[i];
        }

        for (i = 0; i < sz; i++)
        {
            idx = Tidx( double(src[i]) / (maxvsrc + EPSILON) * (nbin - 1) + 0.5f) + Tidx( double(ref[i]) / (maxvref + EPSILON) * (nbin - 1) + 0.5f)*nbin;

            hist[ idx ]++;
        }

        idx=0;
        for( j = 0; j < nbin; j++)
        {
            ps = 0;
            for( i = 0; i < nbin; i++)
            {
                if (hist[idx] != 0)
                {
                    p = (double)(hist[idx]) / sz;
                    srHist -= p * log(p);
                    ps += p;
                }
                idx++;
            }

            if (ps != 0)
                sHist -= ps * log(ps);
        }

        idx=0;
        for ( i = 0; i < nbin; i++)
        {
            idx = i;
            pr = 0;
            for( j = 0; j < nbin; j++)
            {
                pr += hist[idx];
                idx += nbin;
            }
            pr = pr / sz;

            if (pr != 0)
                rHist -= pr * log(pr);
        }
    } // #pragma acc

    nmi = (sHist + rHist) /(EPSILON + srHist) - 1;

    // de-alloc
    del1dp<Tidx>(hist);

    //
    return 0;
}

// normalized cross correlation
template <class Tsrc, class Tref, class Tidx>
int computeNCC(Tsrc *src, Tref *ref, Tidx sz, double& ncc)
{
    if( !src || !ref )
    {
        cout<<"Invalid inputs for computNCC func."<<endl;
        return -1;
    }

    //
    Tidx i;
    double stdS=0, stdR=0, meanS=0, meanR=0, jointSR=0, valS=0, valR=0;

    //#pragma acc region copyin(src[0:sz-1],ref[0:sz-1])
    {
#pragma omp parallel for shared(sz,src,ref) reduction(+:meanS,meanR) private(i)
        for (i = 0; i < sz; i++)
        {
            meanS += (double)(src[i]);
            meanR += (double)(ref[i]);
        }

        meanS /= (double)sz;
        meanR /= (double)sz;

#pragma omp parallel for shared(sz,src,ref,meanS,meanR) reduction(+:stdS,stdR,jointSR) private(i,valS,valR)
        for (i = 0; i < sz; i++)
        {
            valS = (double)(src[i]) - meanS;
            valR = (double)(ref[i]) - meanR;

            jointSR += valS*valR;
            stdS += valS*valS;
            stdR += valR*valR;
        }

        stdS = sqrt(stdS);
        stdR = sqrt(stdR);

        // NCC 0.5*([-1,1]+1)
        if(stdS && stdR)
        {
            ncc = jointSR/(stdS*stdR);
        }
        else
        {
            ncc = -INF;
            cout<<"Error in computing NCC. "<<stdS<<" "<<stdR<<" "<<ncc<<endl;
            return -1;
        }

    } // #pragma acc

    //
    return 0;
}

//
/// 1d vector
//

template <class T>
Vec1D <T> :: Vec1D()
{
    items.clear();
}

template <class T>
Vec1D <T> :: ~Vec1D()
{
    items.clear();
}

template <class T>
bool Vec1D <T> :: isNumber(const string& s)
{
    string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

template <class T>
int Vec1D <T> :: str2num(string s)
{
    istringstream ss(s);
    string str;

    while(ss >> str)
    {
        if(isNumber(str))
        {
            T num;
            stringstream(str) >> num;
            items.push_back(num);
        }
    }

    if(items.empty())
    {
        cout<<"Invalid inputs.\n";
        return -1;
    }

    //
    return 0;
}

template <class T>
int Vec1D <T> :: getFileList(string fn)
{
    string str;
    ifstream fin(const_cast<char*>(fn.c_str()));

    if(fin.is_open())
    {
        while( getline(fin, str) )
        {
            items.push_back(str);
        }
        fin.close();
    }
    else
    {
        cout << "Fail to open the input file list." << endl;
        return -1;
    }

    //
    return 0;
}

template <class T>
void Vec1D <T> :: zeros(long size)
{
    for(long i=0; i<size; i++)
    {
        items.push_back(0);
    }
}

//
/// 2d line
//

template <class T>
Line2D <T> :: Line2D()
{
    k = 0;
    b = 0;
    s = 0;
}

template <class T>
Line2D <T> :: Line2D(T slope, T intercept)
{
    k = slope;
    b = intercept;
}

template <class T>
Line2D <T> :: ~Line2D()
{
}

template <class T>
void Line2D <T> :: slope(T slope)
{
    k = slope;
}

template <class T>
void Line2D <T> :: intercept(T intercept)
{
    b = intercept;
}

template <class T>
void Line2D <T> :: score(T score)
{
    s = score;
}

template <class Tdata, class Tidx>
int findOptimalPath(Tdata *p, Tidx x, Tdata minY0, Tdata maxY0, Tdata minY, Tdata maxY, Tdata &k, Tdata &b)
{
    Tidx i,j;
    Tdata x0, x1, y0, y1;
    Tdata sum;

    Tidx NBESTLINES = 5;
    vector< Line2D<Tdata> > linelist;

    for(b=minY0; b<=maxY0; b++)
    {
        for(y1=minY; y1<=maxY; y1++)
        {
            k = (y1 - b)/x;

            sum = 0;
            for(i=0; i<=x; i++)
            {
                j = k*i + b + 0.5;

                sum += p[j*(x+1) + i];
            }

            // insert this LINE
            Line2D<Tdata> line(k,b);
            line.score(sum);

            if(linelist.size()<1)
            {
                linelist.push_back(line);
            }
            else
            {
                for(Tidx it=linelist.size(); it!=0; it--)
                {
                    if(sum>=linelist.at(it-1).s)
                    {
                        linelist.insert(linelist.begin() + it, 1, line);
                        if(linelist.size()>NBESTLINES)
                            linelist.erase(linelist.begin());
                        break;
                    }
                    else
                        continue;
                }
                if(sum<linelist.at(0).s && linelist.size()<NBESTLINES)
                {
                    linelist.insert(linelist.begin(), line);
                }
            }

        }
    }

    k=linelist[linelist.size()-1].k;
    b=linelist[linelist.size()-1].b;

    return 0;
}

template <class Tdata, class Tidx>
int reconstructStack(Tdata *slices, Tidx n, Tdata *&stack, Tidx x, Tidx y, Tidx z, double k, double b)
{
    //
    if(n<1)
    {
        cout<<"Need n>=1 slices!\n";
        return -1;
    }

    if(slices==NULL)
    {
        cout<<"Invalid slices!\n";
        return -1;
    }

    //
    Tidx start, pre, next, interval = 0;
    Tdata sum = 0;

    for(Tidx i=0; i<n; i++)
    {
        if(i==0)
        {
            pre = k*i+b;
            start = (Tidx)(pre + 0.5);
        }
        else
        {
            next = k*i+b;
            sum += next - pre;
            pre = next;
        }
    }

    interval = (Tidx) (sum / (n - 1));

    //
    Tidx pagesz = x*y;

    //
    new1dp<Tdata, Tidx>(stack, pagesz*z);

    //
    for(Tidx i=0; i<n; i++)
    {
        Tidx offslices = i*pagesz;
        Tidx offstack = (start + i*interval)*pagesz;

        for(Tidx j=0; j<pagesz; j++)
        {
            stack[offstack + j] = slices[offslices + j];
        }
    }

    //
    return 0;
}



#endif // __IMAGE_HXX__
