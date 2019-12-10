#include <iostream>
#include <math.h>
#include "kdtree.h"

#define R 6371

bool compare_Lat(struct Node *a, struct Node *b)
{
	return a->lat < b->lat;
}

bool compare_Lon(struct Node *a, struct Node *b)
{
	return a->lon < b->lon;
}

struct kdtree *create_kdtree(int k)
{
	struct kdtree *kd = (struct kdtree *)malloc(sizeof(struct kdtree));

	kd->dim = k;
	kd->size = 0;
	kd->root = NULL;
	return kd;
}

void build_tree(struct kdtree *tree, list<struct Node *> *my_list)
{
	list<Node *> left_list;
	list<Node *> right_list;
	(*my_list).sort(compare_Lat);
	list<Node *>::iterator median = (*my_list).begin();
	advance(median, (*my_list).size() / 2);
	tree->root = *median;
	(*median)->comp = 0;
	left_list.splice(left_list.begin(), (*my_list), (*my_list).begin(), median);
	right_list.splice(right_list.begin(), (*my_list), ++median, (*my_list).end());
	tree->size += 1;
	build_tree_rec(tree, tree->root, &left_list, &right_list, 1, 2);
}

void build_tree_rec(struct kdtree *tree, struct Node *root, list<struct Node *> *left_list, list<struct Node *> *right_list, int dim, int k)
{
	list<Node *> left_list_1;
	list<Node *> left_list_2;

	list<Node *> right_list_1;
	list<Node *> right_list_2;

	if (dim == 0)
	{
		(*left_list).sort(compare_Lat);
		(*right_list).sort(compare_Lat);
	}
	else if (dim == 1)
	{
		(*left_list).sort(compare_Lon);
		(*right_list).sort(compare_Lon);
	}

	if ((*left_list).size() == 1)
	{
		list<Node *>::iterator median_left = (*left_list).begin();
		root->left = *median_left;
		(*median_left)->parent = root;
		(*median_left)->comp = dim;
		tree->size += 1;
	}
	else if ((*left_list).size() == 2)
	{
		list<Node *>::iterator median_left = (*left_list).begin();
		advance(median_left, (*left_list).size() / 2);
		root->left = *median_left;
		(*median_left)->parent = root;
		(*median_left)->comp = dim;
		left_list_1.splice(left_list_1.begin(), (*left_list), (*left_list).begin(), median_left);
		tree->size += 1;
		build_tree_rec(tree, root->left, &left_list_1, &left_list_2, ((dim + 1) % k), k);
	}
	else if ((*left_list).size() > 2)
	{
		list<Node *>::iterator median_left = (*left_list).begin();
		advance(median_left, (*left_list).size() / 2);
		root->left = *median_left;
		(*median_left)->parent = root;
		(*median_left)->comp = dim;
		left_list_1.splice(left_list_1.begin(), (*left_list), (*left_list).begin(), median_left);
		left_list_2.splice(left_list_2.begin(), (*left_list), ++median_left, (*left_list).end());
		tree->size += 1;
		build_tree_rec(tree, root->left, &left_list_1, &left_list_2, ((dim + 1) % k), k);
	}

	if ((*right_list).size() == 1)
	{
		list<Node *>::iterator median_right = (*right_list).begin();
		root->right = *median_right;
		(*median_right)->parent = root;
		(*median_right)->comp = dim;
		tree->size += 1;
	}
	else if ((*right_list).size() == 2)
	{
		list<Node *>::iterator median_right = (*right_list).begin();
		advance(median_right, (*right_list).size() / 2);
		root->right = *median_right;
		(*median_right)->parent = root;
		(*median_right)->comp = dim;
		right_list_1.splice(right_list_1.begin(), (*right_list), (*right_list).begin(), median_right);
		tree->size += 1;
		build_tree_rec(tree, root->right, &right_list_1, &right_list_2, ((dim + 1) % k), k);
	}
	else if ((*right_list).size() > 2)
	{
		list<Node *>::iterator median_right = (*right_list).begin();
		advance(median_right, (*right_list).size() / 2);
		root->right = *median_right;
		(*median_right)->parent = root;
		(*median_right)->comp = dim;
		right_list_1.splice(right_list_1.begin(), (*right_list), (*right_list).begin(), median_right);
		right_list_2.splice(right_list_2.begin(), (*right_list), ++median_right, (*right_list).end());
		tree->size += 1;
		build_tree_rec(tree, root->right, &right_list_1, &right_list_2, ((dim + 1) % k), k);
	}
}

double distance(struct Node *a, struct Node *b)
{
	double x, y, d;

	x = (a->lon - b->lon) * cos((a->lat + b->lat) / 2);
	y = (a->lat - b->lat);
	d = sqrt(x * x + y * y) * R;

	return d;
}

double distance_squared(struct Node *a, struct Node *b)
{
	double x, y, d;

	x = a->lat - b->lat;
	y = a->lon - b->lon;
	d = (x * x) + (y * y);

	return d;
}

struct Node *get_NN(struct Node *n, struct Node *goal, struct Node *best)
{
	if (n == NULL)
	{
		return best;
	}

	if (distance(goal, best) > distance(goal, n))
	{
		best = n;
	}

	struct Node *good, *bad;

	double goal_axis, test_axis;

	if (n->comp == 0)
	{
		if (n->lat > goal->lat)
		{
			good = n->left;
			bad = n->right;
		}
		else
		{
			good = n->right;
			bad = n->left;
		}
	}
	else if (n->comp == 1)
	{
		if (n->lon > goal->lon)
		{
			good = n->left;
			bad = n->right;
		}
		else
		{
			good = n->right;
			bad = n->left;
		}
	}

	if (n->comp == 0)
	{
		goal_axis = goal->lat;
		test_axis = n->lat;
	}
	else
	{
		goal_axis = goal->lon;
		test_axis = n->lon;
	}

	best = get_NN(good, goal, best);
	if (distance_squared(goal, best) > pow((goal_axis - test_axis), 2))
	{
		best = get_NN(bad, goal, best);
	}

	return best;
}

void get_K_NN(struct Node *n, struct Node *goal, list<Node *> *best)
{
	if (n == NULL)
	{
		return;
	}

	if (distance(goal, (*best).back()) > distance(goal, n))
	{
		for (list<Node *>::iterator it = (*best).begin(); it != (*best).end(); ++it)
		{
			if (distance(goal, (*it)) > distance(goal, n))
			{
				(*best).insert(it, n);
				(*best).pop_back();
				break;
			}
		}
	}

	struct Node *good, *bad;

	double goal_axis, test_axis;

	if (n->comp == 0)
	{
		if (n->lat > goal->lat)
		{
			good = n->left;
			bad = n->right;
		}
		else
		{
			good = n->right;
			bad = n->left;
		}
		goal_axis = goal->lat;
		test_axis = n->lat;
	}
	else if (n->comp == 1)
	{
		if (n->lon > goal->lon)
		{
			good = n->left;
			bad = n->right;
		}
		else
		{
			good = n->right;
			bad = n->left;
		}
		goal_axis = goal->lon;
		test_axis = n->lon;
	}

	get_K_NN(good, goal, best);

	double squared = pow(goal_axis - test_axis, 2);
	double precision = 0.575;

	if (distance_squared(goal, (*best).back()) > (squared * precision))
	{
		get_K_NN(bad, goal, best);
	}

	return;
}