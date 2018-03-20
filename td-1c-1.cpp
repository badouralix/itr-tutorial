#include <climits>
#include <csignal>
#include <iostream>

unsigned int incr(const unsigned int, double*, volatile bool*);
void handler(int, siginfo_t*, void*);

int main(int argc, char* argv[])
{
    unsigned int iLoops = 0;
    const unsigned int nLoops = UINT_MAX;
    double counter = 0.0;
    volatile bool stop = false;

    // Define sigaction
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGRTMIN, &sa, NULL);

    // Define sigevent
    struct sigevent sev;
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;
    sev.sigev_value.sival_ptr = (void *) &stop;

    // Define timer
    timer_t tid;
    timer_create(CLOCK_REALTIME, &sev, &tid);
    itimerspec its;
    its.it_value.tv_sec = 1;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 0;

    // Start timer
    timer_settime(tid, 0, &its, NULL);
    iLoops = incr(nLoops, &counter, &stop);

    std::cout << "Loop interations: " << iLoops << std::endl;
    std::cout << "Counter value: " << counter << std::endl;

    // Start timer
    stop = false;
    timer_settime(tid, 0, &its, NULL);
    iLoops = incr(nLoops, &counter, &stop);

    std::cout << "Loop interations: " << iLoops << std::endl;
    std::cout << "Counter value: " << counter << std::endl;

    return EXIT_SUCCESS;
}

unsigned int incr(const unsigned int nLoops, double* pCounter, volatile bool* pStop)
{
    unsigned int i = 0;

    while (! *pStop && i < nLoops) {
        *pCounter += 1.0;
        i++;
    }

    return i;
}

void handler(int sig, siginfo_t* si, void*)
{
    bool* stop = (bool *) si->si_value.sival_ptr;
    *stop = true;
}
