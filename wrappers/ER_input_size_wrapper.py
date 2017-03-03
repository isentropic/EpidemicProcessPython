
# import glob
import time
import os
# import numpy as np
# import random
# import math

import sys


fullpath = os.path.dirname(os.path.realpath(__file__))
# additionalcomment = 'Mu=0.001'
fullpath = fullpath + "/Code"

N = int(float(sys.argv[1]))
k = int(sys.argv[2])

# N = 16e5
# k = 5

p = k/float(N)
start = time.time()
print "Creating ER network with size of %i, p: %f" % (N, p)


# os.system("g++ `root-config --cflags` `root-config --libs` -I %s %s/Particle.cpp %s/Magnet.cpp %s/main.cpp -o %s/TBMT_yay2" % (fullpath, fullpath, fullpath, fullpath, fullpath))
# os.system("%s/TBMT_yay2 %s/particle_creation/particle_creation_%d.txt " % (fullpath,fullpath, i + 1))
try:
    os.remove("%s/ER_compiled_main" % (fullpath))
except OSError:
    print "could not delete 1"
    pass

# os.system("g++  -std=c++11 -I %s %s/Linked_list.cpp %s/ER_main.cpp -o %s/ER_compiled_main" %(fullpath,fullpath,fullpath,fullpath))
os.system("g++  -std=c++11 -I %s %s/Linked_list.cpp %s/test_ER_main.cpp -o %s/ER_compiled_main" %(fullpath,fullpath,fullpath,fullpath))
# os.system("%s/ER_compiled_main %i %f" %(fullpath,N,p))
os.system("%s/ER_compiled_main %i %i" % (fullpath, N, int(k)))

try:
    os.remove("%s/ER_compiled_main" %(fullpath))
except OSError:
    "could not delete 1"
    pass


print "Time taken", time.time()-start
# os.system("python grapher.py")


'''
tree = TTree ()

tree.ReadFile("nodes_stats.txt", "node:local:global:mu")
tree.Draw ("mu", "", "colz")
raw_input ("Press Enter..")
'''
