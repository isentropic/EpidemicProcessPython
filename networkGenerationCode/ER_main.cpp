// Copyright <Zhanibek> 2017

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "Linked_list.hpp"

int N = 8e3; // = 5000; //Number of Nodes

float mu_con = 1; // fraction intercommunity links
float p = mu_con;

// std::default_random_engine eng((std::random_device())());
// std::uniform_real_distribution<float> fdis(0, 1);

void print_vector(std::vector<int> the_vector) {
  std::cout << "_______________\n";
  for (int j = 0; j < the_vector.size(); j++) {
    std::cout << the_vector[j] << "\n";
  }
  std::cout << "_______________\n";
}

int sum_vector(std::vector<int> the_vector) {
  int sum = 0;
  for (int j = 0; j < the_vector.size(); j++) {
    sum = sum + the_vector[j];
  }
  return sum;
}

int main(int argc, const char *argv[]) {
  //////// **INPUT PARAMETER DECLARATION///////////
  // id_of_process = argv[1];
  N = atoi(argv[1]);
  int k = atoi(argv[2]);
  // N = atoi(argv[4]);
  ////////////////////////////////////////////////

  std::vector<Linked_list *> nodes(N);
  std::cout << "Nodes[N] " << N << "initialized\n";
  for (int i = 0; i < N; i++) {
    nodes[i] = new Linked_list(i + 1, false);
  }

  srand((unsigned)time(NULL));
  // nodes[nodes_b4community+i]->add_Node(nodes_b4community+k+1);

  int total_links = N * k / 2;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> random_node(1, N);
  int node1;
  int node2;

  int progress = 0;

  for (int i = 0; i < total_links; i++) {
    if (float(i) / total_links * 100 > progress) {
      std::cout << progress << "%         \r";
      progress++;
    }
    node1 = random_node(gen);
    node2 = random_node(gen);
    if (nodes[node1 - 1]->contains(node2) == false && node1 != node2) {

      nodes[node1 - 1]->add_Node(node2);
      nodes[node2 - 1]->add_Node(node1);

      nodes[node1 - 1]->connectivity++;
      nodes[node2 - 1]->connectivity++;
    }
  }
  std::cout << "\n";
  // NETWORK FORMED

  // FILE OUTPUT

  // degree distribution
  std::ofstream degree_distr("../networkRaw/degree_distr_ER.txt");
  for (int i = 0; i < N; i++) {
    degree_distr << i << "\t" << nodes[i]->connectivity << "\n";
  }

  ///
  std::ofstream total_graph("../networkRaw/total_graph.txt");
  // total_graph<<"#node\tneighbors ... \tNodes: "<<N<<"\n";
  std::vector<int> temp_nodes;
  for (int i = 0; i < N; i++) {
    temp_nodes = nodes[i]->containing_nodes();
    total_graph << i + 1;
    for (int j = 0; j < temp_nodes.size(); j++) {
      total_graph << "\t" << temp_nodes[j];
    }
    total_graph << "\n";
    temp_nodes.clear();
  }

  std::cout << "Success!\n";
  return 0;
}
