#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>
#include <stdio.h>

/*  To use this exported function of dll, include this header
 *  in your project.
 */

#ifdef BUILD_DLL
    #define DLLEXPORT __declspec(dllexport)
#else
    #define DLLEXPORT __declspec(dllimport)
#endif

/* Some 20-sim typedefs */
typedef double XXDouble;
typedef char*  XXString;

/* 20-sim string defines to allow retrieval of a string from a 20-sim double array */
#define XXString2Double *(XXDouble *)&
#define XXDouble2String *(XXString *)&

#define INITIALIZE_OK     0
#define INITIALIZE_FAILED 1
#define FUNCTION_OK       0
#define FUNCTION_FAILED   1
#define TERMINATE_OK      0
#define TERMINATE_FAILED  1

#endif // __MAIN_H__
