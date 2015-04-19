//fifo for music group

#include <queue>
#include <iostream>
//#define BOOST_TEST_MODULE MyTest
//#include <boost/test/unit_test.hpp>
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
        enum { default_size = 5 };
        explicit simple_cbuf(size_t size = default_size){
            pBuf = new T[size + 1];
            max_index = size;
            head = tail = 0;
        }

        ~simple_cbuf(){
            delete []pBuf;
        }

        void pop(){
            if (!empty()){
                tail = next(tail);
            }
        }
        bool pop_try(){
            if (empty()){ return false; }
            tail = next(tail);

            return true;
        }
        bool push(T new_value){
            if (full()){ return false; }

            pBuf[head] = new_value;
            head = next(head);
            
            return true;
        }

    private:
        T * pBuf;
        int  head,  tail, max_index; 
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

        int top() const{ return pBuf[tail]; }

};

int main(){


    testStruct myTestStruct = { 1, 2, 3 };

    cout << "main function" << endl; 
    simple_cbuf<testStruct> buffer(5);
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
#if 0
    pthread_t writer_thread, reader_thread;

    void *status;

    pthread_create(&writer_thread, NULL, &writer, NULL);
    pthread_create(&reader_thread, NULL, &reader, NULL);

    pthread_join(writer_thread, &status);
    pthread_join(reader_thread, &status);

    pthread_exit(NULL);
#endif


    return 0;
}
