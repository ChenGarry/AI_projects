/******************************************************************************/
/*!
\file   semaphore.h
\author Garry Chen
\par    DP email: garry.c\@digipen.edu
\par    DigiPen login: garry.c
\par    Course: CS355
\par    Assignment #02
\par    Section: A
\date   03/03/2021

\brief
  This is the header file for semaphore class.

*/
/******************************************************************************/
#include <mutex>				// mutex
#include <condition_variable>	// condition_variable

class Semaphore
{
public:
	void wait();
	void signal();
	Semaphore(int max_thread);
private:
	int _max_thread;
	int _counter;
	std::mutex _m;
	std::condition_variable _cv;
};
