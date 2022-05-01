// Zadanie domowe (6.6)


#include <iostream>
#include <math.h>
#include <stdio.h>

using namespace std;


typedef double (*fun_3a)(double, double, double);
typedef int (*bound_3a)(double, double, double);


int b1(double x, double y, double z) {
    if (x>=0 && x<=1 && y>=0 && y<=x && z>=0 && z<=x*y)
        return 1;
    return 0;
}

int b2(double x, double y, double z) {
    return 1;
}

double f1(double x, double y, double z) {
    return 100*pow(x, 3) * pow(y,2) * z;
}

double integrate3D(fun_3a f, bound_3a b, const double ran_x[2], const double ran_y[2], const double ran_z[2], int n)
{
    double sum = 0, x, y, z;
    double step_x = (ran_x[1] - ran_x[0]) / (double) n;
    double step_y = (ran_y[1] - ran_y[0]) / (double) n;
    double step_z = (ran_z[1] - ran_z[0]) / (double) n;

    z = ran_z[0];
    for (int i=0; i<n; i++) {
        y = ran_y[0];
        for (int j=0; j<n; j++) {
            x = ran_x[0];
            for (int k=0; k<n; k++) {
                if ( b(x, y, z) )
                    sum += f(x, y, z);
                x += step_x;
            }
            y += step_y;
        }
        z += step_z;
    }
    return sum / pow(n, 3.0);
}

int main()
{
    double wyn, std_ran[] = {0, 1};
    wyn = integrate3D(f1, b1, std_ran, std_ran, std_ran, 200);
    printf("%f \n", wyn);
    return 0;
}
