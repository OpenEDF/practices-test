/* heap mamage of malloc and free  */

typedef struct {
    struct ll_t node;
     size_t size;
    char *block;
} alloc_node_t;


