// MandleBrotSet2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

struct complex {
    double real;
    double imag;
    complex(double x, double y);
};

complex::complex(double x=0, double y=0) {
    real = x;
    imag = y;
}

complex operator +(complex x, complex y) {
    return complex(double(x.real + y.real),double(x.imag + y.imag));
}

complex operator *(complex x, complex y) {
    return complex(double(x.real * y.real - x.imag * y.imag), double(x.real * y.imag + x.imag * y.real));
}

complex operator *(double x, complex y) {
    return complex(double(x * y.real), double(x * y.imag));
}

void print(complex x, bool newline = false){

    std::string endstr = newline ? "\n" : " ";
    std::cout << x.real << "+" << x.imag << "i" << endstr;

}

complex conj(complex x) {
    return complex(double(x.real), double(-x.imag));
}

double abs(complex x) {
    return std::sqrt(x.real*x.real + x.imag*x.imag);
}

int main()
{
    double rmin = -2, rmax = 1;
    double imin = -1, imax = 1;

    //rmin = -0.75; rmax = -0.50;
    //imin = 0.35; imax = 0.52;

    int n = 500, m = 500;

    double rstep = (rmax - rmin) / n, istep = (imax - imin) / m;

    std::vector<std::vector<complex>> vec(n, std::vector<complex>(m, complex(0, 0)));

    //initialising consts
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            vec[i][j] = complex(rmin + i * rstep, imin + j * istep);
        }
    }

    std::cout << "Consts Initialised\n";

    //applying mandlebrot transform
    int itrCtr = 0;
    int MAX_ITR = 200;
    complex zConst;

    std::ofstream outdata;
    std::string end;
    outdata.open("mbdata.txt");

    outdata << rmin << "," << rmax << "," << imin << "," << imax << "\n";

    for (int i = 0; i < n; i++) {

        if (i % 275 == 0) { std::cout << "Building set ... " << i << '/' << n << "\n"; }

        for (int j = 0; j < m; j++) {
            zConst = vec[i][j]; //set ZConst to initial value of vec
            while (abs(vec[i][j]) < 2 && itrCtr < MAX_ITR) {
                vec[i][j] = vec[i][j] * vec[i][j] + zConst;
                itrCtr++;
            }
            end = (j != m - 1) ? "," : "";
            outdata << itrCtr << end;

            itrCtr = 0;
        }
        outdata << "\n";
    }

    outdata.close();

    std::cout << "...Finished!\nData Written To File!";

    return 0;
}