from preamble import *

im1_bgr = cv2.imread('images/image1.png');
im1 = cv2.cvtColor(im1_bgr,cv2.COLOR_BGR2RGB)
y, cb, cr = jl.rgb2ycbcr(im1)
plt.figure(0), plt.imshow(y, 'gray', clim=(0, 255))
plt.title('original'), plt.colorbar()

Yb = jl.bdct(y, (8, 8))

Q1 = 50
Ybq = jl.bquant(Yb, Q1)
Ybr = jl.brec(Ybq, Q1)
yr = jl.ibdct(Ybr, (8, 8), (512, 768))
plt.figure(1), plt.imshow(yr, 'gray', clim=(0, 255))
plt.title('reconstructed quantized image (Q1 = 50)'), plt.colorbar()

print(f"psnr = {Error(y, yr).psnr}")

Q1 = 20
Ybq_ = jl.bquant(Yb, Q1)
Ybr_ = jl.brec(Ybq_, Q1)
yr_ = jl.ibdct(Ybr_, (8, 8), (512, 768))
plt.figure(2), plt.imshow(yr_, 'gray', clim=(0, 255))
plt.title('reconstructed quantized image (Q1 = 20)'), plt.colorbar()


print(f"psnr = {Error(y, yr_).psnr}")

# QUESTION: 16
# yr (half-good):
#   Q1 = 50
#   psnr = 34.67134051313512
#   subjective = quite good
# yr_ (good):
#   Q1 = 20
#   psnr = 39.32175726220302
#   subjective = good

Qm = jl.jpgqmtx()
print(Qm.reshape(8, 8))


# QUESTION: 17
"""
[[ 16  12  14  14  18  24  49  72]
 [ 11  12  13  17  22  35  64  92]
 [ 10  14  16  22  37  55  78  95]
 [ 16  19  24  29  56  64  87  98]
 [ 24  26  40  51  68  81 103 112]
 [ 40  58  57  87 109 104 121 100]
 [ 51  60  69  80 103 113 120 103]
 [ 61  55  56  62  77  92 101  99]]
"""


# QUESTION: 18
# High frequency components are quantized to longer steps.
# This means that low frequency components are more valuable to JPEG.

JPEGMEAN = np.mean(Qm)
print(JPEGMEAN)


# QUESTION: 19
# Average quantization step length
# JPEGMEAN = 57.625

for i, Q2 in [(3, 2.8), (4, 0.8)]:
    Ybq = jl.bquant(Yb, jl.jpgqmtx()*Q2)
    Ybr = jl.brec(Ybq, jl.jpgqmtx()*Q2)
    yr = jl.ibdct(Ybr, (8, 8), (512, 768))
    plt.figure(i), plt.imshow(yr, 'gray', clim=(0, 255))
    plt.title(f'JPEG (Q2 = {Q2})'), plt.colorbar()
    print(f"Q2 = {Q2}:")
    print(f"    JPEGMEAN*Q2 = {JPEGMEAN*Q2}")
    print(f"    psnr = {Error(y, yr).psnr}")

plt.show()

# QUESTION: 20
#Q2 = 2.8:
#   JPEGMEAN*Q2 = 161.35
#   psnr = 34.78608299992914
#   subjective = quite good
#Q2 = 0.8:
#   JPEGMEAN*Q2 = 46.1
#   psnr = 39.35459811876927
#   subjective = good


# QUESTION: 21
#   Q1 = 20 and JPEGMEAN*Q2 = 46.1 both gave good results
#   So around twice as much quantization. (46.1 / 20 = 2.305)
