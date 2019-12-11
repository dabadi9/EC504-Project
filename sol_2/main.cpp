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

int max_index(int a, int b, int c, int d, int e)
{
    if (a >= b && a >= c && a >= d && a >= e)
    {
        return 0;
    }
    else if (b >= c && b >= d && b >= e)
    {
        return 1;
    }
    else if (c >= d && c >= e)
    {
        return 2;
    }
    else if (d >= e)
    {
        return 3;
    }
    else
    {
        return 4;
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
    int k, m, index, count[5];
    list<Node *>::iterator it, checking;

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
        m = max(k, 5);
        for (int i = 0; i < m; i++)
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

        it = closest.begin();

        for (int i = 0; i < k; i++)
        {
            cout << (*it)->state << "   " << (*it)->county << " is " << distance((*it), search) << " km away" << endl;
            ++it;
        }

        index = 5;

        for (int k = 0; k < 5; k++)
        {
            it = closest.begin();
            advance(it, k);

            for (int i = 0; i < 5; i++)
            {
                if (i != k)
                {
                    checking = closest.begin();
                    advance(checking, i);
                    if ((*it)->state.compare((*checking)->state) == 0 && (*it)->county.compare((*checking)->county) == 0)
                    {
                        count[k]++;
                    }
                }
            }
            if (count[k] >= 3)
            {
                index = k;
                break;
            }
        }

        if (index == 5)
        {
            index = max_index(count[0], count[1], count[2], count[3], count[4]);
        }

        it = closest.begin();
        advance(it, index);

        cout << "\nMajority between 5 nearest points - " << (*it)->state << " " << (*it)->county << "\n"
             << endl;
    }

    return 0;
}