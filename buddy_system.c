#include<stdio.h>
#include<stdlib.h>

struct mem_node {
    int start;
    int size;
    int occupied;
    struct mem_node *prev;
    struct mem_node *next;
} head;


struct mem_node alloc_space(int alloc_size){
    return head;
}

/*
void free_space(struct mem_node){

}
*/


int main() { 
    // initialize
    head.start = 0;
    head.size = 1 << 20; // 1M
    head.occupied = 0;
    head.prev = NULL;
    head.next = NULL;
    
    
    
    return 0;
}