// "Copyright 2017 [Zhanibek Omarov]"
#include <boost/python.hpp>
#include <string>
#include <vector>
#include <random>
#include "Linked_list.hpp"
typedef boost::python::tuple tuple;

class epidemicProcess {
 public:
  epidemicProcess() {
  }

  // epidemicProcess();
  void initializeGraph(std::string pathToGraph);
  void clearTheGraph();

  tuple runEpidProcess(double lambda, double mu);


  double getAverageDegree();

 private:
  int N;
  std::vector<Linked_list *> nodes;
};

BOOST_PYTHON_MODULE(libpyEpidProcess) {
  using namespace boost::python;
  class_<epidemicProcess>("epidemicProcess")
        .def("initializeGraph", &epidemicProcess::initializeGraph)
        .def("clearTheGraph", &epidemicProcess::clearTheGraph)
        .def("runEpidProcess", &epidemicProcess::runEpidProcess)
        .def("getAverageDegree", &epidemicProcess::getAverageDegree)
        ;
}
