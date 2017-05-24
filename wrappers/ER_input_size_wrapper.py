import time
import os
import sys

fullpath = os.path.dirname(os.path.realpath(__file__))
fullpath = fullpath + "/.."
print fullpath

N = int(float(sys.argv[1]))
k = int(sys.argv[2])

start = time.time()
print "Creating ER network with size of %i, k: %i" % (N, k)


os.chdir("%s/networkGenerationCode" % (fullpath))
os.system(
    "g++  -std=c++11 Linked_list.cpp ER_main.cpp -o ER_compiled_main"
)
os.system("./ER_compiled_main %i %i" % (N, int(k)))

print "Time taken", time.time() - start
