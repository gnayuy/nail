
# download loci_tools.jar from http://downloads.openmicroscopy.org/bio-formats

# java -cp loci_tools5.3.4.jar loci.formats.tools.ImageConverter <input-file> <output-file.tif>

INPUT=$1
OUTPUT=$2

java -cp loci_tools5.3.4.jar loci.formats.tools.ImageConverter ${INPUT} ${OUTPUT}
