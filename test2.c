#include <stdio.h>
#include "pfl2.h"

void init_node_pool(){
  int i;
  unsigned int mask;
  node[0].start_address = NULL;         // dummy node will always be in
  node[0].state = IN_USE;               //   use as an artifical boundary
  node[0].size = 0;                     //   for the coalesce logic
  node[0].prev = &node[1];
  node[0].next = &node[1];
  node[1].start_address = mem;          // initial free region node
  node[1].state = FREE;
  node[1].size = MEM_SIZE;
  node[1].prev = &node[0];
  node[1].next = &node[0];
  for( i = 2; i < NUM_NODES; i++ ){     // other nodes are available for
    node[i].start_address = NULL;       //   splitting a free region into
    node[i].state = FREE;               //   an allocated region and a
    node[i].size = 0;                   //   remaining free region
    node[i].prev = NULL;
    node[i].next = NULL;
  }
  head = &node[0];
  for( i = 0; i < NUM_BITMAP_WORDS; i++ ){      // all nodes start free
    node_bitmap[i] = ~0;
  }
  mask = 1 << (NODES_PER_WORD - 1);             // except the dummy node
  node_bitmap[0] = node_bitmap[0] ^ mask;       //   and the node for the
  mask = mask >> 1;                             //   initial free region
  node_bitmap[0] = node_bitmap[0] ^ mask;
  bitmap_first_free = 0;                        // assumes NODES_PER_WORD
}                                               //   is > 2

void print_regions(){
  struct region_node *pn;
  printf( "\nregion list - addr state size\n" );
  pn = head->next;
  while( pn != head ){
    printf( "%p: %d %d\n", pn->start_address, pn->state, pn->size);
    pn = pn->next;
  }
  printf( "\n" );
}

void print_bitmaps(){
  int i;
  printf( "\nbitmap words\n" );
  for( i = 0; i < NUM_BITMAP_WORDS; i++ ){
    printf( "0x%08x", node_bitmap[i] );         // assumes 8 hex digits
    if( bitmap_first_free == i ){
      printf( "<- first free\n" );
    }else{
      printf( "\n" );
    }
  }
  printf( "\n" );
}

int main(){
  int i, rc;
  unsigned char *ptr[150];

  init_node_pool();

  print_regions();
  print_bitmaps();

/* general test */
  ptr[0] = alloc( 200 );
  printf("alloc 200\n");
  print_regions();
  print_bitmaps();
  rc = release( ptr[0] );
  printf( "alloc 200, release, expect rc of 0, rc = %d\n", rc );
  print_regions();
  rc = release( ptr[0] );
  printf( "re-release, expect rc of 1, rc = %d\n", rc );
  print_regions();
  print_bitmaps();

/* edge case tests */
  ptr[0] = alloc( -1 );
  printf( "alloc -1, expect NULL, ptr = %p\n", ptr[0] );
  ptr[0] = alloc( 0 );
  printf( "alloc 0, expect NULL, ptr = %p\n", ptr[0] );
  rc = release( ptr[0] );
  printf( "release, expect rc of 2, rc = %d\n", rc );
  print_regions();

  ptr[0] = alloc( 1 );
  printf("alloc 1\n");
  print_regions();
  print_bitmaps();
  rc = release( ptr[0] );
  printf( "alloc 1, release, expect rc of 0, rc = %d\n", rc );
  print_regions();
  print_bitmaps();

  ptr[0] = alloc( MEM_SIZE - 8 );
  printf("alloc MEM_SIZE-8\n");
  print_regions();
  rc = release( ptr[0] );
  printf( "alloc MEM_SIZE-8, release, expect rc of 0, rc = %d\n", rc );
  print_regions();

  ptr[0] = alloc( MEM_SIZE );
  rc = release( ptr[0] );
  printf( "alloc MEM_SIZE, release, expect rc of 2, rc = %d\n", rc );
  print_regions();

  ptr[0] = alloc( MEM_SIZE - 8 );
  ptr[1] = alloc( 1 );
  printf( "alloc MEM_SIZE-8 then 1, expect NULL, ptr = %p\n", ptr[1] );
  rc = release( ptr[0] );
  print_regions();

/* coalescing tests */
  printf( "coalescing tests\n" );

  ptr[0] = alloc( 256 );
  ptr[1] = alloc( 256 );
  ptr[2] = alloc( 256 );
  ptr[3] = alloc( 256 );
  ptr[4] = alloc( 256 );

  print_regions();
  print_bitmaps();
  release( ptr[2] );
  print_regions();
  print_bitmaps();

  ptr[2] = alloc( 256 );
  print_regions();
  print_bitmaps();
  release( ptr[1] );
  release( ptr[2] );
  print_regions();
  print_bitmaps();

  ptr[1] = alloc( 256 );
  ptr[2] = alloc( 256 );
  print_regions();
  print_bitmaps();
  release( ptr[3] );
  release( ptr[2] );
  print_regions();
  print_bitmaps();

  ptr[2] = alloc( 256 );
  ptr[3] = alloc( 256 );
  print_regions();
  print_bitmaps();
  release( ptr[1] );
  release( ptr[3] );
  release( ptr[2] );
  print_regions();
  print_bitmaps();

  release( ptr[0] );
  release( ptr[4] );
  print_regions();
  print_bitmaps();

/* bitmap tests */
  printf( "allocate 124 regions to use up most nodes\n" );
  for( i = 0; i < 124; i++ ){
    ptr[i] = alloc( 8 );
  }
  print_bitmaps();
  printf("alloc ptr[124]\n");
  ptr[124] = alloc( 8 );
  print_bitmaps();
  printf("alloc ptr[125]\n");
  ptr[125] = alloc( 8 );
  print_bitmaps();
  printf("alloc ptr[126]\n");
  ptr[126] = alloc( 8 );
  printf( "expect the last node to have been given all remaining space\n ");
  print_regions();
  print_bitmaps();
  printf("release ptr[98]\n");
  release( ptr[98] );
  print_bitmaps();
  printf("release ptr[99]\n");
  release( ptr[99] );
  print_bitmaps();
  printf("release ptr[100]\n");
  release( ptr[100] );
  print_bitmaps();
  printf("release ptr[33]\n");
  release( ptr[33] );
  print_bitmaps();
  printf("release ptr[32]\n");
  release( ptr[32] );
  print_bitmaps();
  printf("release ptr[31]\n");
  release( ptr[31] );
  print_bitmaps();
  printf("release ptr[30]\n");
  release( ptr[30] );
  print_bitmaps();
  printf("release ptr[29]\n");
  release( ptr[29] );
  print_bitmaps();
  printf("release ptr[28]\n");
  release( ptr[28] );
  print_bitmaps();
  printf("release all\n");
  for( i = 126; i >= 0; i-- ){
    release( ptr[i] );
  }
  print_regions();
  print_bitmaps();

  return 0;
}
