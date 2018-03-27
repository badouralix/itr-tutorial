#include <ctime>
#include <iostream>
#include <pthread.h>
#include "td-4a-2.h"

Thread::Thread(int schedPolicy)
{
    pthread_attr_init(&posixAttr);
    pthread_attr_setinheritsched(&posixAttr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&posixAttr, schedPolicy);
}

Thread::Thread(pthread_t posixId, int schedPolicy) : posixId(posixId)
{
    pthread_attr_init(&posixAttr);
    pthread_attr_setinheritsched(&posixAttr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&posixAttr, schedPolicy);
}

void Thread::setSchedPolicy(int schedPolicy)
{
    pthread_attr_setschedpolicy(&posixAttr, schedPolicy);

    // see http://www.yonch.com/tech/82-linux-thread-priority
    if (pthread_setschedparam(posixId, schedPolicy, NULL) != 0) {
         std::cout << "Unsuccessful in setting thread realtime prio" << std::endl;
         return;
    }
}

void Thread::start()
{
    pthread_create(&posixId, &posixAttr, call_run, this);
}

void Thread::join()
{
    pthread_join(posixId, NULL);
}

bool Thread::join(double timeout_ms)
{
    struct timespec ts;

    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_nsec += (int) (timeout_ms * 1e6);
    ts.tv_sec += (int) (ts.tv_nsec * 1e-9);
    ts.tv_nsec %= (int) 1e9;

    return (bool) pthread_timedjoin_np(posixId, NULL, &ts);
}

void Thread::sleep_ms(double delay_ms)
{
    struct timespec ts, remain;

    remain.tv_sec = (int) (delay_ms * 1e-3);
    remain.tv_nsec = (int) (delay_ms * 1e6) % (int) 1e9;

    while (remain.tv_sec > 0 || remain.tv_nsec > 0) {
        ts.tv_sec = remain.tv_sec;
        ts.tv_nsec = remain.tv_nsec;

        nanosleep(&ts, &remain);
    }

}

void Thread::run()
{

}

void* Thread::call_run(void* v_thread)
{
    Thread* thread = (Thread*) v_thread;
    thread->run();

    return NULL;
}

Thread::~Thread()
{
    pthread_attr_destroy(&posixAttr);
}
