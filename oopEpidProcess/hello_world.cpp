#include <string>
std::string sayhello() { return "HI"; }

#include <boost/python.hpp>

BOOST_PYTHON_MODULE(libhello_world) {
  using namespace boost::python;
  def("sayhello", sayhello);
}
