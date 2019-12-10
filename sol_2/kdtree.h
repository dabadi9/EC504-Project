#ifndef KDTREE_H
#define KDTREE_H

#include <string.h>
#include <list>

using namespace std;

struct Node
{
    double lat;
    double lon;
    string state;
    string county;
    int comp;
    struct Node *left, *right, *parent;
};

struct kdtree
{
    struct Node *root;
    int size, dim;
};

struct kdtree *create_kdtree(int k);
void build_tree(struct kdtree *tree, list<struct Node *> *my_list);
void build_tree_rec(struct kdtree *tree, struct Node *root, list<struct Node *> *left_list, list<struct Node *> *right_list, int dim, int k);
struct Node *get_NN(struct Node *n, struct Node *goal, struct Node *best);
double distance(struct Node *a, struct Node *b);
void get_K_NN(struct Node *n, struct Node *goal, list<Node *> *best);

// void insert_node(struct kdtree *tree, struct Node *n);
// struct Node *insert_nodeRec(Node *root, )

#endif /* _KDTREE_H_ */
