import random
import sys, os
#import numpy as np
import math
import warnings
#from matplotlib import pyplot as plt
from types import *
#import time
import glob

def RandTargetXYZ(targ):
    radius=500.0
    angle=random.random()*2.0*math.pi
    vx=radius*math.cos(angle)
    vy=radius*math.sin(angle)
    zStarts=[4470.0,4690.0,4908.0,5250.0,5638.0,5770.0]
    zEnds=[4495.0,4715.0,4984.0,5350.0,5645.0,5782.0]
    vz=zStarts[targ]+random.random()*(zEnds[targ]-zStarts[targ])
    return vx,vy,vz

if __name__ == "__main__":
    Ek_lo = float(sys.argv[1])
    Ek_hi = float(sys.argv[2])
    m = float(sys.argv[3])
    run = sys.argv[4]
    n_event = int(sys.argv[5])
    pdg = sys.argv[6]
    name = sys.argv[7]

    filedir = "Run_"+run+"_ALLTargets_"+name+"PlusMuon_PDG_"+pdg+"_Ek_lo_"+str(Ek_lo)+"_Ek_hi_"+str(Ek_hi)
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
        target = (int)(random.random()*6.0)
        x,y,z = RandTargetXYZ(target)
        outfile.writelines(pdg+" "+str(x)+" "+str(y)+" "+str(z)+" "+str(px)+" "+str(py)+" "+str(pz)+" "+str(E)+"\n")
        outfile.writelines("-13 "+str(x)+" "+str(y)+" "+str(z)+" 0.0 0.0 5000.0 5001.1171242\n")
        outfile.writelines("\n")
    outfile.close()
