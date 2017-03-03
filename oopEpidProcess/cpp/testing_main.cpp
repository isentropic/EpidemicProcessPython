// Copyright <Zhanibek> 2017

#include <iostream>
#include "epidemicProcess.hpp"
int main(int argc, char const *argv[]) {
  /* code */
  epidemicProcess theProcess ("../../networkRaw/total_graph.txt");

  double rtime = 0;
  double ratio = 0;

  theProcess.runEpidProcess(ratio, rtime);


  std::cout << "The Ratio: " << ratio << '\n';
  std::cout << "The time" << rtime << '\n';
  return 0;
}
