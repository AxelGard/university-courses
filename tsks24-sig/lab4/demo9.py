from preamble import *

im1_bgr = cv2.imread('images/image1.png');
im1 = cv2.cvtColor(im1_bgr,cv2.COLOR_BGR2RGB)
y, cb, cr = jl.rgb2ycbcr(im1)
plt.figure(0), plt.imshow(y, 'gray', clim=(0, 255))
plt.title('original'), plt.colorbar()

Yb = jl.bdct(y, (8, 8))
Ybq = jl.bquant(Yb, jl.jpgqmtx()*0.8)

dct_block = np.arange(64)
Z = jl.zigzag_matrix()
zigzagged_block = Z@dct_block #@ means matrix multiplication in numpy
zigzag_indices = jl.zigzag_indices()

print(np.all(zigzagged_block == zigzag_indices))

plt.figure(1)
plt.plot(np.mean(np.abs(Ybq),axis=1)[1:])
plt.plot((Z@np.mean(np.abs(Ybq),axis=1))[1:])
plt.legend(["original","zig-zag"])
plt.show()


# QUESTION: 23
# Run-length encoding means that when the same value is found multiple
# times in a row it is replaced by a count of that value instead. So
# for example if we have seven 0's in a row this would be represented as 7x0 instead.
# zig-zag scanning makes this process more efficient as it groups the values
# together with similar values. So that repeating values accur more often.
# Thus making run-length encoding more efficient. 
