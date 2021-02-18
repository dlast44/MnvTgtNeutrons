import random
import sys, os
#import numpy as np
import math
import warnings
#from matplotlib import pyplot as plt
from types import *
#import time
import glob

if __name__ == "__main__":
    n_subruns = int(sys.argv[1])
    run = sys.argv[2]

    for i in range(n_subruns):
        os.system("python Target4FluxWriter.py 0 5000 938.272 "+run+" 1000 2212 Proton")
