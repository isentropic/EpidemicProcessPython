#include <fstream>
#include <stdio.h>
#include <algorithm>
#include "Linked_list.hpp"
void Linked_list::add_Node(int num){
	//connectivity++;
	
	//////////
	//////////

	Node *temp;
	temp = head_node;
	if(num == headnumber){
		std::cout<<"Error, self connected Node no: "<< num<< "\n";
	}
	Node *nodeToAdd = new Node(num);

	while(temp->next_node!=NULL){
		temp = temp->next_node;
	}
	(temp->next_node) = nodeToAdd;
}

void Linked_list::remove_Node(int num){
	//connectivity--;
	Node *temp;
	temp = head_node;
	if (temp->next_node->number == num){
		temp->next_node = temp->next_node->next_node;
		return;
	}

	while(temp->next_node!=NULL){
		if (temp->next_node->number == num){
			temp->next_node = temp->next_node->next_node;
			return;
		}
		temp = temp->next_node;
	}
	std::cout<<"failed to remove a node \n";
}

bool Linked_list::contains(int num){
	Node *temp;
	temp = head_node;
	while (temp->next_node!=NULL){
		temp = temp->next_node;
		if (temp->number == num){
			return true;
		}
	}
	return false;
}
bool Linked_list::empty(){
	if (head_node->next_node == NULL){
		return true;
	}
	return false;
}




void Linked_list::print_nodes(){
	std::cout<<"# "<<headnumber<<" stat: "<<infected<<"-> ";
	Node *temp;
	temp = head_node;
	while (temp->next_node!=NULL){
		temp = temp->next_node;
		std::cout<<" - "<<"<No: "<<temp->number<<">";
	}
	std::cout<<"\n\n";
}

void Linked_list::infect(){
	infected = true;
}
void Linked_list::heal(){
	infected = false;
}


int Linked_list::get_random_node(){
	//check for null size needed
	std::vector<int> contained_nodes = containing_nodes();
	return contained_nodes[ (rand() % (int)(contained_nodes.size())) ];
}


std::vector<int> Linked_list::containing_nodes(){
	std::vector<int> contained_nodes;
	Node *temp;
	temp = head_node;
	while(temp->next_node!=NULL){
		temp = temp->next_node;
		contained_nodes.push_back(temp->number);
	}
	std::sort(contained_nodes.begin(), contained_nodes.end()); 
	return contained_nodes;
}