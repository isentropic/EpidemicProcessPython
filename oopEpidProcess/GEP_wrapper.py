"""This module runs epidemic proocesses using multiple processes."""
import os
import numpy as np
import time
import multiprocessing as mp
from build import libpyEpidProcess


starting_time = time.time()
fullpath = os.path.dirname(os.path.realpath(__file__))

additionalcomment = ''
samplesizes = ['2e3','4e3', '8e3']
parallel_samples = int(1e3)
no_of_lambda = 51
lambda_min = 0.15
lambda_max = 0.25

mu_equals_lambda = False
themu = 0.4

no_of_workers = mp.cpu_count()-1


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
    if not mu_equals_lambda:
        mus[i] = lambdas[i]
    else:
        mus[i] = themu

"""def worker(thetup):
    global graphObjects
    number = int(mp.current_process().name[11:])
    number = number % no_of_workers
    shortname = 'PoolWorker-'+str(number)
    if shortname in graphObjects:
        theObject = graphObjects[shortname]
        theresult = theObject.runEpidProcess(thetup[0], thetup[1])
        theObject.clearTheGraph()
        # print "Right Process accessing: ", mp.current_process().name
        # print theresult
        return theresult
    else:
        print "Wrong Process accessing", mp.current_process().name
        return (0, 0)"""


class GraphProcess(mp.Process):
    """Extends mp.Process, epidProcess object wrapper."""

    def __init__(self, inputQueue, resultsQueue, graphObject):
        """Constructor of the GraphProcess."""
        super(GraphProcess, self).__init__()
        self.resultsQueue = resultsQueue
        self.inputQueue = inputQueue
        self.graphObject = graphObject
        self.graphObject.initializeGraph("../networkRaw/total_graph.txt")

    def run(self):
        """Run epidemic processes till there are works in queue."""
        try:
            for theTup in iter(self.inputQueue.get, 'STOP'):
                result = self.graphObject.runEpidProcess(theTup[0], theTup[1])
                result = theTup + (result[0], result[1],)  # concatenation
                self.graphObject.clearTheGraph()
                self.resultsQueue.put(result)
        except Exception, e:
            self.resultsQueue.put("%s fail %s with: %s" %
                                  (mp.current_process().name, e.message))
        return True


def getTasksQueue():
    """Return jobs as queue of tuples made of (lambda, mu) with "STOP"."""
    inputtasks = mp.Queue()
    for i in range(no_of_lambda):
        for j in range(parallel_samples):
            inputtasks.put((lambdas[i], mus[i]))
    for i in range(no_of_workers):
        inputtasks.put("STOP")
    return inputtasks


os.system('mkdir -p %s/../results/pGEP%s/p_inf' % (fullpath, samplename))
os.system('mkdir -p %s/../results/pGEP%s/rvslambda' % (fullpath, samplename))

for asize in samplesizes:
    os.system("python ../wrappers/ER_input_size_wrapper.py %s 5" % (asize))

    inputQ = getTasksQueue()

    processes = []
    resultsQ = mp.Queue()
    for w in range(no_of_workers):
        p = GraphProcess(inputQ, resultsQ, libpyEpidProcess.epidemicProcess())
        p.start()
        processes.append(p)
    print "Processes Started"
    for p in processes:
        p.join()
    # for p in processes:
    print "Processes Ended"
    resultsQ.put("STOP")

    my3rdfile = open('%s/../results/pGEP%s/raw_totalres%s.txt' %
                     (fullpath, samplename, asize), 'w')

    my3rdfile.write("#lambda\tmu\tr\ttime\n")
    for result in iter(resultsQ.get, "STOP"):
        my3rdfile.write("%f\t%f\t%f\t%f\n" %
                        (result[0], result[1], result[2], result[3]))
    my3rdfile.close()

'''
if __name__ == '__main__':
    pool = mp.Pool(no_of_workers)
    os.system('mkdir -p %s/../results/pGEP%s/p_inf' % (fullpath, samplename))
    os.system('mkdir -p %s/../results/pGEP%s/rvslambda' % (fullpath, samplename))

    for asize in samplesizes:
        os.system("python ../wrappers/ER_input_size_wrapper.py %s 5" % (asize))
        # os.system("python input_size_wrapper.py %s 0.1"%(asize))

        for i in range(no_of_workers):
            graphObjects["PoolWorker-%i" % (i+1)].initializeGraph(
                        "../networkRaw/total_graph.txt")

        my1stfile = open('%s/../results/pGEP%s/p_inf/raw_pinfres%s.txt' %
                         (fullpath, samplename, asize), 'w')
        my2ndfile = open('%s/../results/pGEP%s/rvslambda/raw_rvslres%s.txt' %
                         (fullpath, samplename, asize), 'w')

        my1stfile.write("#lambda\ttime\tsurvivers\n")
        my2ndfile.write('#ration\tlambda\tmu\n')

        print "Workers Start the Process"
        pretotalresults = []
        pretotalresults = pool.map(worker, inputwork)
        print "Process ended"

        for i in range(no_of_lambda):
            print "At lambda = %f" % (lambdas[i])

            terminating_times = []
            terminating_ratios = []
            for j in range(parallel_samples):
                for tpl in pretotalresults[i * parallel_samples:(i + 1) * parallel_samples]:
                    terminating_times.append(tpl[0])
                    terminating_ratios.append(tpl[1])

            time_init = 1.0
            time_fin = max(terminating_times)
            steps_for_t = 1000
            timearray = np.linspace(time_init, time_fin, steps_for_t)

            # Deal with P_raw
            survivers = np.zeros(steps_for_t)

            for j in range(steps_for_t):
                survivingratio = 0.0
                for k in range(parallel_samples):
                    if terminating_times[k] > timearray[j]:
                        survivingratio = survivingratio + 1.0

                survivingratio = survivingratio / float(parallel_samples)
                survivers[j] = survivingratio
                my1stfile.write("%f\t%f\t%f\n" %
                                (lambdas[i], timearray[j], survivers[j]))

            # Deal with with rsmall_raw
            for k in range(parallel_samples):
                my2ndfile.write("%f\t%f\t%f\n" %
                                (terminating_ratios[k], lambdas[i], mus[i]))

    print "\n PROCESS FINISHED, total elapsed time: ", time.time() - starting_time
    pool.close()
'''
