//============================================================================
// Name        : convolution.cpp
// Author      : Tim Darrah
// Email	   : timothy.s.darrah@vanderbilt.edu
// Version     :
// Copyright   : MIT
// Description : Signal convolution
//============================================================================

#include "convolution.hpp"
#include <stdint.h>

Convolution::Convolution(double* sig_src, double* imp_res, int sig_len, int imp_len)
{
	this->sig_src = sig_src;
	this->imp_res = imp_res;
	this->sig_len = sig_len;
	this->imp_len = imp_len;
}


void Convolution::calc_convolution(double* sig_dest)
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



void Convolution::calc_first_diff(double* sig_dest)
{
	for(int i=0; i<this->sig_len; i++)
	{
		sig_dest[i] = sig_src[i] - this->sig_src[i-1];
	}
}


void Convolution::convolution(double* sig_dest)
{
	double* in1;
	double* in2;
	double* out = sig_dest;

	double* _in1; //intermediate pointer px
	double* _in2; //intermediate pointer py
	double* in1_; //intermediate pointer psrc1
	double* in2_; //intermediate pointer psrc2

	double sum, acc0, acc1, acc2, acc3;
	double x0, x1, x2, x3, c0; //hold state and coefficient vals
	uint32_t j,k,c, block_count, block_size1, block_size2, block_size3;

	if(this->sig_len >= this->imp_len)
	{
		in1 = this->sig_src;
		in2 = this->imp_res;
	}
	else
	{
		in1 = this->imp_res;
		in2 = this->sig_src;
		j = this->imp_len;
		this->imp_len = this->sig_len;
		this->sig_len = j;
	}

	block_size1 = this->imp_len-1;
	block_size2 = this->sig_len - block_size1;
	block_size3 = block_size1;

	_in1 = in1;
	_in2 = in2;
	c = 1;

	while(block_size1 >0)
	{
		sum = 0.0;
		k = c >> 2; // % 4
		while(k > 0)
		{
			sum += *_in1++ * *_in2--;
			sum += *_in1++ * *_in2--;
			sum += *_in1++ * *_in2--;
			sum += *_in1++ * *_in2--;
			k--;
		}
		k = c % 4;
		while(k > 0)
		{
			sum += *_in1++ * *_in2--;
			k--;
		}
		*out++ = sum;
		_in2 = in2 + c;
		_in1 = in1;
		c++;
		block_size1--;
	}

	_in1 = in1;

}

void Convolution::running_sum(double* sig_dest)
{

}

void Convolution::first_diff(double* sig_dest)
{

}









