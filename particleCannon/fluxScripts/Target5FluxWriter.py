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

    filedir = "Run_"+run+"_Target5_"+name+"PlusMuon_PDG_"+pdg+"_Ek_lo_"+str(Ek_lo)+"_Ek_hi_"+str(Ek_hi)
    filebase ="SIM_ExtParticle_"
    while (not len(glob.glob1("../../fluxes_for_PC/",filedir))):
        print "CREATING DIRECTORY!"
        os.system("mkdir ../../fluxes_for_PC/"+filedir)

    number = len(glob.glob1("../../fluxes_for_PC/"+filedir+"/",filebase+"*.txt"))+1
    print number
    outfile = open("../../fluxes_for_PC/"+filedir+"/"+filebase+"{num:04d}.txt".format(num=number),"w")
    
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
        z = random.random()*12.0+5770.0
        outfile.writelines(pdg+" "+str(x)+" "+str(y)+" "+str(z)+" "+str(px)+" "+str(py)+" "+str(pz)+" "+str(E)+"\n")
        outfile.writelines("-13 "+str(x)+" "+str(y)+" "+str(z)+" 0.0 0.0 3000.0 3001.86009594\n")
        outfile.writelines("\n")
    outfile.close()
