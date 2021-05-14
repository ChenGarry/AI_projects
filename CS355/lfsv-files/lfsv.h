/******************************************************************************/
/*!
\file   lfsv.h
\author Garry Chen
\par    DP email: garry.c\@digipen.edu
\par    DigiPen login: garry.c
\par    Course: CS355
\par    Assignment #03
\par    Section: A
\date   04/19/2021

\brief
  This is the header file for LFSV class.

*/
/******************************************************************************/
#include <iostream>       // std::cout
#include <atomic>         // std::atomic
#include <thread>         // std::thread
#include <vector>         // std::vector
#include <deque>          // std::deque
#include <mutex>          // std::mutex

struct Pair {
    std::vector<int>* pointer;
    long              ref_count;
    Pair() : pointer(nullptr), ref_count(1) {}
    Pair(std::vector<int>* pt, long r) : pointer(pt), ref_count(r) {}
}; // __attribute__((aligned(16),packed));
// for some compilers alignment needed to stop std::atomic<Pair>::load to segfault

class MemoryBank {
    std::deque< std::vector<int>* > slots;
    std::mutex m;
    public:
    MemoryBank() : slots(6000) {
        for ( int i=0; i<6000; ++i ) {
            slots[i] = reinterpret_cast<std::vector<int>*>( new char[ sizeof(std::vector<int>) ] );
        }
    }
    ~MemoryBank() {
        for ( auto pt : slots ) {
            delete [] reinterpret_cast<char *>(pt); 
        }
    }
    std::vector<int>* Get() {
        std::lock_guard<std::mutex> lock( m );
        auto back = slots.back();
        slots.pop_back();
        return back;
    }

    void Store(std::vector<int>* pt) {
        std::lock_guard<std::mutex> lock( m );
        slots.push_front(pt);        
    }
};

class LFSV {
    MemoryBank mb;
    std::atomic< Pair > pdata;
    public:

    LFSV() : mb(), pdata( Pair{ new ( mb.Get() ) std::vector<int>, 1 } ) {
    //    std::cout << "Is lockfree " << pdata.is_lock_free() << std::endl;
    }   

    ~LFSV() { 
        Pair temp = pdata.load();
        temp.pointer->~vector();
        mb.Store(temp.pointer);
    }

    void Insert( int const & v ) {
        Pair pdata_new, pdata_old;
        pdata_new.pointer = mb.Get();
        bool isIni = false;
        do {
            if(isIni) // reset vector if needed
                pdata_new.pointer->~vector();
            else
                isIni = true;

            pdata_old = pdata.load();
            pdata_old.ref_count = 1;
            pdata_new.pointer = new ( pdata_new.pointer ) std::vector<int>( *pdata_old.pointer );
            pdata_new.ref_count = 1;

            // working on a local copy
            std::vector<int>::iterator b = pdata_new.pointer->begin();
            std::vector<int>::iterator e = pdata_new.pointer->end();
            if ( b==e || v>=pdata_new.pointer->back() ) { pdata_new.pointer->push_back( v ); } //first in empty or last element
            else {
                for ( ; b!=e; ++b ) {
                    if ( *b >= v ) {
                        pdata_new.pointer->insert( b, v );
                        break;
                    }
                }
            }

        } while ( !(this->pdata).compare_exchange_weak( pdata_old, pdata_new  ));
        // clear and store old data
        pdata_old.pointer->~vector();
        mb.Store( pdata_old.pointer );
    }

    int operator[] ( int pos ) { // not a const method anymore
        Pair pdata_new, pdata_old;
        do { // before read - increment counter, use CAS
            pdata_old = pdata.load();
            pdata_new = pdata_old;
            ++pdata_new.ref_count;
        } while ( !(this->pdata).compare_exchange_weak( pdata_old, pdata_new  ));

        // counter is >1 now - safely read
        int ret_val = pdata_new.pointer->at(pos);

        do { // before return - decrement counter, use CAS
            pdata_old = pdata.load();
            pdata_new = pdata_old;
            --pdata_new.ref_count;
        } while ( !(this->pdata).compare_exchange_weak( pdata_old, pdata_new  ));

        return ret_val;
    }
};
