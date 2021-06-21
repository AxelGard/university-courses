from preamble import *

im1_bgr = cv2.imread('images/image1.png');
im1 = cv2.cvtColor(im1_bgr,cv2.COLOR_BGR2RGB)
plt.figure(0), plt.imshow(im1)

im1r = im1[:,:,0]
im1g = im1[:,:,1]
im1b = im1[:,:,2]
plt.figure(1), plt.imshow(im1r,'gray'), plt.title('Red'), plt.colorbar()
plt.figure(2), plt.imshow(im1g,'gray'), plt.title('Green'), plt.colorbar()
plt.figure(3), plt.imshow(im1b,'gray'), plt.title('Blue'), plt.colorbar()


# QUESTION: 1
# color | R | G | B
# red = (hi, lo, lo)
# green = (lo, hi, lo)
# blue = (lo, lo, hi)
# yellow = (hi, hi, lo)
# cyan = (lo, hi, hi)
# magenta = (hi, lo, hi)
# white = (hi, hi, hi)
# black = (lo, lo, lo)


y, cb, cr = jl.rgb2ycbcr(im1)

plt.figure(4), plt.imshow(y, 'gray', clim=(0, 255)), plt.title('y'), plt.colorbar()
plt.figure(5), plt.imshow(cb, 'gray'), plt.title('cb'), plt.colorbar()
plt.figure(6), plt.imshow(cr, 'gray'), plt.title('cr'), plt.colorbar()
plt.show()

# QUESTION: 2
# We see the most amount of details in y

class Error: # Also exists in preamble
    def __init__(self, original, altered):
        self.mse = np.mean((original-altered)**2)
        self.psnr = 10*np.log10(255**2/self.mse)


print(f"psnr: {Error(y, cb).psnr}")
