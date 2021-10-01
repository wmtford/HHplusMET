import os
from ROOT import *

run1Donly = True
if run1Donly:
        for i in range(0, 54):
                h=175+i*25
                os.system("python QuickDataCardsABCDNorm_Higgsino.py %d 1" %(h))

#For 2D higgsino
else:
        namesFile=open("higgsino2DFileNames.txt", 'r');
        for line in namesFile:
                x = line.split('_')
                hino_mass = int(x[5])
                LSP_mass = int(x[6])
                if hino_mass>810: break;
                os.system("python QuickDataCardsABCDNorm_Higgsino.py %d %d" %(hino_mass,LSP_mass))

        #Add 1D scan
        for i in range(0, 27):
                h=150+i*25
                os.system("python QuickDataCardsABCDNorm_Higgsino.py %d 1" %(h))

