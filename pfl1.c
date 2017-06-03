
#include "pfl1.h"
#include <stdlib.h>
#include <stdlib.h>

typedef struct region_node T;

T* rec_get_free_node(T*);
T* rec_find_spot(int,T*);
void add_used_node(T*,int);
void add_free_node(T*,int);

//traverses array of nodes and finds one that's free; else NULL
T* get_free_node(){

	T* temp = node;
	T* found = rec_get_free_node(temp);	
	return found;
}

void return_node( T* n ){ 

	n->node_state = FREE;
	n->region_state = FREE;
}

//For a positive region size, alloc() searches the memory region list in order,
//looking for a free memory region that is equal in size or larger than the
//requested size.
unsigned char *alloc( int size ){

	if( !size > 0){
		return NULL;
	}
	T* temp = head->next;
	T* found = rec_find_spot(size, temp);
	
	return found->start_address;
}

int release( unsigned char *ptr ){

	return 1;
}

T* rec_get_free_node( T* temp){

	if( temp->region_state != FREE) {
		if( !(unsigned long)temp > (unsigned long)(node+63)) {
			rec_get_free_node( temp + 1);
		} else {
			return NULL;
		}
	} else {
		return temp;
	}
}

T* rec_find_spot(int size_needed, T* temp){

	//region is not free
	if( temp->region_state != FREE) {
		if( temp->next != head) {	
			rec_find_spot(size_needed, temp->next);
		} else {
			return NULL;
		}
	//region is free; is it the right size?
	} else {
		if( !temp->size < size_needed) {
			//found appropriate size
			temp->node_state = IN_USE;
			if( temp->size == size_needed) {
				return temp;
			} else {
				// spot is larger than necessary
				// look for an open node on the array
				T* open = get_free_node();
				if(open == NULL) {
					//no additional nodes available, allocate entire spot
					add_used_node( temp, size_needed);
					return temp;
				}
				//additional nodes are available
				//divide the spot between two nodes
				int total_size = temp->size;
				temp->size = size_needed;
				add_free_node( open, total_size - size_needed);				

				return temp;
			}
		} else {
			//size is too small
			if( temp->next != head) {
				rec_find_spot(size, temp->next);
			} else {
				return NULL;
			}
		}
	}
}

void add_used_node( T* temp, int size){

}

void add_free_node( T* temp, int size){

	temp->node_state = FREE;
	temp->region_state = FREE;
	temp->size = size;
	
	T* ref_node = head->next;
	temp->prev = head;
	temp->next = ref_node;
	ref_node->prev = temp;
	head->next = temp;
}















