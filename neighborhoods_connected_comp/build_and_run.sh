#!/bin/zsh
SCRIPT_DIR=$(dirname "$(realpath "$0")")
in_img_path="$SCRIPT_DIR/images/img22gd2.tif"
echo "$SCRIPT_DIR"

# Section 1: Area fill.
cd "$SCRIPT_DIR/c"
# Make and run the C code.
sec1_row=45
sec1_col=67
sec_1_threshs=(1 2 3)
make
cd "$SCRIPT_DIR"
for thresh in "${sec_1_threshs[@]}";do
    ./c/bin/sec_1.out $in_img_path $sec1_row $sec1_col $thresh
done

cd "$SCRIPT_DIR"
for thresh in "${sec_1_threshs[@]}";do
    python python/convert_tiff_to_jpeg.py -f "$SCRIPT_DIR/images/s1_${sec1_row}_${sec1_col}_$thresh.tif"
done

# Section 2: Image segmentation.
for thresh in "${sec_1_threshs[@]}";do
    ./c/bin/sec_2.out $in_img_path $thresh
done

cd "$SCRIPT_DIR"
for thresh in "${sec_1_threshs[@]}";do
    python python/convert_tiff_to_jpeg.py -f "$SCRIPT_DIR/images/segmentation_$thresh.tif"
    python python/export_segment.py -f "$SCRIPT_DIR/images/segmentation_$thresh.tif"
done
