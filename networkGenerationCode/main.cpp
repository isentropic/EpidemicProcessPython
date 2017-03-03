#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "Linked_list.hpp"

// int k_min = 5; //no of min amount of neighbors
// int k_max = 100; //no of max amount of neighbors

// int gamma_power = 3;

int no_iterations = 100;

int N = 8e3; // = 5000; //Number of Nodes

int s_min = 5;                                    // min community size
int s_max = static_cast<int>((std::pow(N, 0.5))); // max community size

float beta_power = 2.5; // exponent of community distribution

float mu_con = 1; // fraction intercommunity links

float p = mu_con;

float lambda = mu_con;

std::default_random_engine eng((std::random_device())());
std::uniform_real_distribution<float> fdis(0, 1);

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

int power_dist_determenistic(double power_law_exponent, int k_max, int k_min,
                             int N, int *seq) {
  int seq_len = k_max - k_min + 1;
  double Pk_normalization_constant =
      (1.0 - power_law_exponent) / (std::pow(k_max, 1.0 - power_law_exponent) -
                                    std::pow(k_min, 1.0 - power_law_exponent));
  double interval_prob[seq_len];
  // std::cout<<Pk_normalization_constant<<" normconstant \n";

  for (int i = seq_len - 1; i >= 0; i--) {
    interval_prob[i] =
        Pk_normalization_constant * std::pow(k_min + i, -power_law_exponent);
  }
  for (int i = seq_len - 2; i >= 0; i--) {
    interval_prob[i] += interval_prob[i + 1];
  }

  int sum_N_k[seq_len];
  int N_k[seq_len];
  for (int i = 0; i < seq_len; i++) {
    sum_N_k[i] = static_cast<int>(N * interval_prob[i]);
  }
  N_k[seq_len - 1] = sum_N_k[seq_len - 1];
  seq[seq_len - 1] = N_k[seq_len - 1];
  for (int i = seq_len - 2; i >= 0; i--) {
    // for(int i = 0; i<=seq_len-2; i++){
    N_k[i] = sum_N_k[i] - sum_N_k[i + 1];
    seq[i] = N_k[i];
  }

  int directsum = 0;
  int nodes_sum = 0;

  for (int i = 0; i < seq_len; i++) {
    // std::cout<<i<<" at, n_k[i]: "<<N_k[i]<<'\n';
    // directsum+=N_k[i];
    nodes_sum += (k_min + i) * N_k[i];
  }
  // std::cout<<directsum<<" The number\n";
  // std::cout<<nodes_sum<<" Nodes sum\n";

  return nodes_sum;
}

std::vector<int> generate_communites(int total_nodes) {

  std::vector<int> targeted_communities;

  int calculated_N = 0;
  int target_N = total_nodes;
  int trial_N = 10;

  // int holder [s_max-s_min+1];
  // int *pointer_to_holder;
  int *pointer_to_holder = new int[s_max - s_min + 1]();
  // int *myArray = new int[SIZE];
  // pointer_to_holder = holder;

  calculated_N = power_dist_determenistic(beta_power, s_max, s_min, trial_N,
                                          pointer_to_holder);
  int index = 0;
  if (calculated_N > target_N) {
    std::cout << "Wrong trial_N was chosen\n";
  }
  while (calculated_N < target_N) {
    calculated_N = power_dist_determenistic(beta_power, s_max, s_min,
                                            trial_N + index, pointer_to_holder);
    index++;
    // std::cout<<"Converging Value: "<<calculated_N<<"\n";
  }

  // N = calculated_N;
  // std::cout<<"Targeted N: "<<total_nodes<<"\nCalculated N:
  // "<<calculated_N<<"\n";//Resulting Array:\n";
  // for(int i = 0; i<s_max-s_min+1; i++){
  // std::cout<<"communities of size: "<<s_min+i<<" Exists: "<<holder[i]<<"\n";
  //}

  calculated_N = power_dist_determenistic(
      beta_power, s_max, s_min, trial_N + index - 2, pointer_to_holder);
  int difference = target_N - calculated_N;

  for (int i = 0; i < s_max - s_min + 1; i++) {
    for (int j = 0; j < pointer_to_holder[i]; j++) {
      targeted_communities.push_back(s_min + i);
    }
  }

  delete[] pointer_to_holder;

  std::cout << "Targeted N: " << total_nodes
            << "\nCalculated N: " << calculated_N
            << "\nSummed N: " << sum_vector(targeted_communities)
            << "\n"; // Resulting Array:\n";

  targeted_communities[0] += difference;
  return targeted_communities;
}

int main(int argc, const char *argv[]) {

  ////////**INPUT PARAMETER DECLARATION///////////
  // id_of_process = argv[1];
  N = atoi(argv[1]);
  p = atof(argv[2]);
  // N = atoi(argv[4]);
  ////////////////////////////////////////////////

  // new way of generating communites,
  int temp = N;
  std::vector<int> communities = generate_communites(temp);
  // end new way
  std::cout << "Communities Established\n";

  int links = 0;
  // NODES INITIALIZATION
  std::vector<Linked_list *> nodes(N);
  std::cout << "Nodes[N] " << N << "initialized\n";
  for (int i = 0; i < N; i++) {
    nodes[i] = new Linked_list(i + 1, false);
  }

  int no_of_communites = communities.size();
  if (sum_vector(communities) > N) {
    std::cout << "modifying the size of the last community\n";
    // return 0;
    communities[no_of_communites - 1] =
        communities[no_of_communites - 1] - sum_vector(communities) + N;
  }

  if (sum_vector(communities) != N) {
    std::cout << "\nWARNING: \n\nsomething is wrong with community size\n\n";
  }

  std::vector<int> empty_communities(no_of_communites, 0);
  std::vector<int> index_communites(no_of_communites, 0);
  std::cout << "community sum: " << sum_vector(communities) << " Nodes: " << N
            << "\n";

  ////////

  std::random_device seeder;
  std::mt19937 engine(seeder());
  std::uniform_int_distribution<int> random_node(1, N);
  // std::uniform_int_distribution<int> random_community(1,no_of_communites);

  // Assigning communities:

  std::vector<int> chosen_nodes;

  int temp_com = 0;
  int index = 0;
  for (int j = 0; j < no_of_communites; j++) {
    for (int k = 0; k < communities[j]; k++) {
      nodes.at(index)->community_number = j + 1;
      index++;
    }
  }

  // Communities are assigned
  for (int j = 0; j < no_of_communites; j++) {
    int nodes_b4community = 0;
    for (int k = 0; k < j; k++) {
      nodes_b4community = nodes_b4community + communities[k];
    }
    index_communites[j] = nodes_b4community;
    for (int i = 0; i < communities[j]; i++) {
      for (int k = 0; k < communities[j]; k++) {

        if (k != i) {
          nodes[nodes_b4community + i]->add_Node(nodes_b4community + k + 1);
          nodes[nodes_b4community + i]->connectivity++;
          nodes[nodes_b4community + i]->connectivity_local++;
        }
      }
    }
  }

  for (int i = 0; i < N; i++) {
    links = links + nodes[i]->connectivity;
  }
  if (links % 2 == 1) {
    std::cout << "Should not be like this lol\n";
  }
  links = links / 2;

  int links_per_com = 0;
  // linking done

  int links_removed = 0;

  std::cout << "Starting removing extra links\n";
  for (int i = 0; i < no_of_communites; i++) {
    links_per_com = communities[i] * (communities[i] - 1) / 2;
    links_per_com = static_cast<int>(p * links_per_com);
    for (int j = 0; j < links_per_com; j++) {

      int firstnode = (random_node(engine) % communities[i]);
      int safety_while = 0;
      while (nodes.at(firstnode + index_communites[i])->empty()) {
        firstnode = random_node(engine) % communities[i];
        safety_while++;
        if (safety_while > s_max * 10) {
          std::cout << "something is wrong1";
          throw 1;
        }
      }
      firstnode = firstnode + index_communites[i];
      int secondnode = nodes[firstnode]->get_random_node() - 1;
      if (nodes[secondnode]->empty()) {
        std::cout << "Alert, inconsistency at removing random nodes:2nd node "
                     "is empty 1\n";
        throw 1;
      }
      // containing test to be done
      if (!nodes[firstnode]->contains(secondnode + 1)) {
        std::cout << "Alert, inconsistency at removing random nodes 2\n";
        throw 2;
      }

      if (nodes[firstnode]->contains(secondnode + 1) &&
          nodes[secondnode]->contains(firstnode + 1)) {

        nodes[firstnode]->remove_Node(secondnode + 1);
        nodes[firstnode]->connectivity_local--;

        nodes[secondnode]->remove_Node(firstnode + 1);
        nodes[secondnode]->connectivity_local--;

        links_removed++;
      } else {
        std::cout << "Alert, inconsistency at removing random nodes 4\n";
        throw 2;
      }
    }
  }
  std::cout << links_removed << " - links removed,\n";
  std::cout << links << "-links in total\n";

  std::cout << float(links_removed) / links << " - mu\n";

  std::vector<int> to_be_recconected_nodes;
  std::vector<int> no_to_recconnected_nodes;

  for (int i = 0; i < N; i++) {
    if (nodes[i]->connectivity > nodes[i]->connectivity_local) {
      int times = nodes[i]->connectivity - nodes[i]->connectivity_local;
      for (int j = 0; j < times; j++) {
        to_be_recconected_nodes.push_back(i);
      }
    }
  }

  int size_remaining_nodes = to_be_recconected_nodes.size();
  int firstnode;
  int secondnode;

  // ADDING LINKS

  int links_added = 0;

  int safety_while2 = 0;
  bool too_restricted = false;

  std::uniform_int_distribution<int> uni(0, size_remaining_nodes);

  std::cout << "remaing_nodes size" << size_remaining_nodes << "\n";

  while (links_removed > 0 && to_be_recconected_nodes.size() > 0) {

    firstnode = (uni(engine) % to_be_recconected_nodes.size());

    secondnode = (uni(engine) % to_be_recconected_nodes.size());

    int safety_while1 = 0;

    // std::cout<<"test\t";
    while (nodes.at(to_be_recconected_nodes.at(firstnode))->community_number ==
           nodes.at(to_be_recconected_nodes.at(secondnode))->community_number) {
      // while(true){
      // std::cout<<"test3\t";
      secondnode = (uni(engine) % to_be_recconected_nodes.size());
      safety_while1++;

      if (safety_while1 > s_max * 3) {
        // std::cout<<"same community node chosen";
        safety_while2++;
        continue;
      }
      // std::cout<<"test4\t";
    }
    if (safety_while2 > s_max * s_max) {
      std::cout << "too restricted\n";
      // print_vector(to_be_recconected_nodes);
      too_restricted = true;
      break;
    }

    // std::cout<<"test1\t";
    int node1 = to_be_recconected_nodes[firstnode];
    int node2 = to_be_recconected_nodes[secondnode];

    if (firstnode > secondnode) {
      to_be_recconected_nodes.erase(to_be_recconected_nodes.begin() +
                                    firstnode);
      to_be_recconected_nodes.erase(to_be_recconected_nodes.begin() +
                                    secondnode);
    } else {
      to_be_recconected_nodes.erase(to_be_recconected_nodes.begin() +
                                    secondnode);
      to_be_recconected_nodes.erase(to_be_recconected_nodes.begin() +
                                    firstnode);
    }
    size_remaining_nodes -= 2;
    links_removed--;

    nodes[node1]->add_Node(node2 + 1);
    nodes[node1]->connectivity_global++;
    nodes[node2]->add_Node(node1 + 1);
    nodes[node2]->connectivity_global++;
    links_added++;
  }

  if (too_restricted) {
    int safety_while1 = 0;
    int resrsize = to_be_recconected_nodes.size();
    while (to_be_recconected_nodes.size() > 0) {
      firstnode = (uni(engine) % to_be_recconected_nodes.size());
      int node1 = to_be_recconected_nodes[firstnode];
      int node2 = random_node(engine) - 1;
      if (nodes[node1]->community_number != nodes[node2]->community_number) {
        nodes[node1]->add_Node(node2 + 1);
        nodes[node1]->connectivity_global++;
        nodes[node2]->add_Node(node1 + 1);
        nodes[node2]->connectivity_global++;
      } else {
        safety_while1++;
      }

      if (safety_while1 > 100 * resrsize * resrsize) {
        std::cout << "somthing wrong with randomlyconnectying ppl\n";
        break;
      }
    }
    std::cout << "emergency actions taken place: " << resrsize
              << " nodes were connected randomly\n";
  }

  std::cout << "links added " << links_added << "\n";

  // NETWORK FORMED

  // FILE OUTPUT

  // degree distribution
  std::ofstream degree_distr("degree_distr.txt");

  for (int i = 0; i < N; i++) {
    degree_distr << i << "\t"
                 << float(nodes[i]->connectivity_global) /
                        (nodes[i]->connectivity)
                 << "\t" << nodes[i]->connectivity << "\t"
                 << nodes[i]->connectivity_local << "\t"
                 << nodes[i]->connectivity_global << "\t\n";
  }

  float averagedegree = 0;
  for (size_t i = 0; i < N; i++) {
    /* code */
    averagedegree += nodes[i]->connectivity;
  }
  averagedegree = averagedegree / N;

  std::cout << "Average Degree: " << averagedegree << '\n';

  ///
  std::ofstream total_graph("total_graph.txt");
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

  std::ofstream communities_file("communities.txt");
  for (int i = 0; i < N; i++) {
    communities_file << i + 1 << "\t" << nodes[i]->community_number << "\n";
  }

  std::ofstream communities_distr_file("community_distr.txt");

  for (int i = 0; i < no_of_communites; i++) {
    float average_mu_community = 0;
    for (int j = 0; j < communities[i]; j++) {
      average_mu_community +=
          float(nodes[index_communites[i] + j]->connectivity_global) /
          (nodes[index_communites[i] + j]->connectivity);
    }
    average_mu_community = average_mu_community / communities[i];
    communities_distr_file << average_mu_community << "\t" << i + 1 << "\n";
  }

  std::ofstream community_size_file("community_sizes.txt");
  for (int i = 0; i < no_of_communites; i++) {
    community_size_file << i + 1 << "\t" << communities[i] << "\n";
  }

  std::cout << "Success!\n";
  return 0;
}
