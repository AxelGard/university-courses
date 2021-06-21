from preamble import *

im1_bgr = cv2.imread('images/image1.png');
im1 = cv2.cvtColor(im1_bgr,cv2.COLOR_BGR2RGB)
y, cb, cr = jl.rgb2ycbcr(im1)
plt.figure(2), plt.imshow(y, 'gray', clim=(0, 255))
plt.title('y'), plt.colorbar()

X = 2**3
y2 = X*np.floor_divide(y,X)
plt.figure(3), plt.imshow(y2, 'gray', clim=(0, 255)), plt.title('y2 (X = 2^3)'), plt.colorbar()

# QUESTION: 3
# X = 2^3 = 8 to get 5 bpp

X = 2**2
y1 = X*np.floor_divide(y,X)
plt.figure(4), plt.imshow(y1, 'gray', clim=(0, 255)), plt.title('y1 (X = 2^2)'), plt.colorbar()
plt.show()

print(f"y2 psnr = {Error(y, y2).psnr}")
print(f"y1 psnr = {Error(y, y1).psnr}")

# QUESTION: 4
#
# y2 (half-good):
#   bpp = 8 - 3 = 5
#   psnr = 34.886041972117944
#   subjective = quite good
# y1 (good):
#   bpp = 8 - 2 = 6
#   psnr = 40.89692598660048
#   subjective = good
