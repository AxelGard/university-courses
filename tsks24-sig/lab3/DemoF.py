from preamble import *

Im = np.double(imageio.imread('images/circle.tif'))

fig, axes = plt.subplots(nrows=2, ncols=2)
fig.tight_layout()

b = np.array([0.5,0.5])
b2 = np.convolve(b, b).reshape(1, 3)
d = np.array([1,-1.0])
cd = np.convolve(b, d).reshape(1, 3)
sobelx = np.array([[1., 0., -1.], [2., 0., -2.], [1., 0., -1.]]) / 8
sobely = sobelx.T

Imsobelx = signal.convolve2d(Im, sobelx, 'same')
Imsobely = signal.convolve2d(Im, sobely, 'same')

mangngrad_sobel = np.sqrt(Imsobelx**2 + Imsobely**2)

plt.subplot(2,2,1)
plt.imshow(mangngrad_sobel, 'gray',clim=(0, 160))
plt.title('mangngrad sobel')
plt.colorbar()

plt.subplot(2,2,3)
plt.imshow(mangngrad_sobel, 'gray', clim=(90, 160))
plt.title('mangngrad sobel')
plt.colorbar()

Imcdx = signal.convolve2d(Im, cd, 'same')
Imcdy = signal.convolve2d(Im, cd.T, 'same')

mangngrad_cd = np.sqrt(Imcdx**2 + Imcdy**2)

plt.subplot(2,2,2)
plt.imshow(mangngrad_cd, 'gray',clim=(0, 160))
plt.title('mangngrad centraldiff')
plt.colorbar()

plt.subplot(2,2,4)
plt.imshow(mangngrad_cd, 'gray',clim=(90, 160))
plt.title('mangngrad centraldiff')
plt.colorbar()

plt.show()

# QUESTION: 12
# [1,-1] * [1,-1] = [1,-2,1]  +
# [1; -1] * [1; -1] = [1; -2; 1]  =  [0,1,0;
#                                    1,-4,1;
#                                    0,1,0]
