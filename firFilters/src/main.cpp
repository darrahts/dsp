//============================================================================
// Name        : filters.cpp
// Author      : Timothy Darrah
// Version     :
// Copyright   : MIT
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdint.h>
#include <cmath>
#include <fstream>

#define KERNEL_LEN 29
#define SIG_LEN 320

using namespace std;

extern double InputSignal_f32_1kHz_15kHz[SIG_LEN];
double sig_out_ma[SIG_LEN];

double sig_out_windowed[KERNEL_LEN + SIG_LEN];
double kernel[KERNEL_LEN];


/**@note
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

/**@note
 * uses the formula y[i] = y[i-1] + x[i + a] - x[i-b]
 * where a = (kernel_size - 1) / 2
 * where b = a + 1
 */
void better_moving_average(double* sig_src, double* sig_out, int sig_len, int kernel_size)
{
	int idx = (int)floor(kernel_size/2);
	double acc = 0.0; //accumulator
	for(int i=0; i < kernel_size-1;i++)
	{
		acc = acc + sig_src[i];
	}
	sig_out[(kernel_size-1)/2] = acc/kernel_size;
	for(int i = idx; i < (sig_len - idx -1); i++)
	{
		acc = acc + sig_src[i+((kernel_size-1)/2)] - sig_src[i - idx];
		sig_out[i] = (acc / kernel_size);
	}
}

/**@note
 * separates one band of frequencies from another
 * slow execution
 * http://www.dspguide.com/ch16.htm
 */
void window_sinc_lowpass_filter(double* filter_kernel, double cutoff_freq_ratio, int filter_len)
{
	double twoPi = 2*M_PI;
	int half_len = filter_len/2;

	for(int i = 0; i< filter_len; i++)
	{
		if(i-filter_len/2 == 0)
		{
			filter_kernel[i] = twoPi*cutoff_freq_ratio;
		}
		else
		{
			filter_kernel[i] = sin(twoPi*cutoff_freq_ratio*(i-half_len))/(i-half_len);
			filter_kernel[i] = filter_kernel[i] * (.54-.46*cos(twoPi*i/filter_len));
		}
	}
}


void convolution(double* sig_src, double* sig_dest, double* imp_res, int sig_src_len, int imp_res_len)
{
    for(int i =0;i<(sig_src_len + imp_res_len);i++)
    {
        sig_dest[i] = 0;
    }

	for(int i=0;i<sig_src_len;i++)
	{

		for(int j=0;j<imp_res_len;j++)
		{
			sig_dest[i+j] =  sig_dest[i+j] + sig_src[i]*imp_res[j];
		}
	}
}


void moving_average_test()
{
	ofstream f1, f2;

	//calc_moving_average(&InputSignal_f32_1kHz_15kHz[0], &sig_out_ma[0], SIG_LEN, 11);
	better_moving_average(&InputSignal_f32_1kHz_15kHz[0], &sig_out_ma[0], SIG_LEN, 11);

	f1.open("sig_in.dat");
	f2.open("sig_ma.dat");

	for(int i=0; i<SIG_LEN;i++)
	{
		f1<<InputSignal_f32_1kHz_15kHz[i]<<endl;
		f2<<sig_out_ma[i]<<endl;
	}
	f1.close();
	f2.close();

}



int main()
{
	ofstream f1, f2, f3;

	window_sinc_lowpass_filter(kernel, .2, KERNEL_LEN);
	convolution(&InputSignal_f32_1kHz_15kHz[0], &sig_out_windowed[0], &kernel[0], SIG_LEN, KERNEL_LEN);

	f1.open("sig_in2.dat");
	f2.open("kernel.dat");
	f3.open("sig_windowed.dat");

	for(int i=0; i<SIG_LEN+KERNEL_LEN; i++)
	{
		if(i<SIG_LEN)
		{
			f1<<InputSignal_f32_1kHz_15kHz[i]<<endl;
		}

		if(i<KERNEL_LEN)
		{
			f2<<kernel[i]<<endl;
		}
		f3<<sig_out_windowed[i]<<endl;
	}
	f1.close();
	f2.close();
	f3.close();

	return 0;
}




















