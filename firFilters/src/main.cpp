//============================================================================
// Name        : firFilters.cpp
// Author      : Timothy Darrah
// Version     :
// Copyright   : MIT
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdint.h>
#include <cmath>
#include <fstream>

#define SIG_LEN 320

using namespace std;

extern double InputSignal_f32_1kHz_15kHz[SIG_LEN];
double sig_out_ma[SIG_LEN];

/*
 * this is very inefficient as input points are calculated over and over again
 * ex (kernel size = 5): y[j] = (x[j-2] + x[j-1] + x[j] + x[j+1] + x[j+2]) / 5
 * the next point is:  y[j+1] = (x[j-1] + x[j] + x[j+1] + x[j+2] + x[j+3]) / 5
 * a better filter makes use of y[j] when calculating y[j+1]
*/
void calc_moving_average(double* sig_src, double* sig_out, int sig_len, int kernel_size)
{
	int idx = (int)floor(kernel_size/2);

	for(int i=idx; i<(sig_len - idx - 1);i++)
	{
		sig_out[i] = 0;

		for(int j=-idx; j<idx;j++)
		{
			sig_out[i] = sig_out[i] + sig_src[i+j];
		}

		sig_out[i] = sig_out[i]/kernel_size;
	}

}


int main()
{
	ofstream f1, f2;

	calc_moving_average(&InputSignal_f32_1kHz_15kHz[0], &sig_out_ma[0], SIG_LEN, 11);

	f1.open("sig_in.dat");
	f2.open("sig_ma.dat");

	for(int i=0; i<SIG_LEN;i++)
	{
		f1<<InputSignal_f32_1kHz_15kHz[i]<<endl;
		f2<<sig_out_ma[i]<<endl;
	}
	f1.close();
	f2.close();

	return 0;
}




















