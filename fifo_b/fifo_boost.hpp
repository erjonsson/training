#ifndef FIFO_H
#define FIFO_H

#include <mutex>
#include <condition_variable>
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
            std::unique_lock<std::mutex> guard(fifo_b_mutex);
            while (pBuffer->empty()) {
                buffer_not_empty.wait(guard);
            }
            value = pBuffer->front();
            pBuffer->pop_front();
        }

        bool pop_try(T &value) {
            std::lock_guard<std::mutex> guard(fifo_b_mutex);
            if (pBuffer->empty()){
                return false;
            }
            value = pBuffer->front();
            pBuffer->pop_front();
            
            return true;
        }

        bool push(const T& Data) {
            std::lock_guard<std::mutex> guard(fifo_b_mutex);
            if (pBuffer->full()){ 
                return false;
            }
            pBuffer->push_back(Data);
            buffer_not_empty.notify_one();

            return true;
        }

    private:
        std::mutex fifo_b_mutex; 
        std::condition_variable buffer_not_empty;
        boost::circular_buffer<T>*  pBuffer;
        //mutex and conditional variable

};

#endif // FIFO_H
