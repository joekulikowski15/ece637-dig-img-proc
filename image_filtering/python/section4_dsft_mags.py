import io
import os
import matplotlib.pyplot as plt
import matplotlib.colors as cl
import matplotlib.cm as cm
import numpy as np
from PIL import Image


#Definitions of the filter, etc. Part 1.

def Hhat(mu:np.ndarray, nu:np.ndarray)->np.ndarray:
    result = (4/5)**2 * np.sinc(4*mu) * np.sinc(4*nu)
    return result

def H(mu:np.ndarray, nu:np.ndarray, Tmu:float, Tnu:float,
      Nmu:int, Nnu:int)->np.ndarray:
    # Reshape mu, nu to live in axis 0
    mu = np.reshape(mu, [np.size(mu,0), np.size(mu, 1), 1, 1])
    nu = np.reshape(nu, [np.size(nu,0), np.size(nu, 1), 1, 1])
    # Reshape k, l to live in axes 1, 2
    k = np.arange(-Nmu, Nmu, 1).reshape(1, 1, 2*Nmu, 1)
    l = np.arange(-Nnu, Nnu, 1).reshape(1, 1, 1, 2*Nnu)
    # Sum along axis 2, then 2 again after dimensionality reduced. 
    double_sum = np.sum(
        np.sum(Hhat((mu-(2*np.pi*k))/(2*np.pi*Tmu),
                    (nu-(2*np.pi*l))/(2*np.pi*Tnu))
        ,2)
    ,2)
    result = (1/(Tmu * Tnu)) * double_sum
    return result

             

this_dir = os.path.dirname(os.path.realpath(__file__))



n_points = int(5e2)
mu, nu = np.linspace(-3*np.pi, 3*np.pi, n_points), np.linspace(-3*np.pi, 3*np.pi, n_points)
muMesh, nuMesh = np.meshgrid(mu, nu)

HH = H(muMesh, nuMesh, 1, 1, 10, 10)

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
ax1.set_title(r"$max = .63 =  \frac{16}{25}$")
ax1.set_zlabel(r"$|H(e^{j \mu}, e^{j \nu})|$")
ax1.set_ylabel(r"$\mu$")
ax1.set_xlabel(r"$\nu$")
fig1.set_size_inches(8, 5)
fig1.savefig(os.path.join(this_dir,"..","images","DSFTmag3D_p4.jpg"), dpi=500)


lamb = 1.5 
GG = 1 + lamb * (1 - HH)
fig2, ax2 = plt.subplots(subplot_kw={"projection": "3d"})
plt.rcParams["figure.figsize"] = (5,8)
GGplot_negpi_pi = np.copy(GG)
conds = np.logical_or(np.abs(muMesh)>bound,np.abs(nuMesh)>bound)
GGplot_negpi_pi[conds] = np.nan
surf = ax2.plot_surface(muMesh, nuMesh, np.abs(GGplot_negpi_pi),
                    rstride=1, cstride=1,
                    cmap = cm.coolwarm)
ax2.set_xlim([-bound, bound])
ax2.set_ylim([-bound, bound])
ax2.set_title(f"lambda = {lamb}")
ax2.set_zlabel(r"$|G(e^{j \mu}, e^{j \nu})|$")
ax2.set_ylabel(r"$\mu$")
ax2.set_xlabel(r"$\nu$")
fig2.set_size_inches(8, 5)
fig2.savefig(os.path.join(this_dir,"..","images","DSFTmag3D_p4_G.jpg"), dpi=500)


imgblur = Image.open(os.path.join(this_dir, "..", "images", "imgblur.tif"))
imgblur.convert("RGB").save(os.path.join(this_dir, "..", "images", "imgblur.jpg"))

sharp_lambda = Image.open(os.path.join(this_dir, "..", "images", "sharp_lambda_1.500000_.tif"))
sharp_lambda.convert("RGB").save(os.path.join(this_dir, "..", "images", "sharp_lambda1.500000_.jpg"))


