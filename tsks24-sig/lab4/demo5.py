from preamble import *

im1_bgr = cv2.imread('images/image1.png');
im1 = cv2.cvtColor(im1_bgr,cv2.COLOR_BGR2RGB)
y, cb, cr = jl.rgb2ycbcr(im1)
plt.figure(0), plt.imshow(y, 'gray', clim=(0, 255))
plt.title('original'), plt.colorbar()

Y = cv2.dct(y)
plt.figure(1), plt.imshow(np.log(np.abs(Y)+1),'gray')
plt.title('2d cosine transform'), plt.colorbar()

Yq = np.zeros((512,768))
Yq[0:128,0:196] = np.round(Y[0:128,0:196])
plt.figure(2), plt.imshow(np.log(np.abs(Yq)+1),'gray')
plt.title('2d cosine transform (take 1/16 coeffs)'), plt.colorbar()
yq = cv2.idct(Yq)
plt.figure(3), plt.imshow(yq,'gray', clim=(0,255))
plt.title('coded image (yq)'), plt.colorbar()
plt.show()

print(f"Yq min = {np.min(Yq)} and Yq max = {np.max(Yq)}")

# QUESTION: 7
# Yq min = -9437.0 and Yq max = 68923.0
# 1 bit for sign and 17 bits for value as 2^17 > 68923 > 2^16

print(Error(y, yq).psnr)

# QUESTION: 8
# yq:
#   bpp = 18 / 16 = 1.125
#   psnr = 31.033776619093945
#   subjective = decent
