#include <ctime>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>
#include "td-4a-1.h"

// std::vector<std::unique_ptr<Thread> > th(nTasks)

class IncrThread : public Thread
{

private:
    unsigned int nLoops;
    double *pCounter;

public:
    IncrThread(unsigned int nLoops, double* pCounter) :
        nLoops(nLoops), pCounter(pCounter)
    {

    }
    ~IncrThread()
    {

    }

protected:
    void run()
    {
        for (unsigned int i = 0; i < nLoops; i++) {
            *pCounter += 1.0;
        }
    }

};


int main(int argc, char* argv[])
{
    unsigned int nTasks = 0, nLoops;
    double accum, nCounter;
    struct timespec start, stop;

    // Parse args
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " nLoops nTasks" << std::endl;
        return EXIT_FAILURE;
    }

    std::istringstream iss(argv[1]);
    iss >> nLoops;
    iss.clear();
    iss.str(argv[2]);
    iss >> nTasks;

    // Initialize threads
    std::vector<std::unique_ptr<Thread>> threads(nTasks);

    // Call incr and print with time measure
    if( clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
        perror( "clock gettime" );
        exit( EXIT_FAILURE );
    }

    // Create threads
    for (auto it = threads.begin(); it != threads.end(); it++) {
        it->reset(new IncrThread(nLoops, &nCounter));
    }

    // Start threads
    for (auto it = threads.begin(); it != threads.end(); it++) {
        (*it)->start();
    }

    // Join threads
    for (auto it = threads.begin(); it != threads.end(); it++) {
        (*it)->join();
    }

    if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) {
        perror( "clock gettime" );
        exit( EXIT_FAILURE );
    }

    // Print results
    accum = (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) * 1e-9;

    std::cout << "Counter value: " << nCounter << std::endl;
    std::cout << "Timeit: " << accum << "s" << std::endl;

    return EXIT_SUCCESS;
}
