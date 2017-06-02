//http://www.bradrodriguez.com/papers/ms/pat4th-c.html
//http://bitsquid.blogspot.com/2015/08/allocation-adventures-3-buddy-allocator.html
//
//search linked list for an available block of a suitable size 
//
//merge available blocks 
#include "pfl1.h"
#include <stdio.h>

typedef struct region_node T;

T* rec_get_free_node(T*);
T* rec_find_spot(int,T*);
void add_node(unsigned short, T*);

T* get_free_node(){

	T* temp = head->next;
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

	if( size < 0){
		return NULL;
	}
	T* temp = head->next;
	T* found = rec_find_spot(size, temp);
	
	return 'c';
}

int release( unsigned char *ptr ){

	return 1;
}

T* rec_get_free_node( T* temp){

	if( temp->region_state != FREE) {
		if( temp->next != head) {	
			rec_get_free_node( temp->next);
		} else {
			return NULL;
		}
	} else {
		return temp;
	}
}

T* rec_find_spot(int size, T* temp){

	//region is not free
	if( temp->region_state != FREE) {
		if( temp->next != head) {	
			rec_find_spot(size, temp->next);
		} else {
			return NULL;
		}
	//region is free; is it the right size?
	} else {
		if( !temp->size < size) {
			//found appx size
			temp->node_state = IN_USE;
			if( temp->size == size) {
				return temp;
			} else {
				//split the memory and find an available region_node to accept
				// the new spot
				T* open = get_free_node();
				add_node( FREE, open);				

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

void add_node(unsigned short type, T* temp){

	temp->node_state = FREE;
	temp->region_state = FREE;
	
};















