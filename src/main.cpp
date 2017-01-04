// main.cpp
// nail application
// developed by Yang Yu (gnayuy@gmail.com)

#include "nail.h"

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
        else if(FLAGS_f == "help")
        {
            cout<<endl<<"nail -f <function>"<<endl;
            cout<<"function list:"<<endl;
            cout<<"\t"<<"adjustIntensity"<<endl;
            cout<<endl;
        }
        else
        {
            
        }
    }
    
    //
    return 0;
}
