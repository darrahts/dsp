//============================================================================
// Name        : main.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <iostream>
#include <cmath>
#include <stdio.h>
#include <fstream>
#include "fourierTransform.hpp"

#define SIG_LEN 320

extern double InputSignal_f32_1kHz_15kHz[320];
//int sig_len = sizeof(InputSignal_f32_1kHz_15kHz)/sizeof(InputSignal_f32_1kHz_15kHz[0]);

//the dft is mirrored, so really only need half
//double real_out[int(SIG_LEN/2)];
//double imag_out[int(SIG_LEN/2)];
//double mag_out[int(SIG_LEN/2)];

//just use the full dft and show its reflection about the midpoint
double real_out[SIG_LEN];
double imag_out[SIG_LEN];
double mag_out[SIG_LEN];

using namespace std;

int main()
{
	ofstream f1, f2, f3, f4;

	FourierTransform* fourier = new FourierTransform(&InputSignal_f32_1kHz_15kHz[0], SIG_LEN);
	fourier->calc_dft(&real_out[0], &imag_out[0]);
	fourier->calc_dft_magnitude(&real_out[0], &imag_out[0], &mag_out[0]);

	f1.open("input.dat");
	f2.open("real_part.dat");
	f3.open("imag_part.dat");
	f4.open("dft_mag.dat");

	for(int i=0; i< SIG_LEN; i++)
	{
		f1<<InputSignal_f32_1kHz_15kHz[i]<<endl;
		f2<<real_out[i]<<endl;
		f3<<imag_out[i]<<endl;
		f4<<mag_out[i]<<endl;

//		if(i < SIG_LEN/2)
//		{
//			f2<<real_out[i]<<endl;
//			f3<<imag_out[i]<<endl;
//			f4<<mag_out[i]<<endl;
//
//		}
	}
	f1.close();
	f2.close();
	f3.close();
	f4.close();

	return 0;
}









