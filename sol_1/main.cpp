#include <iostream>
#include <sstream>
#include <string.h>
#include <fstream>
#include <list>
#include <math.h>

#define LARGE 99999999
#define R 6371

using namespace std;

struct node
{
    string state;
    string county;
    long double lat;
    long double lon;
};

int read_file(string file_name, list<struct node *> *my_list)
{
    string line, state, county, county_2, county_3, county_4;
    long double lon, lat;
    unsigned int i = 0;
    struct node *loc;
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
            loc = (struct node *)malloc(sizeof(struct node));
            loc->state = state;
            loc->county = county;
            loc->lat = lat;
            loc->lon = lon;

            (*my_list).push_back(loc);
        }
        i++;
    }
    return i;
}

int main(int argc, char *argv[])
{
    char *file_name, *outfile;
    list<struct node *> my_list;
    clock_t startt, endt;
    unsigned int size = 0;
    double get_lat, get_lon, distance[10], TT2, x, y, d, temp_d;
    int k;
    struct node temp_l, current_l, closest[10];

    if (argc < 2)
    {
        cout << "Error: no input file" << endl;
        exit(1);
    }
    file_name = argv[1];

    size = read_file(file_name, &my_list);

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

        for (int i = 0; i < k; i++)
        {
            distance[i] = LARGE;
        }

        startt = clock();
        for (std::list<node *>::iterator it = my_list.begin(); it != my_list.end(); ++it)
        {
            current_l.state = (*it)->state;
            current_l.county = (*it)->county;
            current_l.lat = (*it)->lat;
            current_l.lon = (*it)->lon;
            x = (current_l.lon - get_lon) * cos((current_l.lat + get_lat) / 2);
            y = (current_l.lat - get_lat);
            d = sqrt(x * x + y * y) * R;
            int j;
            for (j = 1; j <= k; j++)
            {
                if (d > distance[k - j])
                {
                    break;
                }
            }

            for (int m = k - j + 1; m < k; m++)
            {
                temp_l.state = closest[m].state;
                temp_l.county = closest[m].county;
                temp_l.lat = closest[m].lat;
                temp_l.lon = closest[m].lon;
                temp_d = distance[m];

                closest[m].state = current_l.state;
                closest[m].county = current_l.county;
                closest[m].lat = current_l.lat;
                closest[m].lon = current_l.lon;
                distance[m] = d;

                current_l.state = temp_l.state;
                current_l.county = temp_l.county;
                current_l.lat = temp_l.lat;
                current_l.lon = temp_l.lon;
                d = temp_d;
            }
        }
        endt = clock();
        TT2 = (double)(endt - startt) / CLOCKS_PER_SEC;

        cout << "Took " << TT2 << " seconds to find " << k << " nearest neighbor" << endl;

        for (int i = 0; i < k; i++)
        {
            cout << closest[i].state << "   " << closest[i].county << " is " << distance[i] << " km away" << endl;
            // cout << closest[i].state << "   " << closest[i].county << "   " << closest[i].lat << "   " << closest[i].lon << endl;
        }
    }

    return 0;
}