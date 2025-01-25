import os
import numpy as np
import matplotlib.pyplot as plt
from BetterSpecAnal import BetterSpecAnal

this_dir = os.path.dirname(os.path.realpath(__file__))
rand_size = 512

img = np.random.uniform(-0.5, 0.5, (rand_size,rand_size))

im_scaled = 255*(img + 0.5)

# Show and save the scaled image
fig, ax = plt.subplots()
ax.imshow(im_scaled, cmap=plt.cm.gray)
fig.savefig(os.path.join(this_dir,"..","images","rand_scaledp2.jpg"), dpi=500)

# Initialize array for filtered values.
filtered = np.zeros(np.shape(im_scaled))

# Apply the filter in the problem
for row in range(np.size(im_scaled,0)):
    for col in range(np.size(im_scaled,1)):
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
        filtered[row, col] = 3 * im_scaled[row, col] + \
                             0.99 * (above + left) - \
                             0.9801 * aboveleft

# Convert to uint8 and add 127 as described.
filtered = filtered.astype(np.uint8) + 127
# Plot and output this array.
fig2, ax2 = plt.subplots()
ax2.imshow(filtered, cmap=plt.cm.gray)
fig2.savefig(os.path.join(this_dir,"..","images","y_plus_127.jpg"), dpi=500)

# Set window shape
window_height=window_width=64
# Calculate theoretical psd
theoretical_psd = np.fft.fftshift((1/window_height**2)*np.abs(np.fft.fft2(filtered))**2)

fig3, ax3 = plt.subplots(subplot_kw={"projection": "3d"})
y = np.linspace(-np.pi, np.pi, rand_size)
x = np.linspace(-np.pi, np.pi, rand_size)
X, Y = np.meshgrid(x, y)
surf = ax3.plot_surface(X, Y, np.log(theoretical_psd), cmap=plt.cm.coolwarm)
ax3.set_xlabel('$\mu$ axis')
ax3.set_ylabel('$\\nu$ axis')
ax3.set_title('Theoretical log(Power Spectral Density)')
fig3.colorbar(surf, shrink=0.5, aspect=5)
# Save the figure.
fig3.savefig(os.path.join(this_dir,"..","images","theoretical_psd_p2.jpg"), dpi=500)

fig4, ax4 = plt.subplots(subplot_kw={"projection": "3d"})
y = np.linspace(-np.pi, np.pi, window_height)
x = np.linspace(-np.pi, np.pi, window_width)
X, Y = np.meshgrid(x, y)
surf = ax4.plot_surface(X, Y, np.log(BetterSpecAnal(filtered)), cmap=plt.cm.coolwarm)
ax4.set_xlabel('$\mu$ axis')
ax4.set_ylabel('$\\nu$ axis')
ax4.set_title('Estimate of log(Power Spectral Density)')
fig4.colorbar(surf, shrink=0.5, aspect=5)
# Save the figure.
fig4.savefig(os.path.join(this_dir,"..","images","BetterSpecAnalP2.jpg"), dpi=500)
