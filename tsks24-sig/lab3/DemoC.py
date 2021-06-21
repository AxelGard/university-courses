from preamble import *

Im = np.double(imageio.imread('images/baboon.tif'))

fig, axes = plt.subplots(nrows=2, ncols=2)
fig.tight_layout()

plt.subplot(2, 2, 1)
plt.imshow(Im, 'gray', clim=(0,255))
plt.title('original image')
plt.colorbar()

b = np.array([0.5, 0.5])
b2 = np.convolve(b, b).reshape(1, 3)
aver = signal.convolve2d(b2, b2.T)

Imaver = signal.convolve2d(Im, aver, 'same')
plt.subplot(2, 2, 2)
plt.imshow(Imaver, 'gray', clim=(0,255))
plt.title('convolved image')
plt.colorbar()

Imaver_ = signal.convolve2d(Im, aver * 2, 'same')
plt.subplot(2, 2, 3)
plt.imshow(Imaver_, 'gray', clim=(0,255))
plt.title('convolved (factor = 8), image')
plt.colorbar()

Imaver = signal.convolve2d(Imaver, aver, 'same')
Imaver = signal.convolve2d(Imaver, aver, 'same')
plt.subplot(2, 2, 4)
plt.imshow(Imaver, 'gray', clim=(0,255))
plt.title('convolved (3 times) image')
plt.colorbar()

plt.show()

# QUESTION: 6
# Small details in the image cease to exist.

# QUESTION: 7
# Defines the size of the output image. same means the same size.

# QUESTION: 8
# With a lower normalization factor the high values (white) become weighted more meaning
# the image will be more white. (at very high values the image will be completely white)

# QUESTION: 9
# The image loses more and more details
