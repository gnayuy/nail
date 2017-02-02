// main.cpp
// nail application
// developed by Yang Yu (gnayuy@gmail.com)

#include "nail.h"

// CLI
DEFINE_string(f, "", "specify which function to call");

DEFINE_string(i, "", "input");
DEFINE_string(o, "", "output");
DEFINE_string(ref, "", "reference");

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

DEFINE_double(gamma, 0.5, "gamma (0.5 by default) a non-linear histogram adjustment");
DEFINE_string(s, "", "a string (e.g. labels, masks, ...)");
DEFINE_bool(exclude, false, "exclude input labes?");
DEFINE_bool(withMask, false, "input a mask image?");
DEFINE_uint64(n, 1, "the number of ...");
DEFINE_uint32(similarity, 2, "similarity metric (NCC=2, NMI=3, ...)");

DEFINE_double(k, 1.0, "slope");
DEFINE_double(b, 0.0, "intercept");

DEFINE_double(l, 0.0, "x0");
DEFINE_double(r, 0.0, "x1");
DEFINE_double(d, 0.0, "y0");
DEFINE_double(u, 0.0, "y1");

//
DEFINE_bool(test, false, "test");
DEFINE_uint64(testOption, 0, "test option");

// main
int main(int argc, char *argv[])
{
    //
    gflags::SetUsageMessage("nail -f <func> -i <input> -o <output>");
    gflags::SetVersionString(string("0.1.1 "));
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
    }
    else
    {
        //
        if(FLAGS_f == "adjustIntensity")
        {
            cout<<"\n nail -f adjustIntensity -i ../data/gcampchannel_mip.tif -o ../data/test.tif \n"<<endl;
            Nail nail;
            nail.adjustIntensity(FLAGS_i, FLAGS_o);
        }
        else if(FLAGS_f == "imageReadWrite")
        {
            cout<<"\n nail -f imageReadWrite -i <input> -o <output> \n"<<endl;
            Nail nail;
            nail.imageReadWrite(FLAGS_i, FLAGS_o);
        }
        else if(FLAGS_f == "gammaFilter")
        {
            cout<<"\n nail -f gammaFilter -i <input> -o <output> -gamma 0.5 \n"<<endl;
            Nail nail;
            nail.gammaFilter(FLAGS_i, FLAGS_o, FLAGS_gamma, ENCODE);
        }
        else if(FLAGS_f == "genMaskImageFromLabels")
        {
            cout<<"\n nail -f genMaskImageFromLabels -i <input> -o <output> -s \"4 9 43\" -exclude true \n"<<endl;
            Nail nail;
            nail.genMaskImageFromLabels(FLAGS_i, FLAGS_o, FLAGS_s, FLAGS_exclude);
        }
        else if(FLAGS_f == "genLabelImage")
        {
            cout<<"\n nail -f genLabelImage -i <input> -o <output> \n"<<endl;
            Nail nail;
            nail.genLabelImage(FLAGS_i, FLAGS_o);
        }
        else if(FLAGS_f == "countVoxels")
        {
            cout<<"\n nail -f countVoxels -i <input> -o <output> -s \"1\"";
            cout<<"\n nail -f countVoxels -i <input> -o <output> -s <mask> -withMask true -n 68 \n"<<endl;
            Nail nail;
            nail.countVoxels(FLAGS_i, FLAGS_o, FLAGS_s, FLAGS_withMask, FLAGS_n);
        }
        else if(FLAGS_f == "binarize")
        {
            cout<<"\n nail -f binarize -i <input> -o <output>\n"<<endl;
            Nail nail;
            nail.binarize(FLAGS_i, FLAGS_o);
        }
        else if(FLAGS_f == "imageCompare")
        {
            cout<<"\n nail -f imageCompare -i <input> -o <output> -ref <reference> -similarity <similarity-metric> \n"<<endl;
            Nail nail;
            nail.imageCompare(FLAGS_i, FLAGS_o, FLAGS_ref, (SimilarityType)(FLAGS_similarity));
        }
        else if(FLAGS_f == "constructStack")
        {
            cout<<"\n nail -f constructStack -i <input> -o <output> -sz <z-dimension> -k <slope> -b <intercept> \n"<<endl;
            Nail nail;
            nail.constructStack(FLAGS_i, FLAGS_o, FLAGS_sz, FLAGS_k, FLAGS_b);
        }
        else if(FLAGS_f == "findOptimalPath")
        {
            cout<<"\n nail -f findOptimalPath -i <input> -o <output> -l <minY0> -r <maxY0> -d <minY> -u <maxY> -sx <x-dimension> -sy <y-dimension> \n"<<endl;
            Nail nail;
            nail.findOptimalPath(FLAGS_i, FLAGS_o, FLAGS_sx, FLAGS_sy, FLAGS_l, FLAGS_r, FLAGS_d, FLAGS_u);
        }
        else if(FLAGS_f == "help")
        {
            cout<<endl<<"nail -f <function>"<<endl;
            cout<<"function list:"<<endl;
            cout<<"\t"<<"adjustIntensity"<<endl;
            cout<<"\t"<<"imageReadWrite"<<endl;
            cout<<"\t"<<"gammaFilter"<<endl;
            cout<<"\t"<<"genMaskImageFromLabels"<<endl;
            cout<<"\t"<<"genLabelImage"<<endl;
            cout<<"\t"<<"countVoxels"<<endl;
            cout<<"\t"<<"binarize"<<endl;
            cout<<"\t"<<"imageCompare"<<endl;
            cout<<"\t"<<"constructStack"<<endl;
            cout<<"\t"<<"findOptimalPath"<<endl;
            cout<<endl;
        }
        else
        {
            
        }
    }
    
    //
    return 0;
}
