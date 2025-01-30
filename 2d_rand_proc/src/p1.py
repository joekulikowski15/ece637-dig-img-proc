import os
import numpy as np
import matplotlib.pyplot as plt
from BetterSpecAnal import BetterSpecAnal
from PIL import Image

this_dir = os.path.dirname(os.path.realpath(__file__))
in_img = Image.open('images/img04g.tif')
img = np.array(in_img)/255.0 # As in the SpecAnal, normalize values.
# save the in_img as jpg
in_img.save(os.path.join(this_dir, '..', 'images', 'img04g.jpg'))

window_height = 64
window_width = 64
n_windows = 25
psd = BetterSpecAnal(img, n_windows=n_windows, window_height=window_height, window_width=window_width)
# Plot the result using a 3-D mesh plot and label the x and y axises properly. 
fig, ax = plt.subplots(subplot_kw={"projection": "3d"})
y = np.linspace(-np.pi, np.pi, window_height)
x = np.linspace(-np.pi, np.pi, window_width)
X, Y = np.meshgrid(x, y)
surf = ax.plot_surface(X, Y, np.log(np.abs(psd)), cmap=plt.cm.coolwarm)
ax.set_xlabel('$\mu$ axis')
ax.set_ylabel('$\\nu$ axis')
ax.set_title('Estimate of log(Power Spectral Density)')
fig.colorbar(surf, shrink=0.5, aspect=5)
# Save the figure.
fig.savefig(os.path.join(this_dir,"..","images","BetterSpecAnalP1.jpg"), dpi=500)