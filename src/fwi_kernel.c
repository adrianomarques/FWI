/*
 * =============================================================================
 * Copyright (c) 2016-2018, Barcelona Supercomputing Center (BSC)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * =============================================================================
 */

#include "fwi/fwi_kernel.h"

/*
 * Initializes an array of length "length" to a random number.
 */
void set_array_to_random_real( real* restrict array, const integer length)
{
    const real randvalue = rand() / (1.0 * RAND_MAX);

    print_debug("Array is being initialized to %f", randvalue);

    set_array_to_constant(array, randvalue, length);
}

/*
 * Initializes an array of length "length" to a constant floating point value.
 */
void set_array_to_constant( real* restrict array, const real value, const integer length)
{
#if defined(_OPENACC)
    #pragma acc kernels copyin(array[0:length])
#endif
    for( integer i = 0; i < length; i++ )
        array[i] = value;
}

void check_memory_shot( const integer dimmz,
                        const integer dimmx,
                        const integer dimmy,
                        coeff_t *c,
                        s_t     *s,
                        v_t     *v,
                        real    *rho)
{
#if defined(DEBUG)
    print_debug("Checking memory shot values");

    real UNUSED(value);
    const integer size = dimmz * dimmx * dimmy;
    for( int i=0; i < size; i++)
    {
        value = c->c11[i];
        value = c->c12[i];
        value = c->c13[i];
        value = c->c14[i];
        value = c->c15[i];
        value = c->c16[i];

        value = c->c22[i];
        value = c->c23[i];
        value = c->c24[i];
        value = c->c25[i];
        value = c->c26[i];

        value = c->c33[i];
        value = c->c34[i];
        value = c->c35[i];
        value = c->c36[i];

        value = c->c44[i];
        value = c->c45[i];
        value = c->c46[i];

        value = c->c55[i];
        value = c->c56[i];
        value = c->c66[i];

        value = v->tl.u[i];
        value = v->tl.v[i];
        value = v->tl.w[i];

        value = v->tr.u[i];
        value = v->tr.v[i];
        value = v->tr.w[i];

        value = v->bl.u[i];
        value = v->bl.v[i];
        value = v->bl.w[i];

        value = v->br.u[i];
        value = v->br.v[i];
        value = v->br.w[i];

        value = rho[i];
    }
#endif /* end of pragma DEBUG */
};


void alloc_memory_shot( const integer dimmz,
                        const integer dimmx,
                        const integer dimmy,
                        coeff_t *c,
                        s_t     *s,
                        v_t     *v,
                        real    **rho)
{
    PUSH_RANGE

    const integer ncells = dimmz * dimmx * dimmy;
    const size_t size    = ncells * sizeof(real);

    print_debug("ptr size = " I " bytes ("I" elements)", 
            size, (size_t) ncells);

    /* allocate coefficients */
    c->c11 = (real*) __malloc( ALIGN_REAL, size);
    c->c12 = (real*) __malloc( ALIGN_REAL, size);
    c->c13 = (real*) __malloc( ALIGN_REAL, size);
    c->c14 = (real*) __malloc( ALIGN_REAL, size);
    c->c15 = (real*) __malloc( ALIGN_REAL, size);
    c->c16 = (real*) __malloc( ALIGN_REAL, size);

    c->c22 = (real*) __malloc( ALIGN_REAL, size);
    c->c23 = (real*) __malloc( ALIGN_REAL, size);
    c->c24 = (real*) __malloc( ALIGN_REAL, size);
    c->c25 = (real*) __malloc( ALIGN_REAL, size);
    c->c26 = (real*) __malloc( ALIGN_REAL, size);

    c->c33 = (real*) __malloc( ALIGN_REAL, size);
    c->c34 = (real*) __malloc( ALIGN_REAL, size);
    c->c35 = (real*) __malloc( ALIGN_REAL, size);
    c->c36 = (real*) __malloc( ALIGN_REAL, size);

    c->c44 = (real*) __malloc( ALIGN_REAL, size);
    c->c45 = (real*) __malloc( ALIGN_REAL, size);
    c->c46 = (real*) __malloc( ALIGN_REAL, size);

    c->c55 = (real*) __malloc( ALIGN_REAL, size);
    c->c56 = (real*) __malloc( ALIGN_REAL, size);
    c->c66 = (real*) __malloc( ALIGN_REAL, size);

    /* allocate velocity components */
    v->tl.u = (real*) __malloc( ALIGN_REAL, size);
    v->tl.v = (real*) __malloc( ALIGN_REAL, size);
    v->tl.w = (real*) __malloc( ALIGN_REAL, size);

    v->tr.u = (real*) __malloc( ALIGN_REAL, size);
    v->tr.v = (real*) __malloc( ALIGN_REAL, size);
    v->tr.w = (real*) __malloc( ALIGN_REAL, size);

    v->bl.u = (real*) __malloc( ALIGN_REAL, size);
    v->bl.v = (real*) __malloc( ALIGN_REAL, size);
    v->bl.w = (real*) __malloc( ALIGN_REAL, size);

    v->br.u = (real*) __malloc( ALIGN_REAL, size);
    v->br.v = (real*) __malloc( ALIGN_REAL, size);
    v->br.w = (real*) __malloc( ALIGN_REAL, size);

    /* allocate stress components   */
    s->tl.zz = (real*) __malloc( ALIGN_REAL, size);
    s->tl.xz = (real*) __malloc( ALIGN_REAL, size);
    s->tl.yz = (real*) __malloc( ALIGN_REAL, size);
    s->tl.xx = (real*) __malloc( ALIGN_REAL, size);
    s->tl.xy = (real*) __malloc( ALIGN_REAL, size);
    s->tl.yy = (real*) __malloc( ALIGN_REAL, size);

    s->tr.zz = (real*) __malloc( ALIGN_REAL, size);
    s->tr.xz = (real*) __malloc( ALIGN_REAL, size);
    s->tr.yz = (real*) __malloc( ALIGN_REAL, size);
    s->tr.xx = (real*) __malloc( ALIGN_REAL, size);
    s->tr.xy = (real*) __malloc( ALIGN_REAL, size);
    s->tr.yy = (real*) __malloc( ALIGN_REAL, size);

    s->bl.zz = (real*) __malloc( ALIGN_REAL, size);
    s->bl.xz = (real*) __malloc( ALIGN_REAL, size);
    s->bl.yz = (real*) __malloc( ALIGN_REAL, size);
    s->bl.xx = (real*) __malloc( ALIGN_REAL, size);
    s->bl.xy = (real*) __malloc( ALIGN_REAL, size);
    s->bl.yy = (real*) __malloc( ALIGN_REAL, size);

    s->br.zz = (real*) __malloc( ALIGN_REAL, size);
    s->br.xz = (real*) __malloc( ALIGN_REAL, size);
    s->br.yz = (real*) __malloc( ALIGN_REAL, size);
    s->br.xx = (real*) __malloc( ALIGN_REAL, size);
    s->br.xy = (real*) __malloc( ALIGN_REAL, size);
    s->br.yy = (real*) __malloc( ALIGN_REAL, size);

    /* allocate density array       */
    *rho = (real*) __malloc( ALIGN_REAL, size);

#if defined(_OPENACC)
    const real* rrho  = *rho;

    coeff_t cc = *c;
    #pragma acc enter data create(cc)
    #pragma acc enter data create(cc.c11[:ncells])
    #pragma acc enter data create(cc.c12[:ncells])
    #pragma acc enter data create(cc.c13[:ncells])
    #pragma acc enter data create(cc.c14[:ncells])
    #pragma acc enter data create(cc.c15[:ncells])
    #pragma acc enter data create(cc.c16[:ncells])
    #pragma acc enter data create(cc.c22[:ncells])
    #pragma acc enter data create(cc.c23[:ncells])
    #pragma acc enter data create(cc.c24[:ncells])
    #pragma acc enter data create(cc.c25[:ncells])
    #pragma acc enter data create(cc.c26[:ncells])
    #pragma acc enter data create(cc.c33[:ncells])
    #pragma acc enter data create(cc.c34[:ncells])
    #pragma acc enter data create(cc.c35[:ncells])
    #pragma acc enter data create(cc.c36[:ncells])
    #pragma acc enter data create(cc.c44[:ncells])
    #pragma acc enter data create(cc.c45[:ncells])
    #pragma acc enter data create(cc.c46[:ncells])
    #pragma acc enter data create(cc.c55[:ncells])
    #pragma acc enter data create(cc.c56[:ncells])
    #pragma acc enter data create(cc.c66[:ncells])

    v_t vv = *v;

    #pragma acc enter data copyin(vv)
    #pragma acc enter data create(vv.tl.u[:ncells])
    #pragma acc enter data create(vv.tl.v[:ncells])
    #pragma acc enter data create(vv.tl.w[:ncells])
    #pragma acc enter data create(vv.tr.u[:ncells])
    #pragma acc enter data create(vv.tr.v[:ncells])
    #pragma acc enter data create(vv.tr.w[:ncells])
    #pragma acc enter data create(vv.bl.u[:ncells])
    #pragma acc enter data create(vv.bl.v[:ncells])
    #pragma acc enter data create(vv.bl.w[:ncells])
    #pragma acc enter data create(vv.br.u[:ncells])
    #pragma acc enter data create(vv.br.v[:ncells])
    #pragma acc enter data create(vv.br.w[:ncells])

    s_t ss = *s;
    #pragma acc enter data copyin(ss)
    #pragma acc enter data create(ss.tl.zz[:ncells])
    #pragma acc enter data create(ss.tl.xz[:ncells])
    #pragma acc enter data create(ss.tl.yz[:ncells])
    #pragma acc enter data create(ss.tl.xx[:ncells])
    #pragma acc enter data create(ss.tl.xy[:ncells])
    #pragma acc enter data create(ss.tl.yy[:ncells])
    #pragma acc enter data create(ss.tr.zz[:ncells])
    #pragma acc enter data create(ss.tr.xz[:ncells])
    #pragma acc enter data create(ss.tr.yz[:ncells])
    #pragma acc enter data create(ss.tr.xx[:ncells])
    #pragma acc enter data create(ss.tr.xy[:ncells])
    #pragma acc enter data create(ss.tr.yy[:ncells])
    #pragma acc enter data create(ss.bl.zz[:ncells])
    #pragma acc enter data create(ss.bl.xz[:ncells])
    #pragma acc enter data create(ss.bl.yz[:ncells])
    #pragma acc enter data create(ss.bl.xx[:ncells])
    #pragma acc enter data create(ss.bl.xy[:ncells])
    #pragma acc enter data create(ss.bl.yy[:ncells])
    #pragma acc enter data create(ss.br.zz[:ncells])
    #pragma acc enter data create(ss.br.xz[:ncells])
    #pragma acc enter data create(ss.br.yz[:ncells])
    #pragma acc enter data create(ss.br.xx[:ncells])
    #pragma acc enter data create(ss.br.xy[:ncells])
    #pragma acc enter data create(ss.br.yy[:ncells])

    #pragma acc enter data create(rrho[:ncells])

#endif /* end of pragma _OPENACC */

    POP_RANGE
};

void free_memory_shot( coeff_t *c,
                       s_t     *s,
                       v_t     *v,
                       real    **rho)
{
    PUSH_RANGE

#if defined(_OPENACC)
    #pragma acc wait

    #pragma acc exit data delete(c->c11)
    #pragma acc exit data delete(c->c12)
    #pragma acc exit data delete(c->c13)
    #pragma acc exit data delete(c->c14)
    #pragma acc exit data delete(c->c15)
    #pragma acc exit data delete(c->c16)
    #pragma acc exit data delete(c->c22)
    #pragma acc exit data delete(c->c23)
    #pragma acc exit data delete(c->c24)
    #pragma acc exit data delete(c->c25)
    #pragma acc exit data delete(c->c26)
    #pragma acc exit data delete(c->c33)
    #pragma acc exit data delete(c->c34)
    #pragma acc exit data delete(c->c35)
    #pragma acc exit data delete(c->c36)
    #pragma acc exit data delete(c->c44)
    #pragma acc exit data delete(c->c45)
    #pragma acc exit data delete(c->c46)
    #pragma acc exit data delete(c->c55)
    #pragma acc exit data delete(c->c56)
    #pragma acc exit data delete(c->c66)
    #pragma acc exit data delete(c)

    #pragma acc exit data delete(v->tl.u)
    #pragma acc exit data delete(v->tl.v)
    #pragma acc exit data delete(v->tl.w)
    #pragma acc exit data delete(v->tr.u)
    #pragma acc exit data delete(v->tr.v)
    #pragma acc exit data delete(v->tr.w)
    #pragma acc exit data delete(v->bl.u)
    #pragma acc exit data delete(v->bl.v)
    #pragma acc exit data delete(v->bl.w)
    #pragma acc exit data delete(v->br.u)
    #pragma acc exit data delete(v->br.v)
    #pragma acc exit data delete(v->br.w)


    #pragma acc exit data delete(s->tl.zz)
    #pragma acc exit data delete(s->tl.xz)
    #pragma acc exit data delete(s->tl.yz)
    #pragma acc exit data delete(s->tl.xx)
    #pragma acc exit data delete(s->tl.xy)
    #pragma acc exit data delete(s->tl.yy)
    #pragma acc exit data delete(s->tr.zz)
    #pragma acc exit data delete(s->tr.xz)
    #pragma acc exit data delete(s->tr.yz)
    #pragma acc exit data delete(s->tr.xx)
    #pragma acc exit data delete(s->tr.xy)
    #pragma acc exit data delete(s->tr.yy)
    #pragma acc exit data delete(s->bl.zz)
    #pragma acc exit data delete(s->bl.xz)
    #pragma acc exit data delete(s->bl.yz)
    #pragma acc exit data delete(s->bl.xx)
    #pragma acc exit data delete(s->bl.xy)
    #pragma acc exit data delete(s->bl.yy)
    #pragma acc exit data delete(s->br.zz)
    #pragma acc exit data delete(s->br.xz)
    #pragma acc exit data delete(s->br.yz)
    #pragma acc exit data delete(s->br.xx)
    #pragma acc exit data delete(s->br.xy)
    #pragma acc exit data delete(s->br.yy)
    #pragma acc exit data delete(s)

    const real* rrho  = *rho;
    #pragma acc exit data delete(rrho)

#endif /* end pragma _OPENACC */

    /* deallocate coefficients */
    __free( (void*) c->c11 );
    __free( (void*) c->c12 );
    __free( (void*) c->c13 );
    __free( (void*) c->c14 );
    __free( (void*) c->c15 );
    __free( (void*) c->c16 );

    __free( (void*) c->c22 );
    __free( (void*) c->c23 );
    __free( (void*) c->c24 );
    __free( (void*) c->c25 );
    __free( (void*) c->c26 );
    __free( (void*) c->c33 );

    __free( (void*) c->c34 );
    __free( (void*) c->c35 );
    __free( (void*) c->c36 );

    __free( (void*) c->c44 );
    __free( (void*) c->c45 );
    __free( (void*) c->c46 );

    __free( (void*) c->c55 );
    __free( (void*) c->c56 );

    __free( (void*) c->c66 );

    /* deallocate velocity components */
    __free( (void*) v->tl.u );
    __free( (void*) v->tl.v );
    __free( (void*) v->tl.w );

    __free( (void*) v->tr.u );
    __free( (void*) v->tr.v );
    __free( (void*) v->tr.w );

    __free( (void*) v->bl.u );
    __free( (void*) v->bl.v );
    __free( (void*) v->bl.w );

    __free( (void*) v->br.u );
    __free( (void*) v->br.v );
    __free( (void*) v->br.w );

    /* deallocate stres components   */
    __free( (void*) s->tl.zz );
    __free( (void*) s->tl.xz );
    __free( (void*) s->tl.yz );
    __free( (void*) s->tl.xx );
    __free( (void*) s->tl.xy );
    __free( (void*) s->tl.yy );

    __free( (void*) s->tr.zz );
    __free( (void*) s->tr.xz );
    __free( (void*) s->tr.yz );
    __free( (void*) s->tr.xx );
    __free( (void*) s->tr.xy );
    __free( (void*) s->tr.yy );

    __free( (void*) s->bl.zz );
    __free( (void*) s->bl.xz );
    __free( (void*) s->bl.yz );
    __free( (void*) s->bl.xx );
    __free( (void*) s->bl.xy );
    __free( (void*) s->bl.yy );

    __free( (void*) s->br.zz );
    __free( (void*) s->br.xz );
    __free( (void*) s->br.yz );
    __free( (void*) s->br.xx );
    __free( (void*) s->br.xy );
    __free( (void*) s->br.yy );


    /* deallocate density array       */
    __free( (void*) *rho );

    POP_RANGE
};

/*
 * Loads initial values from coeffs, stress and velocity.
 *
 * dimmz: number of z planes
 * dimmx: number of x planes
 * FirstYPlane: first Y plane of my local domain (includes HALO)
 * LastYPlane: last Y plane of my local domain (includes HALO)
 */
void load_local_velocity_model ( const real    waveletFreq,
                                 const integer dimmz,
                                 const integer dimmx,
                                 const integer FirstYPlane,
                                 const integer LastYPlane,
                                 coeff_t *c,
                                 s_t     *s,
                                 v_t     *v,
                                 real    *rho)
{
    PUSH_RANGE

    const integer cellsInVolume = dimmz * dimmx * (LastYPlane - FirstYPlane);

    /*
     * Material, velocities and stresses are initizalized
     * accorting to the compilation flags, either randomly
     * or by reading an input velocity model.
     */

    /* initialize stress arrays */
    set_array_to_constant( s->tl.zz, 0, cellsInVolume);
    set_array_to_constant( s->tl.xz, 0, cellsInVolume);
    set_array_to_constant( s->tl.yz, 0, cellsInVolume);
    set_array_to_constant( s->tl.xx, 0, cellsInVolume);
    set_array_to_constant( s->tl.xy, 0, cellsInVolume);
    set_array_to_constant( s->tl.yy, 0, cellsInVolume);
    set_array_to_constant( s->tr.zz, 0, cellsInVolume);
    set_array_to_constant( s->tr.xz, 0, cellsInVolume);
    set_array_to_constant( s->tr.yz, 0, cellsInVolume);
    set_array_to_constant( s->tr.xx, 0, cellsInVolume);
    set_array_to_constant( s->tr.xy, 0, cellsInVolume);
    set_array_to_constant( s->tr.yy, 0, cellsInVolume);
    set_array_to_constant( s->bl.zz, 0, cellsInVolume);
    set_array_to_constant( s->bl.xz, 0, cellsInVolume);
    set_array_to_constant( s->bl.yz, 0, cellsInVolume);
    set_array_to_constant( s->bl.xx, 0, cellsInVolume);
    set_array_to_constant( s->bl.xy, 0, cellsInVolume);
    set_array_to_constant( s->bl.yy, 0, cellsInVolume);
    set_array_to_constant( s->br.zz, 0, cellsInVolume);
    set_array_to_constant( s->br.xz, 0, cellsInVolume);
    set_array_to_constant( s->br.yz, 0, cellsInVolume);
    set_array_to_constant( s->br.xx, 0, cellsInVolume);
    set_array_to_constant( s->br.xy, 0, cellsInVolume);
    set_array_to_constant( s->br.yy, 0, cellsInVolume);

#if defined(DO_NOT_PERFORM_IO)

    /* initialize material coefficients */
    set_array_to_random_real( c->c11, cellsInVolume);
    set_array_to_random_real( c->c12, cellsInVolume);
    set_array_to_random_real( c->c13, cellsInVolume);
    set_array_to_random_real( c->c14, cellsInVolume);
    set_array_to_random_real( c->c15, cellsInVolume);
    set_array_to_random_real( c->c16, cellsInVolume);
    set_array_to_random_real( c->c22, cellsInVolume);
    set_array_to_random_real( c->c23, cellsInVolume);
    set_array_to_random_real( c->c24, cellsInVolume);
    set_array_to_random_real( c->c25, cellsInVolume);
    set_array_to_random_real( c->c26, cellsInVolume);
    set_array_to_random_real( c->c33, cellsInVolume);
    set_array_to_random_real( c->c34, cellsInVolume);
    set_array_to_random_real( c->c35, cellsInVolume);
    set_array_to_random_real( c->c36, cellsInVolume);
    set_array_to_random_real( c->c44, cellsInVolume);
    set_array_to_random_real( c->c45, cellsInVolume);
    set_array_to_random_real( c->c46, cellsInVolume);
    set_array_to_random_real( c->c55, cellsInVolume);
    set_array_to_random_real( c->c56, cellsInVolume);
    set_array_to_random_real( c->c66, cellsInVolume);

    /* initalize velocity components */
    set_array_to_random_real( v->tl.u, cellsInVolume );
    set_array_to_random_real( v->tl.v, cellsInVolume );
    set_array_to_random_real( v->tl.w, cellsInVolume );
    set_array_to_random_real( v->tr.u, cellsInVolume );
    set_array_to_random_real( v->tr.v, cellsInVolume );
    set_array_to_random_real( v->tr.w, cellsInVolume );
    set_array_to_random_real( v->bl.u, cellsInVolume );
    set_array_to_random_real( v->bl.v, cellsInVolume );
    set_array_to_random_real( v->bl.w, cellsInVolume );
    set_array_to_random_real( v->br.u, cellsInVolume );
    set_array_to_random_real( v->br.v, cellsInVolume );
    set_array_to_random_real( v->br.w, cellsInVolume );

    /* initialize density (rho) */
    set_array_to_random_real( rho, cellsInVolume );

#else /* load velocity model from external file */

    /* initialize material coefficients */
    set_array_to_constant( c->c11, 1.0, cellsInVolume);
    set_array_to_constant( c->c12, 1.0, cellsInVolume);
    set_array_to_constant( c->c13, 1.0, cellsInVolume);
    set_array_to_constant( c->c14, 1.0, cellsInVolume);
    set_array_to_constant( c->c15, 1.0, cellsInVolume);
    set_array_to_constant( c->c16, 1.0, cellsInVolume);
    set_array_to_constant( c->c22, 1.0, cellsInVolume);
    set_array_to_constant( c->c23, 1.0, cellsInVolume);
    set_array_to_constant( c->c24, 1.0, cellsInVolume);
    set_array_to_constant( c->c25, 1.0, cellsInVolume);
    set_array_to_constant( c->c26, 1.0, cellsInVolume);
    set_array_to_constant( c->c33, 1.0, cellsInVolume);
    set_array_to_constant( c->c34, 1.0, cellsInVolume);
    set_array_to_constant( c->c35, 1.0, cellsInVolume);
    set_array_to_constant( c->c36, 1.0, cellsInVolume);
    set_array_to_constant( c->c44, 1.0, cellsInVolume);
    set_array_to_constant( c->c45, 1.0, cellsInVolume);
    set_array_to_constant( c->c46, 1.0, cellsInVolume);
    set_array_to_constant( c->c55, 1.0, cellsInVolume);
    set_array_to_constant( c->c56, 1.0, cellsInVolume);
    set_array_to_constant( c->c66, 1.0, cellsInVolume);

    /* initialize density (rho) */
    set_array_to_constant( rho, 1.0, cellsInVolume );

    /* local variables */
    double tstart_outer, tstart_inner;
    double tend_outer, tend_inner;
    double iospeed_inner, iospeed_outer;
    char modelname[300];

     /* open initial model, binary file */
    sprintf( modelname, "../data/inputmodels/velocitymodel_%.2f.bin", waveletFreq );
    print_info("Loading input model %s from disk (this could take a while)", modelname);

    /* start clock, take into account file opening */
    tstart_outer = dtime();
    FILE* model = safe_fopen( modelname, "rb", __FILE__, __LINE__ );

    /* start clock, do not take into account file opening */
    tstart_inner = dtime();

    /* seek to the correct position corresponding to mpi_rank */
    if (fseek ( model, sizeof(real) * WRITTEN_FIELDS * dimmz * dimmx * FirstYPlane, SEEK_SET) != 0)
        print_error("fseek() failed to set the correct position");

    /* initalize velocity components */
    safe_fread( v->tl.u, sizeof(real), cellsInVolume, model, __FILE__, __LINE__ );
    safe_fread( v->tl.v, sizeof(real), cellsInVolume, model, __FILE__, __LINE__ );
    safe_fread( v->tl.w, sizeof(real), cellsInVolume, model, __FILE__, __LINE__ );
    safe_fread( v->tr.u, sizeof(real), cellsInVolume, model, __FILE__, __LINE__ );
    safe_fread( v->tr.v, sizeof(real), cellsInVolume, model, __FILE__, __LINE__ );
    safe_fread( v->tr.w, sizeof(real), cellsInVolume, model, __FILE__, __LINE__ );
    safe_fread( v->bl.u, sizeof(real), cellsInVolume, model, __FILE__, __LINE__ );
    safe_fread( v->bl.v, sizeof(real), cellsInVolume, model, __FILE__, __LINE__ );
    safe_fread( v->bl.w, sizeof(real), cellsInVolume, model, __FILE__, __LINE__ );
    safe_fread( v->br.u, sizeof(real), cellsInVolume, model, __FILE__, __LINE__ );
    safe_fread( v->br.v, sizeof(real), cellsInVolume, model, __FILE__, __LINE__ );
    safe_fread( v->br.w, sizeof(real), cellsInVolume, model, __FILE__, __LINE__ );

    /* stop inner timer */
    tend_inner = dtime() - tstart_inner;

    /* stop timer and compute statistics */
    safe_fclose ( modelname, model, __FILE__, __LINE__ );
    tend_outer = dtime() - tstart_outer;

    const integer bytesForVolume = WRITTEN_FIELDS * cellsInVolume * sizeof(real);

    iospeed_inner = (bytesForVolume / (1000.f * 1000.f)) / tend_inner;
    iospeed_outer = (bytesForVolume / (1000.f * 1000.f)) / tend_outer;

    print_stats("Initial velocity model loaded (%lf GB)", TOGB(1.f * bytesForVolume));
    print_stats("\tInner time %lf seconds (%lf MiB/s)", tend_inner, iospeed_inner);
    print_stats("\tOuter time %lf seconds (%lf MiB/s)", tend_outer, iospeed_outer);
    print_stats("\tDifference %lf seconds", tend_outer - tend_inner);

#if defined(_OPENACC)
    const real* vtlu = v->tl.u;
    const real* vtlv = v->tl.v;
    const real* vtlw = v->tl.w;

    const real* vtru = v->tr.u;
    const real* vtrv = v->tr.v;
    const real* vtrw = v->tr.w;

    const real* vblu = v->bl.u;
    const real* vblv = v->bl.v;
    const real* vblw = v->bl.w;

    const real* vbru = v->br.u;
    const real* vbrv = v->br.v;
    const real* vbrw = v->br.w;

    #pragma acc update device(vtlu[0:cellsInVolume], vtlv[0:cellsInVolume], vtlw[0:cellsInVolume]) \
                       device(vtru[0:cellsInVolume], vtrv[0:cellsInVolume], vtrw[0:cellsInVolume]) \
                       device(vblu[0:cellsInVolume], vblv[0:cellsInVolume], vblw[0:cellsInVolume]) \
                       device(vbru[0:cellsInVolume], vbrv[0:cellsInVolume], vbrw[0:cellsInVolume]) \
                       async(H2D)
#endif /* end of pragma _OPENACC */
#endif /* end of pragma DDO_NOT_PERFORM_IO clause */

    POP_RANGE
};


/*
 * Saves the complete velocity field to disk.
 */
void write_snapshot(char *folder,
                    int suffix,
                    v_t *v,
                    const integer dimmz,
                    const integer dimmx,
                    const integer dimmy)
{
    PUSH_RANGE

#if defined(DO_NOT_PERFORM_IO)
    print_info("We are not writing the snapshot here cause IO is not enabled!");
#else

    int rank = 0;
#if defined(USE_MPI)
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif

    const integer cellsInVolume  = dimmz * dimmx * dimmy;

#if defined(_OPENACC)
    #pragma acc update self(v->tr.u[0:cellsInVolume], v->tr.v[0:cellsInVolume], v->tr.w[0:cellsInVolume]) \
                       self(v->tl.u[0:cellsInVolume], v->tl.v[0:cellsInVolume], v->tl.w[0:cellsInVolume]) \
                       self(v->br.u[0:cellsInVolume], v->br.v[0:cellsInVolume], v->br.w[0:cellsInVolume]) \
                       self(v->bl.u[0:cellsInVolume], v->bl.v[0:cellsInVolume], v->bl.w[0:cellsInVolume])
#endif /* end pragma _OPENACC*/

    /* local variables */
    char fname[300];

    /* open snapshot file and write results */
    sprintf(fname,"%s/snapshot.%03d.%05d", folder, rank, suffix);

#if defined(LOG_IO_STATS)
    double tstart_outer = dtime();
#endif
    FILE *snapshot = safe_fopen(fname,"wb", __FILE__, __LINE__ );
#if defined(LOG_IO_STATS)
    double tstart_inner = dtime();
#endif

    safe_fwrite( v->tr.u, sizeof(real), cellsInVolume, snapshot, __FILE__, __LINE__ );
    safe_fwrite( v->tr.v, sizeof(real), cellsInVolume, snapshot, __FILE__, __LINE__ );
    safe_fwrite( v->tr.w, sizeof(real), cellsInVolume, snapshot, __FILE__, __LINE__ );

    safe_fwrite( v->tl.u, sizeof(real), cellsInVolume, snapshot, __FILE__, __LINE__ );
    safe_fwrite( v->tl.v, sizeof(real), cellsInVolume, snapshot, __FILE__, __LINE__ );
    safe_fwrite( v->tl.w, sizeof(real), cellsInVolume, snapshot, __FILE__, __LINE__ );

    safe_fwrite( v->br.u, sizeof(real), cellsInVolume, snapshot, __FILE__, __LINE__ );
    safe_fwrite( v->br.v, sizeof(real), cellsInVolume, snapshot, __FILE__, __LINE__ );
    safe_fwrite( v->br.w, sizeof(real), cellsInVolume, snapshot, __FILE__, __LINE__ );

    safe_fwrite( v->bl.u, sizeof(real), cellsInVolume, snapshot, __FILE__, __LINE__ );
    safe_fwrite( v->bl.v, sizeof(real), cellsInVolume, snapshot, __FILE__, __LINE__ );
    safe_fwrite( v->bl.w, sizeof(real), cellsInVolume, snapshot, __FILE__, __LINE__ );

#if defined(LOG_IO_STATS)
    /* stop inner timer */
    double tend_inner = dtime();
#endif
    /* close file and stop outer timer */
    safe_fclose(fname, snapshot, __FILE__, __LINE__ );
#if defined(LOG_IO_STATS)
    double tend_outer = dtime();

    double iospeed_inner = (( (double) cellsInVolume * sizeof(real) * 12.f) / (1000.f * 1000.f)) / (tend_inner - tstart_inner);
    double iospeed_outer = (( (double) cellsInVolume * sizeof(real) * 12.f) / (1000.f * 1000.f)) / (tend_outer - tstart_outer);

    print_stats("Write snapshot (%lf GB)", TOGB(cellsInVolume * sizeof(real) * 12));
    print_stats("\tInner time %lf seconds (%lf MB/s)", (tend_inner - tstart_inner), iospeed_inner);
    print_stats("\tOuter time %lf seconds (%lf MB/s)", (tend_outer - tstart_outer), iospeed_outer);
    print_stats("\tDifference %lf seconds", tend_outer - tend_inner);
#endif /* end pragma LOG_IO_STATS */
#endif /* end pragma DO_NOT_PERFORM_IO */

    POP_RANGE
};

/*
 * Reads the complete velocity field from disk.
 */
void read_snapshot(char *folder,
                   int suffix,
                   v_t *v,
                   const integer dimmz,
                   const integer dimmx,
                   const integer dimmy)
{
    PUSH_RANGE

#if defined(DO_NOT_PERFORM_IO)
    print_info("We are not reading the snapshot here cause IO is not enabled!");
#else
    /* local variables */
    char fname[300];

    int rank = 0;
#if defined(USE_MPI)
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
#endif

    /* open file and read snapshot */
    sprintf(fname,"%s/snapshot.%03d.%05d", folder, rank, suffix);

#if defined(LOG_IO_STATS)
    double tstart_outer = dtime();
#endif
    FILE *snapshot = safe_fopen(fname,"rb", __FILE__, __LINE__ );
#if defined(LOG_IO_STATS)
    double tstart_inner = dtime();
#endif

    const integer cellsInVolume  = dimmz * dimmx * dimmy;

    safe_fread( v->tr.u, sizeof(real), cellsInVolume, snapshot, __FILE__, __LINE__ );
    safe_fread( v->tr.v, sizeof(real), cellsInVolume, snapshot, __FILE__, __LINE__ );
    safe_fread( v->tr.w, sizeof(real), cellsInVolume, snapshot, __FILE__, __LINE__ );

    safe_fread( v->tl.u, sizeof(real), cellsInVolume, snapshot, __FILE__, __LINE__ );
    safe_fread( v->tl.v, sizeof(real), cellsInVolume, snapshot, __FILE__, __LINE__ );
    safe_fread( v->tl.w, sizeof(real), cellsInVolume, snapshot, __FILE__, __LINE__ );

    safe_fread( v->br.u, sizeof(real), cellsInVolume, snapshot, __FILE__, __LINE__ );
    safe_fread( v->br.v, sizeof(real), cellsInVolume, snapshot, __FILE__, __LINE__ );
    safe_fread( v->br.w, sizeof(real), cellsInVolume, snapshot, __FILE__, __LINE__ );

    safe_fread( v->bl.u, sizeof(real), cellsInVolume, snapshot, __FILE__, __LINE__ );
    safe_fread( v->bl.v, sizeof(real), cellsInVolume, snapshot, __FILE__, __LINE__ );
    safe_fread( v->bl.w, sizeof(real), cellsInVolume, snapshot, __FILE__, __LINE__ );

#if defined(LOG_IO_STATS)
    /* stop inner timer */
    double tend_inner = dtime() - tstart_inner;
#endif
    /* close file and stop outer timer */
    safe_fclose(fname, snapshot, __FILE__, __LINE__ );
#if defined(LOG_IO_STATS)
    double tend_outer = dtime() - tstart_outer;

    double iospeed_inner = ((cellsInVolume * sizeof(real) * 12.f) / (1000.f * 1000.f)) / tend_inner;
    double iospeed_outer = ((cellsInVolume * sizeof(real) * 12.f) / (1000.f * 1000.f)) / tend_outer;

    print_stats("Read snapshot (%lf GB)", TOGB(cellsInVolume * sizeof(real) * 12));
    print_stats("\tInner time %lf seconds (%lf MiB/s)", tend_inner, iospeed_inner);
    print_stats("\tOuter time %lf seconds (%lf MiB/s)", tend_outer, iospeed_outer);
    print_stats("\tDifference %lf seconds", tend_outer - tend_inner);
#endif

#if defined(_OPENACC)
    #pragma acc update device(v->tr.u[0:cellsInVolume], v->tr.v[0:cellsInVolume], v->tr.w[0:cellsInVolume]) \
                       device(v->tl.u[0:cellsInVolume], v->tl.v[0:cellsInVolume], v->tl.w[0:cellsInVolume]) \
                       device(v->br.u[0:cellsInVolume], v->br.v[0:cellsInVolume], v->br.w[0:cellsInVolume]) \
                       device(v->bl.u[0:cellsInVolume], v->bl.v[0:cellsInVolume], v->bl.w[0:cellsInVolume]) \
                       async(H2D)
#endif /* end pragma _OPENACC */
#endif /* end pragma DO_NOT_PERFORM_IO */

    POP_RANGE
};

void propagate_shot(time_d        direction,
                    v_t           v,
                    s_t           s,
                    coeff_t       coeffs,
                    real          *rho,
                    int           timesteps,
                    int           ntbwd,
                    real          dt,
                    real          dzi,
                    real          dxi,
                    real          dyi,
                    integer       nz0,
                    integer       nzf,
                    integer       nx0,
                    integer       nxf,
                    integer       ny0,
                    integer       nyf,
                    integer       stacki,
                    char          *folder,
                    real          *UNUSED(dataflush),
                    integer       dimmz,
                    integer       dimmx,
                    integer       dimmy)
{
    PUSH_RANGE

    double tglobal_start, tglobal_total = 0.0;
    double tstress_start, tstress_total = 0.0;
    double tvel_start, tvel_total = 0.0;

    for(int t=0; t < timesteps; t++)
    {
        PUSH_RANGE

        if( t % 10 == 0 ) print_info("Computing %d-th timestep", t);

        /* perform IO */
        if ( t%stacki == 0 && direction == BACKWARD) read_snapshot(folder, ntbwd-t, &v, dimmz, dimmx, dimmy);

        tglobal_start = dtime();

        /* wait read_snapshot H2D copies */
#if defined(_OPENACC)
        #pragma acc wait(H2D) if ( (t%stacki == 0 && direction == BACKWARD) || t==0 )
#endif

        /* ------------------------------------------------------------------------------ */
        /*                      VELOCITY COMPUTATION                                      */
        /* ------------------------------------------------------------------------------ */

        /* Phase 1. Computation of the left-most planes of the domain */
        velocity_propagator(v, s, coeffs, rho, dt, dzi, dxi, dyi,
                            nz0 +   HALO,
                            nzf -   HALO,
                            nx0 +   HALO,
                            nxf -   HALO,
                            ny0 +   HALO,
                            ny0 + 2*HALO,
                            dimmz, dimmx,
                            ONE_L);

        /* Phase 1. Computation of the right-most planes of the domain */
        velocity_propagator(v, s, coeffs, rho, dt, dzi, dxi, dyi,
                            nz0 +   HALO,
                            nzf -   HALO,
                            nx0 +   HALO,
                            nxf -   HALO,
                            nyf - 2*HALO,
                            nyf -   HALO,
                            dimmz, dimmx,
                            ONE_R);

#if defined(USE_MPI)
        /* Boundary exchange for velocity values */
        exchange_velocity_boundaries( v, dimmz * dimmx, nyf, ny0);
#endif

        /* Phase 2. Computation of the central planes. */
        tvel_start = dtime();

        velocity_propagator(v, s, coeffs, rho, dt, dzi, dxi, dyi,
                            nz0 +   HALO,
                            nzf -   HALO,
                            nx0 +   HALO,
                            nxf -   HALO,
                            ny0 + 2*HALO,
                            nyf - 2*HALO,
                            dimmz, dimmx,
                            TWO);

#if defined(_OPENACC)
        #pragma acc wait(ONE_L, ONE_R, TWO)
#endif
        tvel_total += (dtime() - tvel_start);

        /* ------------------------------------------------------------------------------ */
        /*                        STRESS COMPUTATION                                      */
        /* ------------------------------------------------------------------------------ */

        /* Phase 1. Computation of the left-most planes of the domain */
        stress_propagator(s, v, coeffs, rho, dt, dzi, dxi, dyi,
                          nz0 +   HALO,
                          nzf -   HALO,
                          nx0 +   HALO,
                          nxf -   HALO,
                          ny0 +   HALO,
                          ny0 + 2*HALO,
                          dimmz, dimmx,
                          ONE_L);

        /* Phase 1. Computation of the right-most planes of the domain */
        stress_propagator(s, v, coeffs, rho, dt, dzi, dxi, dyi,
                          nz0 +   HALO,
                          nzf -   HALO,
                          nx0 +   HALO,
                          nxf -   HALO,
                          nyf - 2*HALO,
                          nyf -   HALO,
                          dimmz, dimmx,
                          ONE_R);

#if defined(USE_MPI)
        /* Boundary exchange for stress values */
        exchange_stress_boundaries( s, dimmz * dimmx, nyf, ny0);
#endif

        /* Phase 2 computation. Central planes of the domain */
        tstress_start = dtime();

        stress_propagator(s, v, coeffs, rho, dt, dzi, dxi, dyi,
                          nz0 +   HALO,
                          nzf -   HALO,
                          nx0 +   HALO,
                          nxf -   HALO,
                          ny0 + 2*HALO,
                          nyf - 2*HALO,
                          dimmz, dimmx,
                          TWO);

#if defined(_OPENACC)
        #pragma acc wait(ONE_L, ONE_R, TWO, H2D, D2H)
#endif
        tstress_total += (dtime() - tstress_start);

        tglobal_total += (dtime() - tglobal_start);

        /* perform IO */
        if ( t%stacki == 0 && direction == FORWARD) write_snapshot(folder, ntbwd-t, &v, dimmz, dimmx, dimmy);

#if defined(USE_MPI)
        MPI_Barrier( MPI_COMM_WORLD );
#endif
        POP_RANGE
    }

    /* compute some statistics */
    double megacells = ((nzf - nz0) * (nxf - nx0) * (nyf - ny0)) / 1e6;
    tglobal_total /= (double) timesteps;
    tstress_total /= (double) timesteps;
    tvel_total    /= (double) timesteps;

    print_stats("Maingrid GLOBAL   computation took %lf seconds - %lf Mcells/s", tglobal_total, (2*megacells) / tglobal_total);
    print_stats("Maingrid STRESS   computation took %lf seconds - %lf Mcells/s", tstress_total,  megacells / tstress_total);
    print_stats("Maingrid VELOCITY computation took %lf seconds - %lf Mcells/s", tvel_total, megacells / tvel_total);

    POP_RANGE
};

#if defined(USE_MPI)
/*
NAME:exchange_boundaries
PURPOSE: data exchanges between the boundary layers of the analyzed volume

v                   (in) struct containing velocity arrays (4 points / cell x 3 components / point = 12 arrays)
plane_size          (in) Number of elements per plane to exchange
nyf                 (in) final plane to be exchanged
ny0                 (in) intial plane to be exchanged

RETURN none
*/
void exchange_velocity_boundaries ( v_t v,
                                    const integer plane_size,
                                    const integer nyf,
                                    const integer ny0 )
{
    PUSH_RANGE

    int     rank;          // mpi local rank
    int     nranks;        // num mpi ranks

    /* Initialize local variables */
    MPI_Comm_rank ( MPI_COMM_WORLD, &rank );
    MPI_Comm_size ( MPI_COMM_WORLD, &nranks );

    const integer num_planes = HALO;
    const integer nelems     = num_planes * plane_size;

    const integer left_recv  = ny0;
    const integer left_send  = ny0+HALO;

    const integer right_recv = nyf-HALO;
    const integer right_send = nyf-2*HALO;

    if ( rank != 0 )
    {
        // [RANK-1] <---> [RANK] communication
        EXCHANGE( &v.tl.u[left_send], &v.tl.u[left_recv], rank-1, rank, nelems );
        EXCHANGE( &v.tl.v[left_send], &v.tl.v[left_recv], rank-1, rank, nelems );
        EXCHANGE( &v.tl.w[left_send], &v.tl.w[left_recv], rank-1, rank, nelems );

        EXCHANGE( &v.tr.u[left_send], &v.tr.u[left_recv], rank-1, rank, nelems );
        EXCHANGE( &v.tr.v[left_send], &v.tr.v[left_recv], rank-1, rank, nelems );
        EXCHANGE( &v.tr.w[left_send], &v.tr.w[left_recv], rank-1, rank, nelems );

        EXCHANGE( &v.bl.u[left_send], &v.bl.u[left_recv], rank-1, rank, nelems );
        EXCHANGE( &v.bl.v[left_send], &v.bl.v[left_recv], rank-1, rank, nelems );
        EXCHANGE( &v.bl.w[left_send], &v.bl.w[left_recv], rank-1, rank, nelems );

        EXCHANGE( &v.br.u[left_send], &v.br.u[left_recv], rank-1, rank, nelems );
        EXCHANGE( &v.br.v[left_send], &v.br.v[left_recv], rank-1, rank, nelems );
        EXCHANGE( &v.br.w[left_send], &v.br.w[left_recv], rank-1, rank, nelems );
    }

    if ( rank != nranks -1 )  //task to exchange stress boundaries
    {
        //                [RANK] <---> [RANK+1] communication
        EXCHANGE( &v.tl.u[right_send], &v.tl.u[right_recv], rank+1, rank, nelems );
        EXCHANGE( &v.tl.v[right_send], &v.tl.v[right_recv], rank+1, rank, nelems );
        EXCHANGE( &v.tl.w[right_send], &v.tl.w[right_recv], rank+1, rank, nelems );

        EXCHANGE( &v.tr.u[right_send], &v.tr.u[right_recv], rank+1, rank, nelems );
        EXCHANGE( &v.tr.v[right_send], &v.tr.v[right_recv], rank+1, rank, nelems );
        EXCHANGE( &v.tr.w[right_send], &v.tr.w[right_recv], rank+1, rank, nelems );

        EXCHANGE( &v.bl.u[right_send], &v.bl.u[right_recv], rank+1, rank, nelems );
        EXCHANGE( &v.bl.v[right_send], &v.bl.v[right_recv], rank+1, rank, nelems );
        EXCHANGE( &v.bl.w[right_send], &v.bl.w[right_recv], rank+1, rank, nelems );

        EXCHANGE( &v.br.u[right_send], &v.br.u[right_recv], rank+1, rank, nelems );
        EXCHANGE( &v.br.v[right_send], &v.br.v[right_recv], rank+1, rank, nelems );
        EXCHANGE( &v.br.w[right_send], &v.br.w[right_recv], rank+1, rank, nelems );
    }

    POP_RANGE
};

/*
NAME:exchange_stress_boundaries
PURPOSE: data exchanges between the boundary layers of the analyzed volume

s                   (in) struct containing stress arrays (4 points / cell x 6 components / point = 24 arrays)
plane_size          (in) Number of elements per plane to exchange
rank                (in) rank id (CPU id)
nranks              (in) number of CPUs
nyf                 (in) final plane to be exchanged
ny0                 (in) intial plane to be exchanged

RETURN none
*/
void exchange_stress_boundaries ( s_t s,
                                  const integer plane_size,
                                  const integer nyf,
                                  const integer ny0 )
{
    PUSH_RANGE

    int     rank;          // mpi local rank
    int     nranks;        // num mpi ranks

    /* Initialize local variables */
    MPI_Comm_rank ( MPI_COMM_WORLD, &rank );
    MPI_Comm_size ( MPI_COMM_WORLD, &nranks );

    const integer num_planes = HALO;
    const integer nelems     = num_planes * plane_size;

    const integer left_recv  = ny0;
    const integer left_send  = ny0+HALO;

    const integer right_recv = nyf-HALO;
    const integer right_send = nyf-2*HALO;

    if ( rank != 0 )
    {
        // [RANK-1] <---> [RANK] communication
        EXCHANGE( &s.tl.zz[left_send], &s.tl.zz[left_recv], rank-1, rank, nelems );
        EXCHANGE( &s.tl.xz[left_send], &s.tl.xz[left_recv], rank-1, rank, nelems );
        EXCHANGE( &s.tl.yz[left_send], &s.tl.yz[left_recv], rank-1, rank, nelems );
        EXCHANGE( &s.tl.xx[left_send], &s.tl.xx[left_recv], rank-1, rank, nelems );
        EXCHANGE( &s.tl.xy[left_send], &s.tl.xy[left_recv], rank-1, rank, nelems );
        EXCHANGE( &s.tl.yy[left_send], &s.tl.yy[left_recv], rank-1, rank, nelems );

        EXCHANGE( &s.tr.zz[left_send], &s.tr.zz[left_recv], rank-1, rank, nelems );
        EXCHANGE( &s.tr.xz[left_send], &s.tr.xz[left_recv], rank-1, rank, nelems );
        EXCHANGE( &s.tr.yz[left_send], &s.tr.yz[left_recv], rank-1, rank, nelems );
        EXCHANGE( &s.tr.xx[left_send], &s.tr.xx[left_recv], rank-1, rank, nelems );
        EXCHANGE( &s.tr.xy[left_send], &s.tr.xy[left_recv], rank-1, rank, nelems );
        EXCHANGE( &s.tr.yy[left_send], &s.tr.yy[left_recv], rank-1, rank, nelems );

        EXCHANGE( &s.bl.zz[left_send], &s.bl.zz[left_recv], rank-1, rank, nelems );
        EXCHANGE( &s.bl.xz[left_send], &s.bl.xz[left_recv], rank-1, rank, nelems );
        EXCHANGE( &s.bl.yz[left_send], &s.bl.yz[left_recv], rank-1, rank, nelems );
        EXCHANGE( &s.bl.xx[left_send], &s.bl.xx[left_recv], rank-1, rank, nelems );
        EXCHANGE( &s.bl.xy[left_send], &s.bl.xy[left_recv], rank-1, rank, nelems );
        EXCHANGE( &s.bl.yy[left_send], &s.bl.yy[left_recv], rank-1, rank, nelems );

        EXCHANGE( &s.br.zz[left_send], &s.br.zz[left_recv], rank-1, rank, nelems );
        EXCHANGE( &s.br.xz[left_send], &s.br.xz[left_recv], rank-1, rank, nelems );
        EXCHANGE( &s.br.yz[left_send], &s.br.yz[left_recv], rank-1, rank, nelems );
        EXCHANGE( &s.br.xx[left_send], &s.br.xx[left_recv], rank-1, rank, nelems );
        EXCHANGE( &s.br.xy[left_send], &s.br.xy[left_recv], rank-1, rank, nelems );
        EXCHANGE( &s.br.yy[left_send], &s.br.yy[left_recv], rank-1, rank, nelems );
    }

    if ( rank != nranks-1 )
    {
        //                [RANK] <---> [RANK+1] communication
        EXCHANGE( &s.tl.zz[right_send], &s.tl.zz[right_recv], rank+1, rank, nelems );
        EXCHANGE( &s.tl.xz[right_send], &s.tl.xz[right_recv], rank+1, rank, nelems );
        EXCHANGE( &s.tl.yz[right_send], &s.tl.yz[right_recv], rank+1, rank, nelems );
        EXCHANGE( &s.tl.xx[right_send], &s.tl.xx[right_recv], rank+1, rank, nelems );
        EXCHANGE( &s.tl.xy[right_send], &s.tl.xy[right_recv], rank+1, rank, nelems );
        EXCHANGE( &s.tl.yy[right_send], &s.tl.yy[right_recv], rank+1, rank, nelems );

        EXCHANGE( &s.tr.zz[right_send], &s.tr.zz[right_recv], rank+1, rank, nelems );
        EXCHANGE( &s.tr.xz[right_send], &s.tr.xz[right_recv], rank+1, rank, nelems );
        EXCHANGE( &s.tr.yz[right_send], &s.tr.yz[right_recv], rank+1, rank, nelems );
        EXCHANGE( &s.tr.xx[right_send], &s.tr.xx[right_recv], rank+1, rank, nelems );
        EXCHANGE( &s.tr.xy[right_send], &s.tr.xy[right_recv], rank+1, rank, nelems );
        EXCHANGE( &s.tr.yy[right_send], &s.tr.yy[right_recv], rank+1, rank, nelems );

        EXCHANGE( &s.bl.zz[right_send], &s.bl.zz[right_recv], rank+1, rank, nelems );
        EXCHANGE( &s.bl.xz[right_send], &s.bl.xz[right_recv], rank+1, rank, nelems );
        EXCHANGE( &s.bl.yz[right_send], &s.bl.yz[right_recv], rank+1, rank, nelems );
        EXCHANGE( &s.bl.xx[right_send], &s.bl.xx[right_recv], rank+1, rank, nelems );
        EXCHANGE( &s.bl.xy[right_send], &s.bl.xy[right_recv], rank+1, rank, nelems );
        EXCHANGE( &s.bl.yy[right_send], &s.bl.yy[right_recv], rank+1, rank, nelems );

        EXCHANGE( &s.br.zz[right_send], &s.br.zz[right_recv], rank+1, rank, nelems );
        EXCHANGE( &s.br.xz[right_send], &s.br.xz[right_recv], rank+1, rank, nelems );
        EXCHANGE( &s.br.yz[right_send], &s.br.yz[right_recv], rank+1, rank, nelems );
        EXCHANGE( &s.br.xx[right_send], &s.br.xx[right_recv], rank+1, rank, nelems );
        EXCHANGE( &s.br.xy[right_send], &s.br.xy[right_recv], rank+1, rank, nelems );
        EXCHANGE( &s.br.yy[right_send], &s.br.yy[right_recv], rank+1, rank, nelems );
    }

    POP_RANGE
};
#endif /* end of pragma USE_MPI */

