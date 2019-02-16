/*
    Author: Tim Darrah
*/

#include "sigstats.hpp"
#include <stdint.h>
#include <stdio.h>

#include <cmath>

SigStats::SigStats(double* _sig_src, int _sig_len)
{
    this->sig_src = _sig_src;
    this->sig_len = _sig_len;
    this->mean = 0.0;
    this->var = 0.0;
    this->std = 0.0;
    this->rms = 0.0;

}


double SigStats::calc_sig_mean()
{
    double _mean = 0.0;
    for(int i = 0; i < sig_len; i++)
    {
        _mean = _mean + sig_src[i];
    }
    _mean = _mean / double(sig_len);
    return _mean;
}

double SigStats::calc_sig_var()
{
    double _mean = calc_sig_mean();
    double _var = 0.0;
    for(int i=0; i < sig_len; i++)
    {
        _var = _var + pow((sig_src[i] - _mean), 2);
    }
    _var = _var / (double)(sig_len - 1);
    return _var;
}

double SigStats::calc_sig_std()
{
    double sig_var = calc_sig_var();
    return sqrt(sig_var);
}


//sig mean with loop unrolling
double SigStats::sig_mean()
{
	//cumulative mean
	double sum=0.0;
	//counter
	uint32_t block_count;
	//loop unrolling counts 4 at a time
	double in1, in2, in3, in4;

	//block_count = sig_len / 2^2
	block_count = sig_len >> 2U;

	double *input = sig_src;
	while(block_count > 0)
	{
		//grab the first element, then increment the counter
		in1 = *input++;
		in2 = *input++;
		in3 = *input++;
		in4 = *input++;

		sum += (in1+in2+in3+in4);
		block_count--;
	}
	//handle the remaining blocks if block size % 4 != 0
	block_count = sig_len % 4;
	while(block_count > 0)
	{
		sum += *input++;
		block_count--;
	}
	this->mean = (sum / (double)sig_len);
	return this->mean;
}

//sig var with loop unrolling
double SigStats::sig_var()
{
	double _mean, _val;
	uint32_t block_count;

	double _sum = 0.0;
	if(sig_len <= 1)
	{
		return 0.0;
	}
	_mean = this->sig_mean();

	block_count = sig_len >> 2U;

	double* input = sig_src;
	block_count = sig_len >>2U;
	int i = 0;
	while(block_count > 0)
	{
		for(i = 0; i < 4; i++)
		{
			_val = *input++ - _mean;
			_sum += _val * _val;
		}

		block_count--;
	}
	block_count = sig_len % 4;
	while(block_count > 0)
	{
		_val = *input++ - _mean;
		_sum += _val * _val;
		block_count--;
	}
	this->var = (_sum / (double)(sig_len - 1.0));
	return this->var;
}

//sig std with loop unrolling
double SigStats::sig_std()
{
	this->std = sqrt(this->var);
    return this->std;;
}


double SigStats::sig_rms()
{
	double sum = 0.0;
	uint32_t block_count;
	double in;

	block_count = sig_len >> 2;
	int i = 0;
	double* input = sig_src;
	while(block_count > 0)
	{
		for(i=0; i<4; i++)
		{
			in = *input++;
			sum += in*in;
		}
		block_count--;
	}
	block_count = sig_len%4;
	while(block_count >0)
	{
		in = *input++;
		sum += in*in;
		block_count--;
	}
	this->rms = sqrt(sum/(double)sig_len);
	return this->rms;
}

void SigStats::calc_stats()
{
	this->sig_mean();
	this->sig_var();
	this->sig_std();
	this->sig_rms();
}





