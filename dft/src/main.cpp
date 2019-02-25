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

#define SIG_LEN 640//320

//extern double InputSignal_f32_1kHz_15kHz[320];
//the dft is mirrored, so really only need half
//just use the full dft and show its reflection about the midpoint
//TODO make these public class members of the FourierTransform class
//double real_out[SIG_LEN];
//double imag_out[SIG_LEN];
//double mag_out[SIG_LEN];
//double sig_orig[SIG_LEN];

extern double ecg_raw_sig[640];
double real_out[SIG_LEN];
double imag_out[SIG_LEN];
double mag_out[SIG_LEN];
double sig_orig[SIG_LEN];

using namespace std;

void dft_test(FourierTransform* fourier)
{
	ofstream f1, f2, f3, f4;
	fourier->calc_dft(&real_out[0], &imag_out[0]);

	fourier->calc_dft_magnitude(&mag_out[0], &real_out[0], &imag_out[0]);

	f1.open("input.dat");
	f2.open("real_part.dat");
	f3.open("imag_part.dat");
	f4.open("dft_mag.dat");

	for(int i=0; i< SIG_LEN; i++)
	{
		f1<<ecg_raw_sig[i]<<endl;
		f2<<real_out[i]<<endl;
		f3<<imag_out[i]<<endl;
		f4<<mag_out[i]<<endl;
	}

	f1.close();
	f2.close();
	f3.close();
	f4.close();
}

void inverse_dft_test(FourierTransform* fourier)
{
	fourier->calc_inverse_dft(&sig_orig[0], &real_out[0], &imag_out[0]);
	ofstream f5;
	f5.open("sig_orig.dat");
	for(int i=0; i< SIG_LEN; i++)
	{
		f5<<sig_orig[i]<<endl;
	}

	f5.close();
}

int main()
{
//	FourierTransform* fourier = new FourierTransform(&InputSignal_f32_1kHz_15kHz[0], SIG_LEN);
//	dft_test(fourier);
//	inverse_dft_test(fourier);

	FourierTransform* fourier = new FourierTransform(&ecg_raw_sig[0], SIG_LEN);
	dft_test(fourier);
	inverse_dft_test(fourier);

	return 0;
}









