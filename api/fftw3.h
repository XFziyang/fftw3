/*
 * Copyright (c) 2003 Matteo Frigo
 * Copyright (c) 2003 Massachusetts Institute of Technology
 *
 * The following statement of license applies *only* to this header file,
 * and *not* to the other files distributed with FFTW or derived therefrom:
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/***************************** NOTE TO USERS *********************************
 *
 *                 THIS IS A HEADER FILE, NOT A MANUAL
 *
 *    If you want to know how to use FFTW, please read the manual,
 *    online at http://www.fftw.org/doc/ and also included with FFTW.
 *    For a quick start, see the manual's tutorial section.
 *
 *   (Reading header files to learn how to use a library is a habit
 *    stemming from code lacking a proper manual.  Arguably, it's a
 *    *bad* habit in most cases, because header files can contain
 *    interfaces that are not part of the public, stable API.)
 *
 ****************************************************************************/

/* header file for fftw3 */
/* (The following is the CVS ID for this file, *not* the version
   number of FFTW:) */
/* $Id: fftw3.h,v 1.78 2006-01-04 00:34:03 athena Exp $ */

#ifndef FFTW3_H
#define FFTW3_H

#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* If <complex.h> is included, use the C99 complex type.  Otherwise
   define a type bit-compatible with C99 complex */
#if !defined(FFTW_NO_Complex) && defined(_Complex_I) && defined(complex) && defined(I)
#  define FFTW_DEFINE_COMPLEX(R, C) typedef R _Complex C
#else
#  define FFTW_DEFINE_COMPLEX(R, C) typedef R C[2]
#endif

#define FFTW_CONCAT(prefix, name) prefix ## name
#define FFTW_MANGLE_DOUBLE(name) FFTW_CONCAT(fftw_, name)
#define FFTW_MANGLE_FLOAT(name) FFTW_CONCAT(fftwf_, name)
#define FFTW_MANGLE_LONG_DOUBLE(name) FFTW_CONCAT(fftwl_, name)

/* annoying Windows syntax for shared-library variables */
#if defined(_WIN32) || defined(__WIN32__)
#  if defined(COMPILING_FFTW) /* we are compiling FFTW: export symbol */
#    define FFTW_DECL(type) __declspec(dllexport) type
#  else /* user is calling FFTW; assume(?) DLL is used and import symbol */
#    define FFTW_DECL(type) __declspec(dllimport) type
#  endif
#else
#  define FFTW_DECL(type) type
#endif

enum fftw_r2r_kind_do_not_use_me {
     FFTW_R2HC=0, FFTW_HC2R=1, FFTW_DHT=2,
     FFTW_REDFT00=3, FFTW_REDFT01=4, FFTW_REDFT10=5, FFTW_REDFT11=6,
     FFTW_RODFT00=7, FFTW_RODFT01=8, FFTW_RODFT10=9, FFTW_RODFT11=10
};

struct fftw_iodim_do_not_use_me {
     int n;                     /* dimension size */
     int is;			/* input stride */
     int os;			/* output stride */
};

/*
  huge second-order macro that defines prototypes for all API
  functions.  We expand this macro for each supported precision
 
  X: name-mangling macro
  R: real data type
  C: complex data type
*/

#define FFTW_DEFINE_API(X, R, C)					\
									\
FFTW_DEFINE_COMPLEX(R, C);						\
									\
typedef struct X(plan_s) *X(plan);					\
extern FFTW_DECL(double) X(timelimit);		                        \
									\
typedef struct fftw_iodim_do_not_use_me X(iodim);			\
									\
typedef enum fftw_r2r_kind_do_not_use_me X(r2r_kind);			\
									\
void X(execute)(const X(plan) p);					\
									\
X(plan) X(plan_dft)(int rank, const int *n,				\
		    C *in, C *out, int sign, unsigned flags);		\
									\
X(plan) X(plan_dft_1d)(int n, C *in, C *out, int sign,			\
		       unsigned flags);					\
X(plan) X(plan_dft_2d)(int nx, int ny,					\
		       C *in, C *out, int sign, unsigned flags);	\
X(plan) X(plan_dft_3d)(int nx, int ny, int nz,				\
		       C *in, C *out, int sign, unsigned flags);	\
									\
X(plan) X(plan_many_dft)(int rank, const int *n,			\
                         int howmany,					\
                         C *in, const int *inembed,			\
                         int istride, int idist,			\
                         C *out, const int *onembed,			\
                         int ostride, int odist,			\
                         int sign, unsigned flags);			\
									\
X(plan) X(plan_guru_dft)(int rank, const X(iodim) *dims,		\
			 int howmany_rank,				\
			 const X(iodim) *howmany_dims,			\
			 C *in, C *out,					\
			 int sign, unsigned flags);			\
X(plan) X(plan_guru_split_dft)(int rank, const X(iodim) *dims,		\
			 int howmany_rank,				\
			 const X(iodim) *howmany_dims,			\
			 R *ri, R *ii, R *ro, R *io,			\
			 unsigned flags);				\
									\
void X(execute_dft)(const X(plan) p, C *in, C *out);			\
void X(execute_split_dft)(const X(plan) p, R *ri, R *ii, R *ro, R *io);	\
									\
X(plan) X(plan_many_dft_r2c)(int rank, const int *n,			\
                             int howmany,				\
                             R *in, const int *inembed,			\
                             int istride, int idist,			\
                             C *out, const int *onembed,		\
                             int ostride, int odist,			\
                             unsigned flags);				\
									\
X(plan) X(plan_dft_r2c)(int rank, const int *n,				\
                        R *in, C *out, unsigned flags);			\
									\
X(plan) X(plan_dft_r2c_1d)(int n,R *in,C *out,unsigned flags);		\
X(plan) X(plan_dft_r2c_2d)(int nx, int ny,				\
			   R *in, C *out, unsigned flags);		\
X(plan) X(plan_dft_r2c_3d)(int nx, int ny,				\
			   int nz,					\
			   R *in, C *out, unsigned flags);		\
									\
									\
X(plan) X(plan_many_dft_c2r)(int rank, const int *n,			\
			     int howmany,				\
			     C *in, const int *inembed,			\
			     int istride, int idist,			\
			     R *out, const int *onembed,		\
			     int ostride, int odist,			\
			     unsigned flags);				\
									\
X(plan) X(plan_dft_c2r)(int rank, const int *n,				\
                        C *in, R *out, unsigned flags);			\
									\
X(plan) X(plan_dft_c2r_1d)(int n,C *in,R *out,unsigned flags);		\
X(plan) X(plan_dft_c2r_2d)(int nx, int ny,				\
			   C *in, R *out, unsigned flags);		\
X(plan) X(plan_dft_c2r_3d)(int nx, int ny,				\
			   int nz,					\
			   C *in, R *out, unsigned flags);		\
									\
X(plan) X(plan_guru_dft_r2c)(int rank, const X(iodim) *dims,		\
			     int howmany_rank,				\
			     const X(iodim) *howmany_dims,		\
			     R *in, C *out,				\
			     unsigned flags);				\
X(plan) X(plan_guru_dft_c2r)(int rank, const X(iodim) *dims,		\
			     int howmany_rank,				\
			     const X(iodim) *howmany_dims,		\
			     C *in, R *out,				\
			     unsigned flags);				\
									\
X(plan) X(plan_guru_split_dft_r2c)(int rank, const X(iodim) *dims,	\
			     int howmany_rank,				\
			     const X(iodim) *howmany_dims,		\
			     R *in, R *ro, R *io,			\
			     unsigned flags);				\
X(plan) X(plan_guru_split_dft_c2r)(int rank, const X(iodim) *dims,	\
			     int howmany_rank,				\
			     const X(iodim) *howmany_dims,		\
			     R *ri, R *ii, R *out,			\
			     unsigned flags);				\
									\
void X(execute_dft_r2c)(const X(plan) p, R *in, C *out);		\
void X(execute_dft_c2r)(const X(plan) p, C *in, R *out);		\
									\
void X(execute_split_dft_r2c)(const X(plan) p, R *in, R *ro, R *io);	\
void X(execute_split_dft_c2r)(const X(plan) p, R *ri, R *ii, R *out);	\
									\
X(plan) X(plan_many_r2r)(int rank, const int *n,			\
                         int howmany,					\
                         R *in, const int *inembed,			\
                         int istride, int idist,			\
                         R *out, const int *onembed,			\
                         int ostride, int odist,			\
                         const X(r2r_kind) *kind, unsigned flags);	\
									\
X(plan) X(plan_r2r)(int rank, const int *n, R *in, R *out,		\
                    const X(r2r_kind) *kind, unsigned flags);		\
									\
X(plan) X(plan_r2r_1d)(int n, R *in, R *out,				\
                       X(r2r_kind) kind, unsigned flags);		\
X(plan) X(plan_r2r_2d)(int nx, int ny, R *in, R *out,			\
                       X(r2r_kind) kindx, X(r2r_kind) kindy,		\
                       unsigned flags);					\
X(plan) X(plan_r2r_3d)(int nx, int ny, int nz,				\
                       R *in, R *out, X(r2r_kind) kindx,		\
                       X(r2r_kind) kindy, X(r2r_kind) kindz,		\
                       unsigned flags);					\
									\
X(plan) X(plan_guru_r2r)(int rank, const X(iodim) *dims,		\
                         int howmany_rank,				\
                         const X(iodim) *howmany_dims,			\
                         R *in, R *out,					\
                         const X(r2r_kind) *kind, unsigned flags);	\
void X(execute_r2r)(const X(plan) p, R *in, R *out);			\
									\
void X(destroy_plan)(X(plan) p);					\
void X(forget_wisdom)(void);						\
void X(cleanup)(void);							\
									\
void X(plan_with_nthreads)(int nthreads);				\
int X(init_threads)(void);						\
void X(cleanup_threads)(void);						\
									\
void X(export_wisdom_to_file)(FILE *output_file);			\
char *X(export_wisdom_to_string)(void);					\
void X(export_wisdom)(void (*write_char)(char c, void *), void *data);	\
int X(import_system_wisdom)(void);					\
int X(import_wisdom_from_file)(FILE *input_file);			\
int X(import_wisdom_from_string)(const char *input_string);		\
int X(import_wisdom)(int (*read_char)(void *), void *data);		\
									\
void X(fprint_plan)(const X(plan) p, FILE *output_file);		\
void X(print_plan)(const X(plan) p);					\
									\
void *X(malloc)(size_t n);						\
void X(free)(void *p);							\
									\
void X(flops)(const X(plan) p, double *add, double *mul, double *fmas);	\
double X(estimate_cost)(const X(plan) p);				\
									\
extern const char X(version)[];						\
extern const char X(cc)[];						\
extern const char X(codelet_optim)[];


/* end of FFTW_DEFINE_API macro */

FFTW_DEFINE_API(FFTW_MANGLE_DOUBLE, double, fftw_complex)
FFTW_DEFINE_API(FFTW_MANGLE_FLOAT, float, fftwf_complex)
FFTW_DEFINE_API(FFTW_MANGLE_LONG_DOUBLE, long double, fftwl_complex)

#define FFTW_FORWARD (-1)
#define FFTW_BACKWARD (+1)

/* documented flags */
#define FFTW_MEASURE (0U)
#define FFTW_DESTROY_INPUT (1U << 0)
#define FFTW_UNALIGNED (1U << 1)
#define FFTW_CONSERVE_MEMORY (1U << 2)
#define FFTW_EXHAUSTIVE (1U << 3) /* NO_EXHAUSTIVE is default */
#define FFTW_PRESERVE_INPUT (1U << 4) /* cancels FFTW_DESTROY_INPUT */
#define FFTW_PATIENT (1U << 5) /* IMPATIENT is default */
#define FFTW_ESTIMATE (1U << 6)
#define FFTW_TIMELIMIT (1U << 30)

/* undocumented beyond-guru flags */
#define FFTW_ESTIMATE_PATIENT (1U << 7)
#define FFTW_BELIEVE_PCOST (1U << 8)
#define FFTW_NO_DFT_R2HC (1U << 9)
#define FFTW_NO_NONTHREADED (1U << 10)
#define FFTW_NO_BUFFERING (1U << 11)
#define FFTW_NO_INDIRECT_OP (1U << 12)
#define FFTW_ALLOW_LARGE_GENERIC (1U << 13) /* NO_LARGE_GENERIC is default */
#define FFTW_NO_RANK_SPLITS (1U << 14)
#define FFTW_NO_VRANK_SPLITS (1U << 15)
#define FFTW_NO_VRECURSE (1U << 16)
#define FFTW_NO_SIMD (1U << 17)
#define FFTW_NO_SLOW (1U << 18)
#define FFTW_NO_FIXED_RADIX_LARGE_N (1U << 19)
#define FFTW_ALLOW_PRUNING (1U << 20)

#ifdef __cplusplus
}  /* extern "C" */
#endif /* __cplusplus */

#endif /* FFTW3_H */
