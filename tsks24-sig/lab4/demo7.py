from preamble import *

im1_bgr = cv2.imread('images/image1.png');
im1 = cv2.cvtColor(im1_bgr,cv2.COLOR_BGR2RGB)
y, cb, cr = jl.rgb2ycbcr(im1)
plt.figure(0), plt.imshow(y, 'gray', clim=(0, 255))
plt.title('original'), plt.colorbar()

Yb = jl.bdct(y, (8, 8))
ulim = np.max(np.abs(Yb))/10
plt.figure(1), plt.imshow(np.abs(Yb), 'gray', clim=(0, ulim))
plt.title('bdct transformed'), plt.colorbar()

yn = jl.ibdct(Yb, (8, 8), (512, 768))
plt.figure(2), plt.imshow(yn, 'gray', clim=(0, 255))
plt.title('inverse transformed'), plt.colorbar()

print(f"Maximum difference: {np.max(np.abs(y-yn))}")
# QUESTION: 12
# Maximum difference: 5.968558980384842e-13

Yb = jl.bdct(y, (8, 8))
Ybq = np.zeros(Yb.shape)
Ybq[(0, 1, 8, 9), :] = np.round(Yb[(0, 1, 8, 9), :])
yq2 = jl.ibdct(Ybq, (8, 8), (512, 768))
plt.figure(3), plt.imshow(yq2, 'gray', clim=(0, 255))
plt.title('coded image 4 coefficients'), plt.colorbar()


print(f"Ybq min = {np.min(Ybq)} and Ybq max = {np.max(Ybq)}")

# QUESTION: 13
# Ybq min = -470.0 and Ybq max = 1880.0
# 1 bit for sign and 11 bits for value as 2^10 < 1880 < 2^11

print(f"psnr = {Error(y, yq2).psnr}")

# QUESTION: 14
#   bdct image seems worse around the eyes when compared to the dct image
# bdct:
#   bpp = 0.75
#   psnr = 30.062575826631015
# dct:
#   bpp = 18 / 16 = 1.125
#   psnr = 31.033776619093945

Ybq9 = np.zeros(Yb.shape)
Ybq9[(0, 1, 2, 8, 9, 10, 16, 17, 18), :] = np.round(Yb[(0, 1, 2, 8, 9, 10, 16, 17, 18), :])
yq29 = jl.ibdct(Ybq9, (8, 8), (512, 768))
plt.figure(4), plt.imshow(yq29, 'gray', clim=(0, 255))
plt.title('coded image 9 coefficients'), plt.colorbar()
plt.show()

print(f"Ybq9 min = {np.min(Ybq9)} and Ybq9 max = {np.max(Ybq9)}")
print(f"psnr = {Error(y, yq29).psnr}")

# QUESTION: 15
# Ybq9 min = -470.0 and Ybq9 max = 1880.0 => same 12 bits needed
# bdct (4 coeffs):
#   bpp = 0.75
#   psnr = 30.062575826631015
#   subjective = quite bad
# bdct (9 coeffs):
#   (we keep 9 out of 64 coefficients so we keep 9/64)
#   bpp = 12 * (9/64) = 12 * 0.140625 = 1.6875
#   psnr = 32.684623950013496
#   subjective = decent
