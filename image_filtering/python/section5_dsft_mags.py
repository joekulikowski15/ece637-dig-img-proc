import io
import os
import matplotlib.pyplot as plt
import matplotlib.colors as cl
import matplotlib.cm as cm
import numpy as np
from PIL import Image


#Definitions of the filter, etc. Part 1.

this_dir = os.path.dirname(os.path.realpath(__file__))

def H(mu:np.ndarray, nu:np.ndarray) -> np.ndarray:
    result = 1/100
    minus_j = 0-1j
    denom = 1 + ((9/10) * (((9/10)*np.exp(minus_j*(mu+nu))) - (np.exp(minus_j*mu) + np.exp(minus_j*nu))))
    result = result * (1/denom)
    return result
n_points = int(5e2)
mu, nu = np.linspace(-3*np.pi, 3*np.pi, n_points), np.linspace(-3*np.pi, 3*np.pi, n_points)
muMesh, nuMesh = np.meshgrid(mu, nu)

HH = H(muMesh, nuMesh)

# Plottling between -pi and pi to show periodicity
plt.rcParams["figure.figsize"] = (5,8)
HHplot_negpi_pi = np.copy(HH)
bound = np.pi
conds = np.logical_or(np.abs(muMesh)>bound,np.abs(nuMesh)>bound)
HHplot_negpi_pi[conds] = np.nan
fig1, ax1 = plt.subplots(subplot_kw={"projection": "3d"})
surf = ax1.plot_surface(muMesh, nuMesh, np.abs(HHplot_negpi_pi),
                        rstride=1, cstride=1,
                        cmap = cm.coolwarm)
ax1.set_xlim([-bound, bound])
ax1.set_ylim([-bound, bound])
ax1.set_zlabel(r"$|H(e^{j \mu}, e^{j \nu})|$")
ax1.set_ylabel(r"$\mu$")
ax1.set_xlabel(r"$\nu$")
fig1.set_size_inches(8, 5)
fig1.savefig(os.path.join(this_dir,"..","images","DSFTmag3D_p5.jpg"), dpi=500)


image = np.zeros((256, 256))
image[127,127] = 1
filtered = np.zeros(np.shape(image))
for col in range(np.size(image,0)):
    for row in range(np.size(image,1)):
        if row-1<0:
            above = 0
        else:
            above = filtered[row-1, col]
        if col-1<0:
            left = 0
        else:
            left = filtered[row, col-1]
        if col-1<0 | row-1<0:
            aboveleft = 0
        else:
            aboveleft = filtered[row-1, col-1]
        filtered[row, col] = 0.01 * image[row, col] + \
                             0.9*(above + left) - \
                             0.81 * (aboveleft)
scaled = filtered*100*255
im = Image.fromarray(scaled.astype(np.uint8))
im.save(os.path.join(this_dir, "..", "images","h_out.tif"))
im.save(os.path.join(this_dir, "..", "images","h_out.jpg"))
IIR_filtered = Image.open(os.path.join(this_dir, "..", "images", "IIR_filtered.tif"))
IIR_filtered.convert("RGB").save(os.path.join(this_dir, "..", "images", "IIR_filtered.jpg"))


