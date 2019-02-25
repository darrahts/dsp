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

double real_out[int(SIG_LEN/2)];
double imag_out[int(SIG_LEN/2)];
double mag_out[int(SIG_LEN/2)];

using namespace std;

int main()
{
	FourierTransform* fourier = new FourierTransform(&InputSignal_f32_1kHz_15kHz[0], SIG_LEN);
	fourier->calc_dft(&real_out[0], &imag_out[0]);
	fourier->calc_dft_magnitude(&real_out[0], &imag_out[0], &mag_out[0]);

	return 0;
}
