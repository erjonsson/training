//fifo 

#include <iostream>
#include <pthread.h>

using namespace std;

struct testStruct {
    int a;
    int b;
    int c;
};

template<class T>
class simple_cbuf {
    public:
        explicit simple_cbuf(size_t size = default_size){
            pBuf = new T[size + 1];
            max_index = size;
            head = tail = 0;
        }

        ~simple_cbuf(){
            delete []pBuf;
        }

        void pop(T &value){
            pthread_mutex_lock(&mutex);
            while (empty()){ pthread_cond_wait(&not_empty, &mutex); }
            value = pBuf[tail];
            tail = next(tail); 
            pthread_mutex_unlock(&mutex);
            
        }
        bool pop_try(T &value){
            if (empty()){ return false; }
            pthread_mutex_lock(&mutex);
            value = pBuf[tail];
            tail = next(tail);
            pthread_mutex_unlock(&mutex);
            return true;
        }
        bool push(T new_value){
            if (full()){ return false; }
            pthread_mutex_lock(&mutex);
            pBuf[head] = new_value;
            head = next(head);
            pthread_mutex_unlock(&mutex);
            pthread_cond_signal(&not_empty); 
            return true;
        }

    private:
        
        bool wrapped() const { return  !(head >= tail); }
        int next(int index) const{
            if (index == max_index){
                index = 0;
            }else{
                index += 1;
            }
        }
        size_t size() const{
            if (!wrapped()){
                return head - tail;
            }else{ // head has wrapped
                return (max_index - head + 1) + tail;
            }
        }

        bool empty() const{ return (tail == head); }
        bool full() const{ return (tail == next(head)); }
};

void * reader(void * arg){
    cout << "entering reader thread" << endl;
    simple_cbuf<testStruct>* buf = reinterpret_cast<simple_cbuf<testStruct>*>(arg); 
    testStruct outStruct;
    for (int i=0; i < 1000; i++){
        buf->pop(outStruct);
        cout << "pop " << outStruct.a << endl;
    }
}

void * writer(void * arg){
    cout << "entering writer thread" << endl;
    testStruct myTestStruct = { 1, 2, 3 };
    simple_cbuf<testStruct>* buf = reinterpret_cast<simple_cbuf<testStruct>*>(arg); 
    for (int i=0; i < 1000000; i++){
        myTestStruct.a++;
        if (buf->push(myTestStruct)){
            cout << "push " << myTestStruct.a << endl;
        }
    }
}

int main(){

    cout << "main function" << endl; 
    simple_cbuf<testStruct>* buf = new simple_cbuf<testStruct>(100);
    pthread_t writer_thread, reader_thread;

    void *status;

    pthread_create(&writer_thread, NULL, &writer, reinterpret_cast<void*>(buf));
    pthread_create(&reader_thread, NULL, &reader, reinterpret_cast<void*>(buf));

    pthread_join(writer_thread, &status);
    pthread_join(reader_thread, &status);

    pthread_exit(NULL);

    return 0;
}
#if 0
    cout << "buffer push(testStruct) " << buffer.push(myTestStruct) << endl;
    cout << "buffer push(testStruct) " << buffer.push(myTestStruct) << endl;
    cout << "buffer push(testStruct) " << buffer.push(myTestStruct) << endl;
    cout << "buffer push(testStruct) " << buffer.push(myTestStruct) << endl;
    cout << "buffer push(testStruct) " << buffer.push(myTestStruct) << endl;
    cout << "buffer push(testStruct) " << buffer.push(myTestStruct) << endl;
    cout << "buffer push(testStruct) " << buffer.push(myTestStruct) << endl;
    cout << "buffer pop_try() " << buffer.pop_try() << endl;
    cout << "buffer pop_try() " << buffer.pop_try() << endl;
    cout << "buffer pop_try() " << buffer.pop_try() << endl;
    cout << "buffer pop_try() " << buffer.pop_try() << endl;
    cout << "buffer pop_try() " << buffer.pop_try() << endl;
    cout << "buffer pop_try() " << buffer.pop_try() << endl;
    cout << "buffer pop_try() " << buffer.pop_try() << endl;
    cout << "buffer pop_try() " << buffer.pop_try() << endl;
#endif
