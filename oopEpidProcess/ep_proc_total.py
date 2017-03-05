# import glob
import os
import numpy as np
# import random
# import math
# import matplotlib as mpl
# import matplotlib.pyplot as plt
import time
import multiprocessing as mp

print "\nSTARTING The Process ... \n"
starting_time = time.time()

# fullpath = '/cappraid01/users/zhanibek/Thesis_s/epid_process'
fullpath = os.path.dirname(os.path.realpath(__file__))
# additionalcomment = 'Mu=0.001'
fullpath = fullpath + "/epid_process"
print "The path is: ", fullpath


additionalcomment = '_ER_4'  # Additional Comment before results directory name
samplesizes = ['4e3', '8e3', '16e3', '32e3']  # Network Sizes
samplename = ''

samplename = ''
for i in samplesizes:
    samplename = samplename + i
samplename = samplename + additionalcomment

os.system('mkdir -p %s/results/randPunited%s/P_infVsLambda' %
          (fullpath, samplename))
os.system('mkdir -p %s/results/randPunited%s/rvslambda' %
          (fullpath, samplename))

parallel_samples = int(1e5)

no_of_lambda = 51
lambda_min = 0.15
lambda_max = 0.25
no_of_workers = mp.cpu_count() - 5
themu = 0.4

increment = (lambda_max - lambda_min) / (no_of_lambda - 1)
lambdas = np.linspace(lambda_min, lambda_max, no_of_lambda)
mus = np.zeros(no_of_lambda)

for i in range(no_of_lambda):
    mus[i] = lambdas[i]

inputwork = []

for i in range(no_of_lambda):
    for j in range(parallel_samples):
        inputwork.append((lambdas[i], mus[i]))
inputwork = np.array(inputwork)


def get_total_value(proc_id, lambdaval, T):
    os.system("%s/total_epid_proc %s %f %f" %
              (fullpath, proc_id, lambdaval, T))
    with open('%s/epid_temp_total_files/remaining_total_nodes_%s.txt' % (fullpath, proc_id), 'r') as theresultsfile:
        filelines = theresultsfile.readlines()
        time = float(filelines[0])
        ratio = float(filelines[1])
        return (time, ratio)


def worker((lambdaval, T)):
    return get_total_value(str(mp.current_process().name), lambdaval, T)


if __name__ == '__main__':

    try:
        os.remove("%s/epid_proc" % (fullpath))
    except OSError:
        "could not delete 1"
        pass

    os.system("g++  -std=c++11 -I %s %s/Linked_list.cpp %s/epid_process_total.cpp -o %s/total_epid_proc" %
              (fullpath, fullpath, fullpath, fullpath))

    # MAIN Starts Here

    pool = mp.Pool(processes=no_of_workers)
    for asize in samplesizes:

        os.system("python ER_input_size_wrapper.py %s 5" % (asize))
        # os.system("python input_size_wrapper.py %s 0.1"%(asize))

        my1stfile = open('%s/results/randPunited%s/P_infVsLambda/P_inf_raw%s.txt' %
                         (fullpath, samplename, asize), 'w')
        my2ndfile = open('%s/results/randPunited%s/rvslambda/raw_results%s.txt' %
                         (fullpath, samplename, asize), 'w')

        my1stfile.write("#lambda\ttime\tsurvivers\n")
        my2ndfile.write('#ration\tlambda\tmu\n')

        print "Workers Start the Process"
        pretotalresults = []
        pretotalresults = (pool.map(worker, inputwork))
        print "Process ended"
        # pool.start()
        # pool.close()
        # pool.join()

        # print pretotalresults
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

    os.system("rm %s/time_epid_proc" % (fullpath))

    print "\n PROCESS FINISHED, total elapsed time: ", time.time() - starting_time
    pool.close()  # no more tasks
