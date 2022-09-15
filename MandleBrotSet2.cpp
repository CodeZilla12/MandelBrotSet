// MandleBrotSet2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <future>
#include <chrono>

struct complex {
    double real;
    double imag;
    int itrCtr;
    complex(double x, double y, int z);
};

complex::complex(double x = 0, double y = 0, int z = 0) {
    //Constructor
    real = x;
    imag = y;
    itrCtr = z;
}

complex operator +(complex x, complex y) {
    //Addition operator to emulate complex addition
    return complex(double(x.real + y.real), double(x.imag + y.imag));
}

complex operator *(complex x, complex y) {
    //Addition operator to emulate complex addition (simplified)
    return complex(double(x.real * y.real - x.imag * y.imag), double(x.real * y.imag + x.imag * y.real));
}

void print(complex x, bool newline = false) {

    std::string endstr = newline ? "\n" : " ";
    std::cout << x.real << "+" << x.imag << "i" << endstr;

}

double abs(complex x) {
    return std::sqrt(x.real * x.real + x.imag * x.imag);
}

void computeRow(std::vector<complex>::iterator start, std::vector<complex>::iterator end, int MAX_ITR = 200, int INF = 2) {
    std::vector<complex>::iterator ptr;

    complex val;
    complex zConst;
    int tmpCtr = 0;

    for (ptr = start; ptr < end; ptr++) {

        zConst = *ptr;
        val = *ptr;
        while (abs(val) < INF && tmpCtr < MAX_ITR) { //applies transform on value until it diverges to infinity or it exceeds MAX_ITR
            val = *ptr;

            *ptr = val * val + zConst;
            tmpCtr++;

        }

        (*ptr).itrCtr = tmpCtr;
        tmpCtr = 0;

    }

}

int main()
{

    float aspect = 9.0/16.0; //aspect ratio of 16:9

    double rmin = -2, rmax = 1;
    double imin = -1, imax = 1;

    int n = 5000, m = n*aspect; //Size of array. Increase to improve resolution

    double rstep = (rmax - rmin) / n, istep = (imax - imin) / m;

    std::vector<std::vector<complex>> vec(n, std::vector<complex>(m, complex(0, 0)));

    //initialising consts from rmin + imin*j -> rmax + imax*j
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            vec[i][j] = complex(rmin + i * rstep, imin + j * istep);
        }
    }

    std::cout << "Consts Initialised\n";

    int itrCtr = 0;
    int MAX_ITR = 200; //Sets cutoff iteration, higher values give better contrast
    int INF = 2;
    complex zConst;

    auto timeStart = std::chrono::high_resolution_clock::now();

    std::vector<std::future<void>> futures;

    std::cout << "Building Set...\n";

    for (int row = 0; row < n; row++) {

        futures.push_back(std::async(computeRow, vec[row].begin(), vec[row].end(), MAX_ITR, INF)); //asynchronously loop over each row of set

    }

    std::cout << "Threads generated...\nApplying Transform:\n";
    int c = 0;
    for (auto& e : futures) {  //Wait for all threads to return
        c++;

        if (c % 500 == 0) std::cout << c << "/" << n << "\n";

        e.get();
    }
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - timeStart).count();

    std::cout << "Set Built in " << duration << " seconds.\nWriting...\n";

    std::ofstream outdata;
    std::string end;
    outdata.open("C:\\Users\\samgl\\source\\repos\\CodeZilla12\\MandelBrotSet\\mbdata.txt");

    outdata << rmin << "," << rmax << "," << imin << "," << imax << "," << MAX_ITR << "\n";

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            end = (j != m - 1) ? "," : "";  //Writes the required number of itrs to file for visualisation in python.
            outdata << vec[i][j].itrCtr << end;
        }
        outdata << "\n";
    }

    outdata.close();

    std::cout << "...Finished!\n";

    std::system("python C:\\Users\\samgl\\source\\repos\\CodeZilla12\\MandelBrotSet\\plot_data.py");
}