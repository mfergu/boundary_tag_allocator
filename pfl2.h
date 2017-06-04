#define NUM_BITMAP_WORDS 4
#define WORD_SHIFT 5
#define NODES_PER_WORD ( 1 << WORD_SHIFT )
#define NUM_NODES ( NUM_BITMAP_WORDS << WORD_SHIFT )
#define MEM_SIZE 100000
#define FREE 0
#define IN_USE 1

struct region_node{
  unsigned char *start_address;
  int state;
  int size;
  struct region_node *prev, *next;
};

struct region_node *head, node[NUM_NODES];
unsigned int node_bitmap[NUM_BITMAP_WORDS];
int bitmap_first_free;
unsigned char mem[MEM_SIZE];

struct region_node *get_free_node();
void return_node( struct region_node * n );
unsigned char *alloc( int size );
int release( unsigned char *ptr );
