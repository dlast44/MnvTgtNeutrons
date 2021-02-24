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
    maxE = sys.argv[3]

    for i in range(n_subruns):
        os.system("python Target5FluxWriter_NoMuon.py 0 "+maxE+" 139.571 "+run+" 1000 211 PiPlus")
        os.system("python Target5FluxWriter_NoMuon.py 0 "+maxE+" 139.571 "+run+" 1000 -211 PiMinus")
        os.system("python Target5FluxWriter_NoMuon.py 0 "+maxE+" 134.977 "+run+" 1000 111 Pi0")
