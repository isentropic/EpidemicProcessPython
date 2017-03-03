// "Copyright 2017 [Zhanibek Omarov]"


#include <string>
#include <vector>
#include <random>
#include "Linked_list.hpp"

class epidemicProcess {
 public:
  epidemicProcess(std::string pathToGraph) {
    initializeGraph(pathToGraph);
    rng = std::mt19937(rd());
  }

  epidemicProcess();
  void initializeGraph(std::string pathToGraph);
  void clearTheGraph();

  void runEpidProcess(double &infectedratio, double &timepassed);


  double getAverageDegree();

 private:
  double lambda;
  double mu;
  int N;
  std::vector<Linked_list *> nodes;


  std::random_device rd;     // only used once to initialise (seed) engine
  std::mt19937 rng;
};
