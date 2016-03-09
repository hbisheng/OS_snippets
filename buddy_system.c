//This is an application demonstrating the management of memory using BUDDY SYSTEM
//Allocable Memory Size: 1M 
#include<iostream>
#define max(x,y) (x >= y) ? x : y
using namespace std;
struct Page
{
    int start; //where this page starts
    int rem;   //the largest bytes of memory that remain in my filials  
    int page_size; // a const number
    Page* parent;
    Page* left;
    Page* right;
    Page(int s, int r, Page* par)
    {
        start = s;
        rem = r;
        page_size = rem;
        parent = par; 
        left = NULL;
        right = NULL;
    }
}; 
Page root(0, 1<<20, NULL);

void update_rem(Page* node) // update REM
{
    if(node->parent)
    {
        int rem_new = max(node->parent->left->rem, node->parent->right-> rem);
        if( node->parent->rem != rem_new)
        {
            node->parent-> rem = rem_new;
            update_rem(node->parent);
        }
    }
}
Page* find_page(Page* cur, int size_p)
{
    if(cur->rem < size_p)
        return NULL;
    //assert that the target exists
    if(!(cur->left)&&!(cur->right)) // no child
    {
        if((cur->rem >> 1) < size_p) // then the target is current page
        {   
            cur->rem = 0;
            update_rem(cur);
            return cur;
        }
        cur->left = new Page(cur->start, cur->rem>>1, cur);
        cur->right = new Page(cur->start + (cur->rem>>1), cur->rem>>1, cur);
        return find_page(cur->left, size_p);
    }
    else // have children
    {
        if(cur->left->rem >= size_p) //the target is in left
            return find_page(cur->left,size_p);
        else // in right
            return find_page(cur->right,size_p); 
    }
}

Page* alloc_page(int size_p)
{
    return find_page(&root, size_p);//DFS to find the best page
}

void free_page(Page* p)
{
    if(p->left || p->right)
    {
        cout << "Internal-node pages can not be deleted";
        return;
    }
    //assert that this is a leave node
    p->rem = p->page_size; // back to initial state
    Page* par = p->parent;
    if(par)
        if(par->left->rem == par->left->page_size && par->right->rem == par->right->page_size)
        {
            //merge nodes and delete children
            delete par-> left;
            delete par-> right;
            par->left = NULL;
            par->right = NULL;
            free_page(par);
        }
} 

void print_allpage(Page* cur, int level)
{
    for(int i = 0; i != level - 1; i++)
        cout << "\t";
    cout << "Start:" << cur->start/(1<<10) << "K ";
    cout << "Size:" << cur-> page_size /(1<<10) << "K "; 
    cout << "REM:" << cur->rem/(1<<10) << "K" << endl;
    if(cur->left)
    {
        print_allpage(cur->left,level+1);
        print_allpage(cur->right,level+1);
    }
}
int main()
{
    Page* a = alloc_page(128*(1<<10));
    Page* b = alloc_page(64*(1<<10));
    Page* c = alloc_page(64*(1<<10));
    Page* d = alloc_page(256*(1<<10));
    Page* e = alloc_page(256*(1<<10));
    Page* f = alloc_page(256*(1<<10));
    cout << "Built all pages(According to The 5th Page in Slice 5-5)" << endl;
    print_allpage(&root, 1);
    cout << endl;

    cout << "Delete EF" << endl;
    free_page(f);
    free_page(e);
    print_allpage(&root, 1);
    cout << endl;

    cout << "DELETE ABC" << endl;
    free_page(a);
    free_page(b);
    free_page(c);
    print_allpage(&root,1);
    cout << endl;

    cout << "DELETE D" << endl;
    free_page(d);
    print_allpage(&root,1);

}
