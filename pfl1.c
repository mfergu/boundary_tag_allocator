
#include "pfl1.h"
#include <stdlib.h>

typedef struct region_node T;

T* rec_get_free_node(T*);
T* rec_find_spot(int,T*);
void add_node(T*,T*,int);
int rec_release(T*,unsigned char*);
void coalesce(T*);

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

unsigned char *alloc( int size ){
	if( !(size > 0)){
		return NULL;
	}
	T* temp = head;
	T* found = rec_find_spot(size, temp);
	//if too large to allocate
	if(found == NULL) return NULL;
	return found->start_address;
}

int release( unsigned char *ptr ){
	T* temp_node = head->next;
	int n = rec_release( temp_node, ptr);
	return n;
}

int rec_release(T* temp, unsigned char *ptr){
	if( temp != head){
		if( temp->start_address != ptr){
			return rec_release(temp->next, ptr);
		} else {
			if( temp->region_state == FREE){
				coalesce(temp);
				return 1;
			} else { 
				temp->region_state = FREE;
				coalesce(temp);
				return 0;
			}
		}
	} else {
		return 2;
	}
}

void coalesce( T* temp) {
	//fix sizes of each node
	//fix pointers to each surrounding nodes
	if( temp->prev->region_state == FREE){
		return_node(temp);
		temp->prev->size += temp->size;
		temp->next->prev = temp->prev;
		temp->prev->next = temp->next;
		temp = temp->prev;
	} else if( temp->next->region_state == FREE){
		return_node(temp->next);
		temp->size += temp->next->size;
		temp->next->next->prev = temp;
		temp->next = temp->next->next;
	}
}

T* rec_get_free_node( T* temp){
	if( temp->node_state != FREE) {
		if( !((unsigned long)temp > (unsigned long)(node + NUM_NODES - 1))) {
			return rec_get_free_node( temp + 1);
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
			return rec_find_spot(size_needed, temp->next);
		} else {
			return NULL;
		}
	//region is free; is it the right size?
	} else {
		if( !(temp->size < size_needed)) {
			//found appropriate size
			temp->node_state = IN_USE;
			if( temp->size == size_needed) {
				temp->region_state = IN_USE;
				return temp;
			} else {
				// spot is larger than necessary
				// look for an open node on the array
				T* open = get_free_node();
				if(open == NULL) {
					//no additional nodes available, allocate entire spot
					add_node( temp, NULL, size_needed);
					return temp;
				}
				//additional nodes are available
				add_node( temp, open, size_needed);
				return temp;
			}
		} else {
			//size is too small
			if( temp->next != head) {
				return rec_find_spot(size_needed, temp->next);
			} else {
				return NULL;
			}
		}
	}
}

void add_node(T* temp, T* open, int size_requested){
	if( open != NULL) {
		//divide the spot between two nodes
		//initialize the newly populated node to match requested size
		open->node_state = IN_USE;
		open->region_state = FREE;
		open->size = temp->size - size_requested;
		temp->size = size_requested;
		open->start_address = temp->start_address + temp->size;
		open->prev = temp;
		temp->next->prev = open;
		open->next = temp->next;
		temp->next = open;
	}
	temp->region_state = IN_USE;
}















