from preamble import *

Im = np.double(imageio.imread('images/circle.tif'))

fig, axes = plt.subplots(nrows=1, ncols=2)
fig.tight_layout()

laplace = np.array([[0., 1., 0.], [1., -4., 1.], [0., 1., 0.]])

plt.subplot(1, 2, 1)
plt.imshow(Im, 'gray', clim=(0,255))
plt.title('original image')
plt.colorbar()

Imlaplace = signal.convolve2d(Im, laplace, 'same')
plt.subplot(1, 2, 2)
plt.imshow(Imlaplace, 'gray', clim=(-200, 200))
plt.title('laplace image')
plt.colorbar()

plt.show()


# QUESTION: 13
# In the right most graph we see that there are two tops (white values in the image)
# and two bottoms (black in the image). This can be seen in the image as the edges
# of the circle, where the right top in the graph is represented in the image as the
# edge to the right in the image. And vise-versa for the other side.
