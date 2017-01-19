// main.cpp
// nail application
// developed by Yang Yu (gnayuy@gmail.com)

#include "nail.h"

// CLI
DEFINE_string(f, "", "specify which function to call");
DEFINE_string(i, "", "input");
DEFINE_string(o, "", "output");
DEFINE_string(r, "", "reference");
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
            // nail -f adjustIntensity -i ../data/gcampchannel_mip.tif -o ../data/test.tif
            Nail nail;
            nail.adjustIntensity(FLAGS_i, FLAGS_o);
        }
        else if(FLAGS_f == "imageReadWrite")
        {
            // nail -f imageReadWrite -i <input> -o <output>
            Nail nail;
            nail.imageReadWrite(FLAGS_i, FLAGS_o);
        }
        else if(FLAGS_f == "gammaFilter")
        {
            // nail -f gammaFilter -i <input> -o <output> -gamma 0.5
            Nail nail;
            nail.gammaFilter(FLAGS_i, FLAGS_o, FLAGS_gamma, ENCODE);
        }
        else if(FLAGS_f == "genMaskImageFromLabels")
        {
            // nail -f genMaskImageFromLabels -i <input> -o <output> -s "4 9 43" -exclude true
            Nail nail;
            nail.genMaskImageFromLabels(FLAGS_i, FLAGS_o, FLAGS_s, FLAGS_exclude);
        }
        else if(FLAGS_f == "genLabelImage")
        {
            // nail -f genLabelImage -i <input> -o <output>
            Nail nail;
            nail.genLabelImage(FLAGS_i, FLAGS_o);
        }
        else if(FLAGS_f == "countVoxels")
        {
            // nail -f countVoxels -i <input> -o <output> -s "1"
            // nail -f countVoxels -i <input> -o <output> -s <mask> -withMask true -n 68
            Nail nail;
            nail.countVoxels(FLAGS_i, FLAGS_o, FLAGS_s, FLAGS_withMask, FLAGS_n);
        }
        else if(FLAGS_f == "binarize")
        {
            // nail -f binarize -i <input> -o <output>
            Nail nail;
            nail.binarize(FLAGS_i, FLAGS_o);
        }
        else if(FLAGS_f == "imageCompare")
        {
            // nail -f imageCompare -i <input> -o <output> -r <reference> -similarity <similarity_metric>
            Nail nail;
            nail.imageCompare(FLAGS_i, FLAGS_o, FLAGS_r, (SimilarityType)(FLAGS_similarity));
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
            cout<<endl;
        }
        else
        {
            
        }
    }
    
    //
    return 0;
}
