#include <iostream>
#include <ctime>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char **argv)
{
    srand(time(NULL));

    string name = argv[1];
    int size = atoi(argv[2]);

    ofstream f;
    f.open(name);

    vector< vector<int> > v;

    for (int i = 0; i < size; i++)
    {
        vector<int> line(size);
        v.push_back(line);
    }

    for (int i = 0; i < size; i++)
        for (int j = i + 1; j < size; j++)
        {
            if (rand() % 100 < 70)
                v[i][j] = 0;
            else
                v[i][j] = rand() % size + 1;
        }

    for (int i = 0; i < size; i++)
        for (int j = 0; j <= i; j++)
            v[i][j] = v[j][i];

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
            f << v[i][j] << ' ';
        f << endl;
    }

    f.close();
    return 0;
}
