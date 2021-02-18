import random
import sys, os
#import numpy as np
import cmath
import warnings
#from matplotlib import pyplot as plt
from types import *
#import time

if __name__ == "__main__":
    Ek_lo = float(sys.argv[1])
    Ek_hi = float(sys.argv[2])
    m = float(sys.argv[3])
    p_lo = cmath.sqrt(Ek_lo*(Ek_lo+2.0*m))
    p_hi = cmath.sqrt(Ek_hi*(Ek_hi+2.0*m))
    print p_lo*1.0e-3, p_hi*1.0e-3
