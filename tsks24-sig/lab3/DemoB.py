from preamble import *

graycmap = plt.get_cmap('gray', 256)
gray_vals = graycmap(np.arange(256))
gray_vals[200:] = [0, 0, 1, 1] # Blue
gray_vals[:50] = [0, 1, 0, 1] # Green
plt.register_cmap('bggray', graycmap.from_list('bggray', gray_vals))

Im = np.double(imageio.imread('images/baboon.tif'))
plt.subplot(1, 2, 1)
plt.imshow(Im, 'bggray', clim=(0,255))
plt.title('blue and green image')
plt.colorbar()

plt.subplot(1, 2, 2)
plt.imshow(Im, 'jet', clim=(0,255))
plt.title('jet image')
plt.colorbar()

plt.show()

# QUESTION: 5
# The baboons nose is now blue in the jet image.
