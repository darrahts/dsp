#include <iostream>
#include <stdio.h>
#include <fstream>

#include "convolution.hpp"

using namespace std;


extern double InputSignal_f32_1kHz_15kHz[320];
extern double Impulse_response[29];

int sig_len = sizeof(InputSignal_f32_1kHz_15kHz)/sizeof(InputSignal_f32_1kHz_15kHz[0]);
int imp_len = sizeof(Impulse_response)/(sizeof(Impulse_response[0]));

void convolution_test(Convolution* conv)
{
	ofstream f1, f2, f3;
	double sig_out[sig_len+imp_len];

	//conv->calc_convolution(&sig_out[0]);
	conv->convolution(&sig_out[0]);

	f1.open("sig_out.dat");
	f2.open("sig_in.dat");
	f3.open("imp_res.dat");

	for(int i = 0; i < sig_len+imp_len; i++)
	{
		f1<<sig_out[i]<<endl;
		if(i < sig_len)
		{
			f2<<InputSignal_f32_1kHz_15kHz[i]<<endl;
		}
		if(i < imp_len)
		{
			f3<<Impulse_response[i]<<endl;
		}
	}
	f1.close();
	f2.close();
	f3.close();
}


void calc_running_sum_test(Convolution* conv)
{
	double sig_out[sig_len];
	ofstream f1;

	conv->calc_running_sum(&sig_out[0]);

	f1.open("running_sum.dat");
	for(int i = 0; i < sig_len; i++)
	{
		f1<<sig_out[i]<<endl;
	}
	f1.close();
}

void calc_first_diff_test(Convolution* conv)
{
	double sig_out[sig_len];
	ofstream f1;

	conv->calc_first_diff(&sig_out[0]);

	f1.open("first_diff.dat");
	for(int i = 0; i < sig_len; i++)
	{
		f1<<sig_out[i]<<endl;
	}
	f1.close();
}



int main()
{
	Convolution* conv = new Convolution(&InputSignal_f32_1kHz_15kHz[0], &Impulse_response[0], sig_len, imp_len);

	convolution_test(conv);

	//calc_running_sum_test(conv);

	//calc_first_diff_test(conv);

	return 0;
}




