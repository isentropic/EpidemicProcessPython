#include <iostream>
#include <fstream>
#include <iomanip> 
#include <cmath>
#include <random>
#include <vector>
#include <string>
#include <sstream>
#include "Linked_list.hpp"

std::string id_of_process = "0";
std::string path = "/cappraid01/users/zhanibek/Thesis_s/";
double lambda = 0.01;
double T = 0.01;
//int id_of_process = 0;


void print_vector(std::vector<int> the_vector){
  std::cout<<"_______________\n";
  for (int j = 0; j<the_vector.size(); j++){
    std::cout<<the_vector[j]<< "\n";
  }
  std::cout<<"_______________\n";
}

int sum_vector(std::vector<int> the_vector){
  int sum = 0;
  for (int j = 0; j<the_vector.size(); j++){
    sum = sum + the_vector[j];
  }
  return sum;
}


int main(int argc, const char * argv[]) {

  id_of_process = argv[1];
  lambda = atof(argv[2]);
  T = atof(argv[3]);
  //N = atoi(argv[4]);
	

  std::vector<int> communities;
  std::vector<Linked_list*> nodes;

  std::ifstream communities_file(path+"communities.txt");
  std::ifstream nodes_file(path+"total_graph.txt");
  if (communities_file.is_open() && nodes_file.is_open()){
    //std::cout<<"files are opened"<<"\n";
  }
  else{
    std::cout<<"couldnt open files, terminating"<<"\n";
    return 0;
  }
  std::string line;
  while (std::getline(nodes_file, line))
    {
      std::istringstream iss(line);
      int a;
      int head;
      bool first = true;
      while(iss>>a){
	if(first){
	  nodes.push_back(new Linked_list(a, false));
	  first = false;
	  head = a;
	}
	else{
	  nodes[head-1]->add_Node(a);
	  nodes[head-1]->connectivity++;
	}
      }
      //std::cout<<"\n newline \n";

      // process pair (a,b)
    }
  int N = nodes.size();
  //std::cout<<nodes.size()<<" size of the nodes\n";


  std::random_device rd;     // only used once to initialise (seed) engine
  std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
  std::uniform_int_distribution<int> uni(1, (int) N); // guaranteed unbiased

  int infected = uni(rng);
  nodes[infected-1]->infect();
  //std::cout<<"Random node to be infected: "<<infected<<"\n";
  //the dynamics
  std::vector<int> neighbors;

  int active_nodes = 1;
  int infected_nodes = 1;
  //std::vector<int> infecting_list;
  std::vector<int> new_infected_list;
  std::vector<int> new_exposing_list;

  std::uniform_real_distribution<float> random_no(0, 1.0);

  double time = 1.0;



  std::ofstream myfile(path+"/epid_process/epid_temp_total_files/remaining_total_nodes_"+id_of_process+".txt");

  while(active_nodes>0){
		
    //myfile<<time<<"\t";
    for(int i = 0; i<N; i++){
      //time = time + 1/double(active_nodes);
      if (nodes[i]->infected && nodes[i]->active){
	time = time + 1/double(active_nodes);
	neighbors = nodes[i]->containing_nodes();

	for (int j = 0; j<neighbors.size(); j++){
				

	  if(!nodes[neighbors[j] - 1]->infected && !nodes[neighbors[j] - 1]->exposed){
	    float infecting_chance = random_no(rng);
	    //std::cout<<infecting_chance<<"infecting chance\n";
	    if (infecting_chance<lambda){
	      new_infected_list.push_back(neighbors[j]);}
	    else{
	      //std::cout<<"A node is exposed \n";
	      new_exposing_list.push_back(neighbors[j]);
	    }
	  }
	  else if(!nodes[neighbors[j] - 1]->infected && nodes[neighbors[j] - 1]->exposed){
	    if (random_no(rng)<T){
	      new_infected_list.push_back(neighbors[j]);
	    }

	  }

	}				
	nodes[i]->active = false;
	neighbors.clear();
      }			
    }

    //exposing
    if(!new_exposing_list.empty()){
      for (int j = 0; j<new_exposing_list.size(); j++){
	nodes[ new_exposing_list[j] - 1]->exposed = true;
      }

    }
    new_exposing_list.clear();
		

    //infecting
    if(!new_infected_list.empty()){
      for (int j = 0; j<new_infected_list.size(); j++){
	nodes[ new_infected_list[j] - 1]->infect();
      }
    }

    new_infected_list.clear();

    active_nodes=0;
    infected_nodes = 0;
    for(int i = 0; i<N; i++){
      if(nodes[i]->active){
	active_nodes++;
      }
      if(nodes[i]->infected) infected_nodes++;
    }
    //myfile<<infected_nodes<<"\t"<<active_nodes<<"\n";

    /*\
      int no_of_infected_nodes = 0;
      for(int i = 0; i<N; i++){
      if(nodes[i].infected){
      no_of_infected_nodes++;
      }
      }
      std::cout<<"Infected at step "<<" nodes:"<<no_of_infected_nodes<<'\n';
    */

  }//for loop ended
  
  
  
  
  int infected_after=0;
  for(int i = 0; i<N; i++){
    if(nodes[i]->infected){
      infected_after++;
    }
  }
  myfile<<std::setprecision(5);
  myfile<<time<<'\n'<<float(infected_after)/N<<"\n";	

  //std::cout<<float(infected_after)/N<<" remainder\n";

  //myfile<<((float)infected_after) /N;

  if (infected_after == 0){
    std::cout<<"inconsistency in cpp, 0 infected";
  }
  
  myfile.close();
  return 0;
}
