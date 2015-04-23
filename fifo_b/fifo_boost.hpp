#ifndef FIFO_H
#define FIFO_H

#include <boost/circular_buffer.hpp>

template<class T>
class Fifo_b {

    public:
        Fifo_b(size_t size) {
            pBuffer = new boost::circular_buffer<T>(size); 
        }

        ~Fifo_b(){
            delete[] pBuffer;
        }

        void pop(T &value) {
          //  while (pBuffer->empty()){ pthread_cond_wait(&not_empty, &mutex); }
            value = pBuffer->front();
            pBuffer->pop_front();
        }

        bool pop_try(T &value) {
            if (pBuffer->empty()){
                //todo conditional
                return false;
            }
            value = pBuffer->front();
            pBuffer->pop_front();
            
            return true;
        }

        bool push(const T& Data) {
            if (pBuffer->full()){ 

                return false;
            }
            pBuffer->push_back(Data);
                //signal here

            return true;
        }

    private:
        boost::circular_buffer<T>*  pBuffer;
        //mutex and conditional variable

};

#endif // FIFO_H
