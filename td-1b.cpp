#include <iostream>
#include <sstream>
#include <ctime>

void incr(unsigned int nLoops, double* pCounter)
{
    for (unsigned int i = 0; i < nLoops; i++) {
        (*pCounter)++;
    }
}

int main(int argc, char* argv[])
{
    unsigned int nLoops = 0;
    double accum, counter = 0.0;
    struct timespec start, stop;

    // Parse args
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " nLoops" << std::endl;
        return EXIT_FAILURE;
    }

    std::istringstream iss(argv[1]);
    iss >> nLoops;

    // Call incr and print with time measure
    // See timeit: https://users.pja.edu.pl/~jms/qnx/help/watcom/clibref/qnx/clock_gettime.html
    if( clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
        perror( "clock gettime" );
        exit( EXIT_FAILURE );
    }

    incr(nLoops, &counter);

    if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) {
        perror( "clock gettime" );
        exit( EXIT_FAILURE );
    }

    accum = (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) * 1e-9;

    std::cout << "Counter value: " << counter << std::endl;
    std::cout << "Timeit: " << accum << "s" << std::endl;

    return EXIT_SUCCESS;
}
