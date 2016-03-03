#include "ThreadTemplate.h"

/**
 * This is the function that would be called in main and from here it'll run the rest of the code and get the final output.
 * @param numThreads The number of threads that you want your code to have.
 */
void run( const unsigned int numThreads )
{
    vector< vector< job > > jobs;
    loadJobs( jobs, numThreads );

    // The jobs are now loaded and the threads can be created

    mutex threadLock;
    vector< future< vector< output > > > threads;
    for ( unsigned int i = 0; i < numThreads; i++ )
    {
        threads.push_back( std::async(std::launch::async, singleThread, ref( jobs.at( i ) ), i, ref( threadLock ) ) );
    }

    // The threads have been created and are running

    vector< vector< output > > results;
    for ( unsigned int i = 0; i < numThreads; i++ )
    {
        results.push_back( threads.at( i ).get() ); // It'll wait at this point until the threads are done
    }

    // We can now output the results to a file or something

    ofstream outFile( "output.txt" );

    for ( const auto& vec : results )
    {
        for ( const auto& out : vec )
        {
            outFile << out.integral << endl;
        }
    }

    // It's now complete!

}

/**
 * This function can be modified to take in other information like a filename or something. The main idea here is to create the jobs for each thread. It's really IMPORTANT to understand that there's overhead in creating threads so if possible you should only create them once. Instead of creating a new thread for each job, rather create a thread that takes all of the jobs it needs to finish and runs sequentially.
 * @param jobs       The jobs for each thread.
 * @param numThreads The number of threads.
 */
void loadJobs( vector< vector< job > >& jobs, const unsigned int numThreads )
{
    // Initialize the jobs vector
    jobs.clear();
    for ( unsigned int i = 0; i < numThreads; i++ )
    {
        jobs.push_back( vector< job >() );
    }

    const int numJobs = 10000; // I'm just using this as an example, in most cases you'll be reading from a file or something!
    for ( unsigned int i = 0; i < numJobs; i++ )
    {
        double A = abs( static_cast< double >( rand() )/static_cast< double >( rand() ) );
        double f = abs( static_cast< double >( rand() )/static_cast< double >( rand() ) );
        double t0 = abs( static_cast< double >( rand() )/static_cast< double >( rand() ) );
        double t1 = t0 + ( rand() % 3 );
        unsigned int N = rand() % 500;
        job generatedJob( A, f, t0, t1, N );
        jobs.at( i % numThreads ).push_back( generatedJob ); //This spreads the jobs over all of the threads
    }
}

vector< output > singleThread( const vector< job >& jobs, const unsigned int threadID, mutex& threadLock )
{
    vector< output > result;

    stringstream message;
    message << "This is thread " << threadID << " I'm starting up!" << endl;
    print( message, threadLock );

    const unsigned int tenPercent = floor( jobs.size() / 100.0 );

    for ( unsigned int i = 0; i < jobs.size(); i++ )
    {
        const job& singleJob = jobs.at( i );
        double integral = 0.0;
        const double& t0 = singleJob.t0;
        const double& t1 = singleJob.t1;
        const double& Ts = singleJob.Ts;
        const double& A = singleJob.A;
        const double& f = singleJob.f;
        for ( double t = t0 + Ts; t <= t1; t += Ts )
        {
            integral += Ts * ( A*sin( 2.0 * PI * f * t ) + A*sin( 2.0 * PI * f * (t - Ts) ) ) / 2.0;
        }

        result.push_back( output( integral ) );

        if ( i % tenPercent == 0 )
        {
            message.str("");
            message << "This is thread " << threadID << ", I am now " << static_cast< double >( i )/static_cast< double >( jobs.size() ) * 100.0 << "% complete!" << endl;
            print( message, threadLock );
        }
    }

    return result;
}


/**
 * Make sure only one thread prints to terminal at a time
 * @param message    The message that must be printed
 * @param threadLock Mutually exclusive thread lock
 */
void print( const stringstream& message, mutex& threadLock )
{
    threadLock.lock();
    cout << message.str() << endl;
    threadLock.unlock();
}