#pragma once
#ifndef _INC_WINAPIFAMILY
#define _INC_WINAPIFAMILY
#if defined(_MSC_VER)&&!defined(MOFCOMP_PASS)
#if _MSC_VER >= 1200
#pragma warning(push)
#pragma warning(disable:4001)
#endif
#pragma once
#endif
#ifndef _INC_WINPACKAGEFAMILY
#define _INC_WINPACKAGEFAMILY
#if defined(_MSC_VER)&&!defined(MOFCOMP_PASS)
#if _MSC_VER >= 1200
#pragma warning(push)
#pragma warning(disable:4001)
#endif
#pragma once
#endif
#ifndef WINAPI_PARTITION_SERVER
#define WINAPI_PARTITION_SERVER (WINAPI_FAMILY==WINAPI_FAMILY_SERVER)
#endif
#undef WINAPI_PARTITION_PKG_WINTRUST
#undef WINAPI_PARTITION_PKG_WEBSERVICES
#undef WINAPI_PARTITION_PKG_EVENTLOGSERVICE
#undef WINAPI_PARTITION_PKG_VHD
#undef WINAPI_PARTITION_PKG_PERFCOUNTER
#undef WINAPI_PARTITION_PKG_SECURESTARTUP
#undef WINAPI_PARTITION_PKG_REMOTEFS
#undef WINAPI_PARTITION_PKG_BOOTABLESKU
#undef WINAPI_PARTITION_PKG_CMDTOOLS
#undef WINAPI_PARTITION_PKG_DISM
#undef WINAPI_PARTITION_PKG_CORESETUP
#undef WINAPI_PARTITION_PKG_APPRUNTIME
#undef WINAPI_PARTITION_PKG_ESENT
#undef WINAPI_PARTITION_PKG_WINMGMT
#undef WINAPI_PARTITION_PKG_WNV
#undef WINAPI_PARTITION_PKG_CLUSTER
#undef WINAPI_PARTITION_PKG_VSS
#undef WINAPI_PARTITION_PKG_TRAFFIC
#undef WINAPI_PARTITION_PKG_ISCSI
#undef WINAPI_PARTITION_PKG_STORAGE
#undef WINAPI_PARTITION_PKG_MPSSVC
#undef WINAPI_PARTITION_PKG_APPXDEPLOYMENT
#undef WINAPI_PARTITION_PKG_WER
#define WINAPI_PARTITION_PKG_WINTRUST (WINAPI_PARTITION_SERVER==1)
#define WINAPI_PARTITION_PKG_WEBSERVICES (WINAPI_PARTITION_SERVER==1)
#define WINAPI_PARTITION_PKG_EVENTLOGSERVICE (WINAPI_PARTITION_SERVER==1)
#define WINAPI_PARTITION_PKG_VHD (WINAPI_PARTITION_SERVER==1)
#define WINAPI_PARTITION_PKG_PERFCOUNTER (WINAPI_PARTITION_SERVER==1)
#define WINAPI_PARTITION_PKG_SECURESTARTUP (WINAPI_PARTITION_SERVER==1)
#define WINAPI_PARTITION_PKG_REMOTEFS (WINAPI_PARTITION_SERVER==1)
#define WINAPI_PARTITION_PKG_BOOTABLESKU (WINAPI_PARTITION_SERVER==1)
#define WINAPI_PARTITION_PKG_CMDTOOLS (WINAPI_PARTITION_SERVER==1)
#define WINAPI_PARTITION_PKG_DISM (WINAPI_PARTITION_SERVER==1)
#define WINAPI_PARTITION_PKG_CORESETUP (WINAPI_PARTITION_SERVER==1)
#define WINAPI_PARTITION_PKG_APPRUNTIME (WINAPI_PARTITION_SERVER==1)
#define WINAPI_PARTITION_PKG_ESENT (WINAPI_PARTITION_SERVER==1)
#define WINAPI_PARTITION_PKG_WINMGMT (WINAPI_PARTITION_SERVER==1)
#define WINAPI_PARTITION_PKG_WNV (WINAPI_PARTITION_SERVER==1)
#define WINAPI_PARTITION_PKG_CLUSTER (WINAPI_PARTITION_SERVER==1)
#define WINAPI_PARTITION_PKG_VSS (WINAPI_PARTITION_SERVER==1)
#define WINAPI_PARTITION_PKG_TRAFFIC (WINAPI_PARTITION_SERVER==1)
#define WINAPI_PARTITION_PKG_ISCSI (WINAPI_PARTITION_SERVER==1)
#define WINAPI_PARTITION_PKG_STORAGE (WINAPI_PARTITION_SERVER==1)
#define WINAPI_PARTITION_PKG_MPSSVC (WINAPI_PARTITION_SERVER==1)
#define WINAPI_PARTITION_PKG_APPXDEPLOYMENT (WINAPI_PARTITION_SERVER==1)
#define WINAPI_PARTITION_PKG_WER (WINAPI_PARTITION_SERVER==1)
#if defined(_MSC_VER)&&!defined(MOFCOMP_PASS)
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif
#endif
#endif
#define WINAPI_FAMILY_PC_APP 2
#define WINAPI_FAMILY_PHONE_APP 3
#define WINAPI_FAMILY_SYSTEM 4
#define WINAPI_FAMILY_SERVER 5
#define WINAPI_FAMILY_GAMES 6
#define WINAPI_FAMILY_DESKTOP_APP 100
#define WINAPI_FAMILY_APP WINAPI_FAMILY_PC_APP
#ifndef WINAPI_FAMILY
#define WINAPI_FAMILY WINAPI_FAMILY_DESKTOP_APP
#endif
#if WINAPI_FAMILY != WINAPI_FAMILY_DESKTOP_APP&&\
WINAPI_FAMILY != WINAPI_FAMILY_PC_APP&&\
WINAPI_FAMILY != WINAPI_FAMILY_PHONE_APP&&\
WINAPI_FAMILY != WINAPI_FAMILY_SYSTEM&&\
WINAPI_FAMILY != WINAPI_FAMILY_GAMES&&\
WINAPI_FAMILY != WINAPI_FAMILY_SERVER
#error Unknown WINAPI_FAMILY value. Was it defined in terms of a WINAPI_PARTITION_* value?
#endif
#ifndef WINAPI_PARTITION_DESKTOP
#define WINAPI_PARTITION_DESKTOP (WINAPI_FAMILY==WINAPI_FAMILY_DESKTOP_APP)
#endif
#ifndef WINAPI_PARTITION_APP
#define WINAPI_PARTITION_APP \
(WINAPI_FAMILY==WINAPI_FAMILY_DESKTOP_APP||\
WINAPI_FAMILY==WINAPI_FAMILY_PC_APP||\
WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP)
#endif
#ifndef WINAPI_PARTITION_PC_APP
#define WINAPI_PARTITION_PC_APP \
(WINAPI_FAMILY==WINAPI_FAMILY_DESKTOP_APP||\
WINAPI_FAMILY==WINAPI_FAMILY_PC_APP)
#endif
#ifndef WINAPI_PARTITION_PHONE_APP
#define WINAPI_PARTITION_PHONE_APP (WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP)
#endif
#ifndef WINAPI_PARTITION_GAMES
#define WINAPI_PARTITION_GAMES \
(WINAPI_FAMILY==WINAPI_FAMILY_GAMES||\
WINAPI_FAMILY==WINAPI_FAMILY_DESKTOP_APP)
#endif
#ifndef WINAPI_PARTITION_SYSTEM
#define WINAPI_PARTITION_SYSTEM \
(WINAPI_FAMILY==WINAPI_FAMILY_SYSTEM||\
WINAPI_FAMILY==WINAPI_FAMILY_SERVER)
#endif
#define WINAPI_PARTITION_PHONE WINAPI_PARTITION_PHONE_APP
#define WINAPI_FAMILY_PARTITION(Partitions) (Partitions)
#define _WINAPI_DEPRECATED_DECLARATION __declspec(deprecated("This API cannot be used in the context of the caller's application type."))
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)&&!WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
# define APP_DEPRECATED_HRESULT HRESULT _WINAPI_DEPRECATED_DECLARATION
#endif
#if defined(_MSC_VER)&&!defined(MOFCOMP_PASS)
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif
#endif
#endif
#pragma region Application Family or OneCore Family
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES)
#if !defined(_68K_)&&!defined(_MPPC_)&&!defined(_X86_)&&!defined(_IA64_)&&!defined(_AMD64_)&&!defined(_ARM_)&&!defined(_ARM64_)&&defined(_M_IX86)
#define _X86_
#if !defined(_CHPE_X86_ARM64_)&&defined(_M_HYBRID)
#define _CHPE_X86_ARM64_
#endif
#endif
#if !defined(_68K_)&&!defined(_MPPC_)&&!defined(_X86_)&&!defined(_IA64_)&&!defined(_AMD64_)&&!defined(_ARM_)&&!defined(_ARM64_)&&defined(_M_AMD64)
#define _AMD64_
#endif
#if !defined(_68K_)&&!defined(_MPPC_)&&!defined(_X86_)&&!defined(_IA64_)&&!defined(_AMD64_)&&!defined(_ARM_)&&!defined(_ARM64_)&&defined(_M_ARM)
#define _ARM_
#endif
#if !defined(_68K_)&&!defined(_MPPC_)&&!defined(_X86_)&&!defined(_IA64_)&&!defined(_AMD64_)&&!defined(_ARM_)&&!defined(_ARM64_)&&defined(_M_ARM64)
#define _ARM64_
#endif
#if !defined(_68K_)&&!defined(_MPPC_)&&!defined(_X86_)&&!defined(_IA64_)&&!defined(_AMD64_)&&!defined(_ARM_)&&!defined(_ARM64_)&&defined(_M_M68K)
#define _68K_
#endif
#if !defined(_68K_)&&!defined(_MPPC_)&&!defined(_X86_)&&!defined(_IA64_)&&!defined(_AMD64_)&&!defined(_ARM_)&&!defined(_ARM64_)&&defined(_M_MPPC)
#define _MPPC_
#endif
#if !defined(_68K_)&&!defined(_MPPC_)&&!defined(_X86_)&&!defined(_M_IX86)&&!defined(_AMD64_)&&!defined(_ARM_)&&!defined(_ARM64_)&&defined(_M_IA64)
#if !defined(_IA64_)
#define _IA64_
#endif
#endif
#ifndef _MAC
#if defined(_68K_)||defined(_MPPC_)
#define _MAC
#endif
#endif
#if defined (_MSC_VER)
#if (_MSC_VER >= 800)
#ifndef __cplusplus
#pragma warning(disable:4116)
#endif
#endif
#endif
#ifndef RC_INVOKED
#if (_MSC_VER >= 800)
#pragma warning(disable:4514)
#ifndef __WINDOWS_DONT_DISABLE_PRAGMA_PACK_WARNING__
#pragma warning(disable:4103)
#endif
#if _MSC_VER >= 1200
#pragma warning(push)
#endif
#pragma warning(disable:4001)
#pragma warning(disable:4201)
#pragma warning(disable:4214)
#endif
#endif
#pragma once
#define _WINDEF_
#ifdef __cplusplus
extern "C"{
#endif
#ifdef _M_CEE_PURE
#define WINAPI_INLINE __clrcall
#endif
#define _MINWINDEF_
#pragma region Application Family or OneCore Family or Games Family
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES)
#ifndef NO_STRICT
#ifndef STRICT
#define STRICT 1
#endif
#endif
#ifdef _MAC
#ifndef _WIN32
#define _WIN32
#endif
#endif
#ifndef WIN32
#define WIN32
#endif
#ifdef __cplusplus
extern "C"{
#endif
#ifndef BASETYPES
#define BASETYPES
typedef unsigned long ULONG;
typedef ULONG* PULONG;
typedef unsigned short USHORT;
typedef USHORT* PUSHORT;
typedef unsigned char UCHAR;
typedef UCHAR* PUCHAR;
typedef _Null_terminated_ char* PSZ;
#endif
#define MAX_PATH 260
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif
#ifndef OPTIONAL
#define OPTIONAL
#endif
#undef far
#undef near
#undef pascal
#define far
#define near
#if (!defined(_MAC))&&((_MSC_VER >= 800)||defined(_STDCALL_SUPPORTED))
#define pascal __stdcall
#else
#define pascal
#endif
#if defined(DOSWIN32)||defined(_MAC)
#define cdecl _cdecl
#ifndef CDECL
#define CDECL _cdecl
#endif
#else
#define cdecl
#ifndef CDECL
#define CDECL
#endif
#endif
#ifdef _MAC
#define CALLBACK PASCAL
#define WINAPI CDECL
#define WINAPIV CDECL
#define APIENTRY WINAPI
#define APIPRIVATE CDECL
#ifdef _68K_
#define PASCAL __pascal
#else
#define PASCAL
#endif
#elif (_MSC_VER >= 800)||defined(_STDCALL_SUPPORTED)
#define CALLBACK __stdcall
#define WINAPI __stdcall
#define WINAPIV __cdecl
#define APIENTRY WINAPI
#define APIPRIVATE __stdcall
#define PASCAL __stdcall
#else
#define CALLBACK
#define WINAPI
#define WINAPIV
#define APIENTRY WINAPI
#define APIPRIVATE
#define PASCAL pascal
#endif
#ifndef _M_CEE_PURE
#ifndef WINAPI_INLINE
#define WINAPI_INLINE WINAPI
#endif
#endif
#undef FAR
#undef NEAR
#define FAR far
#define NEAR near
#ifndef CONST
#define CONST const
#endif
typedef unsigned long DWORD;
typedef int BOOL;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef float FLOAT;
typedef FLOAT* PFLOAT;
typedef BOOL near* PBOOL;
typedef BOOL far* LPBOOL;
typedef BYTE near* PBYTE;
typedef BYTE far* LPBYTE;
typedef int near* PINT;
typedef int far* LPINT;
typedef WORD near* PWORD;
typedef WORD far* LPWORD;
typedef long far* LPLONG;
typedef DWORD near* PDWORD;
typedef DWORD far* LPDWORD;
typedef void far* LPVOID;
typedef CONST void far* LPCVOID;
typedef int INT;
typedef unsigned int UINT;
typedef unsigned int* PUINT;
#ifndef NT_INCLUDED
#ifndef _WINNT_
#define _WINNT_
#if _MSC_VER >= 1200
#pragma warning(push)
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#endif
#pragma warning(disable:4200)
#pragma warning(disable:4201)
#pragma warning(disable:4214)
#ifdef __cplusplus
extern "C"{
#endif
#pragma once
#if (_MSC_VER >= 1915)
#pragma warning(disable:4845)
#endif
#ifndef DECLSPEC_NOINITALL
#if (_MSC_VER >= 1915)&&!defined(MIDL_PASS)
#define DECLSPEC_NOINITALL __declspec(no_init_all)
#else
#define DECLSPEC_NOINITALL
#endif
#endif
#define ANYSIZE_ARRAY 1
#ifndef DUMMYUNIONNAME
#if defined(NONAMELESSUNION)||!defined(_MSC_EXTENSIONS)
#define DUMMYUNIONNAME u
#define DUMMYUNIONNAME2 u2
#define DUMMYUNIONNAME3 u3
#define DUMMYUNIONNAME4 u4
#define DUMMYUNIONNAME5 u5
#define DUMMYUNIONNAME6 u6
#define DUMMYUNIONNAME7 u7
#define DUMMYUNIONNAME8 u8
#define DUMMYUNIONNAME9 u9
#else
#define DUMMYUNIONNAME
#define DUMMYUNIONNAME2
#define DUMMYUNIONNAME3
#define DUMMYUNIONNAME4
#define DUMMYUNIONNAME5
#define DUMMYUNIONNAME6
#define DUMMYUNIONNAME7
#define DUMMYUNIONNAME8
#define DUMMYUNIONNAME9
#endif
#endif
#ifndef DUMMYSTRUCTNAME
#if defined(NONAMELESSUNION)||!defined(_MSC_EXTENSIONS)
#define DUMMYSTRUCTNAME s
#define DUMMYSTRUCTNAME2 s2
#define DUMMYSTRUCTNAME3 s3
#define DUMMYSTRUCTNAME4 s4
#define DUMMYSTRUCTNAME5 s5
#define DUMMYSTRUCTNAME6 s6
#else
#define DUMMYSTRUCTNAME
#define DUMMYSTRUCTNAME2
#define DUMMYSTRUCTNAME3
#define DUMMYSTRUCTNAME4
#define DUMMYSTRUCTNAME5
#define DUMMYSTRUCTNAME6
#endif
#endif
#ifndef SPECSTRINGS_H
#define SPECSTRINGS_H
#if _MSC_VER
#pragma once
#endif
#if _MSC_VER >= 1200
#pragma warning(push)
#pragma warning(disable:4668)
#endif
#if !defined(_SAL_VERSION_SAL2)
#if defined(__BUILDMACHINE__)||defined(_USE_SAL2_ONLY)
#define _SAL_VERSION_SAL2(_A) SAL_2_Clean_Violation_using ## _A
#else
#define _SAL_VERSION_SAL2(_A)
#endif
#ifdef _USE_SAL2_ONLY
#define _SAL2_STRICT
#define _SAL_VERSION_CHECK(_A) _SAL_VERSION_SAL2(_A)
#else
#define _SAL_VERSION_CHECK(_A)
#endif
#ifndef SAL_VERSION_CHECK
#define SAL_VERSION_CHECK(_A) _SAL_VERSION_CHECK(_A)
#define SAL_VERSION_SAL2(_A) _SAL_VERSION_SAL2(_A)
#endif
#endif
#ifndef _SAL_L_Source_
#if _USE_ATTRIBUTES_FOR_SAL /*IFSTRIP=IGN*/
#define _SAL_L_Source_(Name,args,annotes) _SA_annotes3(SAL_name,#Name,"","2") _Group_(annotes _SAL_nop_impl_)
#else
#define _SAL_L_Source_(Name,args,annotes) _SA_annotes3(SAL_name,#Name,"","2") _GrouP_(annotes _SAL_nop_impl_)
#endif
#endif
#ifndef __SAL_H_FULL_VER
#define __SAL_H_FULL_VER 140050727
#endif
#ifdef __cplusplus
extern "C"{
#endif
#if __SAL_H_FULL_VER <= 140050727
#if !defined(__midl)&&defined(_PREFAST_)&&_MSC_VER >= 1000
/* Missing from RTM sal.h */
#define __inner_bound _SAL_L_Source_(__inner_bound,(),_SA_annotes0(SAL_bound))
#define __inner_range(lb,ub) _SAL_L_Source_(__inner_range,(lb,ub),_SA_annotes2(SAL_range,lb,ub))
#define __inner_assume_bound_dec __inline __nothrow void __AssumeBoundInt(_Post_ __inner_bound int i) {i;}
#define __inner_assume_bound(i) __AssumeBoundInt(i);
#define __inner_allocator _SAL_L_Source_(__inner_allocater,(),_SA_annotes0(SAL_allocator))
#define __static_context(ctx,annotes) \
_SAL_L_Source_(__static_context,(ctx,annotes),_SA_annotes1(SAL_context,ctx) _Group_(__nop_impl(annotes)))
#define __failure(x) __static_context(SAL_return_convention,\
_SAL_L_Source_(__failure,(x),_SA_annotes1(SAL_failure,x)))
__ANNOTATION(SAL_valueUndefined(void));
#define __valueUndefined _SAL_L_Source_(__valueUndefined,(),_SA_annotes0(SAL_valueUndefined))
enum __SAL_failureKind{__failureUnspecified=0,__failureUndefined=1};
__ANNOTATION(SAL_failureDefault(enum __SAL_failureKind));
#define __failureDefault(kind) _SAL_L_Source_(__failureDefault,(kind),__static_context(SAL_return_convention,\
_SA_annotes1(SAL_failureDefault,kind)))
#else
#define __inner_bound
#define __inner_range(lb,ub)
#define __inner_assume_bound_dec
#define __inner_assume_bound(i)
#define __inner_allocator
#define __static_context(ctx,annotes)
#define __failure(x)
#define __valueUndefined
#define __failureDefault(x)
#endif
#define __xcount(size) _SAL1_Source_(__xcount,(size),__notnull __inexpressible_writableTo(size))
#define __in_xcount(size) _SAL1_Source_(__in_xcount,(size),__in _Pre_ __inexpressible_readableTo(size))
#define __out_xcount(size) _SAL1_Source_(__out_xcount,(size),__xcount(size) _Post_ __valid __refparam)
#define __out_xcount_part(size,length) _SAL1_Source_(__out_xcount_part_,(size,length),__out_xcount(size) _Post_ __inexpressible_readableTo(length))
#define __out_xcount_full(size) _SAL1_Source_(__out_xcount_full,(size),__out_xcount_part(size,size))
#define __inout_xcount(size) _SAL1_Source_(__inout_xcount,(size),__out_xcount(size) _Pre_ __valid)
#define __inout_xcount_part(size,length) _SAL1_Source_(__inout_xcount_part,(size,length),__out_xcount_part(size,length) _Pre_ __valid _Pre_ __inexpressible_readableTo(length))
#define __inout_xcount_full(size) _SAL1_Source_(__inout_xcount_full,(size),__inout_xcount_part(size,size))
#define __xcount_opt(size) _SAL1_Source_(__xcount_opt,(size),__xcount(size) __exceptthat __maybenull)
#define __in_xcount_opt(size) _SAL1_Source_(__in_xcount_opt,(size),__in_xcount(size) __exceptthat __maybenull)
#define __out_xcount_opt(size) _SAL1_Source_(__out_xcount_opt,(size),__out_xcount(size) __exceptthat __maybenull)
#define __out_xcount_part_opt(size,length) _SAL1_Source_(__out_xcount_part_opt,(size,length),__out_xcount_part(size,length) __exceptthat __maybenull)
#define __out_xcount_full_opt(size) _SAL1_Source_(__out_xcount_full_opt,(size),__out_xcount_full(size) __exceptthat __maybenull)
#define __inout_xcount_opt(size) _SAL1_Source_(__inout_xcount_opt,(size),__inout_xcount(size) __exceptthat __maybenull)
#define __inout_xcount_part_opt(size,length) _SAL1_Source_(__inout_xcount_part_opt,(size,length),__inout_xcount_part(size,length) __exceptthat __maybenull)
#define __inout_xcount_full_opt(size) _SAL1_Source_(__inout_xcount_full_opt,(size),__inout_xcount_full(size) __exceptthat __maybenull)
#define __deref_xcount(size) _SAL1_Source_(__deref_xcount,(size),__ecount(1) _Post_ __elem_readableTo(1) _Post_ __deref __notnull _Post_ __deref __inexpressible_writableTo(size))
#define __deref_in _SAL1_Source_(__deref_in,(),__in _Pre_ __deref __deref __readonly)
#define __deref_in_ecount(size) _SAL1_Source_(__deref_in_ecount,(size),__deref_in _Pre_ __deref __elem_readableTo(size))
#define __deref_in_bcount(size) _SAL1_Source_(__deref_in_bcount,(size),__deref_in _Pre_ __deref __byte_readableTo(size))
#define __deref_in_xcount(size) _SAL1_Source_(__deref_in_xcount,(size),__deref_in _Pre_ __deref __inexpressible_readableTo(size))
#define __deref_out_xcount(size) _SAL1_Source_(__deref_out_xcount,(size),__deref_xcount(size) _Post_ __deref __valid __refparam)
#define __deref_out_xcount_part(size,length) _SAL1_Source_(__deref_out_xcount_part,(size,length),__deref_out_xcount(size) _Post_ __deref __inexpressible_readableTo(length))
#define __deref_out_xcount_full(size) _SAL1_Source_(__deref_out_xcount_full,(size),__deref_out_xcount_part(size,size))
#define __deref_out_xcount(size) _SAL1_Source_(__deref_out_xcount,(size),__deref_xcount(size) _Post_ __deref __valid __refparam)
#define __inout_xcount_opt(size) _SAL1_Source_(__inout_xcount_opt,(size),__inout_xcount(size) __exceptthat __maybenull)
#define __deref_xcount(size) _SAL1_Source_(__deref_xcount,(size),__ecount(1) _Post_ __elem_readableTo(1) _Post_ __deref __notnull _Post_ __deref __inexpressible_writableTo(size))
#define __deref_in _SAL1_Source_(__deref_in,(),__in _Pre_ __deref __deref __readonly)
#define __deref_in_ecount(size) _SAL1_Source_(__deref_in_ecount,(size),__deref_in _Pre_ __deref __elem_readableTo(size))
#define __deref_in_bcount(size) _SAL1_Source_(__deref_in_bcount,(size),__deref_in _Pre_ __deref __byte_readableTo(size))
#define __deref_in_xcount(size) _SAL1_Source_(__deref_in_xcount,(size),__deref_in _Pre_ __deref __inexpressible_readableTo(size))
#define __deref_out_xcount(size) _SAL1_Source_(__deref_out_xcount,(size),__deref_xcount(size) _Post_ __deref __valid __refparam)
#define __deref_out_xcount_part(size,length) _SAL1_Source_(__deref_out_xcount_part,(size,length),__deref_out_xcount(size) _Post_ __deref __inexpressible_readableTo(length))
#define __deref_out_xcount_full(size) _SAL1_Source_(__deref_out_xcount_full,(size),__deref_out_xcount_part(size,size))
#define __deref_out_xcount(size) _SAL1_Source_(__deref_out_xcount,(size),__deref_xcount(size) _Post_ __deref __valid __refparam)
#define __deref_inout_xcount(size) _SAL1_Source_(__deref_inout_xcount,(size),__deref_inout _Pre_ __deref __inexpressible_writableTo(size) _Post_ __deref __inexpressible_writableTo(size))
#define __deref_inout_xcount_part(size,length) _SAL1_Source_(__deref_inout_xcount_part,(size,length),__deref_inout_xcount(size) _Pre_ __deref __inexpressible_readableTo(length) _Post_ __deref __inexpressible_readableTo(length))
#define __deref_inout_xcount_full(size) _SAL1_Source_(__deref_inout_xcount_full,(size),__deref_inout_xcount_part(size,size))
#define __deref_xcount_opt(size) _SAL1_Source_(__deref_xcount_opt,(size),__deref_xcount(size) _Post_ __deref __exceptthat __maybenull)
#define __deref_in_opt _SAL1_Source_(__deref_in_opt,(),__deref_in _Pre_ __deref __exceptthat __maybenull)
#define __deref_in_opt_out _SAL1_Source_(__deref_in_opt_out,(),__deref_inout _Pre_ __deref __exceptthat __maybenull _Post_ __deref __notnull)
#define __deref_in_ecount_opt(size) _SAL1_Source_(__deref_in_ecount_opt,(size),__deref_in_ecount(size) _Pre_ __deref __exceptthat __maybenull)
#define __deref_in_bcount_opt(size) _SAL1_Source_(__deref_in_bcount_opt_,(size),__deref_in_bcount(size) _Pre_ __deref __exceptthat __maybenull)
#define __deref_in_xcount_opt(size) _SAL1_Source_(__deref_in_xcount_opt,(size),__deref_in_xcount(size) _Pre_ __deref __exceptthat __maybenull)
#define __deref_out_xcount_opt(size) _SAL1_Source_(__deref_out_xcount_opt,(size),__deref_out_xcount(size) _Post_ __deref __exceptthat __maybenull)
#define __deref_out_xcount_part_opt(size,length) _SAL1_Source_(__deref_out_xcount_part_opt,(size,length),__deref_out_xcount_part(size,length) _Post_ __deref __exceptthat __maybenull)
#define __deref_out_xcount_full_opt(size) _SAL1_Source_(__deref_out_xcount_full_opt,(size),__deref_out_xcount_full(size) _Post_ __deref __exceptthat __maybenull)
#define __deref_inout_xcount_opt(size) _SAL1_Source_(__deref_inout_xcount_opt,(size),__deref_inout_xcount(size) _Pre_ __deref __exceptthat __maybenull _Post_ __deref __exceptthat __maybenull)
#define __deref_inout_xcount_part_opt(size,length) _SAL1_Source_(__deref_inout_xcount_part_opt,(size,length),__deref_inout_xcount_part(size,length) _Pre_ __deref __exceptthat __maybenull _Post_ __deref __exceptthat __maybenull)
#define __deref_inout_xcount_full_opt(size) _SAL1_Source_(__deref_inout_xcount_full_opt,(size),__deref_inout_xcount_full(size) _Pre_ __deref __exceptthat __maybenull _Post_ __deref __exceptthat __maybenull)
#define __deref_opt_xcount(size) _SAL1_Source_(__deref_opt_xcount,(size),__deref_xcount(size) __exceptthat __maybenull)
#define __deref_opt_in _SAL1_Source_(__deref_opt_in,(),__deref_in __exceptthat __maybenull)
#define __deref_opt_in_ecount(size) _SAL1_Source_(__deref_opt_in_ecount,(size),__deref_in_ecount(size) __exceptthat __maybenull)
#define __deref_opt_in_bcount(size) _SAL1_Source_(__deref_opt_in_bcount,(size),__deref_in_bcount(size) __exceptthat __maybenull)
#define __deref_opt_in_xcount(size) _SAL1_Source_(__deref_opt_in_xcount,(size),__deref_in_xcount(size) __exceptthat __maybenull)
#define __deref_opt_out_xcount(size) _SAL1_Source_(__deref_opt_out_xcount,(size),__deref_out_xcount(size) __exceptthat __maybenull)
#define __deref_opt_out_xcount_part(size,length) _SAL1_Source_(__deref_opt_out_xcount_part,(size,length),__deref_out_xcount_part(size,length) __exceptthat __maybenull)
#define __deref_opt_out_xcount_full(size) _SAL1_Source_(__deref_opt_out_xcount_full,(size),__deref_out_xcount_full(size) __exceptthat __maybenull)
#define __deref_opt_inout_xcount(size) _SAL1_Source_(__deref_opt_inout_xcount,(size),__deref_inout_xcount(size) __exceptthat __maybenull)
#define __deref_opt_inout_xcount_part(size,length) _SAL1_Source_(__deref_opt_inout_xcount_part,(size,length),__deref_inout_xcount_part(size,length) __exceptthat __maybenull)
#define __deref_opt_inout_xcount_full(size) _SAL1_Source_(__deref_opt_inout_xcount_full,(size),__deref_inout_xcount_full(size) __exceptthat __maybenull)
#define __deref_opt_xcount_opt(size) _SAL1_Source_(__deref_opt_xcount_opt,(size),__deref_xcount_opt(size) __exceptthat __maybenull)
#define __deref_opt_in_opt _SAL1_Source_(__deref_opt_in_opt,(),__deref_in_opt __exceptthat __maybenull)
#define __deref_opt_in_ecount_opt(size) _SAL1_Source_(__deref_opt_in_ecount_opt,(size),__deref_in_ecount_opt(size) __exceptthat __maybenull)
#define __deref_opt_in_bcount_opt(size) _SAL1_Source_(__deref_opt_in_bcount_opt,(size),__deref_in_bcount_opt(size) __exceptthat __maybenull)
#define __deref_opt_in_xcount_opt(size) _SAL1_Source_(__deref_opt_in_xcount_opt,(size),__deref_in_xcount_opt(size) __exceptthat __maybenull)
#define __deref_opt_out_xcount_opt(size) _SAL1_Source_(__deref_opt_out_xcount_opt,(size),__deref_out_xcount_opt(size) __exceptthat __maybenull)
#define __deref_opt_out_xcount_part_opt(size,length) _SAL1_Source_(__deref_opt_out_xcount_part_opt,(size,length),__deref_out_xcount_part_opt(size,length) __exceptthat __maybenull)
#define __deref_opt_out_xcount_full_opt(size) _SAL1_Source_(__deref_opt_out_scount_full_opt,(size),__deref_out_xcount_full_opt(size) __exceptthat __maybenull)
#define __deref_opt_inout_xcount_opt(size) _SAL1_Source_(__deref_opt_inout_xcount_opt,(size),__deref_inout_xcount_opt(size) __exceptthat __maybenull)
#define __deref_opt_inout_xcount_part_opt(size,length) _SAL1_Source_(__deref_inout_xcount_part_opt(size,(size,length),length) __exceptthat __maybenull)
#define __deref_opt_inout_xcount_full_opt(size) _SAL1_Source_(__deref_opt_inout_scount_full_opt,(size),__deref_inout_xcount_full_opt(size) __exceptthat __maybenull)
#define __deref_in_ecount_iterator(size,incr) _SAL1_Source_(__deref_in_ecount_iterator,(size,incr),__inout _Pre_ __deref __elem_readableTo(size) __deref_out_range(==,_Old_(*_Curr_)+incr))
#define __deref_out_ecount_iterator(size,incr) _SAL1_Source_(__deref_out_ecount_iterator,(size,incr),__inout _Pre_ __deref __elem_writableTo(size) __deref_out_range(==,_Old_(*_Curr_)+incr))
#define __deref_inout_ecount_iterator(size,incr) _SAL1_Source_(__deref_inout_ecount_iterator,(size,incr),__inout _Pre_ __deref __elem_readableTo(size) _Pre_ __deref __elem_writableTo(size) __deref_out_range(==,_Old_(*_Curr_)+incr))
#define __post_bcount(size) _SAL1_Source_(__post_bcount,(size),_Post_ __byte_writableTo(size))
#define __post_ecount(size) _SAL1_Source_(__post_ecount,(size),_Post_ __elem_writableTo(size))
#define __deref_realloc_bcount(insize,outsize) _SAL1_Source_(__deref_realloc_bcount,(insize,outsize),__inout _Pre_ __deref __byte_readableTo(insize) _Post_ __deref __byte_writableTo(outsize))
#define __in_ecount_or_z(c) _SAL1_Source_(__in_ecount_or_z,(c),_When_(_String_length_(_Curr_)<(c),__in_z) \
_When_(_String_length_(_Curr_) >= (c),__in_ecount(c)))
#define __post_nullnullterminated
#ifndef RC_INVOKED
#undef __nullnullterminated
#define __nullnullterminated _SAL1_Source_(__nullnullterminated,(),__inexpressible_readableTo("string terminated by two nulls") __nullterminated)
#undef __post_nullnullterminated
#define __post_nullnullterminated _SAL1_Source_(__post_nullnullterminated,(),_Post_ __inexpressible_readableTo("string terminated by two nulls") _Post_ __nullterminated)
#endif
#endif
#if (_MSC_VER >= 1000)&&!defined(__midl)&&defined(_PREFAST_)
#else
#define __file_parser(typ)
#define __file_parser_class(typ)
#define __file_parser_library(typ)
#define __source_code_content(typ)
#define __class_code_content(typ)
#define __analysis_assert(e)
#define __analysis_hint(hint)
#define __analysis_noreturn
/* Internal defintions */
#define __inner_data_source(src_raw)
#define __inner_this_data_source(src_raw)
#define __inner_out_validated(typ_raw)
#define __inner_this_out_validated(typ_raw)
#define __inner_assume_validated_dec
#define __inner_assume_validated(p)
#define __inner_transfer(formal)
#define __inner_encoded
#define __inner_adt_prop(adt,prop)
#define __inner_adt_add_prop(adt,prop)
#define __inner_adt_remove_prop(adt,prop)
#define __inner_adt_transfer_prop(arg)
#define __inner_adt_type_props(typ)
#define __inner_volatile
#define __inner_nonvolatile
#define __inner_possibly_notnullterminated
#define _Memory_origin_(context)
#define _Memory_origin_when_(previousContext,context)
#define _Accessible_bytes_(context,expr)
#define _Accessible_bytes_when_(previousContext,context,expr)
#define _Pre_accessible_bytes_(context,expr)
#define _Pre_accessible_bytes_when_(context,previousContext,expr)
#define _User_
#define _User_on_(expr)
#define _User_always_
#define _User_always_and_needs_probe_on_(mode)
#define _Kernel_entry_
#define _Kernel_entry_always_
#endif
#define __field_ecount(size) _SAL1_Source_(__field_ecount,(size),__notnull __elem_writableTo(size))
#define __field_bcount(size) _SAL1_Source_(__field_bcount,(size),__notnull __byte_writableTo(size))
#define __field_xcount(size) _SAL1_Source_(__field_xcount,(size),__notnull __inexpressible_writableTo(size))
#define __field_ecount_opt(size) _SAL1_Source_(__field_ecount_opt,(size),__maybenull __elem_writableTo(size))
#define __field_bcount_opt(size) _SAL1_Source_(__field_bcount_opt,(size),__maybenull __byte_writableTo(size))
#define __field_xcount_opt(size) _SAL1_Source_(__field_xcount_opt,(size),__maybenull __inexpressible_writableTo(size))
#define __field_ecount_part(size,init) _SAL1_Source_(__field_ecount_part,(size,init),__notnull __elem_writableTo(size) __elem_readableTo(init))
#define __field_bcount_part(size,init) _SAL1_Source_(__field_bcount_part,(size,init),__notnull __byte_writableTo(size) __byte_readableTo(init))
#define __field_xcount_part(size,init) _SAL1_Source_(__field_xcount_part,(size,init),__notnull __inexpressible_writableTo(size) __inexpressible_readableTo(init))
#define __field_ecount_part_opt(size,init) _SAL1_Source_(__field_ecount_part_opt,(size,init),__maybenull __elem_writableTo(size) __elem_readableTo(init))
#define __field_bcount_part_opt(size,init) _SAL1_Source_(__field_bcount_part_opt,(size,init),__maybenull __byte_writableTo(size) __byte_readableTo(init))
#define __field_xcount_part_opt(size,init) _SAL1_Source_(__field_xcount_part_opt,(size,init),__maybenull __inexpressible_writableTo(size) __inexpressible_readableTo(init))
#define __field_ecount_full(size) _SAL1_Source_(__field_ecount_full,(size),__field_ecount_part(size,size))
#define __field_bcount_full(size) _SAL1_Source_(__field_bcount_full,(size),__field_bcount_part(size,size))
#define __field_xcount_full(size) _SAL1_Source_(__field_xcount_full,(size),__field_xcount_part(size,size))
#define __field_ecount_full_opt(size) _SAL1_Source_(__field_ecount_full_opt,(size),__field_ecount_part_opt(size,size))
#define __field_bcount_full_opt(size) _SAL1_Source_(__field_bcount_full_opt,(size),__field_bcount_part_opt(size,size))
#define __field_xcount_full_opt(size) _SAL1_Source_(__field_xcount_full_opt,(size),__field_xcount_part_opt(size,size))
#define __field_nullterminated _SAL1_Source_(__field_nullterminated,(),__nullterminated)
#define __struct_bcount(size) _SAL1_Source_(__struct_bcount,(size),__byte_writableTo(size))
#define __struct_xcount(size) _SAL1_Source_(__struct_xcount,(size),__inexpressible_writableTo(size))
#define __out_awcount(expr,size) _SAL1_Source_(__out_awcount,(expr,size),_Pre_ __notnull \
__byte_writableTo((expr) ? (size):(size)*2) \
_Post_ __valid __refparam)
#define __in_awcount(expr,size) _SAL1_Source_(__in_awcount,(expr,size),_Pre_ __valid \
_Pre_ _Notref_ __deref __readonly \
__byte_readableTo((expr) ? (size):(size)*2))
#define __post_invalid _SAL1_Source_(__post_invalid,(),_Post_ __notvalid)
/* integer related macros */
#define __allocator _SAL_L_Source_(__allocator,(),__inner_allocator)
#define __deallocate(kind) _SAL_L_Source_(__deallocate,(kind),_Pre_ __notnull __post_invalid)
#define __deallocate_opt(kind) _SAL_L_Source_(__deallocate_opt,(kind),_Pre_ __maybenull __post_invalid)
#define __bound _SAL_L_Source_(__bound,(),__inner_bound)
#define __range(lb,ub) _SAL_L_Source_(__range,(lb,ub),__inner_range(lb,ub))
#define __in_bound _SAL_L_Source_(__in_bound,(),_Pre_ __inner_bound)
#define __out_bound _SAL_L_Source_(__out_bound,(),_Post_ __inner_bound)
#define __deref_out_bound _SAL_L_Source_(__deref_out_bound,(),_Post_ __deref __inner_bound)
#define __in_range(lb,ub) _SAL_L_Source_(__in_range,(lb,ub),_Pre_ __inner_range(lb,ub))
#define __out_range(lb,ub) _SAL_L_Source_(__out_range,(lb,ub),_Post_ __inner_range(lb,ub))
#define __deref_in_range(lb,ub) _SAL_L_Source_(__deref_in_range,(lb,ub),_Pre_ __deref __inner_range(lb,ub))
#define __deref_out_range(lb,ub) _SAL_L_Source_(__deref_out_range,(lb,ub),_Post_ __deref __inner_range(lb,ub))
#define __deref_inout_range(lb,ub) _SAL_L_Source_(__deref_inout_range,(lb,ub),__deref_in_range(lb,ub) __deref_out_range(lb,ub))
#define __field_range(lb,ub) _SAL_L_Source_(__field_range,(lb,ub),__range(lb,ub))
#define __field_data_source(src_sym) _SAL_L_Source_(__field_data_source,(lb,ub),__inner_data_source(#src_sym))
#define __range_max(a,b) _SAL_L_Source_(__range_max,(a,b),__range(==,a>b ? a:b))
#define __range_min(a,b) _SAL_L_Source_(__range_min,(a,b),__range(==,a<b ? a:b))
/* Penetration review macros */
#define __in_data_source(src_sym) _SAL_L_Source_(__in_data_source,(src_sym),_Pre_ __inner_data_source(#src_sym))
#define __out_data_source(src_sym) _SAL_L_Source_(__out_data_source,(src_sym),_Post_ __inner_data_source(#src_sym))
#define __out_validated(typ_sym) _SAL_L_Source_(__out_validated,(src_sym),__inner_out_validated(#typ_sym))
#define __this_out_data_source(src_sym) _SAL_L_Source_(__this_out_data_source,(src_sym),__inner_this_data_source(#src_sym))
#define __this_out_validated(typ_sym) _SAL_L_Source_(__this_out_validated,(src_sym),__inner_this_out_validated(#typ_sym))
#define __transfer(formal) _SAL_L_Source_(__transfer,(src_sym),_Post_ __inner_transfer(formal))
#define __rpc_entry _SAL_L_Source_(__rpc_entry,(formal),__inner_control_entrypoint(RPC))
#define __kernel_entry _SAL_L_Source_(__kernel_entry,(),__inner_control_entrypoint(UserToKernel))
#define __gdi_entry _SAL_L_Source_(__gdi_entry,(),__inner_control_entrypoint(GDI))
#define __encoded_pointer _SAL_L_Source_(__encoded_pointer,(),__inner_encoded)
#define __encoded_array _SAL_L_Source_(__encoded_array,(),__inner_encoded)
#define __field_encoded_pointer _SAL_L_Source_(__field_encoded_pointer,(),__inner_encoded)
#define __field_encoded_array _SAL_L_Source_(__field_encoded_array,(),__inner_encoded)
#if defined(_MSC_EXTENSIONS)||defined(_PREFAST_)||defined(OACR)
#define __type_has_adt_prop(adt,prop) _SAL_L_Source_(__type_has_adt_prop,(adt,prop),__inner_adt_prop(adt,prop))
#define __out_has_adt_prop(adt,prop) _SAL_L_Source_(__out_has_adt_prop,(adt,prop),_Post_ __inner_adt_add_prop(adt,prop))
#define __out_not_has_adt_prop(adt,prop) _SAL_L_Source_(__out_not_has_adt_prop,(adt,prop),_Post_ __inner_adt_remove_prop(adt,prop))
#define __out_transfer_adt_prop(arg) _SAL_L_Source_(__out_transfer_adt_prop,(arg),_Post_ __inner_adt_transfer_prop(arg))
#define __out_has_type_adt_props(typ) _SAL_L_Source_(__out_has_type_adt_props,(typ),_Post_ __inner_adt_type_props(typ))
/* useful PFD related macros */
#define __possibly_notnullterminated _SAL_L_Source_(__possibly_notnullterminated,(),__inner_possibly_notnullterminated)
/* Windows Internal */
#define __volatile _SAL_L_Source_(__volatile,(),__inner_volatile)
#define __nonvolatile _SAL_L_Source_(__nonvolatile,(),__inner_nonvolatile)
#else
#define __out_has_type_adt_props(typ) /* nothing */
#endif
#define __deref_volatile _SAL_L_Source_(__deref_volatile,(),__deref __volatile)
#define __deref_nonvolatile _SAL_L_Source_(__deref_nonvolatile,(),__deref __nonvolatile)
/* declare stub functions for macros */
__inner_assume_validated_dec
__inner_assume_bound_dec
#define __analysis_assume_nullterminated(x) _Analysis_assume_nullterminated_(x)
#define __assume_validated(p) __inner_assume_validated(p)
#define __assume_bound(i) __inner_assume_bound(i)
#ifndef _Unreferenced_parameter_
#define _Unreferenced_parameter_ _SAL2_Source_(_Unreferenced_parameter_,(),_Const_)
#endif
#ifndef _Frees_ptr_
#define _Frees_ptr_ _SAL_L_Source_(_Frees_ptr_,(),_Pre_notnull_ _Post_ptr_invalid_ __drv_freesMem(Mem))
#endif
#ifndef _Frees_ptr_opt_
#define _Frees_ptr_opt_ _SAL_L_Source_(_Frees_ptr_opt_,(),_Pre_maybenull_ _Post_ptr_invalid_ __drv_freesMem(Mem))
#endif
#define _Reallocation_function_(after,before,size) \
_Success_((after) != NULL||(size)==0) \
_At_((after),_Post_maybenull_ _Post_writable_byte_size_(size) \
_When_(((before)==NULL||(size)>0),_Must_inspect_result_)) \
_At_((before),_Post_ptr_invalid_ __drv_freesMem(Mem))
#define _Ret_reallocated_bytes_(before,size) \
_Reallocation_function_(_Curr_,before,size)
#define _In_NLS_string_(size) _SAL_L_Source_(_In_NLS_string_,(size),_When_((size)<0,_In_z_) \
_When_((size) >= 0,_In_reads_(size)))
#define _Flt_CompletionContext_Outptr_ \
_SAL_L_Source_(_Flt_CompletionContext_Outptr_,(),_Outptr_result_maybenull_ _Pre_valid_ \
_At_(*_Curr_,_Pre_null_ \
_When_(return != FLT_PREOP_SUCCESS_WITH_CALLBACK&&return != FLT_PREOP_SYNCHRONIZE,_Post_null_)))
#define _Flt_ConnectionCookie_Outptr_ \
_SAL_L_Source_(_Flt_ConnectionCookie_Outptr_,(),_Outptr_result_maybenull_ _Pre_valid_ \
_At_(*_Curr_,_Pre_null_ _On_failure_(_Post_null_)))
#ifndef _Writes_and_advances_ptr_
#define _Writes_and_advances_ptr_(size) \
_SAL2_Source_(_Writes_and_advances_ptr_,(size),_At_((void*)_Curr_,_Inout_) \
_At_(_Curr_,\
_Pre_writable_size_(size) \
_Post_writable_size_(size) \
_Post_satisfies_(_Curr_-_Old_(_Curr_)==_Old_(size)-size)) \
_At_(_Old_(_Curr_),\
_Post_readable_size_(_Old_(size)-size)))
#endif
#ifndef _Writes_bytes_and_advances_ptr_
#define _Writes_bytes_and_advances_ptr_(size) \
_SAL2_Source_(_Writes_bytes_and_advances_ptr,(size),_At_((void*)_Curr_,_Inout_) \
_At_(_Curr_,\
_Pre_writable_byte_size_(size) \
_Post_writable_byte_size_(size) \
_Post_satisfies_(((char*)_Curr_)-((char*)_Old_(_Curr_))==_Old_(size)-size)) \
_At_(_Old_(_Curr_),\
_Post_readable_byte_size_(_Old_(size)-size)))
#endif
#define _Post_equals_last_error_ _SAL2_Source_(_Post_equals_last_error_,(),_Post_satisfies_(_Curr_ != 0))
#ifndef _Translates_Win32_to_HRESULT_
#define _Translates_Win32_to_HRESULT_(errorCode) \
_SAL2_Source_(_Translates_Win32_to_HRESULT_,(errorCode),\
_Always_(\
_When_((HRESULT)errorCode <= 0,\
_At_(_Curr_,_Post_equal_to_((HRESULT)errorCode))) \
_When_((HRESULT)errorCode>0,\
_At_(_Curr_,_Post_equal_to_((HRESULT)((errorCode & 0x0FFFF) | (FACILITY_WIN32<<16) | 0x80000000))))))
#endif
#ifndef _Translates_NTSTATUS_to_HRESULT_
#define _Translates_NTSTATUS_to_HRESULT_(status) \
_SAL2_Source_(_Translates_NTSTATUS_to_HRESULT_,(status),\
_Always_(\
_Post_equal_to_((HRESULT)(status | FACILITY_NT_BIT))))
#endif
#ifndef _Translates_last_error_to_HRESULT_
#define _Translates_last_error_to_HRESULT_ \
_SAL2_Source_(_Translates_last_error_to_HRESULT_,(),\
_Always_(\
_Post_satisfies_(_Curr_<0)))
#endif
#ifdef __cplusplus
}
#endif
#ifdef _PREFIX_
#if __cplusplus
extern "C" void __pfx_assert(bool,const char*);
extern "C" void __pfx_assume(bool,const char*);
#else
void __pfx_assert(int,const char*);
void __pfx_assume(int,const char*);
#endif
#undef __analysis_assume
#undef __analysis_assert
#define __analysis_assume(e) (__pfx_assume(e,"pfx_assume"),__assume(e));
#define __analysis_assert(e) (__pfx_assert(e,"pfx_assert"),__assume(e));
#endif
#if !(defined(RC_INVOKED)||defined(SORTPP_PASS))
#ifndef __SPECSTRINGS_STRICT_LEVEL
#define __SPECSTRINGS_STRICT_LEVEL 1
#endif
#if (_MSC_VER >= 1400)&&!defined(__midl)&&!defined(_PREFAST_)&&(__SPECSTRINGS_STRICT_LEVEL>0)
#pragma once
#include <specstrings_undef.h>
#define __ecount(size) _SAL_VERSION_CHECK(__ecount)
#define __bcount(size) _SAL_VERSION_CHECK(__bcount)
#define __xcount(size) _SAL_VERSION_CHECK(__xcount)
#define __in _SAL_VERSION_CHECK(__in)
#define __in_ecount(size) _SAL_VERSION_CHECK(__in_ecount)
#define __in_bcount(size) _SAL_VERSION_CHECK(__in_bcount)
#define __in_xcount(size) _SAL_VERSION_CHECK(__in_xcount)
#define __in_z _SAL_VERSION_CHECK(__in_z)
#define __in_ecount_z(size) _SAL_VERSION_CHECK(__in_ecount_z)
#define __in_bcount_z(size) _SAL_VERSION_CHECK(__in_bcount_z)
#define __out _SAL_VERSION_CHECK(__out)
#define __out_ecount(size) _SAL_VERSION_CHECK(__out_ecount)
#define __out_bcount(size) _SAL_VERSION_CHECK(__out_bcount)
#define __out_xcount(size) _SAL_VERSION_CHECK(__out_xcount)
#define __out_ecount_part(size,len) _SAL_VERSION_CHECK(__out_ecount_part)
#define __out_bcount_part(size,len) _SAL_VERSION_CHECK(__out_bcount_part)
#define __out_xcount_part(size,len) _SAL_VERSION_CHECK(__out_xcount_part)
#define __out_ecount_full(size) _SAL_VERSION_CHECK(__out_ecount_full)
#define __out_bcount_full(size) _SAL_VERSION_CHECK(__out_bcount_full)
#define __out_xcount_full(size) _SAL_VERSION_CHECK(__out_xcount_full)
#define __out_z				 _SAL_VERSION_CHECK(__out_z)
#define __out_ecount_z(size) _SAL_VERSION_CHECK(__out_ecount_z)
#define __out_bcount_z(size) _SAL_VERSION_CHECK(__out_bcount_z)
#define __inout _SAL_VERSION_CHECK(__inout)
#define __inout_ecount(size) _SAL_VERSION_CHECK(__inout_ecount)
#define __inout_bcount(size) _SAL_VERSION_CHECK(__inout_bcount)
#define __inout_xcount(size) _SAL_VERSION_CHECK(__inout_xcount)
#define __inout_ecount_part(size,len) _SAL_VERSION_CHECK(__inout_ecount_part)
#define __inout_bcount_part(size,len) _SAL_VERSION_CHECK(__inout_bcount_part)
#define __inout_xcount_part(size,len) _SAL_VERSION_CHECK(__inout_xcount_part)
#define __inout_ecount_full(size) _SAL_VERSION_CHECK(__inout_ecount_full)
#define __inout_bcount_full(size) _SAL_VERSION_CHECK(__inout_bcount_full)
#define __inout_xcount_full(size) _SAL_VERSION_CHECK(__inout_xcount_full)
#define __inout_z __allowed(on_parameter)
#define __inout_ecount_z(size) __allowed(on_parameter)
#define __inout_bcount_z(size) __allowed(on_parameter)
#define __ecount_opt(size) __allowed(on_parameter)
#define __bcount_opt(size) __allowed(on_parameter)
#define __xcount_opt(size) __allowed(on_parameter)
#define __in_opt _SAL_VERSION_CHECK(__in_opt)
#define __in_ecount_opt(size) _SAL_VERSION_CHECK(__in_ecount_opt)
#define __in_bcount_opt(size) _SAL_VERSION_CHECK(__in_bcount_opt)
#define __in_z_opt __allowed(on_parameter)
#define __in_ecount_z_opt(size) __allowed(on_parameter)
#define __in_bcount_z_opt(size) __allowed(on_parameter)
#define __in_xcount_opt(size) __allowed(on_parameter)
#define __out_opt _SAL_VERSION_CHECK(__out_opt)
#define __out_ecount_opt(size) _SAL_VERSION_CHECK(__out_ecount_opt)
#define __out_bcount_opt(size) _SAL_VERSION_CHECK(__out_bcount_opt)
#define __out_xcount_opt(size) __allowed(on_parameter)
#define __out_ecount_part_opt(size,len) __allowed(on_parameter)
#define __out_bcount_part_opt(size,len) __allowed(on_parameter)
#define __out_xcount_part_opt(size,len) __allowed(on_parameter)
#define __out_ecount_full_opt(size) __allowed(on_parameter)
#define __out_bcount_full_opt(size) __allowed(on_parameter)
#define __out_xcount_full_opt(size) __allowed(on_parameter)
#define __out_ecount_z_opt(size) __allowed(on_parameter)
#define __out_bcount_z_opt(size) __allowed(on_parameter)
#define __inout_opt _SAL_VERSION_CHECK(__inout_opt)
#define __inout_ecount_opt(size) _SAL_VERSION_CHECK(__inout_ecount_opt)
#define __inout_bcount_opt(size) _SAL_VERSION_CHECK(__inout_bcount_opt)
#define __inout_xcount_opt(size) _SAL_VERSION_CHECK(__inout_xcount_opt)
#define __inout_ecount_part_opt(size,len) _SAL_VERSION_CHECK(__inout_ecount_part_opt)
#define __inout_bcount_part_opt(size,len) _SAL_VERSION_CHECK(__inout_bcount_part_opt)
#define __inout_xcount_part_opt(size,len) _SAL_VERSION_CHECK(__inout_xcount_part_opt)
#define __inout_ecount_full_opt(size) _SAL_VERSION_CHECK(__inout_ecount_full_opt)
#define __inout_bcount_full_opt(size) _SAL_VERSION_CHECK(__inout_bcount_full_opt)
#define __inout_xcount_full_opt(size) _SAL_VERSION_CHECK(__inout_xcount_full_opt)
#define __inout_z_opt __allowed(on_parameter)
#define __inout_ecount_z_opt(size) __allowed(on_parameter)
#define __inout_ecount_z_opt(size) __allowed(on_parameter)
#define __inout_bcount_z_opt(size) __allowed(on_parameter)
#define __deref_ecount(size) __allowed(on_parameter)
#define __deref_bcount(size) __allowed(on_parameter)
#define __deref_xcount(size) __allowed(on_parameter)
#define __deref_in _SAL_VERSION_CHECK(__deref_in)
#define __deref_in_ecount(size) _SAL_VERSION_CHECK(__deref_in_ecount)
#define __deref_in_bcount(size) _SAL_VERSION_CHECK(__deref_in_bcount)
#define __deref_in_xcount(size) _SAL_VERSION_CHECK(__deref_in_xcount)
#define __deref_out _SAL_VERSION_CHECK(__deref_out)
#define __deref_out_ecount(size) _SAL_VERSION_CHECK(__deref_out_ecount)
#define __deref_out_bcount(size) _SAL_VERSION_CHECK(__deref_out_bcount)
#define __deref_out_xcount(size) _SAL_VERSION_CHECK(__deref_out_xcount)
#define __deref_out_ecount_part(size,len) _SAL_VERSION_CHECK(__deref_out_ecount_part)
#define __deref_out_bcount_part(size,len) _SAL_VERSION_CHECK(__deref_out_bcount_part)
#define __deref_out_xcount_part(size,len) _SAL_VERSION_CHECK(__deref_out_xcount_part)
#define __deref_out_ecount_full(size) _SAL_VERSION_CHECK(__deref_out_ecount_full)
#define __deref_out_bcount_full(size) _SAL_VERSION_CHECK(__deref_out_bcount_full)
#define __deref_out_xcount_full(size) _SAL_VERSION_CHECK(__deref_out_xcount_full)
#define __deref_out_z __allowed(on_parameter)
#define __deref_out_ecount_z(size) __allowed(on_parameter)
#define __deref_out_bcount_z(size) __allowed(on_parameter)
#define __deref_inout _SAL_VERSION_CHECK(__deref_inout)
#define __deref_inout_ecount(size) _SAL_VERSION_CHECK(__deref_inout_ecount)
#define __deref_inout_bcount(size) _SAL_VERSION_CHECK(__deref_inout_bcount)
#define __deref_inout_xcount(size) _SAL_VERSION_CHECK(__deref_inout_xcount)
#define __deref_inout_ecount_part(size,len) __allowed(on_parameter)
#define __deref_inout_bcount_part(size,len) __allowed(on_parameter)
#define __deref_inout_xcount_part(size,len) __allowed(on_parameter)
#define __deref_inout_ecount_full(size) __allowed(on_parameter)
#define __deref_inout_bcount_full(size) __allowed(on_parameter)
#define __deref_inout_xcount_full(size) __allowed(on_parameter)
#define __deref_inout_z __allowed(on_parameter)
#define __deref_inout_ecount_z(size) __allowed(on_parameter)
#define __deref_inout_bcount_z(size) __allowed(on_parameter)
#define __deref_ecount_opt(size) __allowed(on_parameter)
#define __deref_bcount_opt(size) __allowed(on_parameter)
#define __deref_xcount_opt(size) __allowed(on_parameter)
#define __deref_in_opt __allowed(on_parameter)
#define __deref_in_opt_out __allowed(on_parameter)
#define __deref_in_ecount_opt(size) __allowed(on_parameter)
#define __deref_in_bcount_opt(size) __allowed(on_parameter)
#define __deref_in_xcount_opt(size) __allowed(on_parameter)
#define __deref_out_opt _SAL_VERSION_CHECK(__deref_out_opt)
#define __deref_out_ecount_opt(size) _SAL_VERSION_CHECK(__deref_out_ecount_opt)
#define __deref_out_bcount_opt(size) _SAL_VERSION_CHECK(__deref_out_bcount_opt)
#define __deref_out_xcount_opt(size) _SAL_VERSION_CHECK(__deref_out_xcount_opt)
#define __deref_out_ecount_part_opt(size,len) _SAL_VERSION_CHECK(__deref_out_ecount_part_opt)
#define __deref_out_bcount_part_opt(size,len) _SAL_VERSION_CHECK(__deref_out_bcount_part_opt)
#define __deref_out_xcount_part_opt(size,len) _SAL_VERSION_CHECK(__deref_out_xcount_part_opt)
#define __deref_out_ecount_full_opt(size) _SAL_VERSION_CHECK(__deref_out_ecount_full_opt)
#define __deref_out_bcount_full_opt(size) _SAL_VERSION_CHECK(__deref_out_bcount_full_opt)
#define __deref_out_xcount_full_opt(size) _SAL_VERSION_CHECK(__deref_out_xcount_full_opt)
#define __deref_out_z_opt __allowed(on_parameter)
#define __deref_out_ecount_z_opt(size) __allowed(on_parameter)
#define __deref_out_bcount_z_opt(size) __allowed(on_parameter)
#define __deref_inout_opt __allowed(on_parameter)
#define __deref_inout_ecount_opt(size) __allowed(on_parameter)
#define __deref_inout_bcount_opt(size) __allowed(on_parameter)
#define __deref_inout_xcount_opt(size) __allowed(on_parameter)
#define __deref_inout_ecount_part_opt(size,len) __allowed(on_parameter)
#define __deref_inout_bcount_part_opt(size,len) __allowed(on_parameter)
#define __deref_inout_xcount_part_opt(size,len) __allowed(on_parameter)
#define __deref_inout_ecount_full_opt(size) __allowed(on_parameter)
#define __deref_inout_bcount_full_opt(size) __allowed(on_parameter)
#define __deref_inout_xcount_full_opt(size) __allowed(on_parameter)
#define __deref_inout_z_opt __allowed(on_parameter)
#define __deref_inout_ecount_z_opt(size) __allowed(on_parameter)
#define __deref_inout_bcount_z_opt(size) __allowed(on_parameter)
#define __deref_opt_ecount(size) __allowed(on_parameter)
#define __deref_opt_bcount(size) __allowed(on_parameter)
#define __deref_opt_xcount(size) __allowed(on_parameter)
#define __deref_opt_in __allowed(on_parameter)
#define __deref_opt_in_ecount(size) __allowed(on_parameter)
#define __deref_opt_in_bcount(size) __allowed(on_parameter)
#define __deref_opt_in_xcount(size) __allowed(on_parameter)
#define __deref_opt_out _SAL_VERSION_CHECK(__deref_opt_out)
#define __deref_opt_out_ecount(size) _SAL_VERSION_CHECK(__deref_opt_out_ecount)
#define __deref_opt_out_bcount(size) _SAL_VERSION_CHECK(__deref_opt_out_bcount)
#define __deref_opt_out_xcount(size) _SAL_VERSION_CHECK(__deref_opt_out_xcount)
#define __deref_opt_out_ecount_part(size,len) __allowed(on_parameter)
#define __deref_opt_out_bcount_part(size,len) __allowed(on_parameter)
#define __deref_opt_out_xcount_part(size,len) __allowed(on_parameter)
#define __deref_opt_out_ecount_full(size) __allowed(on_parameter)
#define __deref_opt_out_bcount_full(size) __allowed(on_parameter)
#define __deref_opt_out_xcount_full(size) __allowed(on_parameter)
#define __deref_opt_inout __allowed(on_parameter)
#define __deref_opt_inout_ecount(size) __allowed(on_parameter)
#define __deref_opt_inout_bcount(size) __allowed(on_parameter)
#define __deref_opt_inout_xcount(size) __allowed(on_parameter)
#define __deref_opt_inout_ecount_part(size,len) __allowed(on_parameter)
#define __deref_opt_inout_bcount_part(size,len) __allowed(on_parameter)
#define __deref_opt_inout_xcount_part(size,len) __allowed(on_parameter)
#define __deref_opt_inout_ecount_full(size) __allowed(on_parameter)
#define __deref_opt_inout_bcount_full(size) __allowed(on_parameter)
#define __deref_opt_inout_xcount_full(size) __allowed(on_parameter)
#define __deref_opt_inout_z __allowed(on_parameter)
#define __deref_opt_inout_ecount_z(size) __allowed(on_parameter)
#define __deref_opt_inout_bcount_z(size) __allowed(on_parameter)
#define __deref_opt_ecount_opt(size) __allowed(on_parameter)
#define __deref_opt_bcount_opt(size) __allowed(on_parameter)
#define __deref_opt_xcount_opt(size) __allowed(on_parameter)
#define __deref_opt_in_opt __allowed(on_parameter)
#define __deref_opt_in_ecount_opt(size) __allowed(on_parameter)
#define __deref_opt_in_bcount_opt(size) __allowed(on_parameter)
#define __deref_opt_in_xcount_opt(size) __allowed(on_parameter)
#define __deref_opt_out_opt __allowed(on_parameter)
#define __deref_opt_out_ecount_opt(size) __allowed(on_parameter)
#define __deref_opt_out_bcount_opt(size) __allowed(on_parameter)
#define __deref_opt_out_xcount_opt(size) __allowed(on_parameter)
#define __deref_opt_out_ecount_part_opt(size,len) __allowed(on_parameter)
#define __deref_opt_out_bcount_part_opt(size,len) __allowed(on_parameter)
#define __deref_opt_out_xcount_part_opt(size,len) __allowed(on_parameter)
#define __deref_opt_out_ecount_full_opt(size) __allowed(on_parameter)
#define __deref_opt_out_bcount_full_opt(size) __allowed(on_parameter)
#define __deref_opt_out_xcount_full_opt(size) __allowed(on_parameter)
#define __deref_opt_out_z_opt __allowed(on_parameter)
#define __deref_opt_out_ecount_z_opt(size) __allowed(on_parameter)
#define __deref_opt_out_bcount_z_opt(size) __allowed(on_parameter)
#define __deref_opt_inout_opt __allowed(on_parameter)
#define __deref_opt_inout_ecount_opt(size) __allowed(on_parameter)
#define __deref_opt_inout_bcount_opt(size) __allowed(on_parameter)
#define __deref_opt_inout_xcount_opt(size) __allowed(on_parameter)
#define __deref_opt_inout_ecount_part_opt(size,len) __allowed(on_parameter)
#define __deref_opt_inout_bcount_part_opt(size,len) __allowed(on_parameter)
#define __deref_opt_inout_xcount_part_opt(size,len) __allowed(on_parameter)
#define __deref_opt_inout_ecount_full_opt(size) __allowed(on_parameter)
#define __deref_opt_inout_bcount_full_opt(size) __allowed(on_parameter)
#define __deref_opt_inout_xcount_full_opt(size) __allowed(on_parameter)
#define __deref_opt_inout_z_opt __allowed(on_parameter)
#define __deref_opt_inout_ecount_z_opt(size) __allowed(on_parameter)
#define __deref_opt_inout_bcount_z_opt(size) __allowed(on_parameter)
#define __deref_in_ecount_iterator(size,incr) __allowed(on_parameter)
#define __deref_out_ecount_iterator(size,incr) __allowed(on_parameter)
#define __deref_inout_ecount_iterator(size,incr) __allowed(on_parameter)
#define __deref_realloc_bcount(insize,outsize) __allowed(on_parameter)
#define _Outptr_ __allowed(on_parameter)
#define _Outptr_result_maybenull_ __allowed(on_parameter)
#define _Outptr_opt_ __allowed(on_parameter)
#define _Outptr_opt_result_maybenull_ __allowed(on_parameter)
#define _Outptr_result_z_ __allowed(on_parameter)
#define _Outptr_opt_result_z_ __allowed(on_parameter)
#define _Outptr_result_maybenull_z_ __allowed(on_parameter)
#define _Outptr_opt_result_maybenull_z_ __allowed(on_parameter)
#define _Outptr_result_nullonfailure_ __allowed(on_parameter)
#define _Outptr_opt_result_nullonfailure_ __allowed(on_parameter)
#define _COM_Outptr_ __allowed(on_parameter)
#define _COM_Outptr_result_maybenull_ __allowed(on_parameter)
#define _COM_Outptr_opt_ __allowed(on_parameter)
#define _COM_Outptr_opt_result_maybenull_ __allowed(on_parameter)
#define _Outptr_result_buffer_(size) __allowed(on_parameter)
#define _Outptr_opt_result_buffer_(size) __allowed(on_parameter)
#define _Outptr_result_buffer_to_(size,count) __allowed(on_parameter)
#define _Outptr_opt_result_buffer_to_(size,count) __allowed(on_parameter)
#define _Outptr_result_buffer_all_(size) __allowed(on_parameter)
#define _Outptr_opt_result_buffer_all_(size) __allowed(on_parameter)
#define _Outptr_result_buffer_maybenull_(size) __allowed(on_parameter)
#define _Outptr_opt_result_buffer_maybenull_(size) __allowed(on_parameter)
#define _Outptr_result_buffer_to_maybenull_(size,count) __allowed(on_parameter)
#define _Outptr_opt_result_buffer_to_maybenull_(size,count) __allowed(on_parameter)
#define _Outptr_result_buffer_all_maybenull_(size) __allowed(on_parameter)
#define _Outptr_opt_result_buffer_all_maybenull_(size) __allowed(on_parameter)
#define _Outptr_result_bytebuffer_(size) __allowed(on_parameter)
#define _Outptr_opt_result_bytebuffer_(size) __allowed(on_parameter)
#define _Outptr_result_bytebuffer_to_(size,count) __allowed(on_parameter)
#define _Outptr_opt_result_bytebuffer_to_(size,count) __allowed(on_parameter)
#define _Outptr_result_bytebuffer_all_(size) __allowed(on_parameter)
#define _Outptr_opt_result_bytebuffer_all_(size) __allowed(on_parameter)
#define _Outptr_result_bytebuffer_maybenull_(size) __allowed(on_parameter)
#define _Outptr_opt_result_bytebuffer_maybenull_(size) __allowed(on_parameter)
#define _Outptr_result_bytebuffer_to_maybenull_(size,count) __allowed(on_parameter)
#define _Outptr_opt_result_bytebuffer_to_maybenull_(size,count) __allowed(on_parameter)
#define _Outptr_result_bytebuffer_all_maybenull_(size) __allowed(on_parameter)
#define _Outptr_opt_result_bytebuffer_all_maybenull_(size) __allowed(on_parameter)
#define _Deref_out_ _SAL_VERSION_CHECK(_Deref_out_)
#define _Deref_out_opt_ _SAL_VERSION_CHECK(_Deref_out_opt_)
#define _Deref_opt_out_ _SAL_VERSION_CHECK(_Deref_opt_out_)
#define _Deref_opt_out_opt_ _SAL_VERSION_CHECK(_Deref_opt_out_opt_)
#define _In_count_(size) _SAL_VERSION_CHECK(_In_count_)
#define _In_opt_count_(size) _SAL_VERSION_CHECK(_In_opt_count_)
#define _In_bytecount_(size) _SAL_VERSION_CHECK(_In_bytecount_)
#define _In_opt_bytecount_(size) _SAL_VERSION_CHECK(_In_opt_bytecount_)
#define _Out_cap_(size) _SAL_VERSION_CHECK(_Out_cap_)
#define _Out_opt_cap_(size) _SAL_VERSION_CHECK(_Out_opt_cap_)
#define _Out_bytecap_(size) _SAL_VERSION_CHECK(_Out_bytecap_)
#define _Out_opt_bytecap_(size) _SAL_VERSION_CHECK(_Out_opt_bytecap_)
#define _Deref_post_count_(size) _SAL_VERSION_CHECK(_Deref_post_count_)
#define _Deref_post_opt_count_(size) _SAL_VERSION_CHECK(_Deref_post_opt_count_)
#define _Deref_post_bytecount_(size) _SAL_VERSION_CHECK(_Deref_post_bytecount_)
#define _Deref_post_opt_bytecount_(size) _SAL_VERSION_CHECK(_Deref_post_opt_bytecount_)
#define _Deref_post_cap_(size) _SAL_VERSION_CHECK(_Deref_post_cap_)
#define _Deref_post_opt_cap_(size) _SAL_VERSION_CHECK(_Deref_post_opt_cap_)
#define _Deref_post_bytecap_(size) _SAL_VERSION_CHECK(_Deref_post_bytecap_)
#define _Deref_post_opt_bytecap_(size) _SAL_VERSION_CHECK(_Deref_post_opt_bytecap_)
#define _At_(expr,annotes) __allowed(on_parameter_or_return)
#define _When_(expr,annotes) __allowed(on_parameter_or_return)
#define __success(expr) _SAL_VERSION_CHECK(__success)
#define __out_awcount(expr,size) __allowed(on_parameter)
#define __in_awcount(expr,size) __allowed(on_parameter)
#define __nullterminated _SAL_VERSION_CHECK(__nullterminated)
#define __nullnullterminated _SAL_VERSION_CHECK(__nullnullterminated)
#define __reserved _SAL_VERSION_CHECK(__reserved)
#define __checkReturn _SAL_VERSION_CHECK(__checkReturn)
#define __typefix(ctype) __allowed(on_parameter_or_return)
#define __override __allowed(on_function)
#define __callback __allowed(on_function)
#define __format_string __allowed(on_parameter_or_return)
#define __blocksOn(resource) __allowed(on_function)
#define __fallthrough __allowed(as_statement)
#define __range(lb,ub) __allowed(on_return)
#define __in_range(lb,ub) _SAL_VERSION_CHECK(__in_range)
#define __out_range(lb,ub) _SAL_VERSION_CHECK(__out_range)
#define __deref_in_range(lb,ub) __allowed(on_parameter)
#define __deref_out_range(lb,ub) _SAL_VERSION_CHECK(__deref_out_range)
#define __deref_inout_range(lb,ub) __allowed(on_parameter)
#define __field_range(lb,ub) _SAL_VERSION_CHECK(__field_range)
#define __range_max(a,b) __allowed(on_return)
#define __range_min(a,b) __allowed(on_return)
#define __bound __allowed(on_return)
#define __in_bound __allowed(on_parameter)
#define __out_bound __allowed(on_parameter)
#define __deref_out_bound __allowed(on_parameter)
#define __assume_bound(i) __allowed(as_statement_with_arg(i))
#define __analysis_assume_nullterminated(x) \
__allowed(as_statement_with_arg(x))
#define __allocator __allowed(on_function)
#define __deallocate(kind) __allowed(on_parameter)
#define __deallocate_opt(kind) __allowed(on_parameter)
#define __post_invalid __allowed(on_parameter_or_return)
#define __post_nullnullterminated \
__allowed(on_parameter_or_return)
#define __null __allowed(on_typedecl)
#define __notnull __allowed(on_typedecl)
#define __maybenull __allowed(on_typedecl)
#define __exceptthat __allowed(on_typedecl)
#define __field_ecount(size) _SAL_VERSION_CHECK(__field_ecount)
#define __field_bcount(size) _SAL_VERSION_CHECK(__field_bcount)
#define __field_xcount(size) __allowed(on_field)
#define __field_ecount_opt(size) __allowed(on_field)
#define __field_bcount_opt(size) __allowed(on_field)
#define __field_xcount_opt(size) __allowed(on_field)
#define __field_ecount_part(size,init) __allowed(on_field)
#define __field_bcount_part(size,init) __allowed(on_field)
#define __field_xcount_part(size,init) __allowed(on_field)
#define __field_ecount_part_opt(size,init) __allowed(on_field)
#define __field_bcount_part_opt(size,init) __allowed(on_field)
#define __field_xcount_part_opt(size,init) __allowed(on_field)
#define __field_ecount_full(size) __allowed(on_field)
#define __field_bcount_full(size) __allowed(on_field)
#define __field_xcount_full(size) __allowed(on_field)
#define __field_ecount_full_opt(size) __allowed(on_field)
#define __field_bcount_full_opt(size) __allowed(on_field)
#define __field_xcount_full_opt(size) __allowed(on_field)
#define __field_nullterminated __allowed(on_field)
#define __struct_bcount(size) __allowed(on_struct)
#define __struct_xcount(size) __allowed(on_struct)
#define __control_entrypoint(category) __allowed(on_function)
#define __rpc_entry __allowed(on_function)
#define __kernel_entry __allowed(on_function)
#define __gdi_entry __allowed(on_function)
#define __in_data_source(src_sym) __allowed(on_parameter)
#define __out_data_source(src_sym) __allowed(on_parameter)
#define __field_data_source(src_sym) __allowed(on_field)
#define __this_out_data_source(src_syn) __allowed(on_function)
#define __out_validated(filetype_sym) __allowed(on_parameter)
#define __this_out_validated(filetype_sym) __allowed(on_function)
#define __file_parser(filetype_sym) __allowed(on_function)
#define __file_parser_class(filetype_sym) __allowed(on_struct)
#define __file_parser_library(filetype_sym) __allowed(as_global_decl)
#define __source_code_content(codetype_sym) __allowed(as_global_decl)
#define __class_code_content(codetype_sym) __allowed(on_struct)
#define __encoded_pointer
#define __encoded_array
#define __field_encoded_pointer __allowed(on_field)
#define __field_encoded_array __allowed(on_field)
#define __transfer(formal) __allowed(on_parameter_or_return)
#define __assume_validated(exp) __allowed(as_statement_with_arg(exp))
#define __analysis_assume(expr) __allowed(as_statement_with_arg(expr))
#define __analysis_assert(expr) __allowed(as_statement_with_arg(expr))
#define __analysis_hint(hint) __allowed(on_function)
#define __type_has_adt_prop(adt,prop) __allowed(on_typdecl)
#define __out_has_adt_prop(adt,prop) __allowed(on_parameter)
#define __out_not_has_adt_prop(adt,prop) __allowed(on_parameter)
#define __out_transfer_adt_prop(arg) __allowed(on_parameter)
#define __out_has_type_adt_props(typ) __allowed(on_parameter)
#define __possibly_notnullterminated __allowed(on_parameter_or_return)
#define __volatile __allowed(on_global_or_field)
#define __deref_volatile __allowed(on_global_or_field)
#define __nonvolatile __allowed(on_global_or_field)
#define __deref_nonvolatile __allowed(on_global_or_field)
#if (__SPECSTRINGS_STRICT_LEVEL>1)
#endif
#define __in_nz
#define __in_ecount_nz(size)
#define __in_bcount_nz(size)
#define __out_nz
#define __out_nz_opt
#define __out_ecount_nz(size)
#define __out_bcount_nz(size)
#define __inout_nz
#define __inout_ecount_nz(size)
#define __inout_bcount_nz(size)
#define __in_nz_opt
#define __in_ecount_nz_opt(size)
#define __in_bcount_nz_opt(size)
#define __out_ecount_nz_opt(size)
#define __out_bcount_nz_opt(size)
#define __inout_nz_opt
#define __inout_ecount_nz_opt(size)
#define __inout_bcount_nz_opt(size)
#define __deref_out_nz
#define __deref_out_ecount_nz(size)
#define __deref_out_bcount_nz(size)
#define __deref_inout_nz
#define __deref_inout_ecount_nz(size)
#define __deref_inout_bcount_nz(size)
#define __deref_out_nz_opt
#define __deref_out_ecount_nz_opt(size)
#define __deref_out_bcount_nz_opt(size)
#define __deref_inout_nz_opt
#define __deref_inout_ecount_nz_opt(size)
#define __deref_inout_bcount_nz_opt(size)
#define __deref_opt_inout_nz
#define __deref_opt_inout_ecount_nz(size)
#define __deref_opt_inout_bcount_nz(size)
#define __deref_opt_out_nz_opt
#define __deref_opt_out_ecount_nz_opt(size)
#define __deref_opt_out_bcount_nz_opt(size)
#define __deref_opt_inout_nz_opt
#define __deref_opt_inout_ecount_nz_opt(size)
#define __deref_opt_inout_bcount_nz_opt(size)
#define __deref
#define __pre
#define __post
#define __readableTo(count)
#define __writableTo(count)
#define __maybevalid
#define __inexpressible_readableTo(string)
#define __data_entrypoint(category)
#define __readonly
#define __byte_writableTo(count)
#define __byte_readableTo(count)
#define __elem_readableTo(count)
#define __elem_writableTo(count)
#define __valid
#define __notvalid
#define __refparam
#define __precond(condition)
#if !defined(_MSC_EXTENSIONS)&&!defined(_PREFAST_)&&!defined(OACR)
#define __allowed(p) /* nothing */
#else
#define __allowed(p) __$allowed_##p
#define __$allowed_as_global_decl /* empty */
#define __$allowed_as_statement_with_arg(x) \
__pragma(warning(push)) __pragma(warning(disable:4548)) \
do{__noop(x);}while((0,0) __pragma(warning(pop)))
#define __$allowed_as_statement __$allowed_as_statement_with_arg(1)
#define __$allowed_on_function_or_typedecl /* empty */
#if (__SPECSTRINGS_STRICT_LEVEL==1)||(__SPECSTRINGS_STRICT_LEVEL==2)
#define __$allowed_on_typedecl /* empty */
#define __$allowed_on_return /* empty */
#define __$allowed_on_parameter /* empty */
#define __$allowed_on_function /* empty */
#define __$allowed_on_struct /* empty */
#define __$allowed_on_field /* empty */
#define __$allowed_on_parameter_or_return /* empty */
#define __$allowed_on_global_or_field /* empty */
#elif __SPECSTRINGS_STRICT_LEVEL==3
#define __$allowed_on_typedecl /* empty */
/* Define dummy source attributes. Still needs more testing */
#define __$allowed_on_return [returnvalue: OnReturnOnly]
#define __$allowed_on_parameter [OnParameterOnly]
#define __$allowed_on_function [method: OnFunctionOnly]
#define __$allowed_on_struct [OnStructOnly]
#define __$allowed_on_field [OnFieldOnly]
#define __$allowed_on_parameter_or_return [OnParameterOrReturnOnly]
#define __$allowed_on_global_or_field /* empty */
#pragma push_macro("DECL_SA")
#pragma push_macro("SA")
#ifdef __cplusplus
#define SA(x) x
#define DECL_SA(name,loc) \
[repeatable] \
[source_annotation_attribute(loc)] \
struct name##Attribute{name##Attribute(); const char* ignored;};
#else
#define SA(x) SA_##x
#define DECL_SA(name,loc) \
[source_annotation_attribute(loc)] \
struct name{const char* ignored;};\
typedef struct name name;
#endif /* #endif __cplusplus */
DECL_SA(OnParameterOnly,SA(Parameter));
DECL_SA(OnReturnOnly,SA(ReturnValue));
DECL_SA(OnFunctionOnly,SA(Method));
DECL_SA(OnStructOnly,SA(Struct));
DECL_SA(OnFieldOnly,SA(Field));
DECL_SA(OnParameterOrReturnOnly,SA(Parameter) | SA(ReturnValue));
#pragma pop_macro("SA")
#pragma pop_macro("DECL_SA")
#endif
#endif
#endif
#endif
#if (!defined(_Outptr_)||_MSC_FULL_VER <= 160000000)&&!(defined(MIDL_PASS)||defined(__midl)||defined(RC_INVOKED))
#undef __ANNOTATION
#define __ANNOTATION(fun) /* fun */
#undef __PRIMOP
#define __PRIMOP(type,fun)
#endif
#ifndef DRIVERSPECS_H
#define DRIVERSPECS_H
#ifndef SPECSTRINGS_H
#include <specstrings.h>
#endif
// #include "sdv_driverspecs.h"
// #include <concurrencysal.h>
#if _MSC_VER>1000
#pragma once
#endif
#ifdef __cplusplus
extern "C"{
#endif
#if (_MSC_VER >= 1000)&&!defined(__midl)&&defined(_PREFAST_)&&defined(_MSC_EXTENSIONS)
#else
#define __internal_kernel_driver
#define __kernel_code
#define __kernel_driver
#define __user_driver
#define __user_code
#define __drv_Mode_impl(x)
#define __drv_WDM
#define __drv_KMDF
#define __drv_NDIS
#define __prefast_operator_new_throws
#define __prefast_operator_new_null
#define _Analysis_assume_section_locked_(name)
#define _Analysis_assume_section_unlocked_(name)
#endif
#define _IRQL_limited_to_(level) \
ASSERT(KeGetCurrentIrql() <= level); \
_Analysis_assume_(KeGetCurrentIrql() <= level);
#define __drv_deref(annotes) __deref _Group_(annotes _SAL_nop_impl_)
#define __drv_in(annotes) _Pre_ _Group_(annotes _SAL_nop_impl_)
#define __drv_in_deref(annotes) _Pre_ __deref _Group_(annotes _SAL_nop_impl_)
#define __drv_out(annotes) _Post_ _Group_(annotes _SAL_nop_impl_)
#define __drv_out_deref(annotes) _Post_ __deref _Group_(annotes _SAL_nop_impl_)
#define __drv_when(cond,annotes) _When_(cond,annotes _SAL_nop_impl_)
#define __drv_at(expr,annotes) _At_(expr,annotes _SAL_nop_impl_)
#define __drv_fun(annotes) _At_(return,annotes _SAL_nop_impl_)
#define __drv_ret(annotes) _At_(return,annotes _SAL_nop_impl_)
#define __drv_arg(expr,annotes) _At_(expr,annotes)
#define __drv_unit(p) \
typedef int ___drv_unit_##p \
__GENSYM(__prefast_flag_kernel_driver_mode);
#define ___drv_unit_internal_kernel_driver \
_SAL_L_Source_(__drv_unit_internal_kernel_driver,(),_SA_annotes0(SAL_internal_kernel_driver))
#define ___drv_unit_kernel_code \
_SAL_L_Source_(___drv_unit_kernel_code,(),_SA_annotes0(SAL_kernel) _SA_annotes0(SAL_nodriver))
#define ___drv_unit_kernel_driver \
_SAL_L_Source_(___drv_unit_kernel_driver,(),_SA_annotes0(SAL_kernel) _SA_annotes0(SAL_driver))
#define ___drv_unit_user_driver \
_SAL_L_Source_(___drv_unit_user_driver,(),_SA_annotes0(SAL_nokernel) _SA_annotes0(SAL_driver))
#define ___drv_unit_user_code \
_SAL_L_Source_(___drv_unit_user_code,(),_SA_annotes0(SAL_nokernel) _SA_annotes0(SAL_nodriver))
#ifndef __internal_kernel_driver
#define __internal_kernel_driver __drv_unit(internal_kernel_driver)
#define __kernel_code __drv_unit(kernel_code)
#define __kernel_driver __drv_unit(kernel_driver)
#define __user_driver __drv_unit(user_driver)
#define __user_code __drv_unit(user_code)
#endif
#define __drv_defined(x) _Macro_defined_(#x)
#define __drv_functionClass(x) _SAL1_Source_(__drv_functionClass,(x),_Function_class_(x))
__ANNOTATION(SAL_acquire(__In_impl_ char*);)
#define _Kernel_acquires_resource_(kind) \
_SAL2_Source_(_Kernel_acquires_resource_,(#kind),_Post_ _SA_annotes1(SAL_acquire,#kind))
#define __drv_acquiresResource(kind) \
_SAL1_1_Source_(__drv_acquiresResource,(kind),_Acquires_lock_(_Curr_))
__ANNOTATION(SAL_release(__In_impl_ char*);)
#define _Kernel_releases_resource_(kind) \
_SAL2_Source_(_Kernel_releases_resource_,(#kind),_Post_ _SA_annotes1(SAL_release,#kind))
#define __drv_releasesResource(kind) \
_SAL1_1_Source_(__drv_releasesResource,(kind),_Releases_lock_(_Curr_))
__ANNOTATION(SAL_acquireGlobal(__In_impl_ char*,...);)
#define __drv_innerAcquiresGlobal(kind,param) \
_SAL1_1_Source_(_drv_innerAcquiresGlobal,(#kind,param\t),_Post_ _SA_annotes2(SAL_acquireGlobal,#kind,param\t))
#define __drv_acquiresResourceGlobal(kind,param) \
_SAL1_1_Source_(__drv_acquiresResourceGlobal,(kind,param),_Acquires_lock_(param))
__ANNOTATION(SAL_releaseGlobal(__In_impl_ char*,...);)
#define __drv_innerReleasesGlobal(kind,param) \
_SAL1_1_Source_(__drv_InnerReleasesGlobal,(#kind,param\t),_Post_ _SA_annotes2(SAL_releaseGlobal,#kind,param\t))
#define __drv_releasesResourceGlobal(kind,param) \
_SAL1_1_Source_(__drv_releasesResourceGlobal,(kind,param),_Releases_lock_(param))
__ANNOTATION(SAL_mustHold(__In_impl_ char*);)
#define _Kernel_requires_resource_held_(kind) \
_SAL2_Source_(_Kernel_requires_resource_held_,(#kind),_Pre_ _SA_annotes1(SAL_mustHold,#kind))
#define __drv_mustHold(kind) \
_SAL_L_Source_(_drv_mustHold,(kind),\
_When_(!_Arg_comp_(#kind,"Memory"),_Requires_lock_held_(_Curr_)) \
_When_(_Arg_comp_(#kind,"Memory"),_Kernel_requires_resource_held_(kind)))
__ANNOTATION(SAL_mustHoldGlobal(__In_impl_ char*,...);)
#define __drv_innerMustHoldGlobal(kind,param) \
_SAL_L_Source_(__drv_innerMustHoldGlobal,(#kind,param\t),_Pre_ _SA_annotes2(SAL_mustHoldGlobal,#kind,param\t))
#define __drv_mustHoldGlobal(kind,param) \
_SAL1_1_Source_(__drv_mustHoldGlobal,(kind,param),_Requires_lock_held_(param))
__ANNOTATION(SAL_neverHold(__In_impl_ char*);)
#define _Kernel_requires_resource_not_held_(kind) \
_SAL2_Source_(_Kernel_requires_resource_not_held_,(#kind),_Pre_ _SA_annotes1(SAL_neverHold,#kind))
#define __drv_neverHold(kind) \
_SAL1_1_Source_(__drv_neverHold,(kind),_Requires_lock_not_held_(_Curr_))
__ANNOTATION(SAL_neverHoldGlobal(__In_impl_ char*,...);)
#define __drv_innerNeverHoldGlobal(kind,param) \
_SAL_L_Source_(__drv_innterNeverHoldGlobal,(#kind,param\t),_Pre_ _SA_annotes2(SAL_neverHoldGlobal,#kind,param\t))
#define __drv_neverHoldGlobal(kind,param) \
_SAL1_1_Source_(__drv_neverHoldGlobal,(kind,param),_Requires_lock_not_held_(param))
__PRIMOP(int,_Holds_resource_(__In_impl_ __deferTypecheck char*,__In_impl_ char*);)
__PRIMOP(int,_Holds_resource_global_(__In_impl_ char*,...);)
#define __drv_setsIRQL(irql) /* see kernelspecs.h,legacy */
#define _IRQL_raises_(irql) /* see kernelspecs.h */
#define __drv_raisesIRQL _SAL1_1_Source_(__drv_raisesIRQL,(),_IRQL_raises_) /* legacy */
#define _IRQL_requires_(irql) /* see kernelspecs.h */
#define __drv_requiresIRQL(irql) _SAL1_1_Source_(__drv_requiresIRQL,(irql),_IRQL_requires_(irql)) /* legacy */
#define _IRQL_requires_max_(irql) /* see kernelspecs.h */
#define __drv_maxIRQL(irql) _SAL1_1_Source_(__drv_maxIRQL,(irql),_IRQL_requires_max_(irql)) /* legacy */
#define _IRQL_requires_min_(irql) /* see kernelspecs.h */
#define __drv_minIRQL(irql) _SAL1_1_Source_(__drv_minIRQL,(irql),_IRQL_requires_min_(irql)) /* legacy */
#define _IRQL_saves_ /* see kernelspecs.h */
#define __drv_savesIRQL _SAL1_1_Source_(__drv_savesIRQL,(),_IRQL_saves_) /* legacy */
#define _IRQL_saves_global_(kind,param) /* see kernelspecs.h */
#define __drv_savesIRQLGlobal(kind,param) _SAL1_1_Source_(__drv_savesIRQLGlobal,(kind,param),_IRQL_saves_global_(kind,param)) /* legacy */
#define _IRQL_restores_ /* see kernelspecs.h */
#define __drv_restoresIRQL _SAL1_1_Source_(__drv_restoresIRQL,(),_IRQL_restores_) /* legacy */
#define _IRQL_restores_global_(kind,param) /* see kernelspecs.h */
#define __drv_restoresIRQLGlobal(kind,param) _SAL1_1_Source_(__drv_restoresIRQLGlobal,(kind,param),_IRQL_restores_global_(kind,param)) /* legacy */
#define _IRQL_always_function_min_(irql) /* see kernelspecs.h */
#define __drv_minFunctionIRQL(irql) _SAL1_1_Source_(__drv_minFunctionIRQL,(irql),_IRQL_always_function_min_(irql)) /* legacy */
#define _IRQL_always_function_max_(irql) /* see kernelspecs.h */
#define __drv_maxFunctionIRQL(irql) _SAL1_1_Source_(__drv_maxFunctionIRQL,(irql),_IRQL_always_function_max_(irql)) /* legacy */
#define _IRQL_requires_same_ /* see kernelspecs.h */
#define __drv_sameIRQL _SAL1_1_Source_(__drv_sameIRQL,(),_IRQL_requires_same_) /* legacy */
#define _IRQL_uses_cancel_ /* see kernelspecs.h */
#define __drv_useCancelIRQL _SAL1_1_Source_(__drv_usesCancelIRQL,(),_IRQL_uses_cancel_) /* legacy */
#undef _IRQL_inout_
#define _IRQL_inout_ \
_IRQL_saves_ _IRQL_restores_
__ANNOTATION(SAL_clearDoInit(enum __SAL_YesNo);)
#define _Kernel_clear_do_init_(yesNo) \
_SAL2_Source_(_Kernel_clear_do_init_,(yesNo),_Post_ _SA_annotes1(SAL_clearDoInit,yesNo))
#define __drv_clearDoInit(yesNo) _SAL1_1_Source_(__drv_clearDoInit,(yesNo),_Kernel_clear_do_init_(yesNo)) /* legacy */
__ANNOTATION(SAL_IoGetDmaAdapter(void);)
#define _Kernel_IoGetDmaAdapter_ \
_SAL2_Source_(_Kernel_IoGetDmaAdapter_,(),_Post_ _SA_annotes0(SAL_IoGetDmaAdapter))
#define __drv_IoGetDmaAdapter _SAL1_1_Source_(__drv_IoGetDmaAdapter,(),_Kernel_IoGetDmaAdapter_) /* legacy */
__ANNOTATION(SAL_return(__In_impl_ __AuToQuOtE char*);)
#define __drv_valueIs(arglist) \
_SAL1_1_Source_(__drv_valueIs,(arglist),_Post_ _SA_annotes1(SAL_return,arglist))
#define __drv_constant _SAL1_1_Source_(__drv_constant,(),_Literal_)
#define __drv_nonConstant _SAL1_1_Source_(__drv_nonConstant,(),_Notliteral_)
__ANNOTATION(SAL_strictTypeMatch(__int64);)
#define __drv_strictTypeMatch(mode) \
_SAL1_1_Source_(__drv_strictTypeMatch,(mode),_Pre_ _SA_annotes1(SAL_strictTypeMatch,mode))
__ANNOTATION(SAL_strictType(__In_impl_ __AuToQuOtE char*);)
#define __drv_strictType(typename,mode) \
_SAL1_1_Source_(__drv_strictType,(typename,mode),_Pre_ _SA_annotes2(SAL_strictType,typename,mode))
#define __drv_typeConst 0
#define __drv_typeCond 1
#define __drv_typeBitset 2
#define __drv_typeExpr 3
#define __drv_notPointer _SAL1_1_Source_(__drv_notPointer,(),_Pre_ _SA_annotes1(SAL_mayBePointer,__no))
#define __drv_isObjectPointer _SAL1_1_Source_(__drv_isObjectPointer,(),_Points_to_data_)
__ANNOTATION(SAL_IsAliased(void);)
#define __drv_aliasesMem _SAL_L_Source_(__drv_aliasesMem,(),_Post_ _SA_annotes0(SAL_IsAliased))
__ANNOTATION(SAL_NeedsRelease(enum __SAL_YesNo);)
#define __drv_allocatesMem(kind) _SAL_L_Source_(__drv_allocatesMem,(kind),_Post_ _SA_annotes1(SAL_NeedsRelease,__yes))
#define __drv_freesMem(kind) _SAL_L_Source_(__drv_freesMem,(kind),_Post_ _SA_annotes1(SAL_NeedsRelease,__no))
__ANNOTATION(SAL_preferredFunction(__In_impl_ __AuToQuOtE char*,
__In_impl_ __AuToQuOtE char*);)
__ANNOTATION(SAL_preferredFunction3(__In_impl_ __AuToQuOtE char*,
__In_impl_ __AuToQuOtE char*,__In_impl_ __int64);)
#define __drv_preferredFunction(func,why) \
_SAL_L_Source_(__drv_preferredFunction,(func,why),_Pre_ _SA_annotes2(SAL_preferredFunction,func,why))
__ANNOTATION(SAL_error(__In_impl_ __AuToQuOtE char*);)
__ANNOTATION(SAL_error2(__In_impl_ __AuToQuOtE char*,__In_impl_ __int64);)
#define __drv_reportError(why) \
_SAL_L_Source_(__drv_reportError,(why),_Pre_ _SA_annotes1(SAL_error,why))
__ANNOTATION(SAL_floatSaved(void);)
#define _Kernel_float_saved_ _SAL2_Source_(_Kernel_float_saved_,(),_Post_ _SA_annotes0(SAL_floatSaved))
#define __drv_floatSaved _SAL1_1_Source_(__drv_floatSaved,(),_Kernel_float_saved_) /* legacy */
__ANNOTATION(SAL_floatRestored(void);)
#define _Kernel_float_restored_ _SAL2_Source_(_Kernel_float_restored_,(),_Post_ _SA_annotes0(SAL_floatRestored))
#define __drv_floatRestored _SAL1_1_Source_(__drv_floatRestored,(),_Kernel_float_restored_) /* legacy */
__ANNOTATION(SAL_floatUsed(void);)
#define _Kernel_float_used_ _SAL2_Source_(_Kernel_float_used_,(),_Post_ _SA_annotes0(SAL_floatUsed))
#define __drv_floatUsed _SAL1_1_Source_(__drv_floatUsed,(),_Kernel_float_used_) /* legacy */
#define __drv_interlocked _SAL1_1_Source_(__drv_interlocked,(),_Interlocked_operand_)
#define __drv_inTry _SAL_L_Source_(__drv_inTry,(),_Pre_ _SA_annotes1(SAL_inTry,__yes))
#define __drv_notInTry _SAL_L_Source_(__drv_notInTry,(),_Pre_ _SA_annotes1(SAL_inTry,__no))
__ANNOTATION(SAL_IsFormatString(__In_impl_ char*);)
#define __drv_formatString(kind)\
_SAL1_1_Source_(__drv_formatString,(kind),_SA_annotes1(SAL_IsFormatString,#kind))
#define _Dispatch_type_(x) _SAL2_Source_(Dispatch_type_,(x),_SA_annotes1(SAL_functionClassNew,#x))
#define __drv_dispatchType(x) \
_SAL1_1_Source_(__drv_displatchType,(x),_SA_annotes1(SAL_functionClassNew,#x))
#define __drv_completionType(kindlist)\
_SAL1_1_Source_(__drv_completionType,(kindlist),_SA_annotes1(SAL_completionType,#kindlist))
__ANNOTATION(SAL_callbackType(__In_impl_ __AuToQuOtE char*);)
#define __drv_callbackType(kind)\
_SAL1_1_Source_(__drv_callbackType,(kind),_SA_annotes1(SAL_callbackType,#kind))
#ifdef _PREFAST_
#else
#define __drv_acquiresExclusiveResource(kind)
#define __drv_releasesExclusiveResource(kind)
#define __drv_acquiresExclusiveResourceGlobal(kind,param)
#define __drv_releasesExclusiveResourceGlobal(kind,param)
#define __drv_acquiresCancelSpinLock
#define __drv_releasesCancelSpinLock
#define __drv_mustHoldCancelSpinLock
#define __drv_holdsCancelSpinLock()
#define __drv_neverHoldCancelSpinLock
#define __drv_acquiresCriticalRegion
#define __drv_releasesCriticalRegion
#define __drv_mustHoldCriticalRegion
#define __drv_neverHoldCriticalRegion
#define __drv_holdsCriticalRegion()
#define __drv_acquiresPriorityRegion
#define __drv_releasesPriorityRegion
#define __drv_mustHoldPriorityRegion
#define __drv_neverHoldPriorityRegion
#define __drv_holdsPriorityRegion()
#endif
#define _IRQL_is_cancel_ /* see kernelspecs.h */
#define __drv_isCancelIRQL _IRQL_is_cancel_ /* legacy */
__PRIMOP(int,_Is_kernel_(void);)
__PRIMOP(int,_Is_driver_(void);)
#ifdef __cplusplus
}
#endif
#endif
#if (!defined(_Outptr_)||_MSC_FULL_VER <= 160000000)&&!(defined(MIDL_PASS)||defined(__midl)||defined(RC_INVOKED))&&!(defined(_SDV_))
#include <no_sal2.h>
#endif
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif
#endif
#ifndef KERNELSPECS_H
#define KERNELSPECS_H
#if _MSC_VER>1000
#pragma once
#endif
#ifdef __cplusplus
extern "C"{
#endif
#define DISPATCH_LEVEL 2
#define APC_LEVEL 1
#define PASSIVE_LEVEL 0
#if defined(_X86_)
#define HIGH_LEVEL 31
#endif
#if defined(_AMD64_)
#define HIGH_LEVEL 15
#endif
#if defined(_ARM_)
#define HIGH_LEVEL 15
#endif
#if defined(_IA64_)
#define HIGH_LEVEL 15
#endif
#if defined(_ARM64_)
#define HIGH_LEVEL 15
#endif
__ANNOTATION(SAL_IRQL(__int64);)
#undef __drv_setsIRQL
#define __drv_setsIRQL(irql) \
_SAL1_Source_(__drv_setsIRQL,(),_Post_ _SA_annotes1(SAL_IRQL,irql)) /* legacy */
__ANNOTATION(SAL_raiseIRQL(__int64);)
#undef _IRQL_raises_
#define _IRQL_raises_(irql) \
_SAL2_Source_(_IRQL_raises_,(),_Post_ _SA_annotes1(SAL_raiseIRQL,irql))
#undef __drv_raisesIRQL
#define __drv_raisesIRQL(irql) _SAL1_Source_(__drv_raisesIRQL,(),_IRQL_raises_(irql)) /* legacy */
__ANNOTATION(SAL_IRQL(__int64);)
#undef _IRQL_requires_
#define _IRQL_requires_(irql) \
_SAL2_Source_(_IRQL_requires_,(irql),_Pre_ _SA_annotes1(SAL_IRQL,irql))
#undef __drv_requiresIRQL
#define __drv_requiresIRQL(irql) _SAL1_Source_(__drv_requiresIRQL,(),_IRQL_requires_(irql)) /* legacy */
__ANNOTATION(SAL_maxIRQL(__int64);)
#undef _IRQL_requires_max_
#define _IRQL_requires_max_(irql) \
_SAL2_Source_(_IRQL_requires_max_,(irql),_Pre_ _SA_annotes1(SAL_maxIRQL,irql))
#undef __drv_maxIRQL
#define __drv_maxIRQL(irql) _SAL1_Source_(__drv_maxIRQL,(),_IRQL_requires_max_(irql)) /* legacy */
__ANNOTATION(SAL_minIRQL(__int64);)
#undef _IRQL_requires_min_
#define _IRQL_requires_min_(irql) \
_SAL2_Source_(_IRQL_requires_min_,(irql),_Pre_ _SA_annotes1(SAL_minIRQL,irql))
#undef __drv_minIRQL
#define __drv_minIRQL(irql) _SAL1_Source_(__drv_minIRQL,(),_IRQL_requires_min_(irql)) /* legacy */
__ANNOTATION(SAL_saveIRQL(void);)
#undef _IRQL_saves_
#define _IRQL_saves_ \
_SAL2_Source_(_IRQL_saves_,(),_Post_ _SA_annotes0(SAL_saveIRQL))
#undef __drv_savesIRQL
#define __drv_savesIRQL _SAL1_Source_(__drv_savesIRQL,(),_IRQL_saves_) /* legacy */
__ANNOTATION(SAL_saveIRQLGlobal(__In_impl_ char*,...);)
#undef _IRQL_saves_global_
#define _IRQL_saves_global_(kind,param) \
_SAL2_Source_(_IRQL_saves_global_,(kind,param),_Post_ _SA_annotes2(SAL_saveIRQLGlobal,#kind,param\t))
#undef __drv_savesIRQLGlobal
#define __drv_savesIRQLGlobal(kind,param) _SAL1_Source_(__drv_savesIRQLGlobal,(),_IRQL_saves_global_(kind,param)) /* legacy */
__ANNOTATION(SAL_restoreIRQL(void);)
#undef _IRQL_restores_
#define _IRQL_restores_ \
_SAL2_Source_(_IRQL_restores_,(),_Post_ _SA_annotes0(SAL_restoreIRQL))
#undef __drv_restoresIRQL
#define __drv_restoresIRQL _SAL1_Source_(__drv_restoresIRQL,(),_IRQL_restores_) /* legacy */
__ANNOTATION(SAL_restoreIRQLGlobal(__In_impl_ char*,...);)
#undef _IRQL_restores_global_
#define _IRQL_restores_global_(kind,param) \
_SAL2_Source_(_IRQL_restores_global_,(kind,param),_Post_ _SA_annotes2(SAL_restoreIRQLGlobal,#kind,param\t))
#undef __drv_restoresIRQLGlobal
#define __drv_restoresIRQLGlobal(kind,param) _SAL1_Source_(__drv_restoresIRQLGlobal,(),_IRQL_restores_global_(kind,param)) /* legacy */
__ANNOTATION(SAL_minFunctionIrql(__int64);)
#undef _IRQL_always_function_min_
#define _IRQL_always_function_min_(irql) \
_SAL2_Source_(_IRQL_always_function_min_,(irql),_Pre_ _SA_annotes1(SAL_minFunctionIrql,irql))
#undef __drv_minFunctionIRQL
#define __drv_minFunctionIRQL(irql) _SAL1_Source_(__drv_minFunctionIRQL,(),_IRQL_always_function_min_(irql)) /* legacy */
__ANNOTATION(SAL_maxFunctionIrql(__int64);)
#undef _IRQL_always_function_max_
#define _IRQL_always_function_max_(irql) \
_SAL2_Source_(_IRQL_always_function_max_,(irql),_Pre_ _SA_annotes1(SAL_maxFunctionIrql,irql))
#undef __drv_maxFunctionIRQL
#define __drv_maxFunctionIRQL(irql) _SAL1_Source_(__drv_maxFunctionIRQL,(),_IRQL_always_function_max_(irql)) /* legacy */
__ANNOTATION(SAL_sameIRQL(void);)
#undef _IRQL_requires_same_
#define _IRQL_requires_same_ \
_SAL2_Source_(_IRQL_requires_same_,(),_Post_ _SA_annotes0(SAL_sameIRQL))
#undef __drv_sameIRQL
#define __drv_sameIRQL _SAL1_Source_(__drv_sameIRQL,(),_IRQL_requires_same_) /* legacy */
__ANNOTATION(SAL_UseCancelIrql(void);)
#undef _IRQL_uses_cancel_
#define _IRQL_uses_cancel_ \
_SAL2_Source_(_IRQL_uses_cancel_,(),_Post_ _SA_annotes0(SAL_UseCancelIrql))
#undef __drv_useCancelIRQL
#define __drv_useCancelIRQL _SAL1_Source_(__drv_useCancelIRQL,(),_IRQL_uses_cancel_) /* legacy */
#undef _IRQL_inout_
#define _IRQL_inout_ \
_IRQL_saves_ _IRQL_restores_
#ifdef _PREFAST_
#endif
#ifdef __cplusplus
}
#endif
#endif
#if defined(STRICT_GS_ENABLED)
#pragma strict_gs_check(push,on)
#endif
#if defined(_M_MRX000)&&!(defined(MIDL_PASS)||defined(RC_INVOKED))&&defined(ENABLE_RESTRICTED)
#define RESTRICTED_POINTER __restrict
#else
#define RESTRICTED_POINTER
#endif
#if defined(_M_MRX000)||defined(_M_ALPHA)||defined(_M_PPC)||defined(_M_IA64)||defined(_M_AMD64)||defined(_M_ARM)||defined(_M_ARM64)
#define ALIGNMENT_MACHINE
#define UNALIGNED __unaligned
#if defined(_WIN64)
#define UNALIGNED64 __unaligned
#else
#define UNALIGNED64
#endif
#else
#undef ALIGNMENT_MACHINE
#define UNALIGNED
#define UNALIGNED64
#endif
#if defined(_WIN64)||defined(_M_ALPHA)
#define MAX_NATURAL_ALIGNMENT sizeof(ULONGLONG)
#define MEMORY_ALLOCATION_ALIGNMENT 16
#else
#define MAX_NATURAL_ALIGNMENT sizeof(DWORD)
#define MEMORY_ALLOCATION_ALIGNMENT 8
#endif
#ifdef __cplusplus
#if _MSC_VER >= 1300
#define TYPE_ALIGNMENT(t) __alignof(t)
#endif
#else
#define TYPE_ALIGNMENT(t) \
FIELD_OFFSET(struct{char x; t test;},test)
#endif
#if defined(_AMD64_)||defined(_X86_)
#define PROBE_ALIGNMENT(_s) TYPE_ALIGNMENT(DWORD)
#elif defined(_IA64_)||defined(_ARM_)||defined(_ARM64_)
#define PROBE_ALIGNMENT(_s) TYPE_ALIGNMENT(DWORD)
#elif !defined(RC_INVOKED)
#error "No Target Architecture"
#endif
#define PROBE_ALIGNMENT32(_s) TYPE_ALIGNMENT(DWORD)
#ifndef SORTPP_PASS
#define C_ASSERT(e) typedef char __C_ASSERT__[(e)?1:-1]
#else
#define C_ASSERT(e) /* nothing */
#endif
#ifndef _BASETSD_H_
#define _BASETSD_H_
#if _MSC_VER >= 1200
#pragma warning(push)
#pragma warning(disable:4668)
#endif
#if !defined(_MAC)&&(defined(_M_MRX000)||defined(_WIN64))&&(_MSC_VER >= 1100)&&!(defined(MIDL_PASS)||defined(RC_INVOKED))
#define POINTER_64 __ptr64
typedef unsigned __int64 POINTER_64_INT;
#if defined(_WIN64)
#define POINTER_32 __ptr32
#else
#define POINTER_32
#endif
#else
#if defined(_MAC)&&defined(_MAC_INT_64)
#define POINTER_64 __ptr64
typedef unsigned __int64 POINTER_64_INT;
#else
#if (_MSC_VER >= 1300)&&!(defined(MIDL_PASS)||defined(RC_INVOKED))
#define POINTER_64 __ptr64
#else
#define POINTER_64
#endif
typedef unsigned long POINTER_64_INT;
#endif
#define POINTER_32
#endif
#if defined(_WIN64)
#define FIRMWARE_PTR
#else
#define FIRMWARE_PTR POINTER_32
#endif
#if (_MSC_FULL_VER >= 140041204)&&!defined(MIDL_PASS)&&!defined(RC_INVOKED)
#define POINTER_SIGNED __sptr
#define POINTER_UNSIGNED __uptr
#else
#define POINTER_SIGNED
#define POINTER_UNSIGNED
#endif
#define SPOINTER_32 POINTER_SIGNED POINTER_32
#define UPOINTER_32 POINTER_UNSIGNED POINTER_32
#if _MSC_VER>1000
#pragma once
#endif
#ifdef __cplusplus
extern "C"{
#endif
typedef signed char INT8,*PINT8;
typedef signed short INT16,*PINT16;
typedef signed int INT32,*PINT32;
typedef signed __int64 INT64,*PINT64;
typedef unsigned char UINT8,*PUINT8;
typedef unsigned short UINT16,*PUINT16;
typedef unsigned int UINT32,*PUINT32;
typedef unsigned __int64 UINT64,*PUINT64;
typedef signed int LONG32,*PLONG32;
typedef unsigned int ULONG32,*PULONG32;
typedef unsigned int DWORD32,*PDWORD32;
#if !defined(_W64)
#if !defined(__midl)&&(defined(_X86_)||defined(_M_IX86)||defined(_ARM_)||defined(_M_ARM))&&_MSC_VER >= 1300
#define _W64 __w64
#else
#define _W64
#endif
#endif
#if (defined(__midl)&&(501<__midl))
typedef [public] __int3264 INT_PTR,*PINT_PTR;
typedef [public] unsigned __int3264 UINT_PTR,*PUINT_PTR;
typedef [public] __int3264 LONG_PTR,*PLONG_PTR;
typedef [public] unsigned __int3264 ULONG_PTR,*PULONG_PTR;
#else
#if defined(_WIN64)
typedef __int64 INT_PTR,*PINT_PTR;
typedef unsigned __int64 UINT_PTR,*PUINT_PTR;
typedef __int64 LONG_PTR,*PLONG_PTR;
typedef unsigned __int64 ULONG_PTR,*PULONG_PTR;
#define __int3264 __int64
#else
typedef _W64 int INT_PTR,*PINT_PTR;
typedef _W64 unsigned int UINT_PTR,*PUINT_PTR;
typedef _W64 long LONG_PTR,*PLONG_PTR;
typedef _W64 unsigned long ULONG_PTR,*PULONG_PTR;
#define __int3264 __int32
#endif
#endif
#ifdef _WIN64
#define ADDRESS_TAG_BIT 0x40000000000UI64
typedef __int64 SHANDLE_PTR;
typedef unsigned __int64 HANDLE_PTR;
typedef unsigned int UHALF_PTR,*PUHALF_PTR;
typedef int HALF_PTR,*PHALF_PTR;
#if !defined(__midl)
__inline
unsigned long
HandleToULong(
const void* h){
return((unsigned long)(ULONG_PTR)h);}
__inline
long
HandleToLong(
const void* h){
return((long)(LONG_PTR)h);}
__inline
void*
ULongToHandle(
const unsigned long h){
return((void*)(UINT_PTR)h);}
__inline
void*
LongToHandle(
const long h){
return((void*)(INT_PTR)h);}
__inline
unsigned long
PtrToUlong(
const void* p){
return((unsigned long)(ULONG_PTR)p);}
__inline
unsigned int
PtrToUint(
const void* p){
return((unsigned int)(UINT_PTR)p);}
__inline
unsigned short
PtrToUshort(
const void* p){
return((unsigned short)(unsigned long)(ULONG_PTR)p);}
__inline
long
PtrToLong(
const void* p){
return((long)(LONG_PTR)p);}
__inline
int
PtrToInt(
const void* p){
return((int)(INT_PTR)p);}
__inline
short
PtrToShort(
const void* p){
return((short)(long)(LONG_PTR)p);}
__inline
void*
IntToPtr(
const int i){
return((void*)(INT_PTR)i);}
__inline
void*
UIntToPtr(
const unsigned int ui){
return((void*)(UINT_PTR)ui);}
__inline
void*
LongToPtr(
const long l){
return((void*)(LONG_PTR)l);}
__inline
void*
ULongToPtr(
const unsigned long ul){
return((void*)(ULONG_PTR)ul);}
#define PtrToPtr64(p) ((void*POINTER_64) p)
#define Ptr64ToPtr(p) ((void *) p)
#define HandleToHandle64(h) (PtrToPtr64(h))
#define Handle64ToHandle(h) (Ptr64ToPtr(h))
__inline
void*
Ptr32ToPtr(
const void* POINTER_32 p){
return((void*)(ULONG_PTR)(unsigned long)p);}
__inline
void*
Handle32ToHandle(
const void* POINTER_32 h){
return((void*)(LONG_PTR)(long)h);}
__inline
void* POINTER_32
PtrToPtr32(
const void* p){
return((void* POINTER_32) (unsigned long) (ULONG_PTR) p);}
#define HandleToHandle32(h) (PtrToPtr32(h))
#endif
#else
#endif
#define HandleToUlong(h) HandleToULong(h)
#define UlongToHandle(ul) ULongToHandle(ul)
#define UlongToPtr(ul) ULongToPtr(ul)
#define UintToPtr(ui) UIntToPtr(ui)
#define MAXUINT_PTR (~((UINT_PTR)0))
#define MAXINT_PTR ((INT_PTR)(MAXUINT_PTR>>1))
#define MININT_PTR (~MAXINT_PTR)
#define MAXULONG_PTR (~((ULONG_PTR)0))
#define MAXLONG_PTR ((LONG_PTR)(MAXULONG_PTR>>1))
#define MINLONG_PTR (~MAXLONG_PTR)
#define MAXUHALF_PTR ((UHALF_PTR)~0)
#define MAXHALF_PTR ((HALF_PTR)(MAXUHALF_PTR>>1))
#define MINHALF_PTR (~MAXHALF_PTR)
typedef ULONG_PTR SIZE_T,*PSIZE_T;
typedef LONG_PTR SSIZE_T,*PSSIZE_T;
#if _WIN32_WINNT >= 0x0600||(defined(__cplusplus)&&defined(WINDOWS_ENABLE_CPLUSPLUS))
#endif
typedef ULONG_PTR DWORD_PTR,*PDWORD_PTR;
typedef __int64 LONG64,*PLONG64;
typedef unsigned __int64 ULONG64,*PULONG64;
typedef unsigned __int64 DWORD64,*PDWORD64;
typedef ULONG_PTR KAFFINITY;
typedef KAFFINITY* PKAFFINITY;
#ifdef __cplusplus
}
#endif
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif
#endif
#ifndef DECLSPEC_IMPORT
#if (defined(_M_IX86)||defined(_M_IA64)||defined(_M_AMD64)||defined(_M_ARM)||defined(_M_ARM64))&&!defined(MIDL_PASS)
#define DECLSPEC_IMPORT __declspec(dllimport)
#else
#define DECLSPEC_IMPORT
#endif
#endif
#ifndef DECLSPEC_NORETURN
#if (_MSC_VER >= 1200)&&!defined(MIDL_PASS)
#define DECLSPEC_NORETURN __declspec(noreturn)
#else
#define DECLSPEC_NORETURN
#endif
#endif
#ifndef DECLSPEC_NOTHROW
#if (_MSC_VER >= 1200)&&!defined(MIDL_PASS)
#define DECLSPEC_NOTHROW __declspec(nothrow)
#else
#define DECLSPEC_NOTHROW
#endif
#endif
#ifndef DECLSPEC_RESTRICT
#if (_MSC_VER >= 1915)&&!defined(MIDL_PASS)
#define DECLSPEC_RESTRICT __declspec(restrict)
#else
#define DECLSPEC_RESTRICT
#endif
#endif
#ifndef DECLSPEC_ALIGN
#if (_MSC_VER >= 1300)&&!defined(MIDL_PASS)
#define DECLSPEC_ALIGN(x) __declspec(align(x))
#else
#define DECLSPEC_ALIGN(x)
#endif
#endif
#ifndef SYSTEM_CACHE_ALIGNMENT_SIZE
#if defined(_AMD64_)||defined(_X86_)
#define SYSTEM_CACHE_ALIGNMENT_SIZE 64
#else
#define SYSTEM_CACHE_ALIGNMENT_SIZE 128
#endif
#endif
#ifndef DECLSPEC_CACHEALIGN
#define DECLSPEC_CACHEALIGN DECLSPEC_ALIGN(SYSTEM_CACHE_ALIGNMENT_SIZE)
#endif
#ifndef DECLSPEC_UUID
#if (_MSC_VER >= 1100)&&defined (__cplusplus)
#define DECLSPEC_UUID(x) __declspec(uuid(x))
#else
#define DECLSPEC_UUID(x)
#endif
#endif
#ifndef DECLSPEC_NOVTABLE
#if (_MSC_VER >= 1100)&&defined(__cplusplus)
#define DECLSPEC_NOVTABLE __declspec(novtable)
#else
#define DECLSPEC_NOVTABLE
#endif
#endif
#ifndef DECLSPEC_SELECTANY
#if (_MSC_VER >= 1100)
#define DECLSPEC_SELECTANY __declspec(selectany)
#else
#define DECLSPEC_SELECTANY
#endif
#endif
#ifndef NOP_FUNCTION
#if (_MSC_VER >= 1210)
#define NOP_FUNCTION __noop
#else
#define NOP_FUNCTION (void)0
#endif
#endif
#ifndef DECLSPEC_ADDRSAFE
#if (_MSC_VER >= 1200)&&(defined(_M_ALPHA)||defined(_M_AXP64))
#define DECLSPEC_ADDRSAFE __declspec(address_safe)
#else
#define DECLSPEC_ADDRSAFE
#endif
#endif
#ifndef DECLSPEC_SAFEBUFFERS
#if (_MSC_VER >= 1600)
#define DECLSPEC_SAFEBUFFERS __declspec(safebuffers)
#else
#define DECLSPEC_SAFEBUFFERS
#endif
#endif
#ifndef DECLSPEC_NOINLINE
#if (_MSC_VER >= 1300)
#define DECLSPEC_NOINLINE __declspec(noinline)
#else
#define DECLSPEC_NOINLINE
#endif
#endif
#ifndef DECLSPEC_SAFEBUFFERS
#if (_MSC_VER >= 1300)
#define DECLSPEC_SAFEBUFFERS __declspec(safebuffers)
#else
#define DECLSPEC_SAFEBUFFERS
#endif
#endif
#ifndef DECLSPEC_GUARDNOCF
#if (_MSC_FULL_VER >= 170065501)||defined(_D1VERSIONLKG171_)
#define DECLSPEC_GUARDNOCF __declspec(guard(nocf))
#else
#define DECLSPEC_GUARDNOCF
#endif
#endif
#ifndef DECLSPEC_GUARD_SUPPRESS
#if (_MSC_FULL_VER >= 181040116)||defined(_D1VERSIONLKG171_)
#define DECLSPEC_GUARD_SUPPRESS __declspec(guard(suppress))
#else
#define DECLSPEC_GUARD_SUPPRESS
#endif
#endif
#ifndef DECLSPEC_CHPE_GUEST
#if _M_HYBRID
#define DECLSPEC_CHPE_GUEST __declspec(hybrid_guest)
#else
#define DECLSPEC_CHPE_GUEST
#endif
#endif
#ifndef DECLSPEC_CHPE_PATCHABLE
#if _M_HYBRID
#define DECLSPEC_CHPE_PATCHABLE __declspec(hybrid_patchable)
#else
#define DECLSPEC_CHPE_PATCHABLE
#endif
#endif
#ifndef FORCEINLINE
#if (_MSC_VER >= 1200)
#define FORCEINLINE __forceinline
#else
#define FORCEINLINE __inline
#endif
#endif
#define CFORCEINLINE FORCEINLINE
#define STKFORCEINLINE FORCEINLINE
#ifndef PFORCEINLINE
#define PFORCEINLINE FORCEINLINE
#endif
typedef void* PVOID;
typedef void* POINTER_64 PVOID64;
#if (_MSC_VER >= 800)||defined(_STDCALL_SUPPORTED)
#define NTAPI __stdcall
#else
#define _cdecl
#define __cdecl
#define NTAPI
#endif
#if !defined(_M_CEE_PURE)
#define NTAPI_INLINE NTAPI
#else
#define NTAPI_INLINE
#endif
#if !defined(_NTSYSTEM_)&&!defined(_NTHALLIB_)
#define NTSYSAPI DECLSPEC_IMPORT
#define NTSYSCALLAPI DECLSPEC_IMPORT
#else
#define NTSYSAPI
#if defined(_NTDLLBUILD_)
#define NTSYSCALLAPI
#else
#define NTSYSCALLAPI DECLSPEC_ADDRSAFE
#endif
#endif
#ifndef VOID
#define VOID void
typedef char CHAR;
typedef short SHORT;
typedef long LONG;
#if !defined(MIDL_PASS)
typedef int INT;
#endif
#endif
#ifndef _MAC
typedef wchar_t WCHAR;
#else
typedef unsigned short WCHAR;
#endif
typedef WCHAR* PWCHAR,*LPWCH,*PWCH;
typedef CONST WCHAR* LPCWCH,*PCWCH;
typedef _Null_terminated_ WCHAR* NWPSTR,*LPWSTR,*PWSTR;
typedef _Null_terminated_ PWSTR* PZPWSTR;
typedef _Null_terminated_ CONST PWSTR* PCZPWSTR;
typedef _Null_terminated_ WCHAR UNALIGNED* LPUWSTR,*PUWSTR;
typedef _Null_terminated_ CONST WCHAR* LPCWSTR,*PCWSTR;
typedef _Null_terminated_ PCWSTR* PZPCWSTR;
typedef _Null_terminated_ CONST PCWSTR* PCZPCWSTR;
typedef _Null_terminated_ CONST WCHAR UNALIGNED* LPCUWSTR,*PCUWSTR;
typedef _NullNull_terminated_ WCHAR* PZZWSTR;
typedef _NullNull_terminated_ CONST WCHAR* PCZZWSTR;
typedef _NullNull_terminated_ WCHAR UNALIGNED* PUZZWSTR;
typedef _NullNull_terminated_ CONST WCHAR UNALIGNED* PCUZZWSTR;
typedef WCHAR* PNZWCH;
typedef CONST WCHAR* PCNZWCH;
typedef WCHAR UNALIGNED* PUNZWCH;
typedef CONST WCHAR UNALIGNED* PCUNZWCH;
#if _WIN32_WINNT >= 0x0600||(defined(__cplusplus)&&defined(WINDOWS_ENABLE_CPLUSPLUS))
typedef CONST WCHAR* LPCWCHAR,*PCWCHAR;
typedef CONST WCHAR UNALIGNED* LPCUWCHAR,*PCUWCHAR;
typedef unsigned long UCSCHAR;
#define UCSCHAR_INVALID_CHARACTER (0xffffffff)
#define MIN_UCSCHAR (0)
#define MAX_UCSCHAR (0x010FFFF)
typedef UCSCHAR* PUCSCHAR;
typedef const UCSCHAR* PCUCSCHAR;
typedef UCSCHAR* PUCSSTR;
typedef UCSCHAR UNALIGNED* PUUCSSTR;
typedef const UCSCHAR* PCUCSSTR;
typedef const UCSCHAR UNALIGNED* PCUUCSSTR;
typedef UCSCHAR UNALIGNED* PUUCSCHAR;
typedef const UCSCHAR UNALIGNED* PCUUCSCHAR;
#endif
typedef CHAR* PCHAR,*LPCH,*PCH;
typedef CONST CHAR* LPCCH,*PCCH;
typedef _Null_terminated_ CHAR* NPSTR,*LPSTR,*PSTR;
typedef _Null_terminated_ PSTR* PZPSTR;
typedef _Null_terminated_ CONST PSTR* PCZPSTR;
typedef _Null_terminated_ CONST CHAR* LPCSTR,*PCSTR;
typedef _Null_terminated_ PCSTR* PZPCSTR;
typedef _Null_terminated_ CONST PCSTR* PCZPCSTR;
typedef _NullNull_terminated_ CHAR* PZZSTR;
typedef _NullNull_terminated_ CONST CHAR* PCZZSTR;
typedef CHAR* PNZCH;
typedef CONST CHAR* PCNZCH;
#ifdef UNICODE
#ifndef _TCHAR_DEFINED
typedef WCHAR TCHAR,*PTCHAR;
typedef WCHAR TBYTE,*PTBYTE;
#define _TCHAR_DEFINED
#endif /* !_TCHAR_DEFINED */
typedef LPWCH LPTCH,PTCH;
typedef LPCWCH LPCTCH,PCTCH;
typedef LPWSTR PTSTR,LPTSTR;
typedef LPCWSTR PCTSTR,LPCTSTR;
typedef LPUWSTR PUTSTR,LPUTSTR;
typedef LPCUWSTR PCUTSTR,LPCUTSTR;
typedef LPWSTR LP;
typedef PZZWSTR PZZTSTR;
typedef PCZZWSTR PCZZTSTR;
typedef PUZZWSTR PUZZTSTR;
typedef PCUZZWSTR PCUZZTSTR;
typedef PZPWSTR PZPTSTR;
typedef PNZWCH PNZTCH;
typedef PCNZWCH PCNZTCH;
typedef PUNZWCH PUNZTCH;
typedef PCUNZWCH PCUNZTCH;
#define __TEXT(quote) L##quote
#else /* UNICODE */
#ifndef _TCHAR_DEFINED
typedef char TCHAR,*PTCHAR;
typedef unsigned char TBYTE,*PTBYTE;
#define _TCHAR_DEFINED
#endif /* !_TCHAR_DEFINED */
typedef LPCH LPTCH,PTCH;
typedef LPCCH LPCTCH,PCTCH;
typedef LPSTR PTSTR,LPTSTR,PUTSTR,LPUTSTR;
typedef LPCSTR PCTSTR,LPCTSTR,PCUTSTR,LPCUTSTR;
typedef PZZSTR PZZTSTR,PUZZTSTR;
typedef PCZZSTR PCZZTSTR,PCUZZTSTR;
typedef PZPSTR PZPTSTR;
typedef PNZCH PNZTCH,PUNZTCH;
typedef PCNZCH PCNZTCH,PCUNZTCH;
#define __TEXT(quote) quote
#endif /* UNICODE */
#define TEXT(quote) __TEXT(quote)
typedef SHORT* PSHORT;
typedef LONG* PLONG;
#define ALL_PROCESSOR_GROUPS 0xffff
typedef struct _PROCESSOR_NUMBER{
WORD Group;
BYTE Number;
BYTE Reserved;
}PROCESSOR_NUMBER,*PPROCESSOR_NUMBER;
typedef struct _GROUP_AFFINITY{
KAFFINITY Mask;
WORD Group;
WORD Reserved[3];
}GROUP_AFFINITY,*PGROUP_AFFINITY;
#if defined(_WIN64)
#define MAXIMUM_PROC_PER_GROUP 64
#else
#define MAXIMUM_PROC_PER_GROUP 32
#endif
#define MAXIMUM_PROCESSORS MAXIMUM_PROC_PER_GROUP
#ifdef STRICT
typedef void* HANDLE;
#if 0&&(_MSC_VER>1000)
#define DECLARE_HANDLE(name) struct name##__; typedef struct name##__ *name
#else
#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name
#endif
#else
typedef PVOID HANDLE;
#define DECLARE_HANDLE(name) typedef HANDLE name
#endif
typedef HANDLE* PHANDLE;
typedef BYTE FCHAR;
typedef WORD FSHORT;
typedef DWORD FLONG;
#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED
#ifdef __midl
typedef LONG HRESULT;
#else
typedef _Return_type_success_(return >= 0) long HRESULT;
#endif
#endif
#ifdef __cplusplus
#define EXTERN_C extern "C"
#define EXTERN_C_START extern "C" {
#define EXTERN_C_END }
#else
#define EXTERN_C extern
#define EXTERN_C_START
#define EXTERN_C_END
#endif
typedef char CCHAR;
typedef DWORD LCID;
typedef PDWORD PLCID;
typedef WORD LANGID;
#ifndef __COMPARTMENT_ID_DEFINED__
#define __COMPARTMENT_ID_DEFINED__
typedef enum{
UNSPECIFIED_COMPARTMENT_ID=0,
DEFAULT_COMPARTMENT_ID
}COMPARTMENT_ID,*PCOMPARTMENT_ID;
#endif
#if defined(_M_IA64)&&!defined(MIDL_PASS)
__declspec(align(16))
#endif
typedef struct _FLOAT128{
__int64 LowPart;
__int64 HighPart;
}FLOAT128;
typedef FLOAT128* PFLOAT128;
#define _ULONGLONG_
#if (!defined (_MAC)&&(!defined(MIDL_PASS)||defined(__midl))&&(!defined(_M_IX86)||(defined(_INTEGRAL_MAX_BITS)&&_INTEGRAL_MAX_BITS >= 64)))
typedef __int64 LONGLONG;
typedef unsigned __int64 ULONGLONG;
#define MAXLONGLONG (0x7fffffffffffffff)
#else
#if defined(_MAC)&&defined(_MAC_INT_64)
typedef __int64 LONGLONG;
typedef unsigned __int64 ULONGLONG;
#define MAXLONGLONG (0x7fffffffffffffff)
#else
typedef double LONGLONG;
typedef double ULONGLONG;
#endif
#endif
typedef LONGLONG* PLONGLONG;
typedef ULONGLONG* PULONGLONG;
typedef LONGLONG USN;
#if defined(MIDL_PASS)
typedef struct _LARGE_INTEGER{
LONGLONG QuadPart;
}LARGE_INTEGER;
#else
typedef union _LARGE_INTEGER{
struct{
DWORD LowPart;
LONG HighPart;
}DUMMYSTRUCTNAME;
struct{
DWORD LowPart;
LONG HighPart;
}u;
LONGLONG QuadPart;
}LARGE_INTEGER;
#endif
typedef LARGE_INTEGER* PLARGE_INTEGER;
#if defined(MIDL_PASS)
typedef struct _ULARGE_INTEGER{
ULONGLONG QuadPart;
}ULARGE_INTEGER;
#else
typedef union _ULARGE_INTEGER{
struct{
DWORD LowPart;
DWORD HighPart;
}DUMMYSTRUCTNAME;
struct{
DWORD LowPart;
DWORD HighPart;
}u;
ULONGLONG QuadPart;
}ULARGE_INTEGER;
#endif
typedef ULARGE_INTEGER* PULARGE_INTEGER;
typedef LONG_PTR RTL_REFERENCE_COUNT,*PRTL_REFERENCE_COUNT;
typedef LONG RTL_REFERENCE_COUNT32,*PRTL_REFERENCE_COUNT32;
typedef struct _LUID{
DWORD LowPart;
LONG HighPart;
}LUID,*PLUID;
#define _DWORDLONG_
typedef ULONGLONG DWORDLONG;
typedef DWORDLONG* PDWORDLONG;
#if defined(MIDL_PASS)||defined(RC_INVOKED)||defined(_M_CEE_PURE) \
|| defined(_68K_)||defined(_MPPC_) \
|| defined(_M_IA64)||defined(_M_AMD64)||defined(_M_ARM)||defined(_M_ARM64) \
|| defined(_M_HYBRID_X86_ARM64)
#define Int32x32To64(a,b) (((__int64)((long)(a)))*((__int64)((long)(b))))
#define UInt32x32To64(a,b) (((unsigned __int64)((unsigned int)(a)))*((unsigned __int64)((unsigned int)(b))))
#define Int64ShllMod32(a,b) (((unsigned __int64)(a))<<(b))
#define Int64ShraMod32(a,b) (((__int64)(a))>>(b))
#define Int64ShrlMod32(a,b) (((unsigned __int64)(a))>>(b))
#endif
#ifdef __cplusplus
extern "C"{
#endif
#if defined(_M_AMD64)
#define RotateLeft8 _rotl8
#define RotateLeft16 _rotl16
#define RotateRight8 _rotr8
#define RotateRight16 _rotr16
unsigned char
__cdecl
_rotl8(
_In_ unsigned char Value,
_In_ unsigned char Shift);
unsigned short
__cdecl
_rotl16(
_In_ unsigned short Value,
_In_ unsigned char Shift);
unsigned char
__cdecl
_rotr8(
_In_ unsigned char Value,
_In_ unsigned char Shift);
unsigned short
__cdecl
_rotr16(
_In_ unsigned short Value,
_In_ unsigned char Shift);
#pragma intrinsic(_rotl8)
#pragma intrinsic(_rotl16)
#pragma intrinsic(_rotr8)
#pragma intrinsic(_rotr16)
#endif /* _M_AMD64 */
#if _MSC_VER >= 1300
#define RotateLeft32 _rotl
#define RotateLeft64 _rotl64
#define RotateRight32 _rotr
#define RotateRight64 _rotr64
unsigned int
__cdecl
_rotl(
_In_ unsigned int Value,
_In_ int Shift);
unsigned __int64
__cdecl
_rotl64(
_In_ unsigned __int64 Value,
_In_ int Shift);
unsigned int
__cdecl
_rotr(
_In_ unsigned int Value,
_In_ int Shift);
unsigned __int64
__cdecl
_rotr64(
_In_ unsigned __int64 Value,
_In_ int Shift);
#pragma intrinsic(_rotl)
#pragma intrinsic(_rotl64)
#pragma intrinsic(_rotr)
#pragma intrinsic(_rotr64)
#endif /* _MSC_VER >= 1300 */
#ifdef __cplusplus
}
#endif
#define ANSI_NULL ((CHAR)0)
#define UNICODE_NULL ((WCHAR)0)
#define UNICODE_STRING_MAX_BYTES ((WORD) 65534)
#define UNICODE_STRING_MAX_CHARS (32767)
typedef BYTE BOOLEAN;
typedef BOOLEAN* PBOOLEAN;
typedef struct _LIST_ENTRY{
struct _LIST_ENTRY* Flink;
struct _LIST_ENTRY* Blink;
}LIST_ENTRY,*PLIST_ENTRY,*RESTRICTED_POINTER PRLIST_ENTRY;
typedef struct _SINGLE_LIST_ENTRY{
struct _SINGLE_LIST_ENTRY* Next;
}SINGLE_LIST_ENTRY,*PSINGLE_LIST_ENTRY;
typedef struct LIST_ENTRY32{
DWORD Flink;
DWORD Blink;
}LIST_ENTRY32;
typedef LIST_ENTRY32* PLIST_ENTRY32;
typedef struct LIST_ENTRY64{
ULONGLONG Flink;
ULONGLONG Blink;
}LIST_ENTRY64;
typedef LIST_ENTRY64* PLIST_ENTRY64;
#ifndef GUID_DEFINED
#define GUID_DEFINED
#if defined(__midl)
typedef struct{
unsigned long Data1;
unsigned short Data2;
unsigned short Data3;
byte Data4[8];
}GUID;
#else
typedef struct _GUID{
unsigned long Data1;
unsigned short Data2;
unsigned short Data3;
unsigned char Data4[8];
}GUID;
#endif
#endif
#ifndef FAR
#ifdef _WIN32
#define FAR
#else
#define FAR _far
#endif
#endif
#ifndef DECLSPEC_SELECTANY
#if (_MSC_VER >= 1100)
#define DECLSPEC_SELECTANY __declspec(selectany)
#else
#define DECLSPEC_SELECTANY
#endif
#endif
#ifndef EXTERN_C
#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif
#endif
#ifdef DEFINE_GUID
#undef DEFINE_GUID
#endif
#ifdef INITGUID
#define DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
EXTERN_C const GUID DECLSPEC_SELECTANY name \
={l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}
#else
#define DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
EXTERN_C const GUID FAR name
#endif
#define DEFINE_OLEGUID(name,l,w1,w2) DEFINE_GUID(name,l,w1,w2,0xC0,0,0,0,0,0,0,0x46)
#ifndef _GUIDDEF_H_
#define _GUIDDEF_H_
#ifndef __LPGUID_DEFINED__
#define __LPGUID_DEFINED__
typedef GUID* LPGUID;
#endif
#ifndef __LPCGUID_DEFINED__
#define __LPCGUID_DEFINED__
typedef const GUID* LPCGUID;
#endif
#ifndef __IID_DEFINED__
#define __IID_DEFINED__
typedef GUID IID;
typedef IID* LPIID;
#define IID_NULL GUID_NULL
#define IsEqualIID(riid1,riid2) IsEqualGUID(riid1,riid2)
typedef GUID CLSID;
typedef CLSID* LPCLSID;
#define CLSID_NULL GUID_NULL
#define IsEqualCLSID(rclsid1,rclsid2) IsEqualGUID(rclsid1,rclsid2)
typedef GUID FMTID;
typedef FMTID* LPFMTID;
#define FMTID_NULL GUID_NULL
#define IsEqualFMTID(rfmtid1,rfmtid2) IsEqualGUID(rfmtid1,rfmtid2)
#ifdef __midl_proxy
#define __MIDL_CONST
#else
#define __MIDL_CONST const
#endif
#ifndef _REFGUID_DEFINED
#define _REFGUID_DEFINED
#ifdef __cplusplus
#define REFGUID const GUID &
#else
#define REFGUID const GUID*__MIDL_CONST
#endif
#endif
#ifndef _REFIID_DEFINED
#define _REFIID_DEFINED
#ifdef __cplusplus
#define REFIID const IID &
#else
#define REFIID const IID*__MIDL_CONST
#endif
#endif
#ifndef _REFCLSID_DEFINED
#define _REFCLSID_DEFINED
#ifdef __cplusplus
#define REFCLSID const IID &
#else
#define REFCLSID const IID*__MIDL_CONST
#endif
#endif
#ifndef _REFFMTID_DEFINED
#define _REFFMTID_DEFINED
#ifdef __cplusplus
#define REFFMTID const IID &
#else
#define REFFMTID const IID*__MIDL_CONST
#endif
#endif
#endif
#if !defined (__midl)
#if !defined (_SYS_GUID_OPERATORS_)
#define _SYS_GUID_OPERATORS_
// #include <string.h>
#ifdef __cplusplus
__inline int InlineIsEqualGUID(REFGUID rguid1,REFGUID rguid2){
return (
((unsigned long*)&rguid1)[0]==((unsigned long*)&rguid2)[0] &&
((unsigned long*)&rguid1)[1]==((unsigned long*)&rguid2)[1] &&
((unsigned long*)&rguid1)[2]==((unsigned long*)&rguid2)[2] &&
((unsigned long*)&rguid1)[3]==((unsigned long*)&rguid2)[3]);}
__inline int IsEqualGUID(REFGUID rguid1,REFGUID rguid2){
return !memcmp(&rguid1,&rguid2,sizeof(GUID));}
#else
#define InlineIsEqualGUID(rguid1,rguid2) \
(((unsigned long *) rguid1)[0]==((unsigned long *) rguid2)[0]&&\
((unsigned long *) rguid1)[1]==((unsigned long *) rguid2)[1]&&\
((unsigned long *) rguid1)[2]==((unsigned long *) rguid2)[2]&&\
((unsigned long *) rguid1)[3]==((unsigned long *) rguid2)[3])
#define IsEqualGUID(rguid1,rguid2) (!memcmp(rguid1,rguid2,sizeof(GUID)))
#endif
#ifdef __INLINE_ISEQUAL_GUID
#undef IsEqualGUID
#define IsEqualGUID(rguid1,rguid2) InlineIsEqualGUID(rguid1,rguid2)
#endif
#define IsEqualIID(riid1,riid2) IsEqualGUID(riid1,riid2)
#define IsEqualCLSID(rclsid1,rclsid2) IsEqualGUID(rclsid1,rclsid2)
#if !defined _SYS_GUID_OPERATOR_EQ_&&!defined _NO_SYS_GUID_OPERATOR_EQ_
#define _SYS_GUID_OPERATOR_EQ_
#ifdef __cplusplus
__inline bool operator==(REFGUID guidOne,REFGUID guidOther){
return !!IsEqualGUID(guidOne,guidOther);}
__inline bool operator!=(REFGUID guidOne,REFGUID guidOther){
return !(guidOne==guidOther);}
#endif
#endif
#endif
#endif
#endif
#ifndef __OBJECTID_DEFINED
#define __OBJECTID_DEFINED
typedef struct _OBJECTID{
GUID Lineage;
DWORD Uniquifier;
}OBJECTID;
#endif
#define MINCHAR 0x80
#define MAXCHAR 0x7f
#define MINSHORT 0x8000
#define MAXSHORT 0x7fff
#define MINLONG 0x80000000
#define MAXLONG 0x7fffffff
#define MAXBYTE 0xff
#define MAXWORD 0xffff
#define MAXDWORD 0xffffffff
#define FIELD_OFFSET(type,field) ((LONG)(LONG_PTR)&(((type *)0)->field))
#define UFIELD_OFFSET(type,field) ((DWORD)(LONG_PTR)&(((type *)0)->field))
#define RTL_FIELD_SIZE(type,field) (sizeof(((type *)0)->field))
#define RTL_NUMBER_OF_V1(A) (sizeof(A)/sizeof((A)[0]))
#if defined(__cplusplus)&&\
!defined(MIDL_PASS)&&\
!defined(RC_INVOKED)&&\
!defined(_PREFAST_)&&\
(_MSC_FULL_VER >= 13009466)&&\
!defined(SORTPP_PASS)
#pragma region Application Family or OneCore Family Or Game Family
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES)
extern "C++"
template <typename T,size_t N>
char(*RtlpNumberOf(UNALIGNED T(&)[N]))[N];
#endif
#pragma endregion
#define RTL_NUMBER_OF_V2(A) (sizeof(*RtlpNumberOf(A)))
#else
#define RTL_NUMBER_OF_V2(A) RTL_NUMBER_OF_V1(A)
#endif
#ifdef ENABLE_RTL_NUMBER_OF_V2
#define RTL_NUMBER_OF(A) RTL_NUMBER_OF_V2(A)
#else
#define RTL_NUMBER_OF(A) RTL_NUMBER_OF_V1(A)
#endif
#define ARRAYSIZE(A) RTL_NUMBER_OF_V2(A)
#define _ARRAYSIZE(A) RTL_NUMBER_OF_V1(A)
#define RTL_FIELD_TYPE(type,field) (((type*)0)->field)
#define RTL_NUMBER_OF_FIELD(type,field) (RTL_NUMBER_OF(RTL_FIELD_TYPE(type,field)))
#define RTL_PADDING_BETWEEN_FIELDS(T,F1,F2) \
((FIELD_OFFSET(T,F2)>FIELD_OFFSET(T,F1)) \
? (FIELD_OFFSET(T,F2)-FIELD_OFFSET(T,F1)-RTL_FIELD_SIZE(T,F1)) \
: (FIELD_OFFSET(T,F1)-FIELD_OFFSET(T,F2)-RTL_FIELD_SIZE(T,F2)))
#if defined(__cplusplus)
#define RTL_CONST_CAST(type) const_cast<type>
#else
#define RTL_CONST_CAST(type) (type)
#endif
#define RTL_BITS_OF(sizeOfArg) (sizeof(sizeOfArg)*8)
#define RTL_BITS_OF_FIELD(type,field) (RTL_BITS_OF(RTL_FIELD_TYPE(type,field)))
#define CONTAINING_RECORD(address,type,field) ((type *)(\
(PCHAR)(address)-\
(ULONG_PTR)(&((type *)0)->field)))
#include <excpt.h>
typedef
_IRQL_requires_same_
_Function_class_(EXCEPTION_ROUTINE)
EXCEPTION_DISPOSITION
NTAPI
EXCEPTION_ROUTINE(
_Inout_ struct _EXCEPTION_RECORD* ExceptionRecord,
_In_ PVOID EstablisherFrame,
_Inout_ struct _CONTEXT* ContextRecord,
_In_ PVOID DispatcherContext);
typedef EXCEPTION_ROUTINE* PEXCEPTION_ROUTINE;
#if ! defined(lint)
#ifdef _PREFAST_
void _Prefast_unreferenced_parameter_impl_(const char*,...);
#define UNREFERENCED_PARAMETER(P) _Prefast_unreferenced_parameter_impl_("PREfast",((void) (P),0))
#define DBG_UNREFERENCED_PARAMETER(P) _Prefast_unreferenced_parameter_impl_("PREfast",((void) (P),0))
#define DBG_UNREFERENCED_LOCAL_VARIABLE(V) _Prefast_unreferenced_parameter_impl_("PREfast",((void) (V),0))
#else
#define UNREFERENCED_PARAMETER(P) (P)
#define DBG_UNREFERENCED_PARAMETER(P) (P)
#define DBG_UNREFERENCED_LOCAL_VARIABLE(V) (V)
#endif
#else
#endif
#if (_MSC_VER>1200)
#define DEFAULT_UNREACHABLE default: __assume(0)
#else
#define DEFAULT_UNREACHABLE
#endif
#ifdef __cplusplus
#if _MSC_VER >= 1900
#define _ENUM_FLAG_CONSTEXPR constexpr
#else
#define _ENUM_FLAG_CONSTEXPR
#endif
#define DEFINE_ENUM_FLAG_OPERATORS(ENUMTYPE) \
extern "C++"{\
inline _ENUM_FLAG_CONSTEXPR ENUMTYPE operator | (ENUMTYPE a,ENUMTYPE b) throw(){return ENUMTYPE(((_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)a) | ((_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)b));}\
inline ENUMTYPE &operator |= (ENUMTYPE &a,ENUMTYPE b) throw(){return (ENUMTYPE &)(((_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type &)a) |= ((_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)b));}\
inline _ENUM_FLAG_CONSTEXPR ENUMTYPE operator & (ENUMTYPE a,ENUMTYPE b) throw(){return ENUMTYPE(((_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)a) & ((_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)b));}\
inline ENUMTYPE &operator &= (ENUMTYPE &a,ENUMTYPE b) throw(){return (ENUMTYPE &)(((_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type &)a) &= ((_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)b));}\
inline _ENUM_FLAG_CONSTEXPR ENUMTYPE operator ~ (ENUMTYPE a) throw(){return ENUMTYPE(~((_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)a));}\
inline _ENUM_FLAG_CONSTEXPR ENUMTYPE operator ^ (ENUMTYPE a,ENUMTYPE b) throw(){return ENUMTYPE(((_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)a) ^ ((_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)b));}\
inline ENUMTYPE &operator ^= (ENUMTYPE &a,ENUMTYPE b) throw(){return (ENUMTYPE &)(((_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type &)a) ^= ((_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)b));}\
}
#else
#define DEFINE_ENUM_FLAG_OPERATORS(ENUMTYPE)
#endif
#if !defined(WINDOWS_IGNORE_PACKING_MISMATCH)&&!defined(__midl)&&!defined(MIDL_PASS)&&!defined(SORTPP_PASS)&&!defined(RC_INVOKED)
#if defined(__cplusplus)&&(_MSC_VER >= 1600)
static_assert(__alignof(LARGE_INTEGER)==8,"Windows headers require the default packing option. Changing this can lead to memory corruption."
" This diagnostic can be disabled by building with WINDOWS_IGNORE_PACKING_MISMATCH defined.");
#elif _MSC_VER >= 1300
#pragma warning(push)
#pragma warning(disable: 4116)
C_ASSERT(TYPE_ALIGNMENT(LARGE_INTEGER)==8);
#pragma warning(pop)
#endif
#endif
#define MAXIMUM_WAIT_OBJECTS 64
#define MAXIMUM_SUSPEND_COUNT MAXCHAR
typedef ULONG_PTR KSPIN_LOCK;
typedef KSPIN_LOCK* PKSPIN_LOCK;
typedef struct DECLSPEC_ALIGN(16) _M128A{
ULONGLONG Low;
LONGLONG High;
}M128A,*PM128A;
typedef struct DECLSPEC_ALIGN(16) _XSAVE_FORMAT{
WORD ControlWord;
WORD StatusWord;
BYTE TagWord;
BYTE Reserved1;
WORD ErrorOpcode;
DWORD ErrorOffset;
WORD ErrorSelector;
WORD Reserved2;
DWORD DataOffset;
WORD DataSelector;
WORD Reserved3;
DWORD MxCsr;
DWORD MxCsr_Mask;
M128A FloatRegisters[8];
#if defined(_WIN64)
M128A XmmRegisters[16];
BYTE Reserved4[96];
#else
M128A XmmRegisters[8];
BYTE Reserved4[224];
#endif
}XSAVE_FORMAT,*PXSAVE_FORMAT;
typedef struct _XSAVE_CET_U_FORMAT{
DWORD64 Ia32CetUMsr;
DWORD64 Ia32Pl3SspMsr;
}XSAVE_CET_U_FORMAT,*PXSAVE_CET_U_FORMAT;
typedef struct DECLSPEC_ALIGN(8) _XSAVE_AREA_HEADER{
DWORD64 Mask;
DWORD64 CompactionMask;
DWORD64 Reserved2[6];
}XSAVE_AREA_HEADER,*PXSAVE_AREA_HEADER;
typedef struct DECLSPEC_ALIGN(16) _XSAVE_AREA{
XSAVE_FORMAT LegacyState;
XSAVE_AREA_HEADER Header;
}XSAVE_AREA,*PXSAVE_AREA;
typedef struct _XSTATE_CONTEXT{
DWORD64 Mask;
DWORD Length;
DWORD Reserved1;
_Field_size_bytes_opt_(Length) PXSAVE_AREA Area;
PVOID Buffer;
}XSTATE_CONTEXT,*PXSTATE_CONTEXT;
typedef struct _SCOPE_TABLE_AMD64{
DWORD Count;
struct{
DWORD BeginAddress;
DWORD EndAddress;
DWORD HandlerAddress;
DWORD JumpTarget;
}ScopeRecord[1];
}SCOPE_TABLE_AMD64,*PSCOPE_TABLE_AMD64;
#ifdef _AMD64_
#if defined(_M_AMD64)&&!defined(RC_INVOKED)&&!defined(MIDL_PASS)
#ifdef __cplusplus
extern "C"{
#endif
#define BitTest _bittest
#define BitTestAndComplement _bittestandcomplement
#define BitTestAndSet _bittestandset
#define BitTestAndReset _bittestandreset
#define InterlockedBitTestAndSet _interlockedbittestandset
#define InterlockedBitTestAndSetAcquire _interlockedbittestandset
#define InterlockedBitTestAndSetRelease _interlockedbittestandset
#define InterlockedBitTestAndSetNoFence _interlockedbittestandset
#define InterlockedBitTestAndReset _interlockedbittestandreset
#define InterlockedBitTestAndResetAcquire _interlockedbittestandreset
#define InterlockedBitTestAndResetRelease _interlockedbittestandreset
#define InterlockedBitTestAndResetNoFence _interlockedbittestandreset
#define BitTest64 _bittest64
#define BitTestAndComplement64 _bittestandcomplement64
#define BitTestAndSet64 _bittestandset64
#define BitTestAndReset64 _bittestandreset64
#define InterlockedBitTestAndSet64 _interlockedbittestandset64
#define InterlockedBitTestAndSet64Acquire _interlockedbittestandset64
#define InterlockedBitTestAndSet64Release _interlockedbittestandset64
#define InterlockedBitTestAndSet64NoFence _interlockedbittestandset64
#define InterlockedBitTestAndReset64 _interlockedbittestandreset64
#define InterlockedBitTestAndReset64Acquire _interlockedbittestandreset64
#define InterlockedBitTestAndReset64Release _interlockedbittestandreset64
#define InterlockedBitTestAndReset64NoFence _interlockedbittestandreset64
_Must_inspect_result_
BOOLEAN
_bittest(
_In_reads_bytes_((Offset / 8)+1) LONG const* Base,
_In_range_(>= ,0) LONG Offset);
BOOLEAN
_bittestandcomplement(
_Inout_updates_bytes_((Offset / 8)+1) LONG* Base,
_In_range_(>= ,0) LONG Offset);
BOOLEAN
_bittestandset(
_Inout_updates_bytes_((Offset / 8)+1) LONG* Base,
_In_range_(>= ,0) LONG Offset);
BOOLEAN
_bittestandreset(
_Inout_updates_bytes_((Offset / 8)+1) LONG* Base,
_In_range_(>= ,0) LONG Offset);
BOOLEAN
_interlockedbittestandset(
_Inout_updates_bytes_((Offset / 8)+1) _Interlocked_operand_ LONG volatile* Base,
_In_range_(>= ,0) LONG Offset);
BOOLEAN
_interlockedbittestandreset(
_Inout_updates_bytes_((Offset / 8)+1) _Interlocked_operand_ LONG volatile* Base,
_In_range_(>= ,0) LONG Offset);
BOOLEAN
_bittest64(
_In_reads_bytes_((Offset / 8)+1) LONG64 const* Base,
_In_range_(>= ,0) LONG64 Offset);
BOOLEAN
_bittestandcomplement64(
_Inout_updates_bytes_((Offset / 8)+1) LONG64* Base,
_In_range_(>= ,0) LONG64 Offset);
BOOLEAN
_bittestandset64(
_Inout_updates_bytes_((Offset / 8)+1) LONG64* Base,
_In_range_(>= ,0) LONG64 Offset);
BOOLEAN
_bittestandreset64(
_Inout_updates_bytes_((Offset / 8)+1) LONG64* Base,
_In_range_(>= ,0) LONG64 Offset);
BOOLEAN
_interlockedbittestandset64(
_Inout_updates_bytes_((Offset / 8)+1) _Interlocked_operand_ LONG64 volatile* Base,
_In_range_(>= ,0) LONG64 Offset);
BOOLEAN
_interlockedbittestandreset64(
_Inout_updates_bytes_((Offset / 8)+1) _Interlocked_operand_ LONG64 volatile* Base,
_In_range_(>= ,0) LONG64 Offset);
#pragma intrinsic(_bittest)
#pragma intrinsic(_bittestandcomplement)
#pragma intrinsic(_bittestandset)
#pragma intrinsic(_bittestandreset)
#pragma intrinsic(_interlockedbittestandset)
#pragma intrinsic(_interlockedbittestandreset)
#pragma intrinsic(_bittest64)
#pragma intrinsic(_bittestandcomplement64)
#pragma intrinsic(_bittestandset64)
#pragma intrinsic(_bittestandreset64)
#pragma intrinsic(_interlockedbittestandset64)
#pragma intrinsic(_interlockedbittestandreset64)
#define BitScanForward _BitScanForward
#define BitScanReverse _BitScanReverse
#define BitScanForward64 _BitScanForward64
#define BitScanReverse64 _BitScanReverse64
_Success_(return != 0)
BOOLEAN
_BitScanForward(
_Out_ DWORD* Index,
_In_ DWORD Mask);
_Success_(return != 0)
BOOLEAN
_BitScanReverse(
_Out_ DWORD* Index,
_In_ DWORD Mask);
_Success_(return != 0)
BOOLEAN
_BitScanForward64(
_Out_ DWORD* Index,
_In_ DWORD64 Mask);
_Success_(return != 0)
BOOLEAN
_BitScanReverse64(
_Out_ DWORD* Index,
_In_ DWORD64 Mask);
#pragma intrinsic(_BitScanForward)
#pragma intrinsic(_BitScanReverse)
#pragma intrinsic(_BitScanForward64)
#pragma intrinsic(_BitScanReverse64)
#define InterlockedIncrement16 _InterlockedIncrement16
#define InterlockedIncrementAcquire16 _InterlockedIncrement16
#define InterlockedIncrementRelease16 _InterlockedIncrement16
#define InterlockedIncrementNoFence16 _InterlockedIncrement16
#define InterlockedDecrement16 _InterlockedDecrement16
#define InterlockedDecrementAcquire16 _InterlockedDecrement16
#define InterlockedDecrementRelease16 _InterlockedDecrement16
#define InterlockedDecrementNoFence16 _InterlockedDecrement16
#define InterlockedCompareExchange16 _InterlockedCompareExchange16
#define InterlockedCompareExchangeAcquire16 _InterlockedCompareExchange16
#define InterlockedCompareExchangeRelease16 _InterlockedCompareExchange16
#define InterlockedCompareExchangeNoFence16 _InterlockedCompareExchange16
#define InterlockedAnd _InterlockedAnd
#define InterlockedAndAcquire _InterlockedAnd
#define InterlockedAndRelease _InterlockedAnd
#define InterlockedAndNoFence _InterlockedAnd
#define InterlockedOr _InterlockedOr
#define InterlockedOrAcquire _InterlockedOr
#define InterlockedOrRelease _InterlockedOr
#define InterlockedOrNoFence _InterlockedOr
#define InterlockedXor _InterlockedXor
#define InterlockedXorAcquire _InterlockedXor
#define InterlockedXorRelease _InterlockedXor
#define InterlockedXorNoFence _InterlockedXor
#define InterlockedIncrement _InterlockedIncrement
#define InterlockedIncrementAcquire _InterlockedIncrement
#define InterlockedIncrementRelease _InterlockedIncrement
#define InterlockedIncrementNoFence _InterlockedIncrement
#define InterlockedDecrement _InterlockedDecrement
#define InterlockedDecrementAcquire _InterlockedDecrement
#define InterlockedDecrementRelease _InterlockedDecrement
#define InterlockedDecrementNoFence _InterlockedDecrement
#define InterlockedAdd _InlineInterlockedAdd
#define InterlockedAddAcquire _InlineInterlockedAdd
#define InterlockedAddRelease _InlineInterlockedAdd
#define InterlockedAddNoFence _InlineInterlockedAdd
#define InterlockedExchange _InterlockedExchange
#define InterlockedExchangeAcquire _InterlockedExchange
#define InterlockedExchangeNoFence _InterlockedExchange
#define InterlockedExchangeAdd _InterlockedExchangeAdd
#define InterlockedExchangeAddAcquire _InterlockedExchangeAdd
#define InterlockedExchangeAddRelease _InterlockedExchangeAdd
#define InterlockedExchangeAddNoFence _InterlockedExchangeAdd
#define InterlockedCompareExchange _InterlockedCompareExchange
#define InterlockedCompareExchangeAcquire _InterlockedCompareExchange
#define InterlockedCompareExchangeRelease _InterlockedCompareExchange
#define InterlockedCompareExchangeNoFence _InterlockedCompareExchange
#define InterlockedAnd64 _InterlockedAnd64
#define InterlockedAnd64Acquire _InterlockedAnd64
#define InterlockedAnd64Release _InterlockedAnd64
#define InterlockedAnd64NoFence _InterlockedAnd64
#define InterlockedAndAffinity InterlockedAnd64
#define InterlockedOr64 _InterlockedOr64
#define InterlockedOr64Acquire _InterlockedOr64
#define InterlockedOr64Release _InterlockedOr64
#define InterlockedOr64NoFence _InterlockedOr64
#define InterlockedOrAffinity InterlockedOr64
#define InterlockedXor64 _InterlockedXor64
#define InterlockedXor64Acquire _InterlockedXor64
#define InterlockedXor64Release _InterlockedXor64
#define InterlockedXor64NoFence _InterlockedXor64
#define InterlockedIncrement64 _InterlockedIncrement64
#define InterlockedIncrementAcquire64 _InterlockedIncrement64
#define InterlockedIncrementRelease64 _InterlockedIncrement64
#define InterlockedIncrementNoFence64 _InterlockedIncrement64
#define InterlockedDecrement64 _InterlockedDecrement64
#define InterlockedDecrementAcquire64 _InterlockedDecrement64
#define InterlockedDecrementRelease64 _InterlockedDecrement64
#define InterlockedDecrementNoFence64 _InterlockedDecrement64
#define InterlockedAdd64 _InlineInterlockedAdd64
#define InterlockedAddAcquire64 _InlineInterlockedAdd64
#define InterlockedAddRelease64 _InlineInterlockedAdd64
#define InterlockedAddNoFence64 _InlineInterlockedAdd64
#define InterlockedExchange64 _InterlockedExchange64
#define InterlockedExchangeAcquire64 InterlockedExchange64
#define InterlockedExchangeNoFence64 InterlockedExchange64
#define InterlockedExchangeAdd64 _InterlockedExchangeAdd64
#define InterlockedExchangeAddAcquire64 _InterlockedExchangeAdd64
#define InterlockedExchangeAddRelease64 _InterlockedExchangeAdd64
#define InterlockedExchangeAddNoFence64 _InterlockedExchangeAdd64
#define InterlockedCompareExchange64 _InterlockedCompareExchange64
#define InterlockedCompareExchangeAcquire64 InterlockedCompareExchange64
#define InterlockedCompareExchangeRelease64 InterlockedCompareExchange64
#define InterlockedCompareExchangeNoFence64 InterlockedCompareExchange64
#define InterlockedCompareExchange128 _InterlockedCompareExchange128
#define InterlockedExchangePointer _InterlockedExchangePointer
#define InterlockedExchangePointerNoFence _InterlockedExchangePointer
#define InterlockedExchangePointerAcquire _InterlockedExchangePointer
#define InterlockedCompareExchangePointer _InterlockedCompareExchangePointer
#define InterlockedCompareExchangePointerAcquire _InterlockedCompareExchangePointer
#define InterlockedCompareExchangePointerRelease _InterlockedCompareExchangePointer
#define InterlockedCompareExchangePointerNoFence _InterlockedCompareExchangePointer
#define InterlockedExchangeAddSizeT(a,b) InterlockedExchangeAdd64((LONG64 *)a,b)
#define InterlockedExchangeAddSizeTAcquire(a,b) InterlockedExchangeAdd64((LONG64 *)a,b)
#define InterlockedExchangeAddSizeTNoFence(a,b) InterlockedExchangeAdd64((LONG64 *)a,b)
#define InterlockedIncrementSizeT(a) InterlockedIncrement64((LONG64 *)a)
#define InterlockedIncrementSizeTNoFence(a) InterlockedIncrement64((LONG64 *)a)
#define InterlockedDecrementSizeT(a) InterlockedDecrement64((LONG64 *)a)
#define InterlockedDecrementSizeTNoFence(a) InterlockedDecrement64((LONG64 *)a)
SHORT
InterlockedIncrement16(
_Inout_ _Interlocked_operand_ SHORT volatile* Addend);
SHORT
InterlockedDecrement16(
_Inout_ _Interlocked_operand_ SHORT volatile* Addend);
SHORT
InterlockedCompareExchange16(
_Inout_ _Interlocked_operand_ SHORT volatile* Destination,
_In_ SHORT ExChange,
_In_ SHORT Comperand);
LONG
InterlockedAnd(
_Inout_ _Interlocked_operand_ LONG volatile* Destination,
_In_ LONG Value);
LONG
InterlockedOr(
_Inout_ _Interlocked_operand_ LONG volatile* Destination,
_In_ LONG Value);
LONG
InterlockedXor(
_Inout_ _Interlocked_operand_ LONG volatile* Destination,
_In_ LONG Value);
LONG64
InterlockedAnd64(
_Inout_ _Interlocked_operand_ LONG64 volatile* Destination,
_In_ LONG64 Value);
LONG64
InterlockedOr64(
_Inout_ _Interlocked_operand_ LONG64 volatile* Destination,
_In_ LONG64 Value);
LONG64
InterlockedXor64(
_Inout_ _Interlocked_operand_ LONG64 volatile* Destination,
_In_ LONG64 Value);
LONG
InterlockedIncrement(
_Inout_ _Interlocked_operand_ LONG volatile* Addend);
LONG
InterlockedDecrement(
_Inout_ _Interlocked_operand_ LONG volatile* Addend);
LONG
InterlockedExchange(
_Inout_ _Interlocked_operand_ LONG volatile* Target,
_In_ LONG Value);
LONG
InterlockedExchangeAdd(
_Inout_ _Interlocked_operand_ LONG volatile* Addend,
_In_ LONG Value);
#if !defined(_X86AMD64_)
__forceinline
LONG
InterlockedAdd(
_Inout_ _Interlocked_operand_ LONG volatile* Addend,
_In_ LONG Value){
return InterlockedExchangeAdd(Addend,Value)+Value;}
#endif
LONG
InterlockedCompareExchange(
_Inout_ _Interlocked_operand_ LONG volatile* Destination,
_In_ LONG ExChange,
_In_ LONG Comperand);
LONG64
InterlockedIncrement64(
_Inout_ _Interlocked_operand_ LONG64 volatile* Addend);
LONG64
InterlockedDecrement64(
_Inout_ _Interlocked_operand_ LONG64 volatile* Addend);
LONG64
InterlockedExchange64(
_Inout_ _Interlocked_operand_ LONG64 volatile* Target,
_In_ LONG64 Value);
LONG64
InterlockedExchangeAdd64(
_Inout_ _Interlocked_operand_ LONG64 volatile* Addend,
_In_ LONG64 Value);
#if !defined(_X86AMD64_)
__forceinline
LONG64
_InlineInterlockedAdd64(
_Inout_ _Interlocked_operand_ LONG64 volatile* Addend,
_In_ LONG64 Value){
return InterlockedExchangeAdd64(Addend,Value)+Value;}
#endif
LONG64
InterlockedCompareExchange64(
_Inout_ _Interlocked_operand_ LONG64 volatile* Destination,
_In_ LONG64 ExChange,
_In_ LONG64 Comperand);
BOOLEAN
InterlockedCompareExchange128(
_Inout_ _Interlocked_operand_ LONG64 volatile* Destination,
_In_ LONG64 ExchangeHigh,
_In_ LONG64 ExchangeLow,
_Inout_ LONG64* ComparandResult);
_Ret_writes_(_Inexpressible_(Unknown)) PVOID
InterlockedCompareExchangePointer(
_Inout_ _At_(*Destination,
_Pre_writable_byte_size_(_Inexpressible_(Unknown))
_Post_writable_byte_size_(_Inexpressible_(Unknown)))
_Interlocked_operand_ PVOID volatile* Destination,
_In_opt_ PVOID Exchange,
_In_opt_ PVOID Comperand);
_Ret_writes_(_Inexpressible_(Unknown)) PVOID
InterlockedExchangePointer(
_Inout_ _At_(*Target,
_Pre_writable_byte_size_(_Inexpressible_(Unknown))
_Post_writable_byte_size_(_Inexpressible_(Unknown)))
_Interlocked_operand_ PVOID volatile* Target,
_In_opt_ PVOID Value);
#pragma intrinsic(_InterlockedIncrement16)
#pragma intrinsic(_InterlockedDecrement16)
#pragma intrinsic(_InterlockedCompareExchange16)
#pragma intrinsic(_InterlockedAnd)
#pragma intrinsic(_InterlockedOr)
#pragma intrinsic(_InterlockedXor)
#pragma intrinsic(_InterlockedIncrement)
#pragma intrinsic(_InterlockedDecrement)
#pragma intrinsic(_InterlockedExchange)
#pragma intrinsic(_InterlockedExchangeAdd)
#pragma intrinsic(_InterlockedCompareExchange)
#pragma intrinsic(_InterlockedAnd64)
#pragma intrinsic(_InterlockedOr64)
#pragma intrinsic(_InterlockedXor64)
#pragma intrinsic(_InterlockedIncrement64)
#pragma intrinsic(_InterlockedDecrement64)
#pragma intrinsic(_InterlockedExchange64)
#pragma intrinsic(_InterlockedExchangeAdd64)
#pragma intrinsic(_InterlockedCompareExchange64)
#if _MSC_VER >= 1500
#pragma intrinsic(_InterlockedCompareExchange128)
#endif
#pragma intrinsic(_InterlockedExchangePointer)
#pragma intrinsic(_InterlockedCompareExchangePointer)
#if (_MSC_VER >= 1600)
#define InterlockedExchange8 _InterlockedExchange8
#define InterlockedExchange16 _InterlockedExchange16
CHAR
InterlockedExchange8(
_Inout_ _Interlocked_operand_ CHAR volatile* Target,
_In_ CHAR Value);
SHORT
InterlockedExchange16(
_Inout_ _Interlocked_operand_ SHORT volatile* Destination,
_In_ SHORT ExChange);
#pragma intrinsic(_InterlockedExchange8)
#pragma intrinsic(_InterlockedExchange16)
#endif /* _MSC_VER >= 1600 */
#if _MSC_FULL_VER >= 140041204
#define InterlockedExchangeAdd8 _InterlockedExchangeAdd8
#define InterlockedAnd8 _InterlockedAnd8
#define InterlockedOr8 _InterlockedOr8
#define InterlockedXor8 _InterlockedXor8
#define InterlockedAnd16 _InterlockedAnd16
#define InterlockedOr16 _InterlockedOr16
#define InterlockedXor16 _InterlockedXor16
char
InterlockedExchangeAdd8(
_Inout_ _Interlocked_operand_ char volatile* _Addend,
_In_ char _Value);
char
InterlockedAnd8(
_Inout_ _Interlocked_operand_ char volatile* Destination,
_In_ char Value);
char
InterlockedOr8(
_Inout_ _Interlocked_operand_ char volatile* Destination,
_In_ char Value);
char
InterlockedXor8(
_Inout_ _Interlocked_operand_ char volatile* Destination,
_In_ char Value);
SHORT
InterlockedAnd16(
_Inout_ _Interlocked_operand_ SHORT volatile* Destination,
_In_ SHORT Value);
SHORT
InterlockedOr16(
_Inout_ _Interlocked_operand_ SHORT volatile* Destination,
_In_ SHORT Value);
SHORT
InterlockedXor16(
_Inout_ _Interlocked_operand_ SHORT volatile* Destination,
_In_ SHORT Value);
#pragma intrinsic (_InterlockedExchangeAdd8)
#pragma intrinsic (_InterlockedAnd8)
#pragma intrinsic (_InterlockedOr8)
#pragma intrinsic (_InterlockedXor8)
#pragma intrinsic (_InterlockedAnd16)
#pragma intrinsic (_InterlockedOr16)
#pragma intrinsic (_InterlockedXor16)
#endif
#define CpuIdEx __cpuidex
VOID
__cpuidex(
int CPUInfo[4],
int Function,
int SubLeaf);
#pragma intrinsic(__cpuidex)
#define CacheLineFlush(Address) _mm_clflush(Address)
VOID
_mm_clflush(
_In_ VOID const* Address);
#pragma intrinsic(_mm_clflush)
VOID
_ReadWriteBarrier(
VOID);
#pragma intrinsic(_ReadWriteBarrier)
#define FastFence __faststorefence
#define LoadFence _mm_lfence
#define MemoryFence _mm_mfence
#define StoreFence _mm_sfence
#define SpeculationFence LoadFence
VOID
__faststorefence(
VOID);
VOID
_mm_lfence(
VOID);
VOID
_mm_mfence(
VOID);
VOID
_mm_sfence(
VOID);
VOID
_mm_pause(
VOID);
VOID
_mm_prefetch(
_In_ CHAR CONST* a,
_In_ int sel);
VOID
_m_prefetchw(
_In_ volatile CONST VOID* Source);
#define _MM_HINT_T0 1
#define _MM_HINT_T1 2
#define _MM_HINT_T2 3
#define _MM_HINT_NTA 0
#pragma intrinsic(__faststorefence)
#pragma intrinsic(_mm_pause)
#pragma intrinsic(_mm_prefetch)
#pragma intrinsic(_mm_lfence)
#pragma intrinsic(_mm_mfence)
#pragma intrinsic(_mm_sfence)
#pragma intrinsic(_m_prefetchw)
#define YieldProcessor _mm_pause
#define MemoryBarrier __faststorefence
#define PreFetchCacheLine(l,a) _mm_prefetch((CHAR CONST *) a,l)
#define PrefetchForWrite(p) _m_prefetchw(p)
#define ReadForWriteAccess(p) (_m_prefetchw(p),*(p))
#define PF_TEMPORAL_LEVEL_1 _MM_HINT_T0
#define PF_TEMPORAL_LEVEL_2 _MM_HINT_T1
#define PF_TEMPORAL_LEVEL_3 _MM_HINT_T2
#define PF_NON_TEMPORAL_LEVEL_ALL _MM_HINT_NTA
#define ReadMxCsr _mm_getcsr
#define WriteMxCsr _mm_setcsr
unsigned int
_mm_getcsr(
VOID);
VOID
_mm_setcsr(
_In_ unsigned int MxCsr);
#pragma intrinsic(_mm_getcsr)
#pragma intrinsic(_mm_setcsr)
#define GetCallersEflags() __getcallerseflags()
unsigned __int32
__getcallerseflags(
VOID);
#pragma intrinsic(__getcallerseflags)
#define GetSegmentLimit __segmentlimit
DWORD
__segmentlimit(
_In_ DWORD Selector);
#pragma intrinsic(__segmentlimit)
#define ReadPMC __readpmc
DWORD64
__readpmc(
_In_ DWORD Counter);
#pragma intrinsic(__readpmc)
#define ReadTimeStampCounter() __rdtsc()
DWORD64
__rdtsc(
VOID);
#pragma intrinsic(__rdtsc)
VOID
__movsb(
_Out_writes_all_(Count) PBYTE Destination,
_In_reads_(Count) BYTE const* Source,
_In_ SIZE_T Count);
VOID
__movsw(
_Out_writes_all_(Count) PWORD Destination,
_In_reads_(Count) WORD const* Source,
_In_ SIZE_T Count);
VOID
__movsd(
_Out_writes_all_(Count) PDWORD Destination,
_In_reads_(Count) DWORD const* Source,
_In_ SIZE_T Count);
VOID
__movsq(
_Out_writes_all_(Count) PDWORD64 Destination,
_In_reads_(Count) DWORD64 const* Source,
_In_ SIZE_T Count);
#pragma intrinsic(__movsb)
#pragma intrinsic(__movsw)
#pragma intrinsic(__movsd)
#pragma intrinsic(__movsq)
VOID
__stosb(
_Out_writes_all_(Count) PBYTE Destination,
_In_ BYTE Value,
_In_ SIZE_T Count);
VOID
__stosw(
_Out_writes_all_(Count) PWORD Destination,
_In_ WORD Value,
_In_ SIZE_T Count);
VOID
__stosd(
_Out_writes_all_(Count) PDWORD Destination,
_In_ DWORD Value,
_In_ SIZE_T Count);
VOID
__stosq(
_Out_writes_all_(Count) PDWORD64 Destination,
_In_ DWORD64 Value,
_In_ SIZE_T Count);
#pragma intrinsic(__stosb)
#pragma intrinsic(__stosw)
#pragma intrinsic(__stosd)
#pragma intrinsic(__stosq)
#define MultiplyHigh __mulh
#define UnsignedMultiplyHigh __umulh
LONGLONG
MultiplyHigh(
_In_ LONG64 Multiplier,
_In_ LONG64 Multiplicand);
ULONGLONG
UnsignedMultiplyHigh(
_In_ DWORD64 Multiplier,
_In_ DWORD64 Multiplicand);
#pragma intrinsic(__mulh)
#pragma intrinsic(__umulh)
#define PopulationCount64 __popcnt64
DWORD64
PopulationCount64(
_In_ DWORD64 operand);
#if _MSC_VER >= 1500
#pragma intrinsic(__popcnt64)
#endif
#define ShiftLeft128 __shiftleft128
#define ShiftRight128 __shiftright128
DWORD64
ShiftLeft128(
_In_ DWORD64 LowPart,
_In_ DWORD64 HighPart,
_In_ BYTE Shift);
DWORD64
ShiftRight128(
_In_ DWORD64 LowPart,
_In_ DWORD64 HighPart,
_In_ BYTE Shift);
#pragma intrinsic(__shiftleft128)
#pragma intrinsic(__shiftright128)
#define Multiply128 _mul128
LONG64
Multiply128(
_In_ LONG64 Multiplier,
_In_ LONG64 Multiplicand,
_Out_ LONG64* HighProduct);
#pragma intrinsic(_mul128)
#ifndef UnsignedMultiply128
#define UnsignedMultiply128 _umul128
DWORD64
UnsignedMultiply128(
_In_ DWORD64 Multiplier,
_In_ DWORD64 Multiplicand,
_Out_ DWORD64* HighProduct);
#pragma intrinsic(_umul128)
#endif
__forceinline
LONG64
MultiplyExtract128(
_In_ LONG64 Multiplier,
_In_ LONG64 Multiplicand,
_In_ BYTE Shift){
LONG64 extractedProduct;
LONG64 highProduct;
LONG64 lowProduct;
BOOLEAN negate;
DWORD64 uhighProduct;
DWORD64 ulowProduct;
lowProduct=Multiply128(Multiplier,Multiplicand,&highProduct);
negate=FALSE;
uhighProduct=(DWORD64)highProduct;
ulowProduct=(DWORD64)lowProduct;
if (highProduct<0){
negate=TRUE;
uhighProduct=(DWORD64)(-highProduct);
ulowProduct=(DWORD64)(-lowProduct);
if (ulowProduct != 0){
uhighProduct -= 1;}}
extractedProduct=(LONG64)ShiftRight128(ulowProduct,uhighProduct,Shift);
if (negate != FALSE){
extractedProduct=-extractedProduct;}
return extractedProduct;}
__forceinline
DWORD64
UnsignedMultiplyExtract128(
_In_ DWORD64 Multiplier,
_In_ DWORD64 Multiplicand,
_In_ BYTE Shift){
DWORD64 extractedProduct;
DWORD64 highProduct;
DWORD64 lowProduct;
lowProduct=UnsignedMultiply128(Multiplier,Multiplicand,&highProduct);
extractedProduct=ShiftRight128(lowProduct,highProduct,Shift);
return extractedProduct;}
BYTE
__readgsbyte(
_In_ DWORD Offset);
WORD
__readgsword(
_In_ DWORD Offset);
DWORD
__readgsdword(
_In_ DWORD Offset);
DWORD64
__readgsqword(
_In_ DWORD Offset);
VOID
__writegsbyte(
_In_ DWORD Offset,
_In_ BYTE Data);
VOID
__writegsword(
_In_ DWORD Offset,
_In_ WORD Data);
VOID
__writegsdword(
_In_ DWORD Offset,
_In_ DWORD Data);
VOID
__writegsqword(
_In_ DWORD Offset,
_In_ DWORD64 Data);
#pragma intrinsic(__readgsbyte)
#pragma intrinsic(__readgsword)
#pragma intrinsic(__readgsdword)
#pragma intrinsic(__readgsqword)
#pragma intrinsic(__writegsbyte)
#pragma intrinsic(__writegsword)
#pragma intrinsic(__writegsdword)
#pragma intrinsic(__writegsqword)
#if !defined(_MANAGED)
VOID
__incgsbyte(
_In_ DWORD Offset);
VOID
__addgsbyte(
_In_ DWORD Offset,
_In_ BYTE Value);
VOID
__incgsword(
_In_ DWORD Offset);
VOID
__addgsword(
_In_ DWORD Offset,
_In_ WORD Value);
VOID
__incgsdword(
_In_ DWORD Offset);
VOID
__addgsdword(
_In_ DWORD Offset,
_In_ DWORD Value);
VOID
__incgsqword(
_In_ DWORD Offset);
VOID
__addgsqword(
_In_ DWORD Offset,
_In_ DWORD64 Value);
#if 0
#pragma intrinsic(__incgsbyte)
#pragma intrinsic(__addgsbyte)
#pragma intrinsic(__incgsword)
#pragma intrinsic(__addgsword)
#pragma intrinsic(__incgsdword)
#pragma intrinsic(__addgsdword)
#pragma intrinsic(__incgsqword)
#pragma intrinsic(__addgsqword)
#endif
#endif
#ifdef __cplusplus
}
#endif
#endif
#define EXCEPTION_READ_FAULT 0
#define EXCEPTION_WRITE_FAULT 1
#define EXCEPTION_EXECUTE_FAULT 8
#if !defined(RC_INVOKED)
#define CONTEXT_AMD64 0x0100000L
#define CONTEXT_CONTROL (CONTEXT_AMD64 | 0x01L)
#define CONTEXT_INTEGER (CONTEXT_AMD64 | 0x02L)
#define CONTEXT_SEGMENTS (CONTEXT_AMD64 | 0x04L)
#define CONTEXT_FLOATING_POINT (CONTEXT_AMD64 | 0x08L)
#define CONTEXT_DEBUG_REGISTERS (CONTEXT_AMD64 | 0x010L)
#define CONTEXT_FULL (CONTEXT_CONTROL | CONTEXT_INTEGER | \
CONTEXT_FLOATING_POINT)
#define CONTEXT_ALL (CONTEXT_CONTROL | CONTEXT_INTEGER | \
CONTEXT_SEGMENTS | CONTEXT_FLOATING_POINT | \
CONTEXT_DEBUG_REGISTERS)
#define CONTEXT_XSTATE (CONTEXT_AMD64 | 0x040L)
#if defined(XBOX_SYSTEMOS)
#define CONTEXT_KERNEL_DEBUGGER 0x04000000L
#endif
#define CONTEXT_EXCEPTION_ACTIVE 0x08000000L
#define CONTEXT_SERVICE_ACTIVE 0x10000000L
#define CONTEXT_EXCEPTION_REQUEST 0x40000000L
#define CONTEXT_EXCEPTION_REPORTING 0x80000000L
#endif
#define INITIAL_MXCSR 0x1f80
#define INITIAL_FPCSR 0x027f
typedef XSAVE_FORMAT XMM_SAVE_AREA32,*PXMM_SAVE_AREA32;
typedef struct DECLSPEC_ALIGN(16) DECLSPEC_NOINITALL _CONTEXT{
DWORD64 P1Home;
DWORD64 P2Home;
DWORD64 P3Home;
DWORD64 P4Home;
DWORD64 P5Home;
DWORD64 P6Home;
DWORD ContextFlags;
DWORD MxCsr;
WORD SegCs;
WORD SegDs;
WORD SegEs;
WORD SegFs;
WORD SegGs;
WORD SegSs;
DWORD EFlags;
DWORD64 Dr0;
DWORD64 Dr1;
DWORD64 Dr2;
DWORD64 Dr3;
DWORD64 Dr6;
DWORD64 Dr7;
DWORD64 Rax;
DWORD64 Rcx;
DWORD64 Rdx;
DWORD64 Rbx;
DWORD64 Rsp;
DWORD64 Rbp;
DWORD64 Rsi;
DWORD64 Rdi;
DWORD64 R8;
DWORD64 R9;
DWORD64 R10;
DWORD64 R11;
DWORD64 R12;
DWORD64 R13;
DWORD64 R14;
DWORD64 R15;
DWORD64 Rip;
union{
XMM_SAVE_AREA32 FltSave;
struct{
M128A Header[2];
M128A Legacy[8];
M128A Xmm0;
M128A Xmm1;
M128A Xmm2;
M128A Xmm3;
M128A Xmm4;
M128A Xmm5;
M128A Xmm6;
M128A Xmm7;
M128A Xmm8;
M128A Xmm9;
M128A Xmm10;
M128A Xmm11;
M128A Xmm12;
M128A Xmm13;
M128A Xmm14;
M128A Xmm15;
}DUMMYSTRUCTNAME;
}DUMMYUNIONNAME;
M128A VectorRegister[26];
DWORD64 VectorControl;
DWORD64 DebugControl;
DWORD64 LastBranchToRip;
DWORD64 LastBranchFromRip;
DWORD64 LastExceptionToRip;
DWORD64 LastExceptionFromRip;
}CONTEXT,*PCONTEXT;
typedef struct _IMAGE_RUNTIME_FUNCTION_ENTRY RUNTIME_FUNCTION,*PRUNTIME_FUNCTION;
typedef SCOPE_TABLE_AMD64 SCOPE_TABLE,*PSCOPE_TABLE;
#define RUNTIME_FUNCTION_INDIRECT 0x1
#define UNW_FLAG_NHANDLER 0x0
#define UNW_FLAG_EHANDLER 0x1
#define UNW_FLAG_UHANDLER 0x2
#define UNW_FLAG_CHAININFO 0x4
#define UNW_FLAG_NO_EPILOGUE 0x80000000UL
#define UNWIND_CHAIN_LIMIT 32
#define UNWIND_HISTORY_TABLE_SIZE 12
typedef struct _UNWIND_HISTORY_TABLE_ENTRY{
DWORD64 ImageBase;
PRUNTIME_FUNCTION FunctionEntry;
}UNWIND_HISTORY_TABLE_ENTRY,*PUNWIND_HISTORY_TABLE_ENTRY;
typedef struct _UNWIND_HISTORY_TABLE{
DWORD Count;
BYTE LocalHint;
BYTE GlobalHint;
BYTE Search;
BYTE Once;
DWORD64 LowAddress;
DWORD64 HighAddress;
UNWIND_HISTORY_TABLE_ENTRY Entry[UNWIND_HISTORY_TABLE_SIZE];
}UNWIND_HISTORY_TABLE,*PUNWIND_HISTORY_TABLE;
typedef
_Function_class_(GET_RUNTIME_FUNCTION_CALLBACK)
PRUNTIME_FUNCTION
GET_RUNTIME_FUNCTION_CALLBACK(
_In_ DWORD64 ControlPc,
_In_opt_ PVOID Context);
typedef GET_RUNTIME_FUNCTION_CALLBACK* PGET_RUNTIME_FUNCTION_CALLBACK;
typedef
_Function_class_(OUT_OF_PROCESS_FUNCTION_TABLE_CALLBACK)
DWORD
OUT_OF_PROCESS_FUNCTION_TABLE_CALLBACK(
_In_ HANDLE Process,
_In_ PVOID TableAddress,
_Out_ PDWORD Entries,
_Out_ PRUNTIME_FUNCTION* Functions);
typedef OUT_OF_PROCESS_FUNCTION_TABLE_CALLBACK* POUT_OF_PROCESS_FUNCTION_TABLE_CALLBACK;
#define OUT_OF_PROCESS_FUNCTION_TABLE_CALLBACK_EXPORT_NAME \
"OutOfProcessFunctionTableCallback"
typedef struct _DISPATCHER_CONTEXT{
DWORD64 ControlPc;
DWORD64 ImageBase;
PRUNTIME_FUNCTION FunctionEntry;
DWORD64 EstablisherFrame;
DWORD64 TargetIp;
PCONTEXT ContextRecord;
PEXCEPTION_ROUTINE LanguageHandler;
PVOID HandlerData;
PUNWIND_HISTORY_TABLE HistoryTable;
DWORD ScopeIndex;
DWORD Fill0;
}DISPATCHER_CONTEXT,*PDISPATCHER_CONTEXT;
struct _EXCEPTION_POINTERS;
typedef
LONG
(*PEXCEPTION_FILTER) (
struct _EXCEPTION_POINTERS* ExceptionPointers,
PVOID EstablisherFrame);
typedef
VOID
(*PTERMINATION_HANDLER) (
BOOLEAN AbnormalTermination,
PVOID EstablisherFrame);
typedef struct _KNONVOLATILE_CONTEXT_POINTERS{
union{
PM128A FloatingContext[16];
struct{
PM128A Xmm0;
PM128A Xmm1;
PM128A Xmm2;
PM128A Xmm3;
PM128A Xmm4;
PM128A Xmm5;
PM128A Xmm6;
PM128A Xmm7;
PM128A Xmm8;
PM128A Xmm9;
PM128A Xmm10;
PM128A Xmm11;
PM128A Xmm12;
PM128A Xmm13;
PM128A Xmm14;
PM128A Xmm15;
}DUMMYSTRUCTNAME;
}DUMMYUNIONNAME;
union{
PDWORD64 IntegerContext[16];
struct{
PDWORD64 Rax;
PDWORD64 Rcx;
PDWORD64 Rdx;
PDWORD64 Rbx;
PDWORD64 Rsp;
PDWORD64 Rbp;
PDWORD64 Rsi;
PDWORD64 Rdi;
PDWORD64 R8;
PDWORD64 R9;
PDWORD64 R10;
PDWORD64 R11;
PDWORD64 R12;
PDWORD64 R13;
PDWORD64 R14;
PDWORD64 R15;
}DUMMYSTRUCTNAME;
}DUMMYUNIONNAME2;
}KNONVOLATILE_CONTEXT_POINTERS,*PKNONVOLATILE_CONTEXT_POINTERS;
#endif
typedef struct _SCOPE_TABLE_ARM{
DWORD Count;
struct{
DWORD BeginAddress;
DWORD EndAddress;
DWORD HandlerAddress;
DWORD JumpTarget;
}ScopeRecord[1];
}SCOPE_TABLE_ARM,*PSCOPE_TABLE_ARM;
typedef struct _SCOPE_TABLE_ARM64{
DWORD Count;
struct{
DWORD BeginAddress;
DWORD EndAddress;
DWORD HandlerAddress;
DWORD JumpTarget;
}ScopeRecord[1];
}SCOPE_TABLE_ARM64,*PSCOPE_TABLE_ARM64;
typedef struct _KNONVOLATILE_CONTEXT_POINTERS_ARM64{
PDWORD64 X19;
PDWORD64 X20;
PDWORD64 X21;
PDWORD64 X22;
PDWORD64 X23;
PDWORD64 X24;
PDWORD64 X25;
PDWORD64 X26;
PDWORD64 X27;
PDWORD64 X28;
PDWORD64 Fp;
PDWORD64 Lr;
PDWORD64 D8;
PDWORD64 D9;
PDWORD64 D10;
PDWORD64 D11;
PDWORD64 D12;
PDWORD64 D13;
PDWORD64 D14;
PDWORD64 D15;
}KNONVOLATILE_CONTEXT_POINTERS_ARM64,*PKNONVOLATILE_CONTEXT_POINTERS_ARM64;
#ifdef __cplusplus
extern "C"{
#endif
#if !defined(_DBGRAISEASSERTIONFAILURE_)&&!defined(RC_INVOKED)&&!defined(MIDL_PASS)
#define _DBGRAISEASSERTIONFAILURE_
#if defined(_PREFAST_)
__analysis_noreturn
FORCEINLINE
VOID
DbgRaiseAssertionFailure(
VOID);
#endif
#if defined(_AMD64_)
#if defined(_M_AMD64)
VOID
__int2c(
VOID);
#pragma intrinsic(__int2c)
#if !defined(_PREFAST_)
#define DbgRaiseAssertionFailure() __int2c()
#endif
#endif
#elif defined(_IA64_)
#if defined(_M_IA64)
void
__break(
_In_ int StIIM);
#pragma intrinsic (__break)
#define BREAK_DEBUG_BASE 0x080000
#define ASSERT_BREAKPOINT (BREAK_DEBUG_BASE+3)
#if !defined(_PREFAST_)
#define DbgRaiseAssertionFailure() __break(ASSERT_BREAKPOINT)
#endif
#endif
#endif
#endif
#ifdef __cplusplus
}
#endif
#ifndef _LDT_ENTRY_DEFINED
#define _LDT_ENTRY_DEFINED
typedef struct _LDT_ENTRY{
WORD LimitLow;
WORD BaseLow;
union{
struct{
BYTE BaseMid;
BYTE Flags1;
BYTE Flags2;
BYTE BaseHi;
}Bytes;
struct{
DWORD BaseMid:8;
DWORD Type:5;
DWORD Dpl:2;
DWORD Pres:1;
DWORD LimitHi:4;
DWORD Sys:1;
DWORD Reserved_0:1;
DWORD Default_Big:1;
DWORD Granularity:1;
DWORD BaseHi:8;
}Bits;
}HighWord;
}LDT_ENTRY,*PLDT_ENTRY;
#endif
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#if !defined(RC_INVOKED)&&!defined(MIDL_PASS)
#if ((defined(_M_AMD64)||defined(_M_IX86))&&!defined(_M_HYBRID_X86_ARM64))||defined(_M_CEE_PURE)
#ifdef __cplusplus
extern "C"{
#endif
FORCEINLINE
CHAR
ReadAcquire8(
_In_ _Interlocked_operand_ CHAR const volatile* Source){
CHAR Value;
Value=*Source;
return Value;}
FORCEINLINE
CHAR
ReadNoFence8(
_In_ _Interlocked_operand_ CHAR const volatile* Source){
CHAR Value;
Value=*Source;
return Value;}
FORCEINLINE
VOID
WriteRelease8(
_Out_ _Interlocked_operand_ CHAR volatile* Destination,
_In_ CHAR Value){
*Destination=Value;
return;}
FORCEINLINE
VOID
WriteNoFence8(
_Out_ _Interlocked_operand_ CHAR volatile* Destination,
_In_ CHAR Value){
*Destination=Value;
return;}
FORCEINLINE
SHORT
ReadAcquire16(
_In_ _Interlocked_operand_ SHORT const volatile* Source){
SHORT Value;
Value=*Source;
return Value;}
FORCEINLINE
SHORT
ReadNoFence16(
_In_ _Interlocked_operand_ SHORT const volatile* Source){
SHORT Value;
Value=*Source;
return Value;}
FORCEINLINE
VOID
WriteRelease16(
_Out_ _Interlocked_operand_ SHORT volatile* Destination,
_In_ SHORT Value){
*Destination=Value;
return;}
FORCEINLINE
VOID
WriteNoFence16(
_Out_ _Interlocked_operand_ SHORT volatile* Destination,
_In_ SHORT Value){
*Destination=Value;
return;}
FORCEINLINE
LONG
ReadAcquire(
_In_ _Interlocked_operand_ LONG const volatile* Source){
LONG Value;
Value=*Source;
return Value;}
CFORCEINLINE
LONG
ReadNoFence(
_In_ _Interlocked_operand_ LONG const volatile* Source){
LONG Value;
Value=*Source;
return Value;}
CFORCEINLINE
VOID
WriteRelease(
_Out_ _Interlocked_operand_ LONG volatile* Destination,
_In_ LONG Value){
*Destination=Value;
return;}
FORCEINLINE
VOID
WriteNoFence(
_Out_ _Interlocked_operand_ LONG volatile* Destination,
_In_ LONG Value){
*Destination=Value;
return;}
FORCEINLINE
LONG64
ReadAcquire64(
_In_ _Interlocked_operand_ LONG64 const volatile* Source){
LONG64 Value;
Value=*Source;
return Value;}
CFORCEINLINE
LONG64
ReadNoFence64(
_In_ _Interlocked_operand_ LONG64 const volatile* Source){
LONG64 Value;
Value=*Source;
return Value;}
CFORCEINLINE
VOID
WriteRelease64(
_Out_ _Interlocked_operand_ LONG64 volatile* Destination,
_In_ LONG64 Value){
*Destination=Value;
return;}
FORCEINLINE
VOID
WriteNoFence64(
_Out_ _Interlocked_operand_ LONG64 volatile* Destination,
_In_ LONG64 Value){
*Destination=Value;
return;}
#ifdef __cplusplus
}
#endif
#endif
FORCEINLINE
CHAR
ReadRaw8(
_In_ _Interlocked_operand_ CHAR const volatile* Source){
CHAR Value;
Value=*(CHAR*)Source;
return Value;}
FORCEINLINE
VOID
WriteRaw8(
_Out_ _Interlocked_operand_ CHAR volatile* Destination,
_In_ CHAR Value){
*(CHAR*)Destination=Value;
return;}
FORCEINLINE
SHORT
ReadRaw16(
_In_ _Interlocked_operand_ SHORT const volatile* Source){
SHORT Value;
Value=*(SHORT*)Source;
return Value;}
FORCEINLINE
VOID
WriteRaw16(
_Out_ _Interlocked_operand_ SHORT volatile* Destination,
_In_ SHORT Value){
*(SHORT*)Destination=Value;
return;}
FORCEINLINE
LONG
ReadRaw(
_In_ _Interlocked_operand_ LONG const volatile* Source){
LONG Value;
Value=*(LONG*)Source;
return Value;}
CFORCEINLINE
VOID
WriteRaw(
_Out_ _Interlocked_operand_ LONG volatile* Destination,
_In_ LONG Value){
*(LONG*)Destination=Value;
return;}
FORCEINLINE
LONG64
ReadRaw64(
_In_ _Interlocked_operand_ LONG64 const volatile* Source){
LONG64 Value;
Value=*(LONG64*)Source;
return Value;}
FORCEINLINE
VOID
WriteRaw64(
_Out_ _Interlocked_operand_ LONG64 volatile* Destination,
_In_ LONG64 Value){
*(LONG64*)Destination=Value;
return;}
FORCEINLINE
BYTE
ReadUCharAcquire(
_In_ _Interlocked_operand_ BYTE const volatile* Source){
return (BYTE)ReadAcquire8((PCHAR)Source);}
FORCEINLINE
BYTE
ReadUCharNoFence(
_In_ _Interlocked_operand_ BYTE const volatile* Source){
return (BYTE)ReadNoFence8((PCHAR)Source);}
FORCEINLINE
BYTE
ReadBooleanAcquire(
_In_ _Interlocked_operand_ BOOLEAN const volatile* Source){
return (BOOLEAN)ReadAcquire8((PCHAR)Source);}
FORCEINLINE
BYTE
ReadBooleanNoFence(
_In_ _Interlocked_operand_ BOOLEAN const volatile* Source){
return (BOOLEAN)ReadNoFence8((PCHAR)Source);}
FORCEINLINE
BYTE
ReadUCharRaw(
_In_ _Interlocked_operand_ BYTE const volatile* Source){
return (BYTE)ReadRaw8((PCHAR)Source);}
FORCEINLINE
VOID
WriteUCharRelease(
_Out_ _Interlocked_operand_ BYTE volatile* Destination,
_In_ BYTE Value){
WriteRelease8((PCHAR)Destination,(CHAR)Value);
return;}
FORCEINLINE
VOID
WriteUCharNoFence(
_Out_ _Interlocked_operand_ BYTE volatile* Destination,
_In_ BYTE Value){
WriteNoFence8((PCHAR)Destination,(CHAR)Value);
return;}
FORCEINLINE
VOID
WriteBooleanRelease(
_Out_ _Interlocked_operand_ BOOLEAN volatile* Destination,
_In_ BOOLEAN Value){
WriteRelease8((PCHAR)Destination,(CHAR)Value);
return;}
FORCEINLINE
VOID
WriteBooleanNoFence(
_Out_ _Interlocked_operand_ BOOLEAN volatile* Destination,
_In_ BOOLEAN Value){
WriteNoFence8((PCHAR)Destination,(CHAR)Value);
return;}
FORCEINLINE
VOID
WriteUCharRaw(
_Out_ _Interlocked_operand_ BYTE volatile* Destination,
_In_ BYTE Value){
WriteRaw8((PCHAR)Destination,(CHAR)Value);
return;}
FORCEINLINE
WORD
ReadUShortAcquire(
_In_ _Interlocked_operand_ WORD const volatile* Source){
return (WORD)ReadAcquire16((PSHORT)Source);}
FORCEINLINE
WORD
ReadUShortNoFence(
_In_ _Interlocked_operand_ WORD const volatile* Source){
return (WORD)ReadNoFence16((PSHORT)Source);}
FORCEINLINE
WORD
ReadUShortRaw(
_In_ _Interlocked_operand_ WORD const volatile* Source){
return (WORD)ReadRaw16((PSHORT)Source);}
FORCEINLINE
VOID
WriteUShortRelease(
_Out_ _Interlocked_operand_ WORD volatile* Destination,
_In_ WORD Value){
WriteRelease16((PSHORT)Destination,(SHORT)Value);
return;}
FORCEINLINE
VOID
WriteUShortNoFence(
_Out_ _Interlocked_operand_ WORD volatile* Destination,
_In_ WORD Value){
WriteNoFence16((PSHORT)Destination,(SHORT)Value);
return;}
FORCEINLINE
VOID
WriteUShortRaw(
_Out_ _Interlocked_operand_ WORD volatile* Destination,
_In_ WORD Value){
WriteRaw16((PSHORT)Destination,(SHORT)Value);
return;}
FORCEINLINE
DWORD
ReadULongAcquire(
_In_ _Interlocked_operand_ DWORD const volatile* Source){
return (DWORD)ReadAcquire((PLONG)Source);}
FORCEINLINE
DWORD
ReadULongNoFence(
_In_ _Interlocked_operand_ DWORD const volatile* Source){
return (DWORD)ReadNoFence((PLONG)Source);}
FORCEINLINE
DWORD
ReadULongRaw(
_In_ _Interlocked_operand_ DWORD const volatile* Source){
return (DWORD)ReadRaw((PLONG)Source);}
CFORCEINLINE
VOID
WriteULongRelease(
_Out_ _Interlocked_operand_ DWORD volatile* Destination,
_In_ DWORD Value){
WriteRelease((PLONG)Destination,(LONG)Value);
return;}
FORCEINLINE
VOID
WriteULongNoFence(
_Out_ _Interlocked_operand_ DWORD volatile* Destination,
_In_ DWORD Value){
WriteNoFence((PLONG)Destination,(LONG)Value);
return;}
FORCEINLINE
VOID
WriteULongRaw(
_Out_ _Interlocked_operand_ DWORD volatile* Destination,
_In_ DWORD Value){
WriteRaw((PLONG)Destination,(LONG)Value);
return;}
FORCEINLINE
DWORD64
ReadULong64Acquire(
_In_ _Interlocked_operand_ DWORD64 const volatile* Source){
return (DWORD64)ReadAcquire64((PLONG64)Source);}
FORCEINLINE
DWORD64
ReadULong64NoFence(
_In_ _Interlocked_operand_ DWORD64 const volatile* Source){
return (DWORD64)ReadNoFence64((PLONG64)Source);}
FORCEINLINE
DWORD64
ReadULong64Raw(
_In_ _Interlocked_operand_ DWORD64 const volatile* Source){
return (DWORD64)ReadRaw64((PLONG64)Source);}
CFORCEINLINE
VOID
WriteULong64Release(
_Out_ _Interlocked_operand_ DWORD64 volatile* Destination,
_In_ DWORD64 Value){
WriteRelease64((PLONG64)Destination,(LONG64)Value);
return;}
FORCEINLINE
VOID
WriteULong64NoFence(
_Out_ _Interlocked_operand_ DWORD64 volatile* Destination,
_In_ DWORD64 Value){
WriteNoFence64((PLONG64)Destination,(LONG64)Value);
return;}
FORCEINLINE
VOID
WriteULong64Raw(
_Out_ _Interlocked_operand_ DWORD64 volatile* Destination,
_In_ DWORD64 Value){
WriteRaw64((PLONG64)Destination,(LONG64)Value);
return;}
#define ReadSizeTAcquire ReadULongPtrAcquire
#define ReadSizeTNoFence ReadULongPtrNoFence
#define ReadSizeTRaw ReadULongPtrRaw
#define WriteSizeTRelease WriteULongPtrRelease
#define WriteSizeTNoFence WriteULongPtrNoFence
#define WriteSizeTRaw WriteULongPtrRaw
#if !defined(_WIN64)
FORCEINLINE
PVOID
ReadPointerAcquire(
_In_ _Interlocked_operand_ PVOID const volatile* Source){
return (PVOID)ReadAcquire((PLONG)Source);}
CFORCEINLINE
PVOID
ReadPointerNoFence(
_In_ _Interlocked_operand_ PVOID const volatile* Source){
return (PVOID)ReadNoFence((PLONG)Source);}
FORCEINLINE
PVOID
ReadPointerRaw(
_In_ _Interlocked_operand_ PVOID const volatile* Source){
return (PVOID)ReadRaw((PLONG)Source);}
CFORCEINLINE
VOID
WritePointerRelease(
_Out_ _Interlocked_operand_ PVOID volatile* Destination,
_In_ PVOID Value){
WriteRelease((PLONG)Destination,(LONG)Value);
return;}
FORCEINLINE
VOID
WritePointerNoFence(
_Out_ _Interlocked_operand_ PVOID volatile* Destination,
_In_opt_ PVOID Value){
WriteNoFence((PLONG)Destination,(LONG)Value);
return;}
FORCEINLINE
VOID
WritePointerRaw(
_Out_ _Interlocked_operand_ PVOID volatile* Destination,
_In_opt_ PVOID Value){
WriteRaw((PLONG)Destination,(LONG)Value);
return;}
#define ReadLongPtrAcquire ReadAcquire
#define ReadLongPtrNoFence ReadNoFence
#define ReadLongPtrRaw ReadRaw
#define WriteLongPtrRelease WriteRelease
#define WriteLongPtrNoFence WriteNoFence
#define WriteLongPtrRaw WriteRaw
#define ReadULongPtrAcquire ReadULongAcquire
#define ReadULongPtrNoFence ReadULongNoFence
#define ReadULongPtrRaw ReadULongRaw
#define WriteULongPtrRelease WriteULongRelease
#define WriteULongPtrNoFence WriteULongNoFence
#define WriteULongPtrRaw WriteULongRaw
#else
FORCEINLINE
PVOID
ReadPointerAcquire(
_In_ _Interlocked_operand_ PVOID const volatile* Source){
return (PVOID)ReadAcquire64((PLONG64)Source);}
CFORCEINLINE
PVOID
ReadPointerNoFence(
_In_ _Interlocked_operand_ PVOID const volatile* Source){
return (PVOID)ReadNoFence64((PLONG64)Source);}
FORCEINLINE
PVOID
ReadPointerRaw(
_In_ _Interlocked_operand_ PVOID const volatile* Source){
return (PVOID)ReadRaw64((PLONG64)Source);}
FORCEINLINE
VOID
WritePointerRelease(
_Out_ _Interlocked_operand_ PVOID volatile* Destination,
_In_ PVOID Value){
WriteRelease64((PLONG64)Destination,(LONG64)Value);
return;}
FORCEINLINE
VOID
WritePointerNoFence(
_Out_ _Interlocked_operand_ PVOID volatile* Destination,
_In_ PVOID Value){
WriteNoFence64((PLONG64)Destination,(LONG64)Value);
return;}
FORCEINLINE
VOID
WritePointerRaw(
_Out_ _Interlocked_operand_ PVOID volatile* Destination,
_In_ PVOID Value){
WriteRaw64((PLONG64)Destination,(LONG64)Value);
return;}
#define ReadLongPtrAcquire ReadAcquire64
#define ReadLongPtrNoFence ReadNoFence64
#define ReadLongPtrRaw ReadRaw64
#define WriteLongPtrRelease WriteRelease64
#define WriteLongPtrNoFence WriteNoFence64
#define WriteLongPtrRaw WriteRaw64
#define ReadULongPtrAcquire ReadULong64Acquire
#define ReadULongPtrNoFence ReadULong64NoFence
#define ReadULongPtrRaw ReadULong64Raw
#define WriteULongPtrRelease WriteULong64Release
#define WriteULongPtrNoFence WriteULong64NoFence
#define WriteULongPtrRaw WriteULong64Raw
#endif
#endif
#endif
#if _MSC_VER >= 1200
#pragma warning(push)
#pragma warning(disable:4214)
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#endif
#if !defined(RC_INVOKED)
#define WOW64_CONTEXT_i386 0x010000
#define WOW64_CONTEXT_i486 0x010000
#define WOW64_CONTEXT_CONTROL (WOW64_CONTEXT_i386 | 0x01L)
#define WOW64_CONTEXT_INTEGER (WOW64_CONTEXT_i386 | 0x02L)
#define WOW64_CONTEXT_SEGMENTS (WOW64_CONTEXT_i386 | 0x04L)
#define WOW64_CONTEXT_FLOATING_POINT (WOW64_CONTEXT_i386 | 0x08L)
#define WOW64_CONTEXT_DEBUG_REGISTERS (WOW64_CONTEXT_i386 | 0x010L)
#define WOW64_CONTEXT_EXTENDED_REGISTERS (WOW64_CONTEXT_i386 | 0x020L)
#define WOW64_CONTEXT_FULL (WOW64_CONTEXT_CONTROL | WOW64_CONTEXT_INTEGER | WOW64_CONTEXT_SEGMENTS)
#define WOW64_CONTEXT_ALL (WOW64_CONTEXT_CONTROL | WOW64_CONTEXT_INTEGER | WOW64_CONTEXT_SEGMENTS | \
WOW64_CONTEXT_FLOATING_POINT | WOW64_CONTEXT_DEBUG_REGISTERS | \
WOW64_CONTEXT_EXTENDED_REGISTERS)
#define WOW64_CONTEXT_XSTATE (WOW64_CONTEXT_i386 | 0x040L)
#define WOW64_CONTEXT_EXCEPTION_ACTIVE 0x08000000
#define WOW64_CONTEXT_SERVICE_ACTIVE 0x10000000
#define WOW64_CONTEXT_EXCEPTION_REQUEST 0x40000000
#define WOW64_CONTEXT_EXCEPTION_REPORTING 0x80000000
#endif
#define WOW64_SIZE_OF_80387_REGISTERS 80
#define WOW64_MAXIMUM_SUPPORTED_EXTENSION 512
typedef struct _WOW64_FLOATING_SAVE_AREA{
DWORD ControlWord;
DWORD StatusWord;
DWORD TagWord;
DWORD ErrorOffset;
DWORD ErrorSelector;
DWORD DataOffset;
DWORD DataSelector;
BYTE RegisterArea[WOW64_SIZE_OF_80387_REGISTERS];
DWORD Cr0NpxState;
}WOW64_FLOATING_SAVE_AREA;
typedef WOW64_FLOATING_SAVE_AREA* PWOW64_FLOATING_SAVE_AREA;
#include "pshpack4.h"
typedef struct _WOW64_CONTEXT{
DWORD ContextFlags;
DWORD Dr0;
DWORD Dr1;
DWORD Dr2;
DWORD Dr3;
DWORD Dr6;
DWORD Dr7;
WOW64_FLOATING_SAVE_AREA FloatSave;
DWORD SegGs;
DWORD SegFs;
DWORD SegEs;
DWORD SegDs;
DWORD Edi;
DWORD Esi;
DWORD Ebx;
DWORD Edx;
DWORD Ecx;
DWORD Eax;
DWORD Ebp;
DWORD Eip;
DWORD SegCs;
DWORD EFlags;
DWORD Esp;
DWORD SegSs;
BYTE ExtendedRegisters[WOW64_MAXIMUM_SUPPORTED_EXTENSION];
}WOW64_CONTEXT;
typedef WOW64_CONTEXT* PWOW64_CONTEXT;
#include "poppack.h"
typedef struct _WOW64_LDT_ENTRY{
WORD LimitLow;
WORD BaseLow;
union{
struct{
BYTE BaseMid;
BYTE Flags1;
BYTE Flags2;
BYTE BaseHi;
}Bytes;
struct{
DWORD BaseMid:8;
DWORD Type:5;
DWORD Dpl:2;
DWORD Pres:1;
DWORD LimitHi:4;
DWORD Sys:1;
DWORD Reserved_0:1;
DWORD Default_Big:1;
DWORD Granularity:1;
DWORD BaseHi:8;
}Bits;
}HighWord;
}WOW64_LDT_ENTRY,*PWOW64_LDT_ENTRY;
typedef struct _WOW64_DESCRIPTOR_TABLE_ENTRY{
DWORD Selector;
WOW64_LDT_ENTRY Descriptor;
}WOW64_DESCRIPTOR_TABLE_ENTRY,*PWOW64_DESCRIPTOR_TABLE_ENTRY;
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif
#define EXCEPTION_NONCONTINUABLE 0x1
#define EXCEPTION_UNWINDING 0x2
#define EXCEPTION_EXIT_UNWIND 0x4
#define EXCEPTION_STACK_INVALID 0x8
#define EXCEPTION_NESTED_CALL 0x10
#define EXCEPTION_TARGET_UNWIND 0x20
#define EXCEPTION_COLLIDED_UNWIND 0x40
#define EXCEPTION_UNWIND (EXCEPTION_UNWINDING | EXCEPTION_EXIT_UNWIND | \
EXCEPTION_TARGET_UNWIND | EXCEPTION_COLLIDED_UNWIND)
#define IS_UNWINDING(Flag) ((Flag & EXCEPTION_UNWIND) != 0)
#define IS_DISPATCHING(Flag) ((Flag & EXCEPTION_UNWIND)==0)
#define IS_TARGET_UNWIND(Flag) (Flag & EXCEPTION_TARGET_UNWIND)
#define EXCEPTION_MAXIMUM_PARAMETERS 15
typedef struct _EXCEPTION_RECORD{
DWORD ExceptionCode;
DWORD ExceptionFlags;
struct _EXCEPTION_RECORD* ExceptionRecord;
PVOID ExceptionAddress;
DWORD NumberParameters;
ULONG_PTR ExceptionInformation[EXCEPTION_MAXIMUM_PARAMETERS];
}EXCEPTION_RECORD;
typedef EXCEPTION_RECORD* PEXCEPTION_RECORD;
typedef struct _EXCEPTION_RECORD32{
DWORD ExceptionCode;
DWORD ExceptionFlags;
DWORD ExceptionRecord;
DWORD ExceptionAddress;
DWORD NumberParameters;
DWORD ExceptionInformation[EXCEPTION_MAXIMUM_PARAMETERS];
}EXCEPTION_RECORD32,*PEXCEPTION_RECORD32;
typedef struct _EXCEPTION_RECORD64{
DWORD ExceptionCode;
DWORD ExceptionFlags;
DWORD64 ExceptionRecord;
DWORD64 ExceptionAddress;
DWORD NumberParameters;
DWORD __unusedAlignment;
DWORD64 ExceptionInformation[EXCEPTION_MAXIMUM_PARAMETERS];
}EXCEPTION_RECORD64,*PEXCEPTION_RECORD64;
typedef struct _EXCEPTION_POINTERS{
PEXCEPTION_RECORD ExceptionRecord;
PCONTEXT ContextRecord;
}EXCEPTION_POINTERS,*PEXCEPTION_POINTERS;
#if defined(_IA64_)
NTSYSAPI
VOID
NTAPI
RtlUnwind2(
_In_opt_ FRAME_POINTERS TargetFrame,
_In_opt_ PVOID TargetIp,
_In_opt_ PEXCEPTION_RECORD ExceptionRecord,
_In_ PVOID ReturnValue,
_In_ PCONTEXT ContextRecord);
#endif
typedef PVOID PACCESS_TOKEN;
typedef PVOID PSECURITY_DESCRIPTOR;
typedef PVOID PSID;
typedef PVOID PCLAIMS_BLOB;
typedef DWORD ACCESS_MASK;
typedef ACCESS_MASK* PACCESS_MASK;
#define DELETE (0x010000L)
#define READ_CONTROL (0x020000L)
#define WRITE_DAC (0x040000L)
#define WRITE_OWNER (0x080000L)
#define SYNCHRONIZE (0x0100000L)
#define STANDARD_RIGHTS_REQUIRED (0x0F0000L)
#define STANDARD_RIGHTS_READ (READ_CONTROL)
#define STANDARD_RIGHTS_WRITE (READ_CONTROL)
#define STANDARD_RIGHTS_EXECUTE (READ_CONTROL)
#define STANDARD_RIGHTS_ALL (0x01F0000L)
#define SPECIFIC_RIGHTS_ALL (0x0FFFFL)
#define ACCESS_SYSTEM_SECURITY (0x01000000L)
#define MAXIMUM_ALLOWED (0x02000000L)
#define GENERIC_READ (0x80000000L)
#define GENERIC_WRITE (0x40000000L)
#define GENERIC_EXECUTE (0x20000000L)
#define GENERIC_ALL (0x10000000L)
typedef struct _GENERIC_MAPPING{
ACCESS_MASK GenericRead;
ACCESS_MASK GenericWrite;
ACCESS_MASK GenericExecute;
ACCESS_MASK GenericAll;
}GENERIC_MAPPING;
typedef GENERIC_MAPPING* PGENERIC_MAPPING;
#include <pshpack4.h>
typedef struct _LUID_AND_ATTRIBUTES{
LUID Luid;
DWORD Attributes;
}LUID_AND_ATTRIBUTES,*PLUID_AND_ATTRIBUTES;
typedef LUID_AND_ATTRIBUTES LUID_AND_ATTRIBUTES_ARRAY[ANYSIZE_ARRAY];
typedef LUID_AND_ATTRIBUTES_ARRAY* PLUID_AND_ATTRIBUTES_ARRAY;
#include <poppack.h>
#ifndef SID_IDENTIFIER_AUTHORITY_DEFINED
#define SID_IDENTIFIER_AUTHORITY_DEFINED
typedef struct _SID_IDENTIFIER_AUTHORITY{
BYTE Value[6];
}SID_IDENTIFIER_AUTHORITY,*PSID_IDENTIFIER_AUTHORITY;
#endif
#ifndef SID_DEFINED
#define SID_DEFINED
typedef struct _SID{
BYTE Revision;
BYTE SubAuthorityCount;
SID_IDENTIFIER_AUTHORITY IdentifierAuthority;
#ifdef MIDL_PASS
[size_is(SubAuthorityCount)] DWORD SubAuthority[*];
#else
DWORD SubAuthority[ANYSIZE_ARRAY];
#endif
}SID,*PISID;
#endif
#define SID_REVISION (1)
#define SID_MAX_SUB_AUTHORITIES (15)
#define SID_RECOMMENDED_SUB_AUTHORITIES (1)
#ifndef MIDL_PASS
#define SECURITY_MAX_SID_SIZE \
(sizeof(SID)-sizeof(DWORD)+(SID_MAX_SUB_AUTHORITIES*sizeof(DWORD)))
#define SECURITY_SID_SIZE(SubAuthorityCount_) (sizeof(SID)-sizeof(DWORD)+\
(SubAuthorityCount_)*sizeof(DWORD))
#define SECURITY_MAX_SID_STRING_CHARACTERS \
(2+4+15+(11*SID_MAX_SUB_AUTHORITIES)+1)
typedef union _SE_SID{
SID Sid;
BYTE Buffer[SECURITY_MAX_SID_SIZE];
}SE_SID,*PSE_SID;
#endif
typedef enum _SID_NAME_USE{
SidTypeUser=1,
SidTypeGroup,
SidTypeDomain,
SidTypeAlias,
SidTypeWellKnownGroup,
SidTypeDeletedAccount,
SidTypeInvalid,
SidTypeUnknown,
SidTypeComputer,
SidTypeLabel,
SidTypeLogonSession
}SID_NAME_USE,*PSID_NAME_USE;
typedef struct _SID_AND_ATTRIBUTES{
#ifdef MIDL_PASS
PISID Sid;
#else
PSID Sid;
#endif
DWORD Attributes;
}SID_AND_ATTRIBUTES,*PSID_AND_ATTRIBUTES;
typedef SID_AND_ATTRIBUTES SID_AND_ATTRIBUTES_ARRAY[ANYSIZE_ARRAY];
typedef SID_AND_ATTRIBUTES_ARRAY* PSID_AND_ATTRIBUTES_ARRAY;
#define SID_HASH_SIZE 32
typedef ULONG_PTR SID_HASH_ENTRY,*PSID_HASH_ENTRY;
typedef struct _SID_AND_ATTRIBUTES_HASH{
DWORD SidCount;
PSID_AND_ATTRIBUTES SidAttr;
SID_HASH_ENTRY Hash[SID_HASH_SIZE];
}SID_AND_ATTRIBUTES_HASH,*PSID_AND_ATTRIBUTES_HASH;
typedef enum{
WinNullSid=0,
WinWorldSid=1,
WinLocalSid=2,
WinCreatorOwnerSid=3,
WinCreatorGroupSid=4,
WinCreatorOwnerServerSid=5,
WinCreatorGroupServerSid=6,
WinNtAuthoritySid=7,
WinDialupSid=8,
WinNetworkSid=9,
WinBatchSid=10,
WinInteractiveSid=11,
WinServiceSid=12,
WinAnonymousSid=13,
WinProxySid=14,
WinEnterpriseControllersSid=15,
WinSelfSid=16,
WinAuthenticatedUserSid=17,
WinRestrictedCodeSid=18,
WinTerminalServerSid=19,
WinRemoteLogonIdSid=20,
WinLogonIdsSid=21,
WinLocalSystemSid=22,
WinLocalServiceSid=23,
WinNetworkServiceSid=24,
WinBuiltinDomainSid=25,
WinBuiltinAdministratorsSid=26,
WinBuiltinUsersSid=27,
WinBuiltinGuestsSid=28,
WinBuiltinPowerUsersSid=29,
WinBuiltinAccountOperatorsSid=30,
WinBuiltinSystemOperatorsSid=31,
WinBuiltinPrintOperatorsSid=32,
WinBuiltinBackupOperatorsSid=33,
WinBuiltinReplicatorSid=34,
WinBuiltinPreWindows2000CompatibleAccessSid=35,
WinBuiltinRemoteDesktopUsersSid=36,
WinBuiltinNetworkConfigurationOperatorsSid=37,
WinAccountAdministratorSid=38,
WinAccountGuestSid=39,
WinAccountKrbtgtSid=40,
WinAccountDomainAdminsSid=41,
WinAccountDomainUsersSid=42,
WinAccountDomainGuestsSid=43,
WinAccountComputersSid=44,
WinAccountControllersSid=45,
WinAccountCertAdminsSid=46,
WinAccountSchemaAdminsSid=47,
WinAccountEnterpriseAdminsSid=48,
WinAccountPolicyAdminsSid=49,
WinAccountRasAndIasServersSid=50,
WinNTLMAuthenticationSid=51,
WinDigestAuthenticationSid=52,
WinSChannelAuthenticationSid=53,
WinThisOrganizationSid=54,
WinOtherOrganizationSid=55,
WinBuiltinIncomingForestTrustBuildersSid=56,
WinBuiltinPerfMonitoringUsersSid=57,
WinBuiltinPerfLoggingUsersSid=58,
WinBuiltinAuthorizationAccessSid=59,
WinBuiltinTerminalServerLicenseServersSid=60,
WinBuiltinDCOMUsersSid=61,
WinBuiltinIUsersSid=62,
WinIUserSid=63,
WinBuiltinCryptoOperatorsSid=64,
WinUntrustedLabelSid=65,
WinLowLabelSid=66,
WinMediumLabelSid=67,
WinHighLabelSid=68,
WinSystemLabelSid=69,
WinWriteRestrictedCodeSid=70,
WinCreatorOwnerRightsSid=71,
WinCacheablePrincipalsGroupSid=72,
WinNonCacheablePrincipalsGroupSid=73,
WinEnterpriseReadonlyControllersSid=74,
WinAccountReadonlyControllersSid=75,
WinBuiltinEventLogReadersGroup=76,
WinNewEnterpriseReadonlyControllersSid=77,
WinBuiltinCertSvcDComAccessGroup=78,
WinMediumPlusLabelSid=79,
WinLocalLogonSid=80,
WinConsoleLogonSid=81,
WinThisOrganizationCertificateSid=82,
WinApplicationPackageAuthoritySid=83,
WinBuiltinAnyPackageSid=84,
WinCapabilityInternetClientSid=85,
WinCapabilityInternetClientServerSid=86,
WinCapabilityPrivateNetworkClientServerSid=87,
WinCapabilityPicturesLibrarySid=88,
WinCapabilityVideosLibrarySid=89,
WinCapabilityMusicLibrarySid=90,
WinCapabilityDocumentsLibrarySid=91,
WinCapabilitySharedUserCertificatesSid=92,
WinCapabilityEnterpriseAuthenticationSid=93,
WinCapabilityRemovableStorageSid=94,
WinBuiltinRDSRemoteAccessServersSid=95,
WinBuiltinRDSEndpointServersSid=96,
WinBuiltinRDSManagementServersSid=97,
WinUserModeDriversSid=98,
WinBuiltinHyperVAdminsSid=99,
WinAccountCloneableControllersSid=100,
WinBuiltinAccessControlAssistanceOperatorsSid=101,
WinBuiltinRemoteManagementUsersSid=102,
WinAuthenticationAuthorityAssertedSid=103,
WinAuthenticationServiceAssertedSid=104,
WinLocalAccountSid=105,
WinLocalAccountAndAdministratorSid=106,
WinAccountProtectedUsersSid=107,
WinCapabilityAppointmentsSid=108,
WinCapabilityContactsSid=109,
WinAccountDefaultSystemManagedSid=110,
WinBuiltinDefaultSystemManagedGroupSid=111,
WinBuiltinStorageReplicaAdminsSid=112,
WinAccountKeyAdminsSid=113,
WinAccountEnterpriseKeyAdminsSid=114,
WinAuthenticationKeyTrustSid=115,
WinAuthenticationKeyPropertyMFASid=116,
WinAuthenticationKeyPropertyAttestationSid=117,
WinAuthenticationFreshKeyAuthSid=118,
WinBuiltinDeviceOwnersSid=119,
}WELL_KNOWN_SID_TYPE;
typedef struct _ACL{
BYTE AclRevision;
BYTE Sbz1;
WORD AclSize;
WORD AceCount;
WORD Sbz2;
}ACL;
typedef ACL* PACL;
typedef struct _ACE_HEADER{
BYTE AceType;
BYTE AceFlags;
WORD AceSize;
}ACE_HEADER;
typedef ACE_HEADER* PACE_HEADER;
#define TRUST_PROTECTED_FILTER_ACE_FLAG (0x40)
typedef enum _ACL_INFORMATION_CLASS{
AclRevisionInformation=1,
AclSizeInformation
}ACL_INFORMATION_CLASS;
#define SECURITY_DESCRIPTOR_REVISION (1)
#define SECURITY_DESCRIPTOR_REVISION1 (1)
#define SECURITY_DESCRIPTOR_MIN_LENGTH (sizeof(SECURITY_DESCRIPTOR))
typedef WORD SECURITY_DESCRIPTOR_CONTROL,*PSECURITY_DESCRIPTOR_CONTROL;
typedef struct _SECURITY_DESCRIPTOR_RELATIVE{
BYTE Revision;
BYTE Sbz1;
SECURITY_DESCRIPTOR_CONTROL Control;
DWORD Owner;
DWORD Group;
DWORD Sacl;
DWORD Dacl;
}SECURITY_DESCRIPTOR_RELATIVE,*PISECURITY_DESCRIPTOR_RELATIVE;
typedef struct _SECURITY_DESCRIPTOR{
BYTE Revision;
BYTE Sbz1;
SECURITY_DESCRIPTOR_CONTROL Control;
PSID Owner;
PSID Group;
PACL Sacl;
PACL Dacl;
}SECURITY_DESCRIPTOR,*PISECURITY_DESCRIPTOR;
typedef struct _SECURITY_OBJECT_AI_PARAMS{
DWORD Size;
DWORD ConstraintMask;
}SECURITY_OBJECT_AI_PARAMS,*PSECURITY_OBJECT_AI_PARAMS;
typedef struct _OBJECT_TYPE_LIST{
WORD Level;
WORD Sbz;
GUID* ObjectType;
}OBJECT_TYPE_LIST,*POBJECT_TYPE_LIST;
typedef enum _AUDIT_EVENT_TYPE{
AuditEventObjectAccess,
AuditEventDirectoryServiceAccess
}AUDIT_EVENT_TYPE,*PAUDIT_EVENT_TYPE;
#define AUDIT_ALLOW_NO_PRIVILEGE 0x1
#define ACCESS_DS_SOURCE_A "DS"
#define ACCESS_DS_SOURCE_W L"DS"
#define ACCESS_DS_OBJECT_TYPE_NAME_A "Directory Service Object"
#define ACCESS_DS_OBJECT_TYPE_NAME_W L"Directory Service Object"
#define SE_PRIVILEGE_ENABLED_BY_DEFAULT (0x01L)
#define SE_PRIVILEGE_ENABLED (0x02L)
#define SE_PRIVILEGE_REMOVED (0X00000004L)
#define SE_PRIVILEGE_USED_FOR_ACCESS (0x80000000L)
#define SE_PRIVILEGE_VALID_ATTRIBUTES (SE_PRIVILEGE_ENABLED_BY_DEFAULT | \
SE_PRIVILEGE_ENABLED | \
SE_PRIVILEGE_REMOVED | \
SE_PRIVILEGE_USED_FOR_ACCESS)
#define PRIVILEGE_SET_ALL_NECESSARY (1)
typedef struct _PRIVILEGE_SET{
DWORD PrivilegeCount;
DWORD Control;
LUID_AND_ATTRIBUTES Privilege[ANYSIZE_ARRAY];
}PRIVILEGE_SET,*PPRIVILEGE_SET;
typedef DWORD ACCESS_REASON;
typedef struct _ACCESS_REASONS{
ACCESS_REASON Data[32];
}ACCESS_REASONS,*PACCESS_REASONS;
typedef struct _SE_ACCESS_REPLY{
DWORD Size;
DWORD ResultListCount;
PACCESS_MASK GrantedAccess;
PDWORD AccessStatus;
PACCESS_REASONS AccessReason;
PPRIVILEGE_SET* Privileges;
}SE_ACCESS_REPLY,*PSE_ACCESS_REPLY;
typedef enum _SECURITY_IMPERSONATION_LEVEL{
SecurityAnonymous,
SecurityIdentification,
SecurityImpersonation,
SecurityDelegation
}SECURITY_IMPERSONATION_LEVEL,*PSECURITY_IMPERSONATION_LEVEL;
#define SECURITY_MAX_IMPERSONATION_LEVEL SecurityDelegation
#define SECURITY_MIN_IMPERSONATION_LEVEL SecurityAnonymous
#define DEFAULT_IMPERSONATION_LEVEL SecurityImpersonation
#define VALID_IMPERSONATION_LEVEL(L) (((L) >= SECURITY_MIN_IMPERSONATION_LEVEL)&&((L) <= SECURITY_MAX_IMPERSONATION_LEVEL))
typedef enum _TOKEN_TYPE{
TokenPrimary=1,
TokenImpersonation
}TOKEN_TYPE;
typedef TOKEN_TYPE* PTOKEN_TYPE;
typedef enum _TOKEN_ELEVATION_TYPE{
TokenElevationTypeDefault=1,
TokenElevationTypeFull,
TokenElevationTypeLimited,
}TOKEN_ELEVATION_TYPE,*PTOKEN_ELEVATION_TYPE;
typedef enum _TOKEN_INFORMATION_CLASS{
TokenUser=1,
TokenGroups,
TokenPrivileges,
TokenOwner,
TokenPrimaryGroup,
TokenDefaultDacl,
TokenSource,
TokenType,
TokenImpersonationLevel,
TokenStatistics,
TokenRestrictedSids,
TokenSessionId,
TokenGroupsAndPrivileges,
TokenSessionReference,
TokenSandBoxInert,
TokenAuditPolicy,
TokenOrigin,
TokenElevationType,
TokenLinkedToken,
TokenElevation,
TokenHasRestrictions,
TokenAccessInformation,
TokenVirtualizationAllowed,
TokenVirtualizationEnabled,
TokenIntegrityLevel,
TokenUIAccess,
TokenMandatoryPolicy,
TokenLogonSid,
TokenIsAppContainer,
TokenCapabilities,
TokenAppContainerSid,
TokenAppContainerNumber,
TokenUserClaimAttributes,
TokenDeviceClaimAttributes,
TokenRestrictedUserClaimAttributes,
TokenRestrictedDeviceClaimAttributes,
TokenDeviceGroups,
TokenRestrictedDeviceGroups,
TokenSecurityAttributes,
TokenIsRestricted,
TokenProcessTrustLevel,
TokenPrivateNameSpace,
TokenSingletonAttributes,
TokenBnoIsolation,
TokenChildProcessFlags,
TokenIsLessPrivilegedAppContainer,
TokenIsSandboxed,
TokenOriginatingProcessTrustLevel,
MaxTokenInfoClass
}TOKEN_INFORMATION_CLASS,*PTOKEN_INFORMATION_CLASS;
typedef struct _TOKEN_USER{
SID_AND_ATTRIBUTES User;
}TOKEN_USER,*PTOKEN_USER;
#ifndef MIDL_PASS
typedef struct _SE_TOKEN_USER{
union{
TOKEN_USER TokenUser;
SID_AND_ATTRIBUTES User;
}DUMMYUNIONNAME;
union{
SID Sid;
BYTE Buffer[SECURITY_MAX_SID_SIZE];
}DUMMYUNIONNAME2;
}SE_TOKEN_USER,PSE_TOKEN_USER;
#define TOKEN_USER_MAX_SIZE (sizeof(TOKEN_USER)+SECURITY_MAX_SID_SIZE)
#endif
typedef struct _TOKEN_GROUPS{
DWORD GroupCount;
#ifdef MIDL_PASS
[size_is(GroupCount)] SID_AND_ATTRIBUTES Groups[*];
#else
SID_AND_ATTRIBUTES Groups[ANYSIZE_ARRAY];
#endif
}TOKEN_GROUPS,*PTOKEN_GROUPS;
typedef struct _TOKEN_PRIVILEGES{
DWORD PrivilegeCount;
LUID_AND_ATTRIBUTES Privileges[ANYSIZE_ARRAY];
}TOKEN_PRIVILEGES,*PTOKEN_PRIVILEGES;
typedef struct _TOKEN_OWNER{
PSID Owner;
}TOKEN_OWNER,*PTOKEN_OWNER;
#ifndef MIDL_PASS
#define TOKEN_OWNER_MAX_SIZE (sizeof(TOKEN_OWNER)+SECURITY_MAX_SID_SIZE)
#endif
typedef struct _TOKEN_PRIMARY_GROUP{
PSID PrimaryGroup;
}TOKEN_PRIMARY_GROUP,*PTOKEN_PRIMARY_GROUP;
#define POLICY_AUDIT_SUBCATEGORY_COUNT (59)
typedef struct _TOKEN_AUDIT_POLICY{
BYTE PerUserPolicy[((POLICY_AUDIT_SUBCATEGORY_COUNT)>>1)+1];
}TOKEN_AUDIT_POLICY,*PTOKEN_AUDIT_POLICY;
#define TOKEN_SOURCE_LENGTH 8
typedef struct _TOKEN_SOURCE{
CHAR SourceName[TOKEN_SOURCE_LENGTH];
LUID SourceIdentifier;
}TOKEN_SOURCE,*PTOKEN_SOURCE;
typedef struct _TOKEN_STATISTICS{
LUID TokenId;
LUID AuthenticationId;
LARGE_INTEGER ExpirationTime;
TOKEN_TYPE TokenType;
SECURITY_IMPERSONATION_LEVEL ImpersonationLevel;
DWORD DynamicCharged;
DWORD DynamicAvailable;
DWORD GroupCount;
DWORD PrivilegeCount;
LUID ModifiedId;
}TOKEN_STATISTICS,*PTOKEN_STATISTICS;
typedef struct _TOKEN_CONTROL{
LUID TokenId;
LUID AuthenticationId;
LUID ModifiedId;
TOKEN_SOURCE TokenSource;
}TOKEN_CONTROL,*PTOKEN_CONTROL;
typedef struct _TOKEN_ORIGIN{
LUID OriginatingLogonSession;
}TOKEN_ORIGIN,*PTOKEN_ORIGIN;
typedef enum _MANDATORY_LEVEL{
MandatoryLevelUntrusted=0,
MandatoryLevelLow,
MandatoryLevelMedium,
MandatoryLevelHigh,
MandatoryLevelSystem,
MandatoryLevelSecureProcess,
MandatoryLevelCount
}MANDATORY_LEVEL,*PMANDATORY_LEVEL;
typedef struct _TOKEN_APPCONTAINER_INFORMATION{
PSID TokenAppContainer;
}TOKEN_APPCONTAINER_INFORMATION,*PTOKEN_APPCONTAINER_INFORMATION;
#ifndef MIDL_PASS
#define TOKEN_APPCONTAINER_SID_MAX_SIZE (sizeof(TOKEN_APPCONTAINER_INFORMATION)+SECURITY_MAX_SID_SIZE)
#endif
typedef struct _TOKEN_SID_INFORMATION{
PSID Sid;
}TOKEN_SID_INFORMATION,*PTOKEN_SID_INFORMATION;
typedef struct _TOKEN_BNO_ISOLATION_INFORMATION{
PWSTR IsolationPrefix;
BOOLEAN IsolationEnabled;
}TOKEN_BNO_ISOLATION_INFORMATION,*PTOKEN_BNO_ISOLATION_INFORMATION;
#define CLAIM_SECURITY_ATTRIBUTE_TYPE_STRING 0x03
typedef struct _CLAIM_SECURITY_ATTRIBUTE_FQBN_VALUE{
DWORD64 Version;
PWSTR Name;
}CLAIM_SECURITY_ATTRIBUTE_FQBN_VALUE,*PCLAIM_SECURITY_ATTRIBUTE_FQBN_VALUE;
typedef struct _CLAIM_SECURITY_ATTRIBUTE_OCTET_STRING_VALUE{
PVOID pValue;
DWORD ValueLength;
}CLAIM_SECURITY_ATTRIBUTE_OCTET_STRING_VALUE,
* PCLAIM_SECURITY_ATTRIBUTE_OCTET_STRING_VALUE;
#define CLAIM_SECURITY_ATTRIBUTE_TYPE_OCTET_STRING 0x10
#define CLAIM_SECURITY_ATTRIBUTE_CUSTOM_FLAGS 0xFFFF0000
typedef struct _CLAIM_SECURITY_ATTRIBUTE_V1{
PWSTR Name;
WORD ValueType;
WORD Reserved;
DWORD Flags;
DWORD ValueCount;
union{
PLONG64 pInt64;
PDWORD64 pUint64;
PWSTR* ppString;
PCLAIM_SECURITY_ATTRIBUTE_FQBN_VALUE pFqbn;
PCLAIM_SECURITY_ATTRIBUTE_OCTET_STRING_VALUE pOctetString;
}Values;
}CLAIM_SECURITY_ATTRIBUTE_V1,*PCLAIM_SECURITY_ATTRIBUTE_V1;
typedef struct _CLAIM_SECURITY_ATTRIBUTE_RELATIVE_V1{
DWORD Name;
WORD ValueType;
WORD Reserved;
DWORD Flags;
DWORD ValueCount;
union{
DWORD pInt64[ANYSIZE_ARRAY];
DWORD pUint64[ANYSIZE_ARRAY];
DWORD ppString[ANYSIZE_ARRAY];
DWORD pFqbn[ANYSIZE_ARRAY];
DWORD pOctetString[ANYSIZE_ARRAY];
}Values;
}CLAIM_SECURITY_ATTRIBUTE_RELATIVE_V1,*PCLAIM_SECURITY_ATTRIBUTE_RELATIVE_V1;
#define CLAIM_SECURITY_ATTRIBUTES_INFORMATION_VERSION_V1 1
#define CLAIM_SECURITY_ATTRIBUTES_INFORMATION_VERSION \
CLAIM_SECURITY_ATTRIBUTES_INFORMATION_VERSION_V1
typedef struct _CLAIM_SECURITY_ATTRIBUTES_INFORMATION{
WORD Version;
WORD Reserved;
DWORD AttributeCount;
union{
PCLAIM_SECURITY_ATTRIBUTE_V1 pAttributeV1;
}Attribute;
}CLAIM_SECURITY_ATTRIBUTES_INFORMATION,*PCLAIM_SECURITY_ATTRIBUTES_INFORMATION;
#define SECURITY_DYNAMIC_TRACKING (TRUE)
#define SECURITY_STATIC_TRACKING (FALSE)
typedef BOOLEAN SECURITY_CONTEXT_TRACKING_MODE,
* PSECURITY_CONTEXT_TRACKING_MODE;
typedef struct _SECURITY_QUALITY_OF_SERVICE{
DWORD Length;
SECURITY_IMPERSONATION_LEVEL ImpersonationLevel;
SECURITY_CONTEXT_TRACKING_MODE ContextTrackingMode;
BOOLEAN EffectiveOnly;
}SECURITY_QUALITY_OF_SERVICE,*PSECURITY_QUALITY_OF_SERVICE;
typedef struct _SE_IMPERSONATION_STATE{
PACCESS_TOKEN Token;
BOOLEAN CopyOnOpen;
BOOLEAN EffectiveOnly;
SECURITY_IMPERSONATION_LEVEL Level;
}SE_IMPERSONATION_STATE,*PSE_IMPERSONATION_STATE;
#define DISABLE_MAX_PRIVILEGE 0x1
#define SANDBOX_INERT 0x2
#define LUA_TOKEN 0x4
#define WRITE_RESTRICTED 0x8
typedef DWORD SECURITY_INFORMATION,*PSECURITY_INFORMATION;
typedef struct _JOB_SET_ARRAY{
HANDLE JobHandle;
DWORD MemberLevel;
DWORD Flags;
}JOB_SET_ARRAY,*PJOB_SET_ARRAY;
#define FLS_MAXIMUM_AVAILABLE 4080
#define TLS_MINIMUM_AVAILABLE 64
typedef struct _EXCEPTION_REGISTRATION_RECORD{
struct _EXCEPTION_REGISTRATION_RECORD* Next;
PEXCEPTION_ROUTINE Handler;
}EXCEPTION_REGISTRATION_RECORD;
typedef EXCEPTION_REGISTRATION_RECORD* PEXCEPTION_REGISTRATION_RECORD;
typedef struct _NT_TIB{
struct _EXCEPTION_REGISTRATION_RECORD* ExceptionList;
PVOID StackBase;
PVOID StackLimit;
PVOID SubSystemTib;
#if defined(_MSC_EXTENSIONS)
union{
PVOID FiberData;
DWORD Version;
};
#else
PVOID FiberData;
#endif
PVOID ArbitraryUserPointer;
struct _NT_TIB* Self;
}NT_TIB;
typedef NT_TIB* PNT_TIB;
typedef struct _NT_TIB32{
DWORD ExceptionList;
DWORD StackBase;
DWORD StackLimit;
DWORD SubSystemTib;
#if defined(_MSC_EXTENSIONS)
union{
DWORD FiberData;
DWORD Version;
};
#else
DWORD FiberData;
#endif
DWORD ArbitraryUserPointer;
DWORD Self;
}NT_TIB32,*PNT_TIB32;
typedef struct _NT_TIB64{
DWORD64 ExceptionList;
DWORD64 StackBase;
DWORD64 StackLimit;
DWORD64 SubSystemTib;
#if defined(_MSC_EXTENSIONS)
union{
DWORD64 FiberData;
DWORD Version;
};
#else
DWORD64 FiberData;
#endif
DWORD64 ArbitraryUserPointer;
DWORD64 Self;
}NT_TIB64,*PNT_TIB64;
#define THREAD_DYNAMIC_CODE_ALLOW 1
#define DYNAMIC_EH_CONTINUATION_TARGET_ADD (0x01)
#define DYNAMIC_EH_CONTINUATION_TARGET_PROCESSED (0x02)
typedef struct _PROCESS_DYNAMIC_EH_CONTINUATION_TARGET{
ULONG_PTR TargetAddress;
ULONG_PTR Flags;
}PROCESS_DYNAMIC_EH_CONTINUATION_TARGET,*PPROCESS_DYNAMIC_EH_CONTINUATION_TARGET;
typedef struct _PROCESS_DYNAMIC_EH_CONTINUATION_TARGETS_INFORMATION{
WORD NumberOfTargets;
WORD Reserved;
DWORD Reserved2;
PPROCESS_DYNAMIC_EH_CONTINUATION_TARGET Targets;
}PROCESS_DYNAMIC_EH_CONTINUATION_TARGETS_INFORMATION,*PPROCESS_DYNAMIC_EH_CONTINUATION_TARGETS_INFORMATION;
#define DYNAMIC_ENFORCED_ADDRESS_RANGE_ADD (0x01)
#define DYNAMIC_ENFORCED_ADDRESS_RANGE_PROCESSED (0x02)
typedef struct _PROCESS_DYNAMIC_ENFORCED_ADDRESS_RANGE{
ULONG_PTR BaseAddress;
SIZE_T Size;
DWORD Flags;
}PROCESS_DYNAMIC_ENFORCED_ADDRESS_RANGE,*PPROCESS_DYNAMIC_ENFORCED_ADDRESS_RANGE;
typedef struct _PROCESS_DYNAMIC_ENFORCED_ADDRESS_RANGES_INFORMATION{
WORD NumberOfRanges;
WORD Reserved;
DWORD Reserved2;
PPROCESS_DYNAMIC_ENFORCED_ADDRESS_RANGE Ranges;
}PROCESS_DYNAMIC_ENFORCED_ADDRESS_RANGES_INFORMATION,*PPROCESS_DYNAMIC_ENFORCED_ADDRESS_RANGES_INFORMATION;
typedef struct _QUOTA_LIMITS{
SIZE_T PagedPoolLimit;
SIZE_T NonPagedPoolLimit;
SIZE_T MinimumWorkingSetSize;
SIZE_T MaximumWorkingSetSize;
SIZE_T PagefileLimit;
LARGE_INTEGER TimeLimit;
}QUOTA_LIMITS,*PQUOTA_LIMITS;
#define QUOTA_LIMITS_HARDWS_MIN_ENABLE 0x01
#define QUOTA_LIMITS_HARDWS_MIN_DISABLE 0x02
#define QUOTA_LIMITS_HARDWS_MAX_ENABLE 0x04
#define QUOTA_LIMITS_HARDWS_MAX_DISABLE 0x08
#define QUOTA_LIMITS_USE_DEFAULT_LIMITS 0x010
typedef union _RATE_QUOTA_LIMIT{
DWORD RateData;
struct{
DWORD RatePercent:7;
DWORD Reserved0:25;
}DUMMYSTRUCTNAME;
}RATE_QUOTA_LIMIT,*PRATE_QUOTA_LIMIT;
typedef struct _QUOTA_LIMITS_EX{
SIZE_T PagedPoolLimit;
SIZE_T NonPagedPoolLimit;
SIZE_T MinimumWorkingSetSize;
SIZE_T MaximumWorkingSetSize;
SIZE_T PagefileLimit;
LARGE_INTEGER TimeLimit;
SIZE_T WorkingSetLimit;
SIZE_T Reserved2;
SIZE_T Reserved3;
SIZE_T Reserved4;
DWORD Flags;
RATE_QUOTA_LIMIT CpuRateLimit;
}QUOTA_LIMITS_EX,*PQUOTA_LIMITS_EX;
typedef struct _IO_COUNTERS{
ULONGLONG ReadOperationCount;
ULONGLONG WriteOperationCount;
ULONGLONG OtherOperationCount;
ULONGLONG ReadTransferCount;
ULONGLONG WriteTransferCount;
ULONGLONG OtherTransferCount;
}IO_COUNTERS;
typedef IO_COUNTERS* PIO_COUNTERS;
#define MAX_HW_COUNTERS 16
#define THREAD_PROFILING_FLAG_DISPATCH 0x01
typedef enum _HARDWARE_COUNTER_TYPE{
PMCCounter,
MaxHardwareCounterType
}HARDWARE_COUNTER_TYPE,*PHARDWARE_COUNTER_TYPE;
typedef enum _PROCESS_MITIGATION_POLICY{
ProcessDEPPolicy,
ProcessASLRPolicy,
ProcessDynamicCodePolicy,
ProcessStrictHandleCheckPolicy,
ProcessSystemCallDisablePolicy,
ProcessMitigationOptionsMask,
ProcessExtensionPointDisablePolicy,
ProcessControlFlowGuardPolicy,
ProcessSignaturePolicy,
ProcessFontDisablePolicy,
ProcessImageLoadPolicy,
ProcessSystemCallFilterPolicy,
ProcessPayloadRestrictionPolicy,
ProcessChildProcessPolicy,
ProcessSideChannelIsolationPolicy,
ProcessUserShadowStackPolicy,
MaxProcessMitigationPolicy
}PROCESS_MITIGATION_POLICY,*PPROCESS_MITIGATION_POLICY;
typedef enum _JOBOBJECTINFOCLASS{
JobObjectBasicAccountingInformation=1,
JobObjectBasicLimitInformation,
JobObjectBasicProcessIdList,
JobObjectBasicUIRestrictions,
JobObjectSecurityLimitInformation,
JobObjectEndOfJobTimeInformation,
JobObjectAssociateCompletionPortInformation,
JobObjectBasicAndIoAccountingInformation,
JobObjectExtendedLimitInformation,
JobObjectJobSetInformation,
JobObjectGroupInformation,
JobObjectNotificationLimitInformation,
JobObjectLimitViolationInformation,
JobObjectGroupInformationEx,
JobObjectCpuRateControlInformation,
JobObjectCompletionFilter,
JobObjectCompletionCounter,
JobObjectReserved1Information=18,
JobObjectReserved2Information,
JobObjectReserved3Information,
JobObjectReserved4Information,
JobObjectReserved5Information,
JobObjectReserved6Information,
JobObjectReserved7Information,
JobObjectReserved8Information,
JobObjectReserved9Information,
JobObjectReserved10Information,
JobObjectReserved11Information,
JobObjectReserved12Information,
JobObjectReserved13Information,
JobObjectReserved14Information=31,
JobObjectNetRateControlInformation,
JobObjectNotificationLimitInformation2,
JobObjectLimitViolationInformation2,
JobObjectCreateSilo,
JobObjectSiloBasicInformation,
JobObjectReserved15Information=37,
JobObjectReserved16Information=38,
JobObjectReserved17Information=39,
JobObjectReserved18Information=40,
JobObjectReserved19Information=41,
JobObjectReserved20Information=42,
JobObjectReserved21Information=43,
JobObjectReserved22Information=44,
JobObjectReserved23Information=45,
JobObjectReserved24Information=46,
JobObjectReserved25Information=47,
MaxJobObjectInfoClass
}JOBOBJECTINFOCLASS;
typedef struct _SILOOBJECT_BASIC_INFORMATION{
DWORD SiloId;
DWORD SiloParentId;
DWORD NumberOfProcesses;
BOOLEAN IsInServerSilo;
BYTE Reserved[3];
}SILOOBJECT_BASIC_INFORMATION,*PSILOOBJECT_BASIC_INFORMATION;
typedef enum _SERVERSILO_STATE{
SERVERSILO_INITING=0,
SERVERSILO_STARTED,
SERVERSILO_SHUTTING_DOWN,
SERVERSILO_TERMINATING,
SERVERSILO_TERMINATED,
}SERVERSILO_STATE,*PSERVERSILO_STATE;
typedef struct _SERVERSILO_BASIC_INFORMATION{
DWORD ServiceSessionId;
SERVERSILO_STATE State;
DWORD ExitStatus;
BOOLEAN IsDownlevelContainer;
PVOID ApiSetSchema;
PVOID HostApiSetSchema;
}SERVERSILO_BASIC_INFORMATION,*PSERVERSILO_BASIC_INFORMATION;
typedef enum _FIRMWARE_TYPE{
FirmwareTypeUnknown,
FirmwareTypeBios,
FirmwareTypeUefi,
FirmwareTypeMax
}FIRMWARE_TYPE,*PFIRMWARE_TYPE;
typedef enum _LOGICAL_PROCESSOR_RELATIONSHIP{
RelationProcessorCore,
RelationNumaNode,
RelationCache,
RelationProcessorPackage,
RelationGroup,
RelationAll=0xffff
}LOGICAL_PROCESSOR_RELATIONSHIP;
#define LTP_PC_SMT 0x1
typedef enum _PROCESSOR_CACHE_TYPE{
CacheUnified,
CacheInstruction,
CacheData,
CacheTrace
}PROCESSOR_CACHE_TYPE;
#define CACHE_FULLY_ASSOCIATIVE 0xFF
typedef struct _CACHE_DESCRIPTOR{
BYTE Level;
BYTE Associativity;
WORD LineSize;
DWORD Size;
PROCESSOR_CACHE_TYPE Type;
}CACHE_DESCRIPTOR,*PCACHE_DESCRIPTOR;
typedef struct _SYSTEM_LOGICAL_PROCESSOR_INFORMATION{
ULONG_PTR ProcessorMask;
LOGICAL_PROCESSOR_RELATIONSHIP Relationship;
union{
struct{
BYTE Flags;
}ProcessorCore;
struct{
DWORD NodeNumber;
}NumaNode;
CACHE_DESCRIPTOR Cache;
ULONGLONG Reserved[2];
}DUMMYUNIONNAME;
}SYSTEM_LOGICAL_PROCESSOR_INFORMATION,*PSYSTEM_LOGICAL_PROCESSOR_INFORMATION;
typedef struct _PROCESSOR_RELATIONSHIP{
BYTE Flags;
BYTE EfficiencyClass;
BYTE Reserved[20];
WORD GroupCount;
_Field_size_(GroupCount) GROUP_AFFINITY GroupMask[ANYSIZE_ARRAY];
}PROCESSOR_RELATIONSHIP,*PPROCESSOR_RELATIONSHIP;
typedef struct _NUMA_NODE_RELATIONSHIP{
DWORD NodeNumber;
BYTE Reserved[20];
GROUP_AFFINITY GroupMask;
}NUMA_NODE_RELATIONSHIP,*PNUMA_NODE_RELATIONSHIP;
typedef struct _CACHE_RELATIONSHIP{
BYTE Level;
BYTE Associativity;
WORD LineSize;
DWORD CacheSize;
PROCESSOR_CACHE_TYPE Type;
BYTE Reserved[20];
GROUP_AFFINITY GroupMask;
}CACHE_RELATIONSHIP,*PCACHE_RELATIONSHIP;
typedef struct _PROCESSOR_GROUP_INFO{
BYTE MaximumProcessorCount;
BYTE ActiveProcessorCount;
BYTE Reserved[38];
KAFFINITY ActiveProcessorMask;
}PROCESSOR_GROUP_INFO,*PPROCESSOR_GROUP_INFO;
typedef struct _GROUP_RELATIONSHIP{
WORD MaximumGroupCount;
WORD ActiveGroupCount;
BYTE Reserved[20];
PROCESSOR_GROUP_INFO GroupInfo[ANYSIZE_ARRAY];
}GROUP_RELATIONSHIP,*PGROUP_RELATIONSHIP;
_Struct_size_bytes_(Size) struct _SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX{
LOGICAL_PROCESSOR_RELATIONSHIP Relationship;
DWORD Size;
union{
PROCESSOR_RELATIONSHIP Processor;
NUMA_NODE_RELATIONSHIP NumaNode;
CACHE_RELATIONSHIP Cache;
GROUP_RELATIONSHIP Group;
}DUMMYUNIONNAME;
};
typedef struct _SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX,*PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX;
typedef enum _CPU_SET_INFORMATION_TYPE{
CpuSetInformation
}CPU_SET_INFORMATION_TYPE,*PCPU_SET_INFORMATION_TYPE;
typedef struct _SYSTEM_CPU_SET_INFORMATION SYSTEM_CPU_SET_INFORMATION,*PSYSTEM_CPU_SET_INFORMATION;
typedef struct _SYSTEM_POOL_ZEROING_INFORMATION{
BOOLEAN PoolZeroingSupportPresent;
}SYSTEM_POOL_ZEROING_INFORMATION,*PSYSTEM_POOL_ZEROING_INFORMATION;
typedef struct _SYSTEM_PROCESSOR_CYCLE_TIME_INFORMATION{
DWORD64 CycleTime;
}SYSTEM_PROCESSOR_CYCLE_TIME_INFORMATION,*PSYSTEM_PROCESSOR_CYCLE_TIME_INFORMATION;
typedef struct _MEMORY_BASIC_INFORMATION{
PVOID BaseAddress;
PVOID AllocationBase;
DWORD AllocationProtect;
#if defined (_WIN64)
WORD PartitionId;
#endif
SIZE_T RegionSize;
DWORD State;
DWORD Protect;
DWORD Type;
}MEMORY_BASIC_INFORMATION,*PMEMORY_BASIC_INFORMATION;
typedef struct _MEMORY_BASIC_INFORMATION32{
DWORD BaseAddress;
DWORD AllocationBase;
DWORD AllocationProtect;
DWORD RegionSize;
DWORD State;
DWORD Protect;
DWORD Type;
}MEMORY_BASIC_INFORMATION32,*PMEMORY_BASIC_INFORMATION32;
typedef struct DECLSPEC_ALIGN(16) _MEMORY_BASIC_INFORMATION64{
ULONGLONG BaseAddress;
ULONGLONG AllocationBase;
DWORD AllocationProtect;
DWORD __alignment1;
ULONGLONG RegionSize;
DWORD State;
DWORD Protect;
DWORD Type;
DWORD __alignment2;
}MEMORY_BASIC_INFORMATION64,*PMEMORY_BASIC_INFORMATION64;
#define CFG_CALL_TARGET_VALID (0x01)
#define CFG_CALL_TARGET_PROCESSED (0x02)
#define CFG_CALL_TARGET_CONVERT_EXPORT_SUPPRESSED_TO_VALID (0x04)
typedef struct _CFG_CALL_TARGET_INFO{
ULONG_PTR Offset;
ULONG_PTR Flags;
}CFG_CALL_TARGET_INFO,*PCFG_CALL_TARGET_INFO;
#define MEM_EXTENDED_PARAMETER_NUMA_NODE_MANDATORY MINLONG64
#define MEM_EXTENDED_PARAMETER_TYPE_BITS 8
typedef struct DECLSPEC_ALIGN(8) MEM_EXTENDED_PARAMETER{
struct{
DWORD64 Type:MEM_EXTENDED_PARAMETER_TYPE_BITS;
DWORD64 Reserved:64-MEM_EXTENDED_PARAMETER_TYPE_BITS;
}DUMMYSTRUCTNAME;
union{
DWORD64 ULong64;
PVOID Pointer;
SIZE_T Size;
HANDLE Handle;
DWORD ULong;
}DUMMYUNIONNAME;
}MEM_EXTENDED_PARAMETER,*PMEM_EXTENDED_PARAMETER;
typedef struct _ENCLAVE_CREATE_INFO_VBS_BASIC{
DWORD Flags;
BYTE OwnerID[32];
}ENCLAVE_CREATE_INFO_VBS_BASIC,*PENCLAVE_CREATE_INFO_VBS_BASIC;
typedef struct _ENCLAVE_LOAD_DATA_VBS_BASIC{
DWORD PageType;
}ENCLAVE_LOAD_DATA_VBS_BASIC,*PENCLAVE_LOAD_DATA_VBS_BASIC;
typedef struct _FILE_ID_128{
BYTE Identifier[16];
}FILE_ID_128,*PFILE_ID_128;
typedef struct _FILE_NOTIFY_INFORMATION{
DWORD NextEntryOffset;
DWORD Action;
DWORD FileNameLength;
WCHAR FileName[1];
}FILE_NOTIFY_INFORMATION,*PFILE_NOTIFY_INFORMATION;
#define FILE_CS_FLAG_CASE_SENSITIVE_DIR 0x01
typedef union _FILE_SEGMENT_ELEMENT{
PVOID64 Buffer;
ULONGLONG Alignment;
}FILE_SEGMENT_ELEMENT,*PFILE_SEGMENT_ELEMENT;
#define IO_COMPLETION_MODIFY_STATE 0x02
#define IO_COMPLETION_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED|SYNCHRONIZE|0x3)
#define IO_QOS_MAX_RESERVATION 1000000000ULL
#ifndef SMB_CCF_APP_INSTANCE_EA_NAME
#define SMB_CCF_APP_INSTANCE_EA_NAME "ClusteredApplicationInstance"
#endif
#ifndef _NETWORK_APP_INSTANCE_EA_DEFINED
#define _NETWORK_APP_INSTANCE_EA_DEFINED
#if (NTDDI_VERSION >= NTDDI_WIN10)
#ifndef NETWORK_APP_INSTANCE_CSV_FLAGS_VALID_ONLY_IF_CSV_COORDINATOR
#define NETWORK_APP_INSTANCE_CSV_FLAGS_VALID_ONLY_IF_CSV_COORDINATOR 0x01
#endif
#endif
#endif
#define DUPLICATE_CLOSE_SOURCE 0x01
#define DUPLICATE_SAME_ACCESS 0x02
typedef DWORD EXECUTION_STATE,*PEXECUTION_STATE;
typedef enum{
LT_DONT_CARE,
LT_LOWEST_LATENCY
}LATENCY_TIME;
#define POWER_REQUEST_CONTEXT_VERSION DIAGNOSTIC_REASON_VERSION
#define POWER_REQUEST_CONTEXT_SIMPLE_STRING DIAGNOSTIC_REASON_SIMPLE_STRING
#define POWER_REQUEST_CONTEXT_DETAILED_STRING DIAGNOSTIC_REASON_DETAILED_STRING
typedef enum _POWER_REQUEST_TYPE{
PowerRequestDisplayRequired,
PowerRequestSystemRequired,
PowerRequestAwayModeRequired,
PowerRequestExecutionRequired
}POWER_REQUEST_TYPE,*PPOWER_REQUEST_TYPE;
#define IMAGE_SIZEOF_SECTION_HEADER 40
#ifndef _RTL_RUN_ONCE_DEF
#define _RTL_RUN_ONCE_DEF
#define RTL_RUN_ONCE_INIT {0}
#define RTL_RUN_ONCE_CHECK_ONLY 0x01UL
#define RTL_RUN_ONCE_ASYNC 0x02UL
#define RTL_RUN_ONCE_INIT_FAILED 0x04UL
#define RTL_RUN_ONCE_CTX_RESERVED_BITS 2
typedef union _RTL_RUN_ONCE{
PVOID Ptr;
}RTL_RUN_ONCE,*PRTL_RUN_ONCE;
#endif
typedef struct _RTL_BARRIER{
DWORD Reserved1;
DWORD Reserved2;
ULONG_PTR Reserved3[2];
DWORD Reserved4;
DWORD Reserved5;
}RTL_BARRIER,*PRTL_BARRIER;
typedef struct _MESSAGE_RESOURCE_BLOCK{
DWORD LowId;
DWORD HighId;
DWORD OffsetToEntries;
}MESSAGE_RESOURCE_BLOCK,*PMESSAGE_RESOURCE_BLOCK;
typedef struct _MESSAGE_RESOURCE_DATA{
DWORD NumberOfBlocks;
MESSAGE_RESOURCE_BLOCK Blocks[1];
}MESSAGE_RESOURCE_DATA,*PMESSAGE_RESOURCE_DATA;
typedef struct _OSVERSIONINFOA{
DWORD dwOSVersionInfoSize;
DWORD dwMajorVersion;
DWORD dwMinorVersion;
DWORD dwBuildNumber;
DWORD dwPlatformId;
CHAR szCSDVersion[128];
}OSVERSIONINFOA,*POSVERSIONINFOA,*LPOSVERSIONINFOA;
typedef struct _OSVERSIONINFOW{
DWORD dwOSVersionInfoSize;
DWORD dwMajorVersion;
DWORD dwMinorVersion;
DWORD dwBuildNumber;
DWORD dwPlatformId;
WCHAR szCSDVersion[128];
}OSVERSIONINFOW,*POSVERSIONINFOW,*LPOSVERSIONINFOW,RTL_OSVERSIONINFOW,*PRTL_OSVERSIONINFOW;
#ifdef UNICODE
typedef OSVERSIONINFOW OSVERSIONINFO;
typedef POSVERSIONINFOW POSVERSIONINFO;
typedef LPOSVERSIONINFOW LPOSVERSIONINFO;
#else
typedef OSVERSIONINFOA OSVERSIONINFO;
typedef POSVERSIONINFOA POSVERSIONINFO;
typedef LPOSVERSIONINFOA LPOSVERSIONINFO;
#endif
typedef struct _OSVERSIONINFOEXA{
DWORD dwOSVersionInfoSize;
DWORD dwMajorVersion;
DWORD dwMinorVersion;
DWORD dwBuildNumber;
DWORD dwPlatformId;
CHAR szCSDVersion[128];
WORD wServicePackMajor;
WORD wServicePackMinor;
WORD wSuiteMask;
BYTE wProductType;
BYTE wReserved;
}OSVERSIONINFOEXA,*POSVERSIONINFOEXA,*LPOSVERSIONINFOEXA;
typedef struct _OSVERSIONINFOEXW{
DWORD dwOSVersionInfoSize;
DWORD dwMajorVersion;
DWORD dwMinorVersion;
DWORD dwBuildNumber;
DWORD dwPlatformId;
WCHAR szCSDVersion[128];
WORD wServicePackMajor;
WORD wServicePackMinor;
WORD wSuiteMask;
BYTE wProductType;
BYTE wReserved;
}OSVERSIONINFOEXW,*POSVERSIONINFOEXW,*LPOSVERSIONINFOEXW,RTL_OSVERSIONINFOEXW,*PRTL_OSVERSIONINFOEXW;
#ifdef UNICODE
typedef OSVERSIONINFOEXW OSVERSIONINFOEX;
typedef POSVERSIONINFOEXW POSVERSIONINFOEX;
typedef LPOSVERSIONINFOEXW LPOSVERSIONINFOEX;
#else
typedef OSVERSIONINFOEXA OSVERSIONINFOEX;
typedef POSVERSIONINFOEXA POSVERSIONINFOEX;
typedef LPOSVERSIONINFOEXA LPOSVERSIONINFOEX;
#endif
#define RTL_UMS_VERSION (0x0100)
typedef enum _RTL_UMS_THREAD_INFO_CLASS{
UmsThreadInvalidInfoClass=0,
UmsThreadUserContext,
UmsThreadPriority,
UmsThreadAffinity,
UmsThreadTeb,
UmsThreadIsSuspended,
UmsThreadIsTerminated,
UmsThreadMaxInfoClass
}RTL_UMS_THREAD_INFO_CLASS,*PRTL_UMS_THREAD_INFO_CLASS;
typedef enum _RTL_UMS_SCHEDULER_REASON{
UmsSchedulerStartup=0,
UmsSchedulerThreadBlocked,
UmsSchedulerThreadYield,
}RTL_UMS_SCHEDULER_REASON,*PRTL_UMS_SCHEDULER_REASON;
typedef
_Function_class_(RTL_UMS_SCHEDULER_ENTRY_POINT)
VOID
NTAPI
RTL_UMS_SCHEDULER_ENTRY_POINT(
_In_ RTL_UMS_SCHEDULER_REASON Reason,
_In_ ULONG_PTR ActivationPayload,
_In_ PVOID SchedulerParam);
typedef RTL_UMS_SCHEDULER_ENTRY_POINT* PRTL_UMS_SCHEDULER_ENTRY_POINT;
#if (NTDDI_VERSION >= NTDDI_WIN10_RS2)
typedef struct _NV_MEMORY_RANGE{
VOID* BaseAddress;
SIZE_T Length;
}NV_MEMORY_RANGE,*PNV_MEMORY_RANGE;
#if defined(_WIN64)
#if (NTDDI_VERSION >= NTDDI_WIN10_19H1)
_IRQL_requires_max_(DPC_LEVEL)
NTSYSAPI
DWORD
NTAPI
RtlFillNonVolatileMemory(
_In_ PVOID NvToken,
_Out_writes_bytes_(Size) VOID UNALIGNED* NvDestination,
_In_ SIZE_T Size,
_In_ CONST BYTE Value,
_In_ DWORD Flags);
#endif
_IRQL_requires_max_(DPC_LEVEL)
NTSYSAPI
DWORD
NTAPI
RtlFlushNonVolatileMemoryRanges(
_In_ PVOID NvToken,
_In_reads_(NumRanges) PNV_MEMORY_RANGE NvRanges,
_In_ SIZE_T NumRanges,
_In_ DWORD Flags);
#else
#define RtlGetNonVolatileToken(B,S,T) \
(ASSERT(!"Call not expected in 32-bit architecture"),STATUS_NOT_IMPLEMENTED)
#define RtlFreeNonVolatileToken(T) \
(ASSERT(!"Call not expected in 32-bit architecture"),STATUS_NOT_IMPLEMENTED)
#define RtlFlushNonVolatileMemory(T,B,S,F) \
(ASSERT(!"Call not expected in 32-bit architecture"),STATUS_NOT_IMPLEMENTED)
#define RtlDrainNonVolatileFlush(T) \
(ASSERT(!"Call not expected in 32-bit architecture"),STATUS_NOT_IMPLEMENTED)
#define RtlWriteNonVolatileMemory(T,D,S,L,F) \
(ASSERT(!"Call not expected in 32-bit architecture"),STATUS_NOT_IMPLEMENTED)
#if (NTDDI_VERSION >= NTDDI_WIN10_19H1)
#define RtlFillNonVolatileMemory(T,B,S,V,F) \
(ASSERT(!"Call not expected in 32-bit architecture"),STATUS_NOT_IMPLEMENTED)
#endif
#define RtlFlushNonVolatileMemoryRanges(T,R,N,F) \
(ASSERT(!"Call not expected in 32-bit architecture"),STATUS_NOT_IMPLEMENTED)
#endif
#endif
#if (NTDDI_VERSION >= NTDDI_WIN10_RS4)
typedef struct _CUSTOM_SYSTEM_EVENT_TRIGGER_CONFIG{
DWORD Size;
PCWSTR TriggerId;
}CUSTOM_SYSTEM_EVENT_TRIGGER_CONFIG,*PCUSTOM_SYSTEM_EVENT_TRIGGER_CONFIG;
#endif
#define IMAGE_POLICY_METADATA_VERSION 1
#define IMAGE_POLICY_SECTION_NAME ".tPolicy"
#define IMAGE_POLICY_METADATA_NAME __ImagePolicyMetadata
#if (NTDDI_VERSION >= NTDDI_WIN10_VB)
NTSYSAPI
BOOLEAN
NTAPI
RtlIsZeroMemory(
_In_ PVOID Buffer,
_In_ SIZE_T Length);
__drv_maxIRQL(APC_LEVEL)
NTSYSAPI
BOOLEAN
NTAPI
RtlNormalizeSecurityDescriptor(
_Inout_ PSECURITY_DESCRIPTOR* SecurityDescriptor,
_In_ DWORD SecurityDescriptorLength,
_Out_opt_ PSECURITY_DESCRIPTOR* NewSecurityDescriptor,
_Out_opt_ PDWORD NewSecurityDescriptorLength,
_In_ BOOLEAN CheckOnly);
#endif
typedef struct _RTL_CRITICAL_SECTION_DEBUG{
WORD Type;
WORD CreatorBackTraceIndex;
struct _RTL_CRITICAL_SECTION* CriticalSection;
LIST_ENTRY ProcessLocksList;
DWORD EntryCount;
DWORD ContentionCount;
DWORD Flags;
WORD CreatorBackTraceIndexHigh;
WORD SpareWORD;
}RTL_CRITICAL_SECTION_DEBUG,*PRTL_CRITICAL_SECTION_DEBUG,RTL_RESOURCE_DEBUG,*PRTL_RESOURCE_DEBUG;
#define RTL_CRITICAL_SECTION_DEBUG_FLAG_STATIC_INIT 0x01
#pragma pack(push,8)
typedef struct _RTL_CRITICAL_SECTION{
PRTL_CRITICAL_SECTION_DEBUG DebugInfo;
LONG LockCount;
LONG RecursionCount;
HANDLE OwningThread;
HANDLE LockSemaphore;
ULONG_PTR SpinCount;
}RTL_CRITICAL_SECTION,*PRTL_CRITICAL_SECTION;
#pragma pack(pop)
typedef struct _RTL_SRWLOCK{
PVOID Ptr;
}RTL_SRWLOCK,*PRTL_SRWLOCK;
#define RTL_SRWLOCK_INIT {0}
typedef struct _RTL_CONDITION_VARIABLE{
PVOID Ptr;
}RTL_CONDITION_VARIABLE,*PRTL_CONDITION_VARIABLE;
#define RTL_CONDITION_VARIABLE_INIT {0}
#define RTL_CONDITION_VARIABLE_LOCKMODE_SHARED 0x1
typedef
VOID
(NTAPI* PAPCFUNC)(
_In_ ULONG_PTR Parameter);
typedef LONG(NTAPI* PVECTORED_EXCEPTION_HANDLER)(
struct _EXCEPTION_POINTERS* ExceptionInfo);
typedef enum _HEAP_INFORMATION_CLASS{
HeapCompatibilityInformation=0,
HeapEnableTerminationOnCorruption=1
#if ((NTDDI_VERSION>NTDDI_WINBLUE)||\
(NTDDI_VERSION==NTDDI_WINBLUE&&defined(WINBLUE_KBSPRING14)))
,
HeapOptimizeResources=3
#endif
}HEAP_INFORMATION_CLASS;
typedef VOID(NTAPI* WAITORTIMERCALLBACKFUNC) (PVOID,BOOLEAN);
typedef VOID(NTAPI* WORKERCALLBACKFUNC) (PVOID);
typedef VOID(NTAPI* APC_CALLBACK_FUNCTION) (DWORD,PVOID,PVOID);
typedef WAITORTIMERCALLBACKFUNC WAITORTIMERCALLBACK;
typedef
VOID
(NTAPI* PFLS_CALLBACK_FUNCTION) (
IN PVOID lpFlsData);
typedef
BOOLEAN
(NTAPI* PSECURE_MEMORY_CACHE_CALLBACK) (
_In_reads_bytes_(Range) PVOID Addr,
_In_ SIZE_T Range);
#define WT_EXECUTEINLONGTHREAD 0x010
#define WT_EXECUTEDELETEWAIT 0x08
#define ACTIVATIONCONTEXTINFOCLASS ACTIVATION_CONTEXT_INFO_CLASS
#ifdef _MSC_EXTENSIONS
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif
typedef const struct _ACTIVATION_CONTEXT_COMPATIBILITY_INFORMATION* PCACTIVATION_CONTEXT_COMPATIBILITY_INFORMATION;
#endif
#define CREATE_BOUNDARY_DESCRIPTOR_ADD_APPCONTAINER_SID 0x1
typedef struct _HARDWARE_COUNTER_DATA{
HARDWARE_COUNTER_TYPE Type;
DWORD Reserved;
DWORD64 Value;
}HARDWARE_COUNTER_DATA,*PHARDWARE_COUNTER_DATA;
#define PERFORMANCE_DATA_VERSION 1
typedef struct _PERFORMANCE_DATA{
WORD Size;
BYTE Version;
BYTE HwCountersCount;
DWORD ContextSwitchCount;
DWORD64 WaitReasonBitMap;
DWORD64 CycleTime;
DWORD RetryCount;
DWORD Reserved;
HARDWARE_COUNTER_DATA HwCounters[MAX_HW_COUNTERS];
}PERFORMANCE_DATA,*PPERFORMANCE_DATA;
#define READ_THREAD_PROFILING_FLAG_DISPATCHING 0x01
#define READ_THREAD_PROFILING_FLAG_HARDWARE_COUNTERS 0x02
#define MAXLOGICALLOGNAMESIZE 256
typedef DWORD TP_VERSION,*PTP_VERSION;
typedef struct _TP_CALLBACK_INSTANCE TP_CALLBACK_INSTANCE,*PTP_CALLBACK_INSTANCE;
typedef VOID(NTAPI* PTP_SIMPLE_CALLBACK)(
_Inout_ PTP_CALLBACK_INSTANCE Instance,
_Inout_opt_ PVOID Context);
typedef struct _TP_POOL TP_POOL,*PTP_POOL;
typedef enum _TP_CALLBACK_PRIORITY{
TP_CALLBACK_PRIORITY_HIGH,
TP_CALLBACK_PRIORITY_NORMAL,
TP_CALLBACK_PRIORITY_LOW,
TP_CALLBACK_PRIORITY_INVALID,
TP_CALLBACK_PRIORITY_COUNT=TP_CALLBACK_PRIORITY_INVALID
}TP_CALLBACK_PRIORITY;
typedef struct _TP_POOL_STACK_INFORMATION{
SIZE_T StackReserve;
SIZE_T StackCommit;
}TP_POOL_STACK_INFORMATION,*PTP_POOL_STACK_INFORMATION;
typedef struct _TP_CLEANUP_GROUP TP_CLEANUP_GROUP,*PTP_CLEANUP_GROUP;
typedef VOID(NTAPI* PTP_CLEANUP_GROUP_CANCEL_CALLBACK)(
_Inout_opt_ PVOID ObjectContext,
_Inout_opt_ PVOID CleanupContext);
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN7)
typedef struct _TP_CALLBACK_ENVIRON_V3{
TP_VERSION Version;
PTP_POOL Pool;
PTP_CLEANUP_GROUP CleanupGroup;
PTP_CLEANUP_GROUP_CANCEL_CALLBACK CleanupGroupCancelCallback;
PVOID RaceDll;
struct _ACTIVATION_CONTEXT* ActivationContext;
PTP_SIMPLE_CALLBACK FinalizationCallback;
union{
DWORD Flags;
struct{
DWORD LongFunction:1;
DWORD Persistent:1;
DWORD Private:30;
}s;
}u;
TP_CALLBACK_PRIORITY CallbackPriority;
DWORD Size;
}TP_CALLBACK_ENVIRON_V3;
typedef TP_CALLBACK_ENVIRON_V3 TP_CALLBACK_ENVIRON,*PTP_CALLBACK_ENVIRON;
#else
typedef struct _TP_CALLBACK_ENVIRON_V1{
TP_VERSION Version;
PTP_POOL Pool;
PTP_CLEANUP_GROUP CleanupGroup;
PTP_CLEANUP_GROUP_CANCEL_CALLBACK CleanupGroupCancelCallback;
PVOID RaceDll;
struct _ACTIVATION_CONTEXT* ActivationContext;
PTP_SIMPLE_CALLBACK FinalizationCallback;
union{
DWORD Flags;
struct{
DWORD LongFunction:1;
DWORD Persistent:1;
DWORD Private:30;
}s;
}u;
}TP_CALLBACK_ENVIRON_V1;
typedef TP_CALLBACK_ENVIRON_V1 TP_CALLBACK_ENVIRON,*PTP_CALLBACK_ENVIRON;
#endif
#if !defined(MIDL_PASS)
FORCEINLINE
VOID
TpInitializeCallbackEnviron(
_Out_ PTP_CALLBACK_ENVIRON CallbackEnviron){
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN7)
CallbackEnviron->Version=3;
#else
CallbackEnviron->Version=1;
#endif
CallbackEnviron->Pool=NULL;
CallbackEnviron->CleanupGroup=NULL;
CallbackEnviron->CleanupGroupCancelCallback=NULL;
CallbackEnviron->RaceDll=NULL;
CallbackEnviron->ActivationContext=NULL;
CallbackEnviron->FinalizationCallback=NULL;
CallbackEnviron->u.Flags=0;
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN7)
CallbackEnviron->CallbackPriority=TP_CALLBACK_PRIORITY_NORMAL;
CallbackEnviron->Size=sizeof(TP_CALLBACK_ENVIRON);
#endif
}
FORCEINLINE
VOID
TpSetCallbackThreadpool(
_Inout_ PTP_CALLBACK_ENVIRON CallbackEnviron,
_In_ PTP_POOL Pool){
CallbackEnviron->Pool=Pool;}
FORCEINLINE
VOID
TpSetCallbackCleanupGroup(
_Inout_ PTP_CALLBACK_ENVIRON CallbackEnviron,
_In_ PTP_CLEANUP_GROUP CleanupGroup,
_In_opt_ PTP_CLEANUP_GROUP_CANCEL_CALLBACK CleanupGroupCancelCallback){
CallbackEnviron->CleanupGroup=CleanupGroup;
CallbackEnviron->CleanupGroupCancelCallback=CleanupGroupCancelCallback;}
FORCEINLINE
VOID
TpSetCallbackActivationContext(
_Inout_ PTP_CALLBACK_ENVIRON CallbackEnviron,
_In_opt_ struct _ACTIVATION_CONTEXT* ActivationContext){
CallbackEnviron->ActivationContext=ActivationContext;}
FORCEINLINE
VOID
TpSetCallbackNoActivationContext(
_Inout_ PTP_CALLBACK_ENVIRON CallbackEnviron){
CallbackEnviron->ActivationContext=(struct _ACTIVATION_CONTEXT*)(LONG_PTR)-1;}
FORCEINLINE
VOID
TpSetCallbackLongFunction(
_Inout_ PTP_CALLBACK_ENVIRON CallbackEnviron){
CallbackEnviron->u.s.LongFunction=1;}
FORCEINLINE
VOID
TpSetCallbackRaceWithDll(
_Inout_ PTP_CALLBACK_ENVIRON CallbackEnviron,
_In_ PVOID DllHandle){
CallbackEnviron->RaceDll=DllHandle;}
FORCEINLINE
VOID
TpSetCallbackFinalizationCallback(
_Inout_ PTP_CALLBACK_ENVIRON CallbackEnviron,
_In_ PTP_SIMPLE_CALLBACK FinalizationCallback){
CallbackEnviron->FinalizationCallback=FinalizationCallback;}
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN7)
FORCEINLINE
VOID
TpSetCallbackPriority(
_Inout_ PTP_CALLBACK_ENVIRON CallbackEnviron,
_In_ TP_CALLBACK_PRIORITY Priority){
CallbackEnviron->CallbackPriority=Priority;}
#endif
FORCEINLINE
VOID
TpSetCallbackPersistent(
_Inout_ PTP_CALLBACK_ENVIRON CallbackEnviron){
CallbackEnviron->u.s.Persistent=1;}
FORCEINLINE
VOID
TpDestroyCallbackEnviron(
_In_ PTP_CALLBACK_ENVIRON CallbackEnviron){
UNREFERENCED_PARAMETER(CallbackEnviron);}
#endif
typedef struct _TP_WORK TP_WORK,*PTP_WORK;
typedef VOID(NTAPI* PTP_WORK_CALLBACK)(
_Inout_ PTP_CALLBACK_INSTANCE Instance,
_Inout_opt_ PVOID Context,
_Inout_ PTP_WORK Work);
typedef struct _TP_TIMER TP_TIMER,*PTP_TIMER;
typedef VOID(NTAPI* PTP_TIMER_CALLBACK)(
_Inout_ PTP_CALLBACK_INSTANCE Instance,
_Inout_opt_ PVOID Context,
_Inout_ PTP_TIMER Timer);
typedef DWORD TP_WAIT_RESULT;
typedef struct _TP_WAIT TP_WAIT,*PTP_WAIT;
typedef VOID(NTAPI* PTP_WAIT_CALLBACK)(
_Inout_ PTP_CALLBACK_INSTANCE Instance,
_Inout_opt_ PVOID Context,
_Inout_ PTP_WAIT Wait,
_In_ TP_WAIT_RESULT WaitResult);
typedef struct _TP_IO TP_IO,*PTP_IO;
#if defined(_M_AMD64)&&!defined(__midl)
__forceinline
struct _TEB*
NtCurrentTeb(
VOID){
return (struct _TEB*)__readgsqword(FIELD_OFFSET(NT_TIB,Self));}
__forceinline
PVOID
GetCurrentFiber(
VOID){
return (PVOID)__readgsqword(FIELD_OFFSET(NT_TIB,FiberData));}
__forceinline
PVOID
GetFiberData(
VOID){
return *(PVOID*)GetCurrentFiber();}
#endif
#ifdef __cplusplus
}
#endif
#if _MSC_VER >= 1200
#pragma warning(pop)
#else
#pragma warning(default:4200)
#pragma warning(default:4201)
#pragma warning(default:4214)
#endif
#endif
#endif
typedef UINT_PTR WPARAM;
typedef LONG_PTR LPARAM;
typedef LONG_PTR LRESULT;
#ifndef NOMINMAX
#ifndef max
#define max(a,b) (((a)>(b)) ? (a):(b))
#endif
#ifndef min
#define min(a,b) (((a)<(b)) ? (a):(b))
#endif
#endif /* NOMINMAX */
#define MAKEWORD(a,b) ((WORD)(((BYTE)(((DWORD_PTR)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD_PTR)(b)) & 0xff)))<<8))
#define MAKELONG(a,b) ((LONG)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff)))<<16))
#define LOWORD(l) ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l) ((WORD)((((DWORD_PTR)(l))>>16) & 0xffff))
#define LOBYTE(w) ((BYTE)(((DWORD_PTR)(w)) & 0xff))
#define HIBYTE(w) ((BYTE)((((DWORD_PTR)(w))>>8) & 0xff))
typedef HANDLE NEAR* SPHANDLE;
typedef HANDLE FAR* LPHANDLE;
typedef HANDLE HGLOBAL;
typedef HANDLE HLOCAL;
typedef HANDLE GLOBALHANDLE;
typedef HANDLE LOCALHANDLE;
#ifndef _MANAGED
#if _MSC_VER >= 1200
#pragma warning(push)
#pragma warning(disable:4255)
#endif
#ifndef _MAC
#ifdef _WIN64
typedef INT_PTR(FAR WINAPI* FARPROC)();
typedef INT_PTR(NEAR WINAPI* NEARPROC)();
typedef INT_PTR(WINAPI* PROC)();
#else
typedef int (FAR WINAPI* FARPROC)();
typedef int (NEAR WINAPI* NEARPROC)();
typedef int (WINAPI* PROC)();
#endif
#else
typedef int (CALLBACK* FARPROC)();
typedef int (CALLBACK* NEARPROC)();
typedef int (CALLBACK* PROC)();
#endif
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif
#else
typedef INT_PTR(WINAPI* FARPROC)(void);
typedef INT_PTR(WINAPI* NEARPROC)(void);
typedef INT_PTR(WINAPI* PROC)(void);
#endif
typedef WORD ATOM;
DECLARE_HANDLE(HKEY);
typedef HKEY* PHKEY;
DECLARE_HANDLE(HMETAFILE);
DECLARE_HANDLE(HINSTANCE);
typedef HINSTANCE HMODULE;
DECLARE_HANDLE(HRGN);
DECLARE_HANDLE(HRSRC);
DECLARE_HANDLE(HSPRITE);
DECLARE_HANDLE(HLSURF);
DECLARE_HANDLE(HSTR);
DECLARE_HANDLE(HTASK);
DECLARE_HANDLE(HWINSTA);
DECLARE_HANDLE(HKL);
#ifndef _MAC
typedef int HFILE;
#else
typedef short HFILE;
#endif
typedef struct _FILETIME{
DWORD dwLowDateTime;
DWORD dwHighDateTime;
}FILETIME,*PFILETIME,*LPFILETIME;
#define _FILETIME_
#ifdef __cplusplus
}
#endif
#endif
#pragma endregion
#ifndef WINVER
#define WINVER 0x0500
#endif
#ifndef NT_INCLUDED
#endif
#ifndef WIN_INTERNAL
#pragma region Application Family or OneCore Family or Games Family
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES)
DECLARE_HANDLE(HWND);
DECLARE_HANDLE(HHOOK);
#endif
#pragma endregion
#ifdef WINABLE
#pragma region Desktop Family
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
DECLARE_HANDLE(HEVENT);
#endif /* WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) */
#pragma endregion
#endif
#endif
#pragma region Application Family
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES)
#if !defined(_MAC)||!defined(GDI_INTERNAL)
#ifdef STRICT
typedef void NEAR* HGDIOBJ;
#else
DECLARE_HANDLE(HGDIOBJ);
#endif
#endif
#if !defined(_MAC)||!defined(WIN_INTERNAL)
DECLARE_HANDLE(HACCEL);
#endif
#if !defined(_MAC)||!defined(GDI_INTERNAL)
DECLARE_HANDLE(HBITMAP);
DECLARE_HANDLE(HBRUSH);
#endif
#if(WINVER >= 0x0400)
DECLARE_HANDLE(HCOLORSPACE);
#endif
#if !defined(_MAC)||!defined(GDI_INTERNAL)
DECLARE_HANDLE(HDC);
#endif
DECLARE_HANDLE(HGLRC);
DECLARE_HANDLE(HDESK);
DECLARE_HANDLE(HENHMETAFILE);
#if !defined(_MAC)||!defined(GDI_INTERNAL)
DECLARE_HANDLE(HFONT);
#endif
DECLARE_HANDLE(HICON);
#if !defined(_MAC)||!defined(WIN_INTERNAL)
DECLARE_HANDLE(HMENU);
#endif
#if !defined(_MAC)||!defined(GDI_INTERNAL)
DECLARE_HANDLE(HPALETTE);
DECLARE_HANDLE(HPEN);
#endif
#if(WINVER >= 0x0400)
DECLARE_HANDLE(HWINEVENTHOOK);
#endif
#endif
#pragma endregion
#if(WINVER >= 0x0500)
#ifndef _MAC
#pragma region Application Family
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM)
DECLARE_HANDLE(HMONITOR);
#endif
#pragma endregion
#endif
#pragma region Desktop Family
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
DECLARE_HANDLE(HUMPD);
#endif
#pragma endregion
#endif
#pragma region Application Family
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES)
#ifndef _MAC
typedef HICON HCURSOR;
#else
DECLARE_HANDLE(HCURSOR);
#endif
typedef DWORD COLORREF;
#endif
#pragma endregion
#pragma region Desktop Family
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
typedef DWORD* LPCOLORREF;
#define HFILE_ERROR ((HFILE)-1)
#endif
#pragma endregion
#pragma region Application Family or Games Family
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES)
typedef struct tagRECT{
LONG left,top,right,bottom;
}RECT,*PRECT,NEAR* NPRECT,FAR* LPRECT;
typedef const RECT FAR* LPCRECT;
typedef struct _RECTL /* rcl */{
LONG left,top,right,bottom;
}RECTL,*PRECTL,*LPRECTL;
typedef const RECTL FAR* LPCRECTL;
typedef struct tagPOINT{
LONG x,y;
}POINT,*PPOINT,NEAR* NPPOINT,FAR* LPPOINT;
typedef struct _POINTL /* ptl */{
LONG x,y;
}POINTL,*PPOINTL;
typedef struct tagSIZE{
LONG cx,cy;
}SIZE,*PSIZE,*LPSIZE;
typedef SIZE SIZEL;
typedef SIZE* PSIZEL,*LPSIZEL;
typedef struct tagPOINTS{
#ifndef _MAC
SHORT x,y;
#else
SHORT y,x;
#endif
}POINTS,*PPOINTS,*LPPOINTS;
#define APP_LOCAL_DEVICE_ID_SIZE 32
typedef struct APP_LOCAL_DEVICE_ID{
BYTE value[APP_LOCAL_DEVICE_ID_SIZE];
}APP_LOCAL_DEVICE_ID;
#endif
#pragma endregion
#define DM_UPDATE 1
#define DM_COPY 2
#define DM_PROMPT 4
#define DM_MODIFY 8
#define DM_IN_BUFFER DM_MODIFY
#define DM_IN_PROMPT DM_PROMPT
#define DM_OUT_BUFFER DM_COPY
#define DM_OUT_DEFAULT DM_UPDATE
#define DC_FIELDS 1
#define DC_PAPERS 2
#define DC_PAPERSIZE 3
#define DC_MINEXTENT 4
#define DC_MAXEXTENT 5
#define DC_BINS 6
#define DC_DUPLEX 7
#define DC_SIZE 8
#define DC_EXTRA 9
#define DC_VERSION 10
#define DC_DRIVER 11
#define DC_BINNAMES 12
#define DC_ENUMRESOLUTIONS 13
#define DC_FILEDEPENDENCIES 14
#define DC_TRUETYPE 15
#define DC_PAPERNAMES 16
#define DC_ORIENTATION 17
#define DC_COPIES 18
#ifdef __cplusplus
}
#endif
#pragma region Desktop Family
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#define _DPI_AWARENESS_CONTEXTS_
DECLARE_HANDLE(DPI_AWARENESS_CONTEXT);
typedef enum DPI_AWARENESS{
DPI_AWARENESS_INVALID=-1,
DPI_AWARENESS_UNAWARE=0,
DPI_AWARENESS_SYSTEM_AWARE=1,
DPI_AWARENESS_PER_MONITOR_AWARE=2
}DPI_AWARENESS;
#define DPI_AWARENESS_CONTEXT_UNAWARE ((DPI_AWARENESS_CONTEXT)-1)
#define DPI_AWARENESS_CONTEXT_SYSTEM_AWARE ((DPI_AWARENESS_CONTEXT)-2)
#define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE ((DPI_AWARENESS_CONTEXT)-3)
#define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 ((DPI_AWARENESS_CONTEXT)-4)
#define DPI_AWARENESS_CONTEXT_UNAWARE_GDISCALED ((DPI_AWARENESS_CONTEXT)-5)
typedef enum DPI_HOSTING_BEHAVIOR{
DPI_HOSTING_BEHAVIOR_INVALID=-1,
DPI_HOSTING_BEHAVIOR_DEFAULT=0,
DPI_HOSTING_BEHAVIOR_MIXED=1
}DPI_HOSTING_BEHAVIOR;
#endif
#if !defined(_MAC)||defined(_WIN32NLS)
#endif
#ifndef _MAC
#endif
#if !defined(_MAC)||defined(_WIN32REG)
#endif
#ifndef _MAC
#endif
#ifdef INC_OLE2
#endif
#ifndef _MAC
#if(WINVER >= 0x0400)
#endif
#endif
#ifndef RC_INVOKED
#if (_MSC_VER >= 800)
#if _MSC_VER >= 1200
#else
#pragma warning(default:4001)
#pragma warning(default:4201)
#pragma warning(default:4214)
#endif
#endif
#endif
#endif
#pragma endregion