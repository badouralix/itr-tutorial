#ifndef TD_4A_H
#define TD_4A_H

#include <pthread.h>

class Thread
{

private:
    pthread_t posixId;
    static void* call_run(void*);

public:
    Thread();
    Thread(pthread_t);
    void start();
    void join();
    ~Thread();

protected:
    virtual void run() = 0;

};

#endif // TD_4A_H