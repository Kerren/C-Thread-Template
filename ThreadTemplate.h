#ifndef THREAD_TEMPLATE_H
#define THREAD_TEMPLATE_H

#include <future>
#include <mutex>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>

using namespace std;

#ifndef PI_CONST
#define PI_CONST

const double PI( 4.0 * atan( 1.0 ) );

#endif

struct job
{
    job( const double amplitude, const double frequency, const double startTime, const double endTime, const unsigned int numIntervals ):
        A( amplitude ),
        f( frequency ),
        t0( startTime ),
        t1( endTime ),
        Ts( (t1 - t0) / ( static_cast< double >( numIntervals ) ) )
    {

    }
    const double A;
    const double f;
    const double t0;
    const double t1;
    const double Ts;
};

struct output
{
    output( const double result ):
        integral( result )
    {

    }
    const double integral;
};

void run( const unsigned int numThreads );
void loadJobs( vector< vector< job > >& jobs, const unsigned int numThreads );


vector< output > singleThread( const vector< job >& jobs, const unsigned int threadID, mutex& threadLock );

void print( const stringstream& message, mutex& threadLock );

#endif