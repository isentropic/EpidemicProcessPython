"""This module runs epidemic proocesses using multiple processes"""
import os
import numpy as np
# import random
# import math
# import matplotlib as mpl
# import matplotlib.pyplot as plt
import time
import multiprocessing as mp
from build import libpyEpidProcess


starting_time = time.time()
fullpath = os.path.dirname(os.path.realpath(__file__))

additionalcomment = ''
samplesizes = ['4e3', '8e3']
parallel_samples = int(1e1)
no_of_lambda = 11
lambda_min = 0.15
lambda_max = 0.25

mu_equals_lambda = False
themu = 0.4

no_of_workers = mp.cpu_count() - 1


print "..................................."
print "\n ...  STARTING The Process  ... \n"
print "The path is: ", fullpath


samplename = ''
for i in samplesizes:
    samplename = samplename + i
samplename = samplename + additionalcomment

increment = (lambda_max - lambda_min) / (no_of_lambda - 1)
lambdas = np.linspace(lambda_min, lambda_max, no_of_lambda)
mus = np.zeros(no_of_lambda)

for i in range(no_of_lambda):
    if mu_equals_lambda:
        mus[i] = lambdas[i]
    else:
        mus[i] = themu

inputwork = []
for i in range(no_of_lambda):
    for j in range(parallel_samples):
        inputwork.append((lambdas[i], mus[i]))
inputwork = np.array(inputwork)


os.system('mkdir -p %s/../results/pGEP%s/p_inf' % (fullpath, samplename))
os.system('mkdir -p %s/../results/pGEP%s/rvslambda' % (fullpath, samplename))


graphObjects = {}
for i in range(no_of_workers):
    graphObjects["PoolWorker-%i" % (i+1)] = libpyEpidProcess.epidemicProcess()
    graphObjects["PoolWorker-%i" % (i+1)].initializeGraph(
                                "../networkRaw/total_graph.txt")
# get_total_value(proc_id, lambdaval, T):
# 'PoolWorker-2'


def worker(muandlambda):
    """...Worker function to return tuples..."""

    theObject = graphObjects[mp.current_process.name]
    theresult = theObject.runEpidProcess(muandlambda[0], muandlambda[1])
    theObject.clearTheGraph()
    return theresult


pool = mp.Pool(processes=no_of_workers)

print "Workers Start the Process"
pretotalresults = []
pretotalresults = (pool.map(worker, inputwork))
print "Process ended"
