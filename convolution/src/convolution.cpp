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
	  double *in1;                               /* inputA pointer */
	  double *in2;                               /* inputB pointer */
	  double *pOut = sig_dest;                        /* output pointer */
	  double *_in1;                                 /* Intermediate inputA pointer */
	  double *_in2;                                 /* Intermediate inputB pointer */
	  double *in1_, *in2_;                      /* Intermediate pointers */
	  double sum, acc0, acc1, acc2, acc3;         /* Accumulator */
	  double x0, x1, x2, x3, c0;                  /* Temporary variables to hold state and coefficient values */
	  uint32_t j, k, c, block_count, block_size1, block_size2, block_size3;     /* loop cers */

	/* The algorithm implementation is based on the lengths of the inputs. */
	/* srcB (e.g. impulse response) is always made to slide across srcA. */
	/* So this->imp_len is always considered as shorter or equal to this->sig_len */

	  if(this->sig_len >= this->imp_len)
	  {
	    /* Initialization of inputA pointer */
	    in1 = this->sig_src;
	    /* Initialization of inputB pointer */
	    in2 = this->imp_res;
	  }
	   else
	  {
	    /* Initialization of inputA pointer */
	    in1 = this->imp_res;
	    /* Initialization of inputB pointer */
	    in2 = this->sig_src;

	        /* this->imp_len is always considered as shorter or equal to this->sig_len */
	    j = this->imp_len;
	    this->imp_len = this->sig_len;
	    this->sig_len = j;
	  }
	    /* conv(x,y) at n = x[n] * y[0] + x[n-1] * y[1] + x[n-2] * y[2] + ...+ x[n-N+1] * y[N -1] */
	 /* The function is internally
	   * divided into three stages according to the number of multiplications that has to be
	   * taken place between inputA samples and inputB samples. In the first stage of the
	   * algorithm, the multiplications increase by one for every iteration.
	   * In the second stage of the algorithm, this->imp_len number of multiplications are done.
	   * In the third stage of the algorithm, the multiplications decrease by one
	   * for every iteration. */
	  block_size1 = this->imp_len - 1;
	  block_size2 = this->sig_len - (this->imp_len - 1);
	  block_size3 = block_size1;

	    /* --------------------------
	   * initializations of stage1
	   * -------------------------*/
	    /* sum = x[0] * y[0]
	   * sum = x[0] * y[1] + x[1] * y[0]
	   * ....
	   * sum = x[0] * y[srcBlen - 1] + x[1] * y[srcBlen - 2] +...+ x[this->imp_len - 1] * y[0]
	   */

	     /* In this stage the MAC operations are increased by 1 for every iteration.
	     The c variable holds the number of MAC operations performed */
	  c = 1;
	  /* Working pointer of inputA */
	  _in1 = in1;
	  /* Working pointer of inputB */
	  _in2 = in2;
	    /* The first stage starts here */
	  while(block_size1 > 0)
	  {
	    /* Accumulator is made zero for every iteration */
	    sum = 0.0;
	     /* Apply loop unrolling and compute 4 MACs simultaneously. */
	    k = c >> 2;
	        /* First part of the processing with loop unrolling.  Compute 4 MACs at a time.
	     ** a second loop below computes MACs for the remaining 1 to 3 samples. */
	       while(k > 0)
	    {
	      /* x[0] * y[this->imp_len - 1] */
	      sum += *_in1++ * *_in2--;

	      /* x[1] * y[this->imp_len - 2] */
	      sum += *_in1++ * *_in2--;

	      /* x[2] * y[this->imp_len - 3] */
	      sum += *_in1++ * *_in2--;

	      /* x[3] * y[this->imp_len - 4] */
	      sum += *_in1++ * *_in2--;

	      /* Decrement the loop cer */
	      k--;
	    }
	   /* If the c is not a multiple of 4, compute any remaining MACs here.
	     ** No loop unrolling is used. */
	    k = c % 0x4;
	    while(k > 0)
	    {
	      /* Perform the multiply-accumulate */
	      sum += *_in1++ * *_in2--;

	      /* Decrement the loop cer */
	      k--;
	    }
	        /* Store the result in the accumulator in the destination buffer. */
	    *pOut++ = sum;

	    /* Update the inputA and inputB pointers for next MAC calculation */
	    _in2 = in2 + c;
	    _in1 = in1;

	    /* Increment the MAC c */
	    c++;
	      /* Decrement the loop cer */
	    block_size1--;
	  }
	    /* --------------------------
	   * Initializations of stage2
	   * ------------------------*/

	  /* sum = x[0] * y[this->imp_len-1] + x[1] * y[this->imp_len-2] +...+ x[this->imp_len-1] * y[0]
	   * sum = x[1] * y[this->imp_len-1] + x[2] * y[this->imp_len-2] +...+ x[this->imp_len] * y[0]
	   * ....
	   * sum = x[this->sig_len-this->imp_len-2] * y[this->imp_len-1] + x[this->sig_len] * y[this->imp_len-2] +...+ x[this->sig_len-1] * y[0]
	   */
	 /* Working pointer of inputA */
	  _in1 = in1;

	  /* Working pointer of inputB */
	  in2_ = in2 + (this->imp_len - 1);
	  _in2 = in2_;

	  /* c is index by which the pointer in1 to be incremented */
	  c = 0;
	    /* -------------------
	   * Stage2 process
	   * ------------------*/

	  /* Stage2 depends on this->imp_len as in this stage this->imp_len number of MACS are performed.
	   * So, to loop unroll over block_size2,
	   * this->imp_len should be greater than or equal to 4 */
	     if(this->imp_len >= 4)
	  {
	    /* Loop unroll over block_size2, by 4 */
	    block_count = block_size2 >> 2;
	     while(block_count > 0)
	    {
	      /* Set all accumulators to zero */
	      acc0 = 0.0;
	      acc1 = 0.0;
	      acc2 = 0.0;
	      acc3 = 0.0;

	      /* read x[0], x[1], x[2] samples */
	      x0 = *(_in1++);
	      x1 = *(_in1++);
	      x2 = *(_in1++);

	    /* Apply loop unrolling and compute 4 MACs simultaneously. */
	      k = this->imp_len >> 2;
	/* First part of the processing with loop unrolling.  Compute 4 MACs at a time.
	       ** a second loop below computes MACs for the remaining 1 to 3 samples. */
	      do
	      {
	        /* Read y[this->imp_len - 1] sample */
	        c0 = *(_in2--);

	        /* Read x[3] sample */
	        x3 = *(_in1);

	        /* Perform the multiply-accumulate */
	        /* acc0 +=  x[0] * y[this->imp_len - 1] */
	        acc0 += x0 * c0;

	        /* acc1 +=  x[1] * y[this->imp_len - 1] */
	        acc1 += x1 * c0;

	        /* acc2 +=  x[2] * y[this->imp_len - 1] */
	        acc2 += x2 * c0;

	        /* acc3 +=  x[3] * y[this->imp_len - 1] */
	        acc3 += x3 * c0;

	        /* Read y[this->imp_len - 2] sample */
	        c0 = *(_in2--);

	        /* Read x[4] sample */
	        x0 = *(_in1 + 1);

	        /* Perform the multiply-accumulate */
	        /* acc0 +=  x[1] * y[this->imp_len - 2] */
	        acc0 += x1 * c0;
	        /* acc1 +=  x[2] * y[this->imp_len - 2] */
	        acc1 += x2 * c0;
	        /* acc2 +=  x[3] * y[this->imp_len - 2] */
	        acc2 += x3 * c0;
	        /* acc3 +=  x[4] * y[this->imp_len - 2] */
	        acc3 += x0 * c0;

	        /* Read y[this->imp_len - 3] sample */
	        c0 = *(_in2--);

	        /* Read x[5] sample */
	        x1 = *(_in1 + 2);

	        /* Perform the multiply-accumulates */
	        /* acc0 +=  x[2] * y[this->imp_len - 3] */
	        acc0 += x2 * c0;
	        /* acc1 +=  x[3] * y[this->imp_len - 2] */
	        acc1 += x3 * c0;
	        /* acc2 +=  x[4] * y[this->imp_len - 2] */
	        acc2 += x0 * c0;
	        /* acc3 +=  x[5] * y[this->imp_len - 2] */
	        acc3 += x1 * c0;

	        /* Read y[this->imp_len - 4] sample */
	        c0 = *(_in2--);

	        /* Read x[6] sample */
	        x2 = *(_in1 + 3);
	        _in1 += 4;

	        /* Perform the multiply-accumulates */
	        /* acc0 +=  x[3] * y[this->imp_len - 4] */
	        acc0 += x3 * c0;
	        /* acc1 +=  x[4] * y[this->imp_len - 4] */
	        acc1 += x0 * c0;
	        /* acc2 +=  x[5] * y[this->imp_len - 4] */
	        acc2 += x1 * c0;
	        /* acc3 +=  x[6] * y[this->imp_len - 4] */
	        acc3 += x2 * c0;


	} while(--k);

	      /* If the this->imp_len is not a multiple of 4, compute any remaining MACs here.
	       ** No loop unrolling is used. */
	      k = this->imp_len % 0x4;

	      while(k > 0)
	      {
	        /* Read y[this->imp_len - 5] sample */
	        c0 = *(_in2--);

	        /* Read x[7] sample */
	        x3 = *(_in1++);

	        /* Perform the multiply-accumulates */
	        /* acc0 +=  x[4] * y[this->imp_len - 5] */
	        acc0 += x0 * c0;
	        /* acc1 +=  x[5] * y[this->imp_len - 5] */
	        acc1 += x1 * c0;
	        /* acc2 +=  x[6] * y[this->imp_len - 5] */
	        acc2 += x2 * c0;
	        /* acc3 +=  x[7] * y[this->imp_len - 5] */
	        acc3 += x3 * c0;

	        /* Reuse the present samples for the next MAC */
	        x0 = x1;
	        x1 = x2;
	        x2 = x3;

	        /* Decrement the loop cer */
	        k--;
	      }
	 /* Store the result in the accumulator in the destination buffer. */
	      *pOut++ = acc0;
	      *pOut++ = acc1;
	      *pOut++ = acc2;
	      *pOut++ = acc3;

	      /* Increment the pointer in1 index, c by 4 */
	      c += 4;

	      /* Update the inputA and inputB pointers for next MAC calculation */
	      _in1 = in1 + c;
	      _in2 = in2_;
	  /* Decrement the loop cer */
	      block_count--;
	    }
	block_count = block_size2 % 0x4;

	    while(block_count > 0)
	    {
	      /* Accumulator is made zero for every iteration */
	      sum = 0.0f;

	      /* Apply loop unrolling and compute 4 MACs simultaneously. */
	      k = this->imp_len >> 2;

	      /* First part of the processing with loop unrolling.  Compute 4 MACs at a time.
	       ** a second loop below computes MACs for the remaining 1 to 3 samples. */
	      while(k > 0)
	      {
	        /* Perform the multiply-accumulates */
	        sum += *_in1++ * *_in2--;
	        sum += *_in1++ * *_in2--;
	        sum += *_in1++ * *_in2--;
	        sum += *_in1++ * *_in2--;

	        /* Decrement the loop cer */
	        k--;
	      }

	      /* If the this->imp_len is not a multiple of 4, compute any remaining MACs here.
	       ** No loop unrolling is used. */
	      k = this->imp_len % 0x4;

	      while(k > 0)
	      {
	        /* Perform the multiply-accumulate */
	        sum += *_in1++ * *_in2--;

	        /* Decrement the loop cer */
	        k--;
	      }

	      /* Store the result in the accumulator in the destination buffer. */
	      *pOut++ = sum;

	      /* Increment the MAC c */
	      c++;

	      /* Update the inputA and inputB pointers for next MAC calculation */
	      _in1 = in1 + c;
	      _in2 = in2_;

	      /* Decrement the loop cer */
	      block_count--;
	    }
	  }
	 else
	  {
	    /* If the this->imp_len is not a multiple of 4,
	     * the block_size2 loop cannot be unrolled by 4 */
	    block_count = block_size2;

	    while(block_count > 0)
	    {
	      /* Accumulator is made zero for every iteration */
	      sum = 0.0;

	      /* this->imp_len number of MACS should be performed */
	      k = this->imp_len;

	      while(k > 0)
	      {
	        /* Perform the multiply-accumulate */
	        sum += *_in1++ * *_in2--;

	        /* Decrement the loop cer */
	        k--;
	      }
	/* Store the result in the accumulator in the destination buffer. */
	      *pOut++ = sum;

	      /* Increment the MAC c */
	      c++;

	      /* Update the inputA and inputB pointers for next MAC calculation */
	      _in1 = in1 + c;
	      _in2 = in2_;

	      /* Decrement the loop cer */
	      block_count--;
	    }
	  }

	/* --------------------------
	   * Initializations of stage3
	   * -------------------------*/

	  /* sum += x[this->sig_len-this->imp_len+1] * y[this->imp_len-1] + x[this->sig_len-this->imp_len+2] * y[this->imp_len-2] +...+ x[this->sig_len-1] * y[1]
	   * sum += x[this->sig_len-this->imp_len+2] * y[this->imp_len-1] + x[this->sig_len-this->imp_len+3] * y[this->imp_len-2] +...+ x[this->sig_len-1] * y[2]
	   * ....
	   * sum +=  x[this->sig_len-2] * y[this->imp_len-1] + x[this->sig_len-1] * y[this->imp_len-2]
	   * sum +=  x[this->sig_len-1] * y[this->imp_len-1]
	   */

	  /* In this stage the MAC operations are decreased by 1 for every iteration.
	     The block_size3 variable holds the number of MAC operations performed */

	  /* Working pointer of inputA */
	  in1_ = (in1 + this->sig_len) - (this->imp_len - 1);
	  _in1 = in1_;

	  /* Working pointer of inputB */
	  in2_ = in2 + (this->imp_len - 1);
	  _in2 = in2_;

	  /* -------------------
	   * Stage3 process
	   * ------------------*/
	 while(block_size3 > 0)
	  {
	    /* Accumulator is made zero for every iteration */
	    sum = 0.0;

	    /* Apply loop unrolling and compute 4 MACs simultaneously. */
	    k = block_size3 >> 2;

	    /* First part of the processing with loop unrolling.  Compute 4 MACs at a time.
	     ** a second loop below computes MACs for the remaining 1 to 3 samples. */
	    while(k > 0)
	    {
	      /* sum += x[this->sig_len - this->imp_len + 1] * y[this->imp_len - 1] */
	      sum += *_in1++ * *_in2--;

	      /* sum += x[this->sig_len - this->imp_len + 2] * y[this->imp_len - 2] */
	      sum += *_in1++ * *_in2--;

	      /* sum += x[this->sig_len - this->imp_len + 3] * y[this->imp_len - 3] */
	      sum += *_in1++ * *_in2--;

	      /* sum += x[this->sig_len - this->imp_len + 4] * y[this->imp_len - 4] */
	      sum += *_in1++ * *_in2--;

	      /* Decrement the loop cer */
	      k--;
	    }
	/* If the block_size3 is not a multiple of 4, compute any remaining MACs here.
	     ** No loop unrolling is used. */
	    k = block_size3 % 0x4;

	    while(k > 0)
	    {
	      /* Perform the multiply-accumulates */
	      /* sum +=  x[this->sig_len-1] * y[this->imp_len-1] */
	      sum += *_in1++ * *_in2--;

	      /* Decrement the loop cer */
	      k--;
	    }

	    /* Store the result in the accumulator in the destination buffer. */
	    *pOut++ = sum;

	    /* Update the inputA and inputB pointers for next MAC calculation */
	    _in1 = ++in1_;
	    _in2 = in2_;

	    /* Decrement the loop cer */
	    block_size3--;
	  }
}








