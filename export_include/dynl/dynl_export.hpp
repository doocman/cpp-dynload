//
// Created by rvons on 2023-06-26.
//

#ifndef DYNAMICLOAD_DYNL_EXPORT_HPP
#define DYNAMICLOAD_DYNL_EXPORT_HPP

#ifdef _MSC_VER
#define DYNL_DECLATTR(X) __declspec(X)
#define DYNLOAD_EXPORT __declspec(dllexport)
#else
#define DYNL_DECLATTR(X) __attribute__((X))
#define DYNLOAD_EXPORT extern
#endif


#endif // DYNAMICLOAD_DYNL_EXPORT_HPP
