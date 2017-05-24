
import glob
import time
import os
import numpy as np
import random
import math

import sys


fullpath = '~/Thesis_s/Code'
start = time.time()

N = int(float(sys.argv[1]))
p = float(sys.argv[2])
print "Creating network with size of %i, p: %f" % (N, p)

# N = 1000
# p = 0.1


# os.system("g++ `root-config --cflags` `root-config --libs` -I %s %s/Particle.cpp %s/Magnet.cpp %s/main.cpp -o %s/TBMT_yay2" % (fullpath, fullpath, fullpath, fullpath, fullpath))
# os.system("%s/TBMT_yay2 %s/particle_creation/particle_creation_%d.txt " % (fullpath,fullpath, i + 1))
os.system("rm %s/compiled_main" % (fullpath))
os.system("g++  -std=c++11 -I %s %s/Linked_list.cpp %s/main.cpp -o %s/compiled_main" %
          (fullpath, fullpath, fullpath, fullpath))

os.system("%s/compiled_main %i %f" % (fullpath, N, p))
os.system("rm %s/compiled_main" % (fullpath))

print "Elapsed Time", time.time() - start
# os.system("python grapher.py")


'''
tree = TTree ()

tree.ReadFile("nodes_stats.txt", "node:local:global:mu")
tree.Draw ("mu", "", "colz")
raw_input ("Press Enter..")
'''
