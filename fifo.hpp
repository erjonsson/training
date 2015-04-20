#ifndef FIFO_H
#define FIFO_H

#include <pthread.h>

template<class T>
class simple_cbuf {
    public:
    enum { default_size = 5 };

    void pop(T &value);
    bool pop_try(T &value);
    bool push(T new_value);

    private:
    T * pBuf;
    int  head, tail, max_index; 
    pthread_cond_t not_empty;
    pthread_mutex_t mutex;

    bool wrapped();
    int next(int index) const;
    size_t size() const;
    bool empty();
    bool full();
};


#endif // FIFO_H
