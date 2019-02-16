#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <cmath>

#include "sigstats.hpp"
using namespace std;

extern double InputSignal_f32_1kHz_15kHz[320];

int sig_len = sizeof(InputSignal_f32_1kHz_15kHz)/sizeof(InputSignal_f32_1kHz_15kHz[0]);



int main()
{
	SigStats *testSignal = new SigStats(&InputSignal_f32_1kHz_15kHz[0], sig_len);

//    double mean = testSignal->calc_sig_mean();
//    double var = testSignal->calc_sig_var();
//    double std = testSignal->calc_sig_std();
//	double mean = testSignal->sig_mean();
//	double var = testSignal->sig_var();
//	double std = testSignal->sig_std();
//	double rms = testSignal->sig_rms();
//    printf("mean: %f\n", mean);
//    printf("var:  %f\n", var);
//    printf("std:  %f\n", std);
//    printf("rms:  %f\n", rms);
//    printf("*****\n");

    testSignal->calc_stats();
    printf("mean: %f\n", testSignal->mean);
    printf("var:  %f\n", testSignal->var);
    printf("std:  %f\n", testSignal->std);
    printf("rms:  %f\n", testSignal->rms);
    testSignal->calc_stats();
    return 0;
}






