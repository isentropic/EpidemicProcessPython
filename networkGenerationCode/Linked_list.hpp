
#ifndef Linked_list_hpp
#define Linked_list_hpp

#include <iostream>
#include <stdio.h>
#include <vector>
#endif /* Linked_list_hpp */
class Linked_list{
	struct Node{
		int number = 0;
		Node * next_node;
		Node(int num){
			next_node = NULL;
			number = num;
			//std::cout<<"node created with "<<if_infected<< "next_node is at:"<<next_node<<"\n";
		}
    };

public:
	int headnumber;
	int connectivity=0;
	int connectivity_local = 0;
	int connectivity_global = 0;

	int no_of_infected_neighbors = 0;
	
	int community_number = 0;
	bool infected;
	Node *head_node;
	Linked_list(int num, bool if_infected){
		headnumber = num;
		infected = if_infected;
		//std::cout<<"Linked_list created, with number "<<num<<std::endl;
		head_node = new Node(num);
		//std::cout<<"the1st node: "<<head_node->infected<<" next_node: "<<head_node->next_node<<"\n";
	}
	Linked_list(){
		//std::cout<<"should be here at Linked_list()\n";
		headnumber = 0;
		//std::cout<<"empty Linked_list created, with number 0"<<std::endl;
		head_node = new Node( 0);
		//head_node = temp_header;
		//std::cout<<"the1st node: "<<head_node->infected<<" next_node: "<<head_node->next_node<<"\n";
	}
	
	void add_Node(int num);
	void remove_Node(int num);

	bool contains(int num);
	bool empty();

	void print_nodes();


	void infect();
	void heal();

	int get_random_node();

	std::vector<int> containing_nodes();


};