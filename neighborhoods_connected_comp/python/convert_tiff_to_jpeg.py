import argparse
import os
from PIL import Image
import matplotlib.pyplot as plt

def main():
    parser = argparse.ArgumentParser(description="This function converts tiffs to jpegs and saves them to the same location.")
    parser.add_argument("-f", "--fname", type=str, help="Full path to the tiff image.")
    parser.add_argument("-t", "--tiff-type", type=str, help="greyscale (g) or color (c)")
    parser.add_argument("-o", "--out-dir", type=str, help="where to save")
    args = parser.parse_args()
    image = Image.open(args.fname)
    im_dir = os.path.dirname(args.fname)
    im_name = os.path.basename(args.fname)
    new_imname = str(im_name).replace("tif", "jpg")
    if args.out_dir is None:
        out_dir = im_dir
    else:
        out_dir = args.out_dir
    if args.tiff_type == "g":
        image.save(os.path.join(out_dir, new_imname))
    else:
        image.convert("RGB").save(os.path.join(out_dir, new_imname))

if __name__ == "__main__":
    main()
