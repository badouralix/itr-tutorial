#include <pthread.h>
#include "td-4a-1.h"

Thread::Thread()
{

}

Thread::Thread(pthread_t posixId) : posixId(posixId)
{

}

void Thread::start()
{
    pthread_create(&posixId, NULL, call_run, this);
}

void Thread::join()
{
    pthread_join(posixId, NULL);
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

}
