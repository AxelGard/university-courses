from preamble import *

im1_bgr = cv2.imread('images/image1.png');
im1 = cv2.cvtColor(im1_bgr,cv2.COLOR_BGR2RGB)
y, cb, cr = jl.rgb2ycbcr(im1)
plt.figure(0), plt.imshow(y, 'gray', clim=(0, 255))
plt.title('original'), plt.colorbar()


factor = 2.
y3 = np.floor(ndimage.interpolation.zoom(y, 1./factor, order=3))
y4 = ndimage.interpolation.zoom(y3, factor, order=3)
plt.figure(1), plt.imshow(y4, 'gray', clim=(0, 255))
plt.title(f'Down-sampled then up-sampled (factor = {factor})'), plt.colorbar()

factor = 1.5
y5 = np.floor(ndimage.interpolation.zoom(y, 1./factor, order=3))
print(y5.shape)
y6 = ndimage.interpolation.zoom(y5, factor, order=3)
plt.figure(2), plt.imshow(y6, 'gray', clim=(0, 255))
plt.title(f'Down-sampled then up-sampled (factor = {factor})'), plt.colorbar()


print(f"y4 psnr = {Error(y, y4).psnr}")
print(f"y6 psnr = {Error(y, y6).psnr}")
# QUESTION: 5
# y4 (half-good):
#   bpp = 2
#   psnr = 35.140862766882506
#   subjective = quite good
# y6 (good):
#   bpp = floor(512/1.5)^2 * 8 / 512^2 ~ 3.5
#   psnr = 39.78324991453115
#   subjective = good

diff = np.abs(y - y4)
plt.figure(3), plt.imshow(diff, 'gray', clim=(np.min(diff), np.max(diff)))
plt.title(f'absolute differense between y and y4'), plt.colorbar()
plt.show()

# QUESTION: 6
# The largest errors are located close to the eyes of the parrots.
# In places with a high frequency.
