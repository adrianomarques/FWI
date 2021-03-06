#ifndef _FWI_PROPAGATOR_CUDA_H_
#define _FWI_PROPAGATOR_CUDA_H_

#define CUDA_CHECK(call) { gpu_assert((call), __FILE__, __LINE__); }
inline void gpu_assert(cudaError_t code, const char* file, int line)
{
    if (code != cudaSuccess)
    {
        fprintf(stderr, "CUDA ERROR: %s:%d, ", file, line);
        fprintf(stderr, "code: %d, reason: %s\n", code, cudaGetErrorString(code));
        exit(code);
    }
}

extern "C"
void compute_component_vcell_TL_cuda ( float* vptr,
                                 const float* szptr,
                                 const float* sxptr,
                                 const float* syptr,
                                 const float* rho,
                                 const float  dt,
                                 const float  dzi,
                                 const float  dxi,
                                 const float  dyi,
                                 const int    nz0,
                                 const int    nzf,
                                 const int    nx0,
                                 const int    nxf,
                                 const int    ny0,
                                 const int    nyf,
                                 const int    SZ,
                                 const int    SX,
                                 const int    SY,
                                 const int    dimmz,
                                 const int    dimmx,
                                 void*        stream);

extern "C"
void compute_component_vcell_TR_cuda ( float* vptr,
                                 const float* szptr,
                                 const float* sxptr,
                                 const float* syptr,
                                 const float* rho,
                                 const float  dt,
                                 const float  dzi,
                                 const float  dxi,
                                 const float  dyi,
                                 const int    nz0,
                                 const int    nzf,
                                 const int    nx0,
                                 const int    nxf,
                                 const int    ny0,
                                 const int    nyf,
                                 const int    SZ,
                                 const int    SX,
                                 const int    SY,
                                 const int    dimmz,
                                 const int    dimmx,
                                 void*        stream);

extern "C"
void compute_component_vcell_BR_cuda ( float* vptr,
                                 const float* szptr,
                                 const float* sxptr,
                                 const float* syptr,
                                 const float* rho,
                                 const float  dt,
                                 const float  dzi,
                                 const float  dxi,
                                 const float  dyi,
                                 const int    nz0,
                                 const int    nzf,
                                 const int    nx0,
                                 const int    nxf,
                                 const int    ny0,
                                 const int    nyf,
                                 const int    SZ,
                                 const int    SX,
                                 const int    SY,
                                 const int    dimmz,
                                 const int    dimmx,
                                 void*        stream);

extern "C"
void compute_component_vcell_BL_cuda ( float* vptr,
                                 const float* szptr,
                                 const float* sxptr,
                                 const float* syptr,
                                 const float* rho,
                                 const float  dt,
                                 const float  dzi,
                                 const float  dxi,
                                 const float  dyi,
                                 const int    nz0,
                                 const int    nzf,
                                 const int    nx0,
                                 const int    nxf,
                                 const int    ny0,
                                 const int    nyf,
                                 const int    SZ,
                                 const int    SX,
                                 const int    SY,
                                 const int    dimmz,
                                 const int    dimmx,
                                 void*        stream);

extern "C"
void compute_component_scell_TR_cuda ( float* sxxptr,
                                       float* syyptr,
                                       float* szzptr,
                                       float* syzptr,
                                       float* sxzptr,
                                       float* sxyptr,
                                 const float* vxu,
                                 const float* vxv,
                                 const float* vxw,
                                 const float* vyu,
                                 const float* vyv,
                                 const float* vyw,
                                 const float* vzu,
                                 const float* vzv,
                                 const float* vzw,
                                 const float* cc11,
                                 const float* cc12,
                                 const float* cc13,
                                 const float* cc14,
                                 const float* cc15,
                                 const float* cc16,
                                 const float* cc22,
                                 const float* cc23,
                                 const float* cc24,
                                 const float* cc25,
                                 const float* cc26,
                                 const float* cc33,
                                 const float* cc34,
                                 const float* cc35,
                                 const float* cc36,
                                 const float* cc44,
                                 const float* cc45,
                                 const float* cc46,
                                 const float* cc55,
                                 const float* cc56,
                                 const float* cc66,
                                 const float  dt,
                                 const float  dzi,
                                 const float  dxi,
                                 const float  dyi,
                                 const int    nz0,
                                 const int    nzf,
                                 const int    nx0,
                                 const int    nxf,
                                 const int    ny0,
                                 const int    nyf,
                                 const int    SZ,
                                 const int    SX,
                                 const int    SY,
                                 const int    dimmz,
                                 const int    dimmx,
                                 void*        stream);

extern "C"
void compute_component_scell_TL_cuda ( float* sxxptr,
                                       float* syyptr,
                                       float* szzptr,
                                       float* syzptr,
                                       float* sxzptr,
                                       float* sxyptr,
                                 const float* vxu,
                                 const float* vxv,
                                 const float* vxw,
                                 const float* vyu,
                                 const float* vyv,
                                 const float* vyw,
                                 const float* vzu,
                                 const float* vzv,
                                 const float* vzw,
                                 const float* cc11,
                                 const float* cc12,
                                 const float* cc13,
                                 const float* cc14,
                                 const float* cc15,
                                 const float* cc16,
                                 const float* cc22,
                                 const float* cc23,
                                 const float* cc24,
                                 const float* cc25,
                                 const float* cc26,
                                 const float* cc33,
                                 const float* cc34,
                                 const float* cc35,
                                 const float* cc36,
                                 const float* cc44,
                                 const float* cc45,
                                 const float* cc46,
                                 const float* cc55,
                                 const float* cc56,
                                 const float* cc66,
                                 const float  dt,
                                 const float  dzi,
                                 const float  dxi,
                                 const float  dyi,
                                 const int    nz0,
                                 const int    nzf,
                                 const int    nx0,
                                 const int    nxf,
                                 const int    ny0,
                                 const int    nyf,
                                 const int    SZ,
                                 const int    SX,
                                 const int    SY,
                                 const int    dimmz,
                                 const int    dimmx,
                                 void*        stream);

extern "C"
void compute_component_scell_BR_cuda ( float* sxxptr,
                                       float* syyptr,
                                       float* szzptr,
                                       float* syzptr,
                                       float* sxzptr,
                                       float* sxyptr,
                                 const float* vxu,
                                 const float* vxv,
                                 const float* vxw,
                                 const float* vyu,
                                 const float* vyv,
                                 const float* vyw,
                                 const float* vzu,
                                 const float* vzv,
                                 const float* vzw,
                                 const float* cc11,
                                 const float* cc12,
                                 const float* cc13,
                                 const float* cc14,
                                 const float* cc15,
                                 const float* cc16,
                                 const float* cc22,
                                 const float* cc23,
                                 const float* cc24,
                                 const float* cc25,
                                 const float* cc26,
                                 const float* cc33,
                                 const float* cc34,
                                 const float* cc35,
                                 const float* cc36,
                                 const float* cc44,
                                 const float* cc45,
                                 const float* cc46,
                                 const float* cc55,
                                 const float* cc56,
                                 const float* cc66,
                                 const float  dt,
                                 const float  dzi,
                                 const float  dxi,
                                 const float  dyi,
                                 const int    nz0,
                                 const int    nzf,
                                 const int    nx0,
                                 const int    nxf,
                                 const int    ny0,
                                 const int    nyf,
                                 const int    SZ,
                                 const int    SX,
                                 const int    SY,
                                 const int    dimmz,
                                 const int    dimmx,
                                 void*        stream);

extern "C"
void compute_component_scell_BL_cuda ( float* sxxptr,
                                       float* syyptr,
                                       float* szzptr,
                                       float* syzptr,
                                       float* sxzptr,
                                       float* sxyptr,
                                 const float* vxu,
                                 const float* vxv,
                                 const float* vxw,
                                 const float* vyu,
                                 const float* vyv,
                                 const float* vyw,
                                 const float* vzu,
                                 const float* vzv,
                                 const float* vzw,
                                 const float* cc11,
                                 const float* cc12,
                                 const float* cc13,
                                 const float* cc14,
                                 const float* cc15,
                                 const float* cc16,
                                 const float* cc22,
                                 const float* cc23,
                                 const float* cc24,
                                 const float* cc25,
                                 const float* cc26,
                                 const float* cc33,
                                 const float* cc34,
                                 const float* cc35,
                                 const float* cc36,
                                 const float* cc44,
                                 const float* cc45,
                                 const float* cc46,
                                 const float* cc55,
                                 const float* cc56,
                                 const float* cc66,
                                 const float  dt,
                                 const float  dzi,
                                 const float  dxi,
                                 const float  dyi,
                                 const int    nz0,
                                 const int    nzf,
                                 const int    nx0,
                                 const int    nxf,
                                 const int    ny0,
                                 const int    nyf,
                                 const int    SZ,
                                 const int    SX,
                                 const int    SY,
                                 const int    dimmz,
                                 const int    dimmx,
                                 void*        stream);

#endif /* end of _FWI_PROPAGATOR_CUDA_H_ definition */
