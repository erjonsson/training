//-----------------------------------------------------------------------------
// fifo_threadsafe.h
// A sized size thread safe FIFO template class.
// 
// Author: Erik Jonsson
// ----------------------------------------------------------------------------
#ifndef FIFO_TS_H
#define FIFO_TS_H

#include <mutex>
#include <condition_variable>
#include <boost/circular_buffer.hpp>

template<class T>
class FifoThreadSafe {

  public:
    // Constructs a queue.
    // @param size - Size of queue.
    FifoThreadSafe (size_t size) {
      pBuffer = new boost::circular_buffer<T> {size}; 
    }

    ~FifoThreadSafe(){
      delete[] pBuffer;
    }

    // Dequeues the first item in the queue.
    // Waits for item to be available (a call to push)
    // if queue is empty.
    // @param item - First item in queue.
    void pop(T &item) {
      std::unique_lock<std::mutex> guard(fifo_ts_mutex);
      while (pBuffer->empty()) {
        buffer_not_empty.wait(guard);
      }
      item = pBuffer->front();
      pBuffer->pop_front();
    }

    // Dequeues the first item in the queue.
    // @param item - First item in queue.
    // @return - true if queue is non empty.
    bool pop_try(T &item) {
      std::lock_guard<std::mutex> guard(fifo_ts_mutex);
      if (pBuffer->empty()){
        return false;
      }
      item = pBuffer->front();
      pBuffer->pop_front();

      return true;
    }

    // Enqueues an item into the queue. 
    // @param T - item to be inserted into queue. 
    // @return - true if queue is non full.
    bool push(const T& item) {
      std::lock_guard<std::mutex> guard(fifo_ts_mutex);
      if (pBuffer->full()){ 
        return false;
      }
      pBuffer->push_back(item);
      buffer_not_empty.notify_one();

      return true;
    }

  private:
    std::mutex fifo_ts_mutex; 
    std::condition_variable buffer_not_empty;
    boost::circular_buffer<T>*  pBuffer;
};

#endif // FIFO_TS_H

