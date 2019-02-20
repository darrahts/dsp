#ifndef CONVOLUTION_H
#define CONVOLUTION_H

class Convolution
{
private:
	double* sig_src;
	double* imp_res;
	int sig_len;
	int imp_len;

public:
	Convolution(double* sig_src, double* imp_res, int sig_len, int imp_len);

	//memory efficient calculations
	void calc_convolution(double* sig_dest);
	void calc_running_sum(double* sig_dest);
	void calc_first_diff(double* sig_dest);

	//time efficient calculations
	void convolution(double* sig_dest);
};

#endif
