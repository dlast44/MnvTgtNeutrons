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
    Ek_lo = float(sys.argv[1])
    Ek_hi = float(sys.argv[2])
    m = float(sys.argv[3])
    run = sys.argv[4]
    n_event = int(sys.argv[5])
    pdg = sys.argv[6]
    name = sys.argv[7]

    filedir = "Run_"+run+"_Target4_"+name+"_PDG_"+pdg+"_Ek_lo_"+str(Ek_lo)+"_Ek_hi_"+str(Ek_hi)
    filebase ="SIM_ExtParticle_"
    while (not len(glob.glob1("../fluxesForPC/",filedir))):
        print "CREATING DIRECTORY!"
        os.system("mkdir ../fluxesForPC/"+filedir)

    number = len(glob.glob1("../fluxesForPC/"+filedir+"/",filebase+"*.txt"))+1
    print number
    outfile = open("../fluxesForPC/"+filedir+"/"+filebase+"{num:04d}.txt".format(num=number),"w")
    
    for i in range(n_event):
        E = random.random()*(Ek_hi-Ek_lo)+Ek_lo+m
        theta = random.random()*math.pi
        phi = random.random()*2.0*math.pi-math.pi
        p = math.sqrt(E**2.0-m**2.0)
        px = p*math.sin(theta)*math.cos(phi)
        py = p*math.sin(theta)*math.sin(phi)
        pz = p*math.cos(theta)
        x = random.random()*500.0-250.0
        y = random.random()*500.0-250.0
        z = random.random()*7.0+5638.0
        outfile.writelines(pdg+" "+str(x)+" "+str(y)+" "+str(z)+" "+str(px)+" "+str(py)+" "+str(pz)+" "+str(E)+"\n")
        outfile.writelines("\n")
    outfile.close()
