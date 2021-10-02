import os
from ROOT import *

for i in range(0, 17):
        g=1000+i*100
        os.system("python -b ../datacards/QuickDataCardsABCDNorm_Gluino.py %d 1" %(g))

#For 2D gluino
#namesFile=open("../src/Gluino2DScanNames_mod.txt", 'r');
#for line in namesFile:
#	x = line.split('_')
#	gluino_mass = int(x[3])
#	LSP_mass = int(x[4])
#	os.system("python -b ../datacards/QuickDataCardsABCDNorm_Gluino.py %d %d" %(gluino_mass,LSP_mass))
