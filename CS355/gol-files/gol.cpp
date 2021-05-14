/******************************************************************************/
/*!
\file   gol.cpp
\author Garry Chen
\par    DP email: garry.c\@digipen.edu
\par    DigiPen login: garry.c
\par    Course: CS355
\par    Assignment #01
\par    Section: A
\date   02/17/2021

\brief
  This is the implementation for gmae of life run function.

*/
/******************************************************************************/
#include "gol.h"
#include <pthread.h>    // pthread_t, pthread_mutex_t
#include <semaphore.h>  // sem_t

namespace { //
// define cell states
#define DEAD  0
#define ALIVE 1

// shared data between threads
struct shared_data
{
    std::vector< bool > &_board;
    const int _num_iter;
    const int _max_x;
    const int _max_y;
    const int _cell_count;
    int _counter = 0;
    shared_data(std::vector<bool> &board, int num_iter, int max_x, int max_y): 
        _board(board), 
        _num_iter(num_iter), 
        _max_x(max_x), 
        _max_y(max_y),
        _cell_count(max_x * max_y),
        _counter(0)
        { }
};

// cell data of each thread
struct cell_data
{
    shared_data &_shared;
    int _index;
    int _neighbors[8];
    int _neighbors_count = 0;
    cell_data(shared_data &shared) : _shared(shared){}
    // pre calculate neighbor index of given cell
    void init(int index) 
    {
        // boundary flags
        bool up = false;
        bool down = false;
        bool left = false;
        bool right = false;
        _index = index;
        // find legal neighbors
        if(_index - _shared._max_x >= 0) {
            _neighbors[_neighbors_count] = _index - _shared._max_x;
            ++_neighbors_count;
            up = true;
        }
        
        if(_index + _shared._max_x < _shared._cell_count) {
            _neighbors[_neighbors_count] = _index + _shared._max_x;
            ++_neighbors_count;
            down = true;
        }

        if(_index % _shared._max_x != 0) {
            _neighbors[_neighbors_count] = _index - 1;
            ++_neighbors_count;
            left = true;
        }

        if((_index +1) % _shared._max_x != 0) {
            _neighbors[_neighbors_count] = _index + 1;
            ++_neighbors_count;
            right = true;
        }

        if(up && left) {
            _neighbors[_neighbors_count] = _index - _shared._max_x - 1;
            ++_neighbors_count;
        }

        if(up && right) {
            _neighbors[_neighbors_count] = _index - _shared._max_x + 1;
            ++_neighbors_count;
        }

        if(down && left) {
            _neighbors[_neighbors_count] = _index + _shared._max_x - 1;
            ++_neighbors_count;
        }

        if(down && right) {
            _neighbors[_neighbors_count] = _index + _shared._max_x + 1;
            ++_neighbors_count;
        }
    }
};

// barrier class using pthread mutex and semaphore
class PBarrier {
    sem_t _sem_wait;
    sem_t _sem_release;
    int _goal;
    int _counter;
public:
    PBarrier(int goal): _goal(goal) ,_counter(0){
        sem_init(&_sem_wait, 0, 1);
        sem_init(&_sem_release, 0, 0);
    }
    // wait for counter to reach goal count, then release all threads.
    void wait() { 
        sem_wait(&_sem_wait);
        ++_counter;
        if(_counter == _goal) { 
            // singal to release 
            // _sem_wait stay 0 to block next batch's release wait
            sem_post(&_sem_release);
        }
        else
            sem_post(&_sem_wait);

        sem_wait(&_sem_release);
        --_counter;
        if(_counter == 0) {
            // singal next batch to start joining wating
            // _sem_release stay 0 to block next batch's release
            sem_post(&_sem_wait);
        }
        else
            sem_post(&_sem_release);
    }
    void set_goal(int goal) { _goal = goal; }
};

void *update_cell( void *ptr );

pthread_mutex_t write_mutex = PTHREAD_MUTEX_INITIALIZER;
PBarrier barrier(0);
} // namespace

#include <iomanip>
std::vector< std::tuple<int,int> > // return vector of coordinates of the alive cells of the final population
run( std::vector< std::tuple<int,int> > initial_population, int num_iter, int max_x, int max_y )
{
    int cell_count = max_x * max_y;
    std::vector<pthread_t> threads(cell_count);
    std::vector<bool> board(cell_count, DEAD);
    shared_data shared(board, num_iter, max_x, max_y);
    std::vector<cell_data> cells(cell_count, shared);
    barrier.set_goal(cell_count);

    // transform board from sparse format to 1d array
    for(auto const& e: initial_population) {
        board[std::get<1>(e) * max_x + std::get<0>(e)] = ALIVE;
    }
    // initialize cell data and create threads
    for(int i = 0; i < cell_count; ++i) {
        cells[i].init(i);
        pthread_create(&threads[i], NULL, update_cell, static_cast<void*>(&cells[i]));
    }

    for(int i = 0; i < cell_count; ++i ) {
        pthread_join( threads[i], 0 );
    }
    // transform board from 1d array to sparse format
    std::vector< std::tuple<int,int> > final_population;
    for(int i = 0; i < cell_count; i++ ) {
        if(board[i] == ALIVE) {
            final_population.push_back({i % max_x, i / max_y});
        }
    }
    return final_population;
}

namespace { // namespace

void *update_cell( void *ptr )
{
    cell_data* data = static_cast<cell_data*>(ptr);
    shared_data & shared = data->_shared;

    for(int i = shared._num_iter; i > 0; --i)
    {        
        // read start
        int neighbors_alive = 0;
        bool old_state = shared._board[data->_index];
        bool new_state = DEAD;
        // check live neighbors
        for(int i = data->_neighbors_count-1; i >= 0; --i) {
            if(shared._board[data->_neighbors[i]] == ALIVE)
                ++neighbors_alive;
        }
        // update cell
        if ( neighbors_alive == 3 || (neighbors_alive == 2 && old_state == ALIVE) ) { 
            new_state = ALIVE;
        }
        // wait all read
        barrier.wait(); 

        // write start
        pthread_mutex_lock(&write_mutex);
        shared._board[data->_index] = new_state;
        pthread_mutex_unlock(&write_mutex);

        // wait all write
        barrier.wait(); 
    }
    
    return ptr;
}
} // namespace


