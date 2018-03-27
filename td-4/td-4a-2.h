#ifndef TD_4A_H
#define TD_4A_H

#include <pthread.h>

class Thread
{

private:
    pthread_t posixId;
    pthread_attr_t posixAttr;
    static void* call_run(void*);

public:
    Thread(int = SCHED_OTHER);
    Thread(pthread_t, int = SCHED_OTHER);
    void setSchedPolicy(int);
    void start();
    void join();
    bool join(double);
    static void sleep_ms(double);
    ~Thread();

protected:
    virtual void run() = 0;

};

#endif // TD_4A_H