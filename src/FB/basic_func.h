#ifndef _BASIC_FUNCTIONS_H_
#define _BASIC_FUNCTIONS_H_

/*
 * SPHERE - Skeleton for PHysical and Engineering REsearch
 *
 * Copyright (c) RIKEN, Japan. All right reserved. 2004-2010
 *
 */

//@file basic_func.h
//@brief basic functions which are independent from SPHERE
//@author keno, FSI Team, VCAD, RIKEN

#include <math.h>

/**
 @fn template <typename T> inline T Linear(SKL_REAL t, T v[2])
 @brief Linear interpolation
 @param t target dividing point [0,1] normalized
 @param v values, array of size 2, from zero to one
 */
template <typename T>
inline T Linear(SKL_REAL t, T v[2]) {
  return ( (1.0-t)*v[0] + t*v[1] );
}

/**
 @fn template <typename T> inline T Bilinear(SKL_REAL t[2], T v[4])
 @brief Bilinear interpolation
 @param t target dividing point [0,1][0,1] normalized
 @param v values, array of size 4
 @note
 - v[0]; c[0,0]
 - v[1]; c[1,0]
 - v[2]; c[0,1]
 - v[3]; c[1,1]
 */
template <typename T>
inline T Bilinear(SKL_REAL t[2], T v[4]) {
  T r[2];
  r[0] = Linear( t[0], &v[0] );
  r[1] = Linear( t[0], &v[2] );
  return ( Linear( t[1], r ) );
}

/**
 @fn template <typename T> inline T Trilinear(SKL_REAL t[3], T v[8])
 @brief Trilinear interpolation
 @param t target dividing point [0,1][0,1][0,1] normalized
 @param v values, array of size 8
 @note
 - v[0]; c[0,0,0]
 - v[1]; c[1,0,0]
 - v[2]; c[0,1,0]
 - v[3]; c[1,1,0]
 - v[4]; c[0,0,1]
 - v[5]; c[1,0,1]
 - v[6]; c[0,1,1]
 - v[7]; c[1,1,1]
 */
template <typename T>
inline T Trilinear(SKL_REAL t[3], T v[8]) {
  T r[2];
  r[0] = Bilinear( &t[0], &v[0] );
  r[1] = Bilinear( &t[0], &v[4] );
  return ( Linear( t[2], r ) );
}

#endif // _BASIC_FUNCTIONS_H_