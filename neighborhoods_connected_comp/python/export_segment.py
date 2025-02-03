import argparse
import os
from PIL import Image
import matplotlib.pyplot as plt
import matplotlib.colors as cl
import numpy as np

def main():
    parser = argparse.ArgumentParser(description="This function converts tiffs to jpegs and saves them to the same location.")
    parser.add_argument("-f", "--fname", type=str, help="Full path to the tiff image.")
    parser.add_argument("-o", "--out-dir", type=str, help="where to save")
    args = parser.parse_args()
    image = Image.open(args.fname)
    im_dir = os.path.dirname(args.fname)
    im_name = os.path.basename(args.fname)
    new_imname = str("segmented"+im_name).replace("tif", "jpg")
    if args.out_dir is None:
        out_dir = im_dir
    else:
        out_dir = args.out_dir
    x = np.array(image)
    N = np.max(x)
    cmap = cl.ListedColormap(np.random.rand(N+1,3))
    plt.imshow(x, cmap=cmap, interpolation='None')
    plt.colorbar()
    plt.title(f'Segmentation, Threshold = {im_name}')
    plt.savefig(os.path.join(out_dir, new_imname), dpi=500)

if __name__ == "__main__":
    main()