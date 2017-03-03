// "Copyright 2017 [Zhanibek Omarov]"

#include <vector>
#include <string>
#include "Linked_list.hpp"


class epidemicProcess {
 public:
  epidemicProcess(std::string pathToGraph) {
    initializeGraph(pathToGraph);
  }

  void initializeGraph(std::string pathToGraph);
  void clearTheGraph();
  void runEpidProcess(double &infectedratio, double &timepassed);
  void startEpidProcess(int nodetostart);
  void startEpidProcess();

  double getAverageDegree()

 private:
  double lambda;
  double mu;
  int N;
  std::vector<Linked_list*> nodes;
};
