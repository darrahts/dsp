/*
 * fourierTransform.hpp
 *
 *  Created on: Feb 22, 2019
 *      Author: darrahts
 */

#ifndef FOURIERTRANSFORM_HPP_
#define FOURIERTRANSFORM_HPP_

class FourierTransform
{
	private:
		double* sig_src;
		int sig_len;

	public:
		FourierTransform(double* sig_src, int sig_len);

		//calculates the dft of a signal
		void calc_dft(double* real_part, double* imag_part);

		//calculates magnitude of dft (i.e. what frequencies are present in signal)
		void calc_dft_magnitude(double* dft_mag, double* real_part, double* imag_part);

//		double* real_part;
//		double* imag_part;
//		double* dft_magnitude;

};



#endif /* FOURIERTRANSFORM_HPP_ */
