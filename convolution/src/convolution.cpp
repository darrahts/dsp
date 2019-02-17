//============================================================================
// Name        : convolution.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "convolution.hpp"

Convolution::Convolution(double* sig_src, double* imp_res, int sig_len, int imp_len)
{
	this->sig_src = sig_src;
	this->imp_res = imp_res;
	this->sig_len = sig_len;
	this->imp_len = imp_len;
}


void Convolution::convolution(double* sig_dest)
{
	int i,j;
	for(i=0; i<(this->sig_len+this->imp_len);i++)
	{
		sig_dest[i] = 0;
	}
	for(i=0;i<this->sig_len;i++)
	{
		//printf("i: %d\n", i);
		for(j=0; j<this->imp_len;j++)
		{
		//	printf("j: %d\n", j);
			sig_dest[i+j] = sig_dest[i+j] + this->sig_src[i]*this->imp_res[j];
		}
	}
}

// Good use for peak detection
void Convolution::calc_running_sum(double* sig_dest)
{
	for(int i=0; i< this->sig_len; i++)
	{
		sig_dest[i] = sig_dest[i-1] + this->sig_src[i];
	}
}














