// "Copyright 2017 [Zhanibek Omarov]"

#include "Linked_list.hpp"
#include "epidemicProcess.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>

std::random_device rd;
std::mt19937 rng(rd());
std::uniform_int_distribution<int> getRandomNode;

void epidemicProcess::initializeGraph(std::string thepath) {
  std::ifstream nodes_file(thepath);

  if (nodes_file.is_open()) {
    // std::cout<<"files are opened"<<"\n";
  } else {
    std::cout << "couldnt open files, terminating"
              << "\n";
    throw 1;
  }

  std::string line;
  while (std::getline(nodes_file, line)) {
    std::istringstream iss(line);
    int a;
    int head;
    bool first = true;
    while (iss >> a) {
      if (first) {
        nodes.push_back(new Linked_list(a, false));
        first = false;
        head = a;
      } else {
        nodes[head - 1]->add_Node(a);
        // nodes[head-1]->connectivity++;
        // already increased within linkedlist.cpp
      }
    }
  }
  N = nodes.size();
}

void epidemicProcess::clearTheGraph() {
  for (int i = 0; i < N; i++) {
    nodes[i]->infected = false;
    nodes[i]->exposed = false;
    nodes[i]->active = false;
  }
}

void epidemicProcess::runEpidProcess(double &infectedratio,
                                     double &timepassed) {
  int infected = getRandomNode(rng);
  nodes[infected - 1]->infect();
  // std::cout<<"Random node to be infected: "<<infected<<"\n";
  // the dynamics
  std::vector<int> neighbors;

  int active_nodes = 1;
  int infected_nodes = 1;
  // std::vector<int> infecting_list;
  std::vector<int> new_infected_list;
  std::vector<int> new_exposing_list;

  std::uniform_real_distribution<float> random_no(0, 1.0);

  double time = 1.0;

  while (active_nodes > 0) {
    // myfile<<time<<"\t";
    for (int i = 0; i < N; i++) {
      // time = time + 1/double(active_nodes);
      if (nodes[i]->infected && nodes[i]->active) {
        time = time + 1.0 / double(active_nodes);
        neighbors = nodes[i]->containing_nodes();
        for (int j = 0; j < neighbors.size(); j++) {
          if (!nodes[neighbors[j] - 1]->infected &&
              !nodes[neighbors[j] - 1]->exposed) {
            float infecting_chance = random_no(rng);
            // std::cout<<infecting_chance<<"infecting chance\n";
            if (infecting_chance < lambda) {
              new_infected_list.push_back(neighbors[j]);
            } else {
              // std::cout<<"A node is exposed \n";
              new_exposing_list.push_back(neighbors[j]);
            }
          } else if (!nodes[neighbors[j] - 1]->infected &&
                     nodes[neighbors[j] - 1]->exposed) {
            if (random_no(rng) < mu) {
              new_infected_list.push_back(neighbors[j]);
            }
          }
        }
        nodes[i]->active = false;
        neighbors.clear();
      }
    }

    // exposing
    if (!new_exposing_list.empty()) {
      for (int j = 0; j < new_exposing_list.size(); j++) {
        nodes[new_exposing_list[j] - 1]->exposed = true;
      }
    }
    new_exposing_list.clear();

    // infecting
    if (!new_infected_list.empty()) {
      for (int j = 0; j < new_infected_list.size(); j++) {
        nodes[new_infected_list[j] - 1]->infect();
      }
    }

    new_infected_list.clear();

    active_nodes = 0;
    infected_nodes = 0;
    for (int i = 0; i < N; i++) {
      if (nodes[i]->active) {
        active_nodes++;
      }
      if (nodes[i]->infected)
        infected_nodes++;
    }
  } // for loop ended

  int infected_after = 0;
  for (int i = 0; i < N; i++) {
    if (nodes[i]->infected) {
      infected_after++;
    }
  }

  infectedratio = float(infected_after) / N;
  timepassed = time;
}

double epidemicProcess::getAverageDegree() {
  int degree = 0;
  for (int i = 0; i < N; i++) {
    degree += nodes[i]->connectivity;
  }
  return degree / double(N);
}
