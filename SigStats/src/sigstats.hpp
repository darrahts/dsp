/*
    Author: Tim Darrah
*/

#ifndef SIGSTATS_H
#define SIGSTATS_H

class SigStats
{
private:
    double* sig_src;
    int sig_len;

public:
    //constructor
    SigStats(double* _sig_src, int _sig_len);

    //calculates all stats
    void calc_stats();

    //uses general loop
    double calc_sig_mean();
    double calc_sig_var();
    double calc_sig_std();

    //these use loop unrolling
    double sig_mean();
    double sig_var();
    double sig_std();
    double sig_rms();
    double mean;
    double var;
    double std;
    double rms;
};

#endif // SigStats_H
