import io
import os
import matplotlib.pyplot as plt
import matplotlib.colors as cl
import matplotlib.cm as cm
import numpy as np
from PIL import Image


#Definitions of the filter, etc. Part 1.

def Hhat(mu:np.ndarray, nu:np.ndarray)->np.ndarray:
    result = (8/9)**2 * np.sinc(8*mu) * np.sinc(8*nu)
    return result

def H(mu:np.ndarray, nu:np.ndarray, Tmu:float, Tnu:float,
      Nmu:int, Nnu:int)->np.ndarray:
    # Reshape mu, nu to live in axis 0
    mu = np.reshape(mu, [np.size(mu,0), np.size(mu, 1), 1, 1])
    nu = np.reshape(nu, [np.size(nu,0), np.size(nu, 1), 1, 1])
    # Reshape k, l to live in axes 1, 2
    k = np.arange(-Nmu, Nmu, 1).reshape(1, 1, 2*Nmu, 1)
    l = np.arange(-Nnu, Nnu, 1).reshape(1, 1, 1, 2*Nnu)
    # Sum along axis 1, then 1 again after dimensionality reduced. 
    double_sum = np.sum(
        np.sum(Hhat((mu-(2*np.pi*k))/(2*np.pi*Tmu),
                    (nu-(2*np.pi*l))/(2*np.pi*Tnu))
        ,2)
    ,2)
    result = (1/(Tmu * Tnu)) * double_sum
    return result

             

this_dir = os.path.dirname(os.path.realpath(__file__))



n_points = int(1e3)
mu, nu = np.linspace(-3*np.pi, 3*np.pi, n_points), np.linspace(-3*np.pi, 3*np.pi, n_points)
muMesh, nuMesh = np.meshgrid(mu, nu)

HH = H(muMesh, nuMesh, 1, 1, 10, 10)


# Plottling between -pi and pi to show periodicity
plt.rcParams["figure.figsize"] = (5,8)
HHplot_negpi_pi = np.copy(HH)
conds = np.logical_or(np.abs(muMesh)>np.pi,np.abs(nuMesh)>np.pi)
HHplot_negpi_pi[conds] = np.nan
fig1, ax1 = plt.subplots(subplot_kw={"projection": "3d"})
surf = ax1.plot_surface(muMesh, nuMesh, np.abs(HHplot_negpi_pi),
                        rstride=2, cstride=2,
                        facecolors = cm.coolwarm(np.abs(HHplot_negpi_pi)))
ax1.set_xlim([-np.pi, np.pi])
ax1.set_ylim([-np.pi, np.pi])
ax1.set_title(r"$max = 0.7866 \approx \frac{64}{81}$")
ax1.set_zlabel(r"$|H(e^{j \mu}, e^{j \nu})|$")
ax1.set_xlabel(r"$\mu$")
ax1.set_ylabel(r"$\nu$")
fig1.set_size_inches(8, 5)
fig1.savefig(os.path.join(this_dir,"..","images","DSFTmag3D.jpg"), dpi=500)

plt.rcParams["figure.figsize"] = (5,8)
HHplot_negpi_pi = np.copy(HH)
conds = np.logical_or(np.abs(muMesh)>3*np.pi,np.abs(nuMesh)>3*np.pi)
HHplot_negpi_pi[conds] = np.nan
fig2, ax2 = plt.subplots(subplot_kw={"projection": "3d"})
surf = ax2.plot_surface(muMesh, nuMesh, np.abs(HHplot_negpi_pi),
                        rstride=2, cstride=2,
                        facecolors = cm.coolwarm(np.abs(HHplot_negpi_pi)))
ax2.set_xlim([-3*np.pi, 3*np.pi])
ax2.set_ylim([-3*np.pi, 3*np.pi])
ax2.set_title(r"$max(s) = 0.7866 \approx \frac{64}{81}$")
ax2.set_zlabel(r"$|H(e^{j \mu}, e^{j \nu})|$")
ax2.set_xlabel(r"$\mu$")
ax2.set_ylabel(r"$\nu$")
fig2.set_size_inches(8, 5)
fig2.savefig(os.path.join(this_dir,"..","images","DSFTmag3D_3pi.jpg"), dpi=500)

# Finishing out section 3...
img03 = Image.open(os.path.join(this_dir, "..", "images", "img03.tif"))
img03.convert("RGB").save(os.path.join(this_dir, "..", "images", "img03.jpg"))
FIR_LPF_filtered = Image.open(os.path.join(this_dir, "..", "images", "FIR_LPF_filtered.tif"))
FIR_LPF_filtered.convert("RGB").save(os.path.join(this_dir, "..", "images", "FIR_LPF_filtered.jpg"))

