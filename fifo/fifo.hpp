#ifndef FIFO_H
#define FIFO_H

#include <pthread.h>

template<class T>
class Fifo {

    public:
        Fifo(size_t size): buffer_size(size), head(0), tail(0) {
            pBuffer = new T[size + 1];
        }

        ~Fifo(){
            delete[] pBuffer;
        }

        void pop(T &value) {
            pthread_mutex_lock(&mutex);
            while (empty()){ pthread_cond_wait(&not_empty, &mutex); }
            value = pBuffer[tail];
            tail = next(tail); 
            pthread_mutex_unlock(&mutex);
        }

        bool pop_try(T &value) {
            pthread_mutex_lock(&mutex);
            if (empty()){
                pthread_mutex_unlock(&mutex);

                return false;
            }
            value = pBuffer[tail];
            tail = next(tail);
            pthread_mutex_unlock(&mutex);
            
            return true;
        }

        bool push(T new_value) {
            pthread_mutex_lock(&mutex);
            if (full()){ 
                pthread_mutex_unlock(&mutex);

                return false;
            }
            pBuffer[head] = new_value;
            head = next(head);
            pthread_mutex_unlock(&mutex);
            pthread_cond_signal(&not_empty); 

            return true;
        }

    private:
        T * pBuffer;
        size_t  head, tail, buffer_size; 
        pthread_cond_t not_empty;
        pthread_mutex_t mutex;

        int next(int index) const {
            if (index == buffer_size) {
                index = 0;
            }else {
                index += 1;
            }
        }

        bool empty() const { return (tail == head); }
        bool full() const { return (tail == next(head)); }
};

#endif // FIFO_H
