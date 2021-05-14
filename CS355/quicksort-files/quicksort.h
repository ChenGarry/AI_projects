/******************************************************************************/
/*!
\file   quicksort.h
\author Garry Chen
\par    DP email: garry.c\@digipen.edu
\par    DigiPen login: garry.c
\par    Course: CS355
\par    Assignment #04
\par    Section: A
\date   04/26/2021

\brief
  This is the header file for quicksort functions.

*/
/******************************************************************************/
#ifndef QUICKSORT
#define QUICKSORT
template< typename T>
void quicksort_rec( T* a, unsigned begin, unsigned end );

template< typename T>
void quicksort_iterative( T* a, unsigned begin, unsigned end );

template< typename T>
void quicksort(T* a, unsigned, unsigned, int);

#include "quicksort.cpp"
#endif
