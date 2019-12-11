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
    char *file_name, *outfile;
    list<struct node *> my_list;
    clock_t startt, endt;
    unsigned int size = 0;
    double get_lat, get_lon, distance[10], TT2, x, y, d, temp_d;
    int k, maximum, count[5], index;
    struct node temp_l, current_l, closest[10], *state_county;

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

        maximum = max(k, 5);

        for (int i = 0; i < maximum; i++)
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
            for (j = 1; j <= maximum; j++)
            {
                if (d > distance[maximum - j])
                {
                    break;
                }
            }

            for (int m = maximum - j + 1; m < maximum; m++)
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
        }

        index = 5;
        for (int k = 0; k < 5; k++)
        {
            state_county = &closest[k];

            for (int i = 0; i < 5; i++)
            {
                if (i != k)
                {
                    if (state_county->state.compare(closest[i].state) == 0 && state_county->county.compare(closest[i].county) == 0)
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

        cout << "\nMajority between 5 nearest points - " << closest[index].state << " " << closest[index].county << "\n"
             << endl;
    }

    return 0;
}