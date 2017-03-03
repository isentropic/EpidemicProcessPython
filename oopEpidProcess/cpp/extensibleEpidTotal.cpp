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
std::string path = "/cappraid01/users/zhanibek/Thesis_s";
double lambda;
double T;
int timesToRun;
int N;

std::random_device rd;     // only used once to initialise (seed) engine
std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
std::uniform_int_distribution<int> getRandomNode;

std::vector<int> communities;
std::vector<Linked_list*> nodes;


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


void initializeGraph(){
  std::ifstream communities_file(path+"/communities.txt");
  std::ifstream nodes_file(path+"/total_graph.txt");
  if (communities_file.is_open() && nodes_file.is_open()){
    //std::cout<<"files are opened"<<"\n";
  }
  else{
    std::cout<<"couldnt open files, terminating"<<"\n"; 
    throw 1;
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
	  //nodes[head-1]->connectivity++; //already increased within linkedlist.cpp
	}
      }
      //std::cout<<"\n newline \n";

      // process pair (a,b)
    } 
  //std::cout<<nodes.size()<<" size of the nodes\n"; 
}

void clearTheGraph(){
  for(int i = 0; i<N; i++){
    nodes[i]->infected = false;
    nodes[i]->exposed = false;
    nodes[i]->active = false;
  }
}

void runEpidemicProcess(double &infectedratio, double &timepassed){
  int infected = getRandomNode(rng);
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

  while(active_nodes>0){
		
    //myfile<<time<<"\t";
    for(int i = 0; i<N; i++){
      //time = time + 1/double(active_nodes);
      if (nodes[i]->infected && nodes[i]->active){
	time = time + 1.0/double(active_nodes);
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
    
  }//for loop ended
  
  int infected_after=0;
  for(int i = 0; i<N; i++){
    if(nodes[i]->infected){
      infected_after++;
    }
  }

  
  infectedratio = float(infected_after)/N;
  timepassed = time;
}


double getAverageDegree(){
  int degree = 0;
  for(int i = 0; i<N; i++){
    degree+= nodes[i]->connectivity;
  }
  return degree/double(N);
}



int main(int argc, const char * argv[]) {

  id_of_process = argv[1];
  lambda = atof(argv[2]);
  T = atof(argv[3]);
  timesToRun = atoi(argv[4]);

  
  initializeGraph();
  
  N = nodes.size();
  //std::cout<<"Graph initialized: "<<N<<"\n";
  
  getRandomNode = std::uniform_int_distribution<int>(1, (int) N); // guaranteed unbiased


  
  std::ofstream longResultsFile(path+"/epid_process/extensibleEpidTotalFiles/totalResults_"+id_of_process+".txt");
  longResultsFile<<"#Ratio\ttime\t";
  longResultsFile<<"Average Degree:"<<getAverageDegree()<<":lambda:"<<lambda<<":Mu:"<<T<<"\n";
  longResultsFile<<std::setprecision(10);
 

  for(int i = 0; i<timesToRun; i++){

    double remainingRatio = 0.0;
    double timepassed = 0.0;

    runEpidemicProcess(remainingRatio, timepassed);
    clearTheGraph();
    longResultsFile<<remainingRatio<<"\t"<<timepassed<<"\n";

  }
  
  longResultsFile.close();
		  
  return 0;
}
