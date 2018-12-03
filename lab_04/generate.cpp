#include <iostream>
#include <math.h>
#include <cmath>
#include <string>
#include <ctime>
#include <fstream> 
#include <stdio.h>
#define SIZE 100000
using namespace std;
int main(int argc, char **argv){
    setlocale(0, "rus");
    srand(time(0));
    string b[SIZE],t="p";
    int a = 1, c[SIZE], c1 = 2, n, k = 0,ii=0,oo=0;
    ii = atoi(argv[1]);
    oo = atoi(argv[2]);
    cout << ii << ' ' << oo;
    ii += 2;
    oo += 2;
    for (int x = 0; x < ii*oo; x++){
        b[x] = " ";
        if (x < oo)b[x] = "#";
        if (x>ii*oo-oo-1)b[x] = "#";
        if (x % oo == 0)b[x] = "#";
        if (x % oo == oo-1)b[x] = "#";
    }
    while (a%2==1)
       a = rand() % (oo-4)+oo*ii-oo+2;
    b[a] = ".";
    b[a-oo] = ".";
    a = 1;
    while (a%2==1)
       a = rand() % (oo-4)+2;
    b[a] = ".";
    b[a+oo] = ".";
    a += oo*2;
    while (c1>1){
        int a1 = a;
        if (k < 15){
            c[c1] = a;
            c1++;
        }
        b[a] = ".";
        k = 0;
        while (a1 == a && k<15){
            n = rand() % 4 + 1;
            if (n == 1 && b[a + 2] ==" ")a++;
            if (n == 2 && b[a + oo*2] ==" ")a += oo;
            if (n == 3 && b[a - oo*2] ==" " )a -= oo;
            if (n == 4 && b[a - 2] ==" ")a--;
            k++;
        }
        if (k == 15){
            c[c1 - 1] = 0;
            c1 -= 2;
            a = c[c1];
            c1++;
        }
        if (a>0)
            b[a] = ".";
        if (a < 0)
            c1 = 0;
        if (k < 15){
            if (n == 1)a++;
            if (n == 2)a += oo;
            if (n == 3)a -= oo;
            if (n == 4)a--;
            b[a] = ".";
        }
    }
    b[oo*2] = "#";
    for (int x = 0; x < oo*ii; x++){
        if (b[x] == "#")b[x] = "";
        if (b[x] == " ")b[x] = "1 ";
        if (x>oo * 2||x<oo)
             if (b[x] == ".")b[x] = "0 ";
    }
    for (int x = 0; x < oo*ii; x++){
        if (b[x] == ".") cout << "0 ";
        else cout << b[x];
        if (x % oo == oo-1)
            cout << endl;
    }

    return 0;
}
