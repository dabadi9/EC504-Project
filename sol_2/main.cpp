#include <iostream>
#include <sstream>
#include <fstream>

#include "kdtree.h"

#define LARGE 99999999

void read_file(string file_name, list<struct Node *> *my_list)
{
    string line, state, county, county_2, county_3, county_4;
    long double lon, lat;
    unsigned int i = 0;
    struct Node *loc;
    ifstream infile(file_name);
    bool error = false;
    list<Node *>::iterator last_insert;

    getline(infile, line);

    while (getline(infile, line))
    {
        error = false;
        istringstream iss(line);
        if (!(iss >> state >> county >> lat >> lon))
        {
            istringstream iss(line);
            if (!(iss >> state >> county >> county_2 >> lat >> lon))
            {
                istringstream iss(line);
                if (!(iss >> state >> county >> county_2 >> county_3 >> lat >> lon))
                {
                    istringstream iss(line);
                    if (!(iss >> state >> county >> county_2 >> county_3 >> county_4 >> lat >> lon))
                    {
                        cout << "ERROR: in line - " << i - 1 << "  " << line << endl;
                        error = true;
                    }
                    else
                    {
                        county = county + " " + county_2 + " " + county_3 + " " + county_4;
                    }
                }
                else
                {
                    county = county + " " + county_2 + " " + county_3;
                }
            }
            else
            {
                county = county + " " + county_2;
            }
        }
        if (!error)
        {
            loc = (struct Node *)malloc(sizeof(struct Node));
            loc->state = state;
            loc->county = county;
            loc->lat = lat;
            loc->lon = lon;
            loc->comp = 0;
            loc->left = NULL;
            loc->right = NULL;
            loc->parent = NULL;
            (*my_list).push_back(loc);
        }
        i++;
    }
}

int main(int argc, char *argv[])
{
    char *file_name;
    list<struct Node *> my_list;
    list<struct Node *> closest;
    struct Node *search, *best;
    clock_t startt, endt;
    double get_lat, get_lon, TT2;
    unsigned int k;

    if (argc < 2)
    {
        cout << "Error: no input file" << endl;
        exit(1);
    }
    file_name = argv[1];

    cout << "--------------------------- Reading file ---------------------------" << endl;

    read_file(file_name, &my_list);

    cout << "Read " << my_list.size() << " lines" << endl;

    struct kdtree *kd = create_kdtree(2);

    cout << "--------------------------- Building tree ---------------------------" << endl;

    build_tree(kd, &my_list);

    cout << "kd->size = " << kd->size << endl;

    search = (struct Node *)malloc(sizeof(struct Node));
    search->parent = NULL;
    search->left = NULL;
    search->right = NULL;

    cout.precision(10);

    while (1)
    {
        cout << "Enter k (1-10): ";
        cin >> k;
        while (k < 1 || k > 10)
        {
            cout << "Enter k (1-10): ";
            cin >> k;
        }
        cout << "Enter latitude and longitude: ";
        cin >> get_lat >> get_lon;

        closest.clear();
        for (unsigned int i = 0; i < k; i++)
        {
            best = (struct Node *)malloc(sizeof(struct Node));
            best->parent = NULL;
            best->left = NULL;
            best->right = NULL;
            best->lat = i;
            best->lon = LARGE;
            closest.push_back(best);
        }

        search->lat = get_lat;
        search->lon = get_lon;

        startt = clock();
        get_K_NN(kd->root, search, &closest);
        endt = clock();

        TT2 = (double)(endt - startt) / CLOCKS_PER_SEC;

        cout << "Took " << TT2 << " seconds to find " << k << " nearest neighbor" << endl;

        for (list<Node *>::iterator it = closest.begin(); it != closest.end(); ++it)
        {
            // cout << (*it)->lat << endl;
            cout << (*it)->state << "   " << (*it)->county << " is " << distance((*it), search) << " km away" << endl;
            // cout << (*it)->state << "   " << (*it)->county << "   " << (*it)->lat << "   " << (*it)->lon << endl;
        }
    }

    return 0;
}