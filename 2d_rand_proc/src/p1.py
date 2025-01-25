import numpy as np
import matplotlib.pyplot as plt
from BetterSpecAnal import BetterSpecAnal
from PIL import Image
in_img = Image.open('img04g.tif')
img = np.array(in_img)/255.0 # As in the SpecAnal, normalize values.
window_height = 64
window_width = 64
n_windows = 25
psd = BetterSpecAnal(img, n_windows=n_windows, window_height=window_height, window_width=window_width)
# Plot the result using a 3-D mesh plot and label the x and y axises properly. 
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
y = np.linspace(-np.pi, np.pi, window_height)
x = np.linspace(-np.pi, np.pi, window_width)
X, Y = np.meshgrid(x, y)

surf = ax.plot_surface(X, Y, np.log(np.abs(psd)), cmap=plt.cm.coolwarm)

ax.set_xlabel('$\mu$ axis')
ax.set_ylabel('$\\nu$ axis')
ax.set_title('log(Power Spectral Density)')
fig.colorbar(surf, shrink=0.5, aspect=5)
plt.show()