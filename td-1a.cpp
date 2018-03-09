#include <iostream>
#include <signal.h>
#include <time.h>

void handler(int sig, siginfo_t* si, void*)
{
    int* counter = (int *) si->si_value.sival_ptr;
    std::cout << "Tick: " << *counter << std::endl;
    *counter += 1;
}

int main(int argc, char *argv[])
{
    // Define counter
    volatile int counter = 0;

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
    sev.sigev_value.sival_ptr = (void *) &counter;

    // Define timer
    timer_t tid;
    timer_create(CLOCK_REALTIME, &sev, &tid);
    itimerspec its;
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = 500 * 1000 * 1000;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 500 * 1000 * 1000;

    // Start timer
    timer_settime(tid, 0, &its, NULL);

    // Wait until counter reaches 16
    while (counter < 16) { }

    // Delete timer
    timer_delete(tid);

    // Return
    return 0;
}
