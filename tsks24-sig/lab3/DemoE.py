from preamble import *

Im = np.double(imageio.imread('images/baboon.tif'))

fig, axes = plt.subplots(nrows=2, ncols=2)
fig.tight_layout()

plt.subplot(2,2,1)
plt.imshow(Im,'gray',clim=(0,255))
plt.title('original image')
plt.colorbar()

b = np.array([0.5,0.5])
b2 = np.convolve(b, b).reshape(1, 3)
d = np.array([1,-1.0])
cd = np.convolve(b, d).reshape(1, 3)
sobelx = np.array([[1., 0., -1.], [2., 0., -2.], [1., 0., -1.]]) / 8
Imsobelx = signal.convolve2d(Im,sobelx,'same')

plt.subplot(2,2,3)
plt.imshow(Imsobelx,'gray',clim=(-128,127))
plt.title('sobelx image')
plt.colorbar()

sobely = sobelx.T
Imsobely = signal.convolve2d(Im,sobely,'same')
plt.subplot(2,2,4)
plt.imshow(Imsobely,'gray',clim=(-128,127))
plt.title('sobely image')
plt.colorbar()

mangngrad = np.sqrt(Imsobelx**2 + Imsobely**2)

plt.subplot(2,2,2)
plt.imshow(mangngrad, 'gray',clim=(0, 160))
plt.title('mangngrad image')
plt.colorbar()

plt.show()
