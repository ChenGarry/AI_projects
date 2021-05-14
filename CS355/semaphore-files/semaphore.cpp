/******************************************************************************/
/*!
\file   semaphore.cpp
\author Garry Chen
\par    DP email: garry.c\@digipen.edu
\par    DigiPen login: garry.c
\par    Course: CS355
\par    Assignment #02
\par    Section: A
\date   03/03/2021

\brief
  This is the implementation for semaphore class.

*/
/******************************************************************************/
#include "semaphore.h"

Semaphore::Semaphore(int max_thread): _max_thread(max_thread), _counter(max_thread)
{
}

void Semaphore::wait()
{
	std::unique_lock<std::mutex> lk(_m);	
	while(_counter == 0) { // blocked when there is no resources
    	_cv.wait(lk);
	}
	--_counter;
	lk.unlock();
}

void Semaphore::signal()
{
	std::unique_lock<std::mutex> lk(_m);
	if(_counter < _max_thread) {
		++_counter;
		if(_counter == 1)
			_cv.notify_one();
	}
	lk.unlock();
}