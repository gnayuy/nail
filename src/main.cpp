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

DEFINE_uint64(x, 0, "offset (voxels) in x axis");
DEFINE_uint64(y, 0, "offset (voxels) in y axis");
DEFINE_uint64(z, 0, "offset (voxels) in z axis");

DEFINE_double(vx, -1.0, "voxel size in x axis");
DEFINE_double(vy, -1.0, "voxel size in y axis");
DEFINE_double(vz, -1.0, "voxel size in z axis");

DEFINE_double(gamma, 0.5, "gamma (0.5 by default) a non-linear histogram adjustment");
DEFINE_string(s, "", "a string (e.g. labels, masks, ...)");
DEFINE_bool(exclude, false, "exclude input labes?");
DEFINE_bool(withMask, false, "input a mask image?");
DEFINE_uint64(n, 1, "the number of ...");
DEFINE_uint32(similarity, 2, "similarity metric (NCC=2, NMI=3, ...)");

DEFINE_double(k, 1.0, "slope");
DEFINE_double(b, 0.0, "intercept");

DEFINE_uint64(step, 1, "step (voxels) in one dimension");
DEFINE_uint64(xstep, 1, "step (voxels) in x dimension");
DEFINE_uint64(ystep, 1, "step (voxels) in y dimension");
DEFINE_uint64(zstep, 1, "step (voxels) in z dimension");

DEFINE_double(sr, 1.0, "sampling ratio (xyz isotropic)");
DEFINE_double(srx, 1.0, "x sampling ratio");
DEFINE_double(sry, 1.0, "y sampling ratio");
DEFINE_double(srz, 1.0, "z sampling ratio");

DEFINE_double(l, 0.0, "x0");
DEFINE_double(r, 0.0, "x1");
DEFINE_double(d, 0.0, "y0");
DEFINE_double(u, 0.0, "y1");

DEFINE_double(min, 0.0, "min value");
DEFINE_double(max, 0.0, "max value");

DEFINE_uint64(interp, 2, "interpolation type (Linear by default)");

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
            nail.imageReadWrite(FLAGS_i, FLAGS_o, FLAGS_vx, FLAGS_vy, FLAGS_vz);
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
        else if(FLAGS_f == "imageResize")
        {
            cout<<"\n nail -f imageResize -i <input> -o <output> -sx <x-dimension> -sy <y-dimension> -sz <z-dimension>\n"<<endl;
            Nail nail;

            if(!doubleVarCompare(FLAGS_sr, 1.0) || !doubleVarCompare(FLAGS_srx, 1.0) || !doubleVarCompare(FLAGS_sry, 1.0) || !doubleVarCompare(FLAGS_srz, 1.0) )
            {
                // sampling
                if(!doubleVarCompare(FLAGS_sr, 1.0))
                {
                    nail.imageSampling(FLAGS_i, FLAGS_o, FLAGS_sr, FLAGS_sr, FLAGS_sr, FLAGS_interp);
                }
                else
                {
                    nail.imageSampling(FLAGS_i, FLAGS_o, FLAGS_srx, FLAGS_sry, FLAGS_srz, FLAGS_interp);
                }
            }
            else
            {
                nail.recenterImage(FLAGS_i, FLAGS_o, FLAGS_sx, FLAGS_sy, FLAGS_sz, FLAGS_sc);
            }
        }
        else if(FLAGS_f == "seq2stack")
        {
            cout<<"\n nail -f seq2stack -i <input> -o <output> -z <first-z-slice> -zstep <z-step> -sz <z-dimension> \n"<<endl;
            Nail nail;
            nail.seq2stack(FLAGS_i, FLAGS_o, FLAGS_z, FLAGS_zstep, FLAGS_sz);
        }
        else if(FLAGS_f == "convert2byte")
        {
            cout<<"\n nail -f convert2byte -i <input> -o <output> \n"<<endl;
            Nail nail;
            nail.convert2byte(FLAGS_i, FLAGS_o);
        }
        else if(FLAGS_f == "convertIndex2Color")
        {
            cout<<"\n nail -f convertIndex2Color -i <input> -o <output> \n"<<endl;
            Nail nail;
            nail.convertIndex2Color(FLAGS_i, FLAGS_o);
        }
        else if(FLAGS_f == "intensityRescale")
        {
            cout<<"\n nail -f intensityRescale -i <input> -o <output> -min <min_intensity> -max <max_intensity> \n"<<endl;
            Nail nail;
            nail.intensityRescale(FLAGS_i, FLAGS_o, FLAGS_min, FLAGS_max);
        }
        else if(FLAGS_f == "imageDivide")
        {
            cout<<"\n nail -f imageDivide -i <input> -ref <refimage> -o <output> \n"<<endl;
            Nail nail;
            nail.imageDivide(FLAGS_i, FLAGS_ref, FLAGS_o);
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
            cout<<"\t"<<"imageResize"<<endl;
            cout<<"\t"<<"seq2stack"<<endl;
            cout<<"\t"<<"convert2byte"<<endl;
            cout<<"\t"<<"convertIndex2Color"<<endl;
            cout<<"\t"<<"intensityRescale"<<endl;
            cout<<"\t"<<"imageDivide"<<endl;
            cout<<endl;
        }
        else
        {
            
        }
    }
    
    //
    return 0;
}
