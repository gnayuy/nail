#!/bin/bash

#
# map JFRC2013 mushroom body compartments to a live GCaMP images
# Usage:
# sh mappingMBC.sh inputFileList.txt outdir/outname z
#
# develop by Yang Yu (yuy@janelia.hhmi.org) 03/01/2017
#

#
is_file_exist()
{
    local f="$1"
    [[ -f "$f" ]] && return 0 || return 1
}

#start=`date +%s.%N`
start=`python3 -c'import time; print(time.time())'`

# hard coded
TOOLKITDIR=/path/Toolkits
GCAMP2017=/path/gcamp2017ds.nii.gz
GCAMP2017_MBC=/path/gcamp2017ds_mbcompartments.nii.gz

NAIL=${TOOLKITDIR}"/nail"
FSLREG=${TOOLKITDIR}"/flirt"
FSLWARP=${TOOLKITDIR}"/applywarp"
FSLCONV=${TOOLKITDIR}"/convert_xfm"
CMTKLREG=${TOOLKITDIR}"/registration"
CMTKNLREG=${TOOLKITDIR}"/warp"
CMTKWARP=${TOOLKITDIR}"/reformatx"

# 1. preprocessing

INPUT=$1
OUTNAME=$2
Z=$3

# 1.1 fiji convert tiff to nii

#for i in $PWD/*/*.tiff; do outdir=${i%*/*}; /Applications/Fiji.app/Contents/MacOS/ImageJ-macosx -macro /Applications/Fiji.app/macros/imageConverter.ijm $i,$outdir; done

# 1.2 seq2stack

InputStack=${OUTNAME}".nii.gz"
InputStack8=${OUTNAME}"_byte.nii.gz"

time $NAIL -f seq2stack -i $INPUT -o $InputStack
time $NAIL -f convert2byte -i $InputStack -o $InputStack8

# 1.3 convert2byte

NZ=$( echo "$Z * 4.0" | bc -l )

# 1.4 sampling

InputUpSampled=${OUTNAME}"_us.nii.gz"

time $NAIL -f imageResize -i $InputStack8 -o $InputUpSampled -srz 4.0

# 1.5 recenter

InputResized=${OUTNAME}"_rs.nii.gz"

time $NAIL -f imageResize -i $InputUpSampled -o $InputResized -sx 256 -sy 128 -sz 109

# 2. alignment

F=$InputResized
M=$GCAMP2017

# 2.1 global alignment

GLOBAL=${OUTNAME}"linear.txt"

GLOBALALIGNEDTIM=${OUTNAME}"globalaligned.nii.gz"
GLOBALALIGNEDMBC=${OUTNAME}"globalaligned_mbc.nii.gz"

GLOBALALIGNEDTIM8=${OUTNAME}"globalaligned_byte.nii.gz"
GLOBALALIGNEDMBC8=${OUTNAME}"globalaligned_mbc_byte.nii.gz"

if ( is_file_exist "$GLOBAL" )
then
echo " GLOBAL Alignment Matrix: $GLOBAL exists"
else
time $FSLREG -in $M -ref $F -omat $GLOBAL -cost mutualinfo -searchrx -18 18 -searchry -18 18 -searchrz -18 18 -dof 12 -datatype char
fi

# globally warp

time $FSLWARP --ref=$F --in=$M --out=$GLOBALALIGNEDTIM --premat=$GLOBAL
time $NAIL -f convert2byte -i $GLOBALALIGNEDTIM -o $GLOBALALIGNEDTIM8

time $FSLWARP --ref=$F --in=$GCAMP2017_MBC --out=$GLOBALALIGNEDMBC --premat=$GLOBAL --interp=nn
time $NAIL -f convert2byte -i $GLOBALALIGNEDMBC -o $GLOBALALIGNEDMBC8

# 2.2 local alignment

MGA=$GLOBALALIGNEDTIM8

AFFINE=${OUTNAME}"_affine.xform"
FFD80=${OUTNAME}"_ffd80.xform"

if ( is_file_exist "$AFFINE" )
then
echo " AFFINE Matrix: $AFFINE exists"
else
time $CMTKLREG --initxlate --dofs 6,9 --auto-multi-levels 4 -o $AFFINE $F $MGA
fi

if ( is_file_exist "$FFD80" )
then
echo " Local Warping Field: $FFD80 exists"
else
time $CMTKNLREG -o $FFD80 --grid-spacing 80 --exploration 30 --coarsest 4 --accuracy 0.8 --refine 4 --energy-weight 1e-1 --initial $AFFINE $F $MGA
fi

# locally warp

LOCALALIGNED=${OUTNAME}"_MBC_localaligned_rs.nii.gz"

time $CMTKWARP --nn -o $LOCALALIGNED --floating $GLOBALALIGNEDMBC8 $F $FFD80

# 3. postprocessing

# 3.1 sampling back

LOCALALIGNEDRS=${OUTNAME}"_MBC_localaligned_us.nii.gz"

time $NAIL -f imageResize -i $LOCALALIGNED -o $LOCALALIGNEDRS -sx 256 -sy 128 -sz ${NZ%*.*}

ALIGNEDMBC=${OUTNAME}"_MBC.nii.gz"

time $NAIL -f imageResize -i $LOCALALIGNEDRS -o $ALIGNEDMBC -srz 0.25 -interp 1

# 3.2 inversely warp to generate a validation superimposed image stack

# 3.2.1 construct 3d stack w/ interpolation

#VStack=${OUTNAME}"_recon.nii.gz"
#VStack8=${OUTNAME}"_recon_byte.nii.gz"

#time $NAIL -f seq2stack -i $INPUT -o $VStack -zstep 4 -sz ${NZ%*.*}
#time $NAIL -f convert2byte -i $VStack -o $VStack8

# 3.2.2 resize to the gcamp2017 downsampled version

#VStackRs=${OUTNAME}"_recon_rs.nii.gz"

#time $NAIL -f imageResize -i $VStack8 -o $VStackRs -sx 256 -sy 128 -sz 109

# 3.2.3 globally warp

#GLOBALINV=${OUTNAME}"linear_inv.txt"

#time $FSLCONV -omat $GLOBALINV -inverse $GLOBAL

#VGLOBALALIGNED=${OUTNAME}"_recon_rs_ga.nii.gz"
#VGLOBALALIGNED8=${OUTNAME}"_recon_rs_ga_byte.nii.gz"

#time $FSLWARP --ref=$GCAMP2017 --in=$VStackRs --out=$VGLOBALALIGNED --premat=$GLOBALINV
#time $NAIL -f convert2byte -i $VGLOBALALIGNED -o $VGLOBALALIGNED8

# 3.2.4 locally warp

#VLOCALALIGNED=${OUTNAME}"_recon_rs_la.nii.gz"

#time $CMTKWARP -o $VLOCALALIGNED --floating $VGLOBALALIGNED8 $GCAMP2017 --inverse $FFD80

# execution time
#end=`date +%s.%N`
end=`python3 -c'import time; print(time.time())'`

runtime=$( echo "$end - $start" | bc -l )
echo "mappingMBC.sh runs $runtime seconds"


