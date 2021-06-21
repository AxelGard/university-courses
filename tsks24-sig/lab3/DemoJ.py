from preamble import *

# QUESTION: 18
# [f-3, f-2, f-1, f0, f1, f2, f3]
# [ f0  f1  f2  f3 f-3 f-2 f-1]
# [ F0  F1  F2  F3 F-3 F-2 F-1]
# [ F-3 F-2 F-1 F0  F1  F2  F3 ]


fig, axes = plt.subplots(nrows=2, ncols=2)
fig.tight_layout()

test = np.array([-3, -2, -1, 0, 1, 2, 3])
ifft = np.fft.ifftshift(test)
print(ifft)
fft = np.fft.fft(ifft)
print(fft)
fft_shift = np.fft.fftshift(fft)
print(fft_shift)
x = np.fft.fftshift(np.fft.ifft(np.fft.ifftshift(fft_shift)))
print(x)

Im = np.load('images/pirat2.npy')
IM = np.fft.fftshift(np.fft.fft2(np.fft.ifftshift(Im)))
plt.subplot(2,2,1)
plt.imshow(np.abs(IM),'gray'), plt.colorbar()
plt.title('abs')
plt.subplot(2,2,2)
plt.imshow(np.angle(IM),'gray'), plt.colorbar()
plt.title('angle')
plt.subplot(2,2,3)
plt.imshow(np.real(IM),'gray'), plt.colorbar()
plt.title('real')
plt.subplot(2,2,4)
plt.imshow(np.imag(IM),'gray'), plt.colorbar()
plt.title('imag')
plt.show()

# QUESTION: 19
# Because of some very large values in the real part meaning that all smaller values
# become very small in comparison to the larger value. This means that only the point
# with the very large value is shown and the other points with are drowned out because of
# the big scale.

fig, axes = plt.subplots(nrows=2, ncols=2)
fig.tight_layout()

maxv = -0.1*np.min(np.real(IM))
plt.subplot(2,2,1)
plt.imshow(np.abs(IM),'gray', clim=(0,maxv))
plt.colorbar()
plt.title('abs')

plt.subplot(2,2,2)
Imangle = np.angle(IM)
Imangle[np.abs(IM) < 10*np.mean(np.abs(IM))] = 0
plt.imshow(Imangle,'gray')
plt.colorbar()
plt.title('angle')

plt.subplot(2,2,3)
plt.imshow(np.real(IM),'gray', clim=(-maxv,maxv))
plt.colorbar()
plt.title('real')

plt.subplot(2,2,4)
plt.imshow(np.imag(IM),'gray', clim=(-maxv,maxv))
plt.colorbar()
plt.title('imag')
plt.show()


# QUESTION: 20
# abs: even
# angle: odd
# real: even
# imag: odd
