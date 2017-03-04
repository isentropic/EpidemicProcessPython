from build import libpyEpidProcess
myobj = libpyEpidProcess.epidemicProcess()

myobj.initializeGraph("../networkRaw/total_graph.txt")
for i in range(100):
	print myobj.runEpidProcess(0.5,0.5);
