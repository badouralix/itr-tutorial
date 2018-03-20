#include <ctime>
#include <iostream>
#include <sstream>
#include <pthread.h>
#include <vector>

struct Param {
    unsigned int nLoops;
    double nCounter;
};

void incr(unsigned int nLoops, double* pCounter)
{
    for (unsigned int i = 0; i < nLoops; i++) {
        *pCounter += 1.0;
    }
}

void* call_incr(void* p)
{
    Param* param = (Param *) p;

    incr(param->nLoops, &param->nCounter);

    return NULL;
}

int main(int argc, char* argv[])
{
    unsigned int nTasks = 0;
    double accum;
    struct timespec start, stop;
    Param param;

    // Parse args
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " nLoops nTasks" << std::endl;
        return EXIT_FAILURE;
    }

    std::istringstream iss_loops(argv[1]);
    iss_loops >> param.nLoops;

    std::istringstream iss_tasks(argv[2]);
    iss_tasks >> nTasks;

    // Initialize threads
    std::vector<pthread_t> threads(nTasks);

    // Call incr and print with time measure
    // See timeit: https://users.pja.edu.pl/~jms/qnx/help/watcom/clibref/qnx/clock_gettime.html
    if( clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
        perror( "clock gettime" );
        exit( EXIT_FAILURE );
    }

    for (std::vector<pthread_t>::iterator it = threads.begin(); it != threads.end(); it++) {
        pthread_create(&(*it), NULL, call_incr, &param);
    }

    for (std::vector<pthread_t>::iterator it = threads.begin(); it != threads.end(); it++) {
        pthread_join(*it, NULL);
    }

    if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) {
        perror( "clock gettime" );
        exit( EXIT_FAILURE );
    }

    // Print results
    accum = (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) * 1e-9;

    std::cout << "Counter value: " << param.nCounter << std::endl;
    std::cout << "Timeit: " << accum << "s" << std::endl;

    return EXIT_SUCCESS;
}
