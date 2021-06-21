from preamble import *
from costrans3 import *


# QUESTION: 9
# dct seems to be very similar to the lower right quadrant of fft2.

# QUESTION: 10
# When we use fft2 we get high frequencies at the edges of the image.
# This is because the transformation wraps around to the other side of the image
# and this results in a sudden change of color (i.e. a high frequency).
# But when using dct we do not get this effect.

print(f"Maximum difference: {np.max(np.abs(A-BPART))}")

# QUESTION: 11
# Maximum difference: 3.637978807091713e-12
