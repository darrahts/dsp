/*
 * fourierTransform.cpp
 *
 *  Created on: Feb 22, 2019
 *      Author: darrahts
 */
#include "fourierTransform.hpp"
#include <stdint.h>
#include <cmath>

FourierTransform::FourierTransform(double* sig_src, int sig_len) // @suppress("Class members should be properly initialized")
{
	this->sig_src = sig_src;
	this->sig_len = sig_len;

}

void FourierTransform::calc_dft(double* real_part, double* imag_part)
{
	for(int i=0; i<this->sig_len;i++)
	{
		for(int j=0; j<this->sig_len; j++)
		{
			real_part[i] = real_part[i] + this->sig_src[j]*cos(2*M_PI*i*j/this->sig_len);
			imag_part[i] = imag_part[i] - this->sig_src[j]*cos(2*M_PI*i*j/this->sig_len);
		}
	}
}

void FourierTransform::calc_dft_magnitude(double* real_part, double* imag_part, double* dft_magnitude)
{
	for(int i=0; i<this->sig_len/2;i++)
	{
		dft_magnitude[i] = sqrt(pow(real_part[i], 2) + pow(imag_part[i],2));
	}
}




