//
// Created by rvons on 2023-06-26.
//

#ifndef DYNAMICLOAD_DYNL_EXPORT_HPP
#define DYNAMICLOAD_DYNL_EXPORT_HPP

#ifdef _MSC_VER
#define DYNL_DECLATTR(X) __declspec(X)
#define DYNLOAD_EXPORT __declspec(dllexport)
#define DYNLOAD_EXPORT_C extern "C" DYNLOAD_EXPORT
#else
#define DYNL_DECLATTR(X) __attribute__((X))
#define DYNLOAD_EXPORT extern
#define DYNLOAD_EXPORT_C extern "C"
#endif


#endif // DYNAMICLOAD_DYNL_EXPORT_HPP
