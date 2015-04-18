//fifo for music group

#include <queue>
#include <iostream>
//#define BOOST_TEST_MODULE MyTest
//#include <boost/test/unit_test.hpp>
#include <pthread.h>

using namespace std;

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

        void pop(){
            if (!empty()){
                tail = next(tail);
            }
        }
        void push(int new_value){
            pBuf[head] = new_value;
            head = next(head);
        }
        
    private:
        int  head,  tail, max_index, * pBuf;
        bool wrapped() const { return  !(head >= tail); }
        int next(int index) const{
            if (index == max_index){
                index = 0;
            }else{
                index += 1;
            }
        }
};

#if 0
void *reader( void *){

    cout << "reader" << endl; 
}
void *writer( void *){

    cout << "writer" << endl; 
}
#endif

int main(){

    cout << "main function" << endl; 
    simple_cbuf<int> buffer(5);
    buffer.push(1);
    cout << "buffer push(1) " << endl;
    buffer.push(2);
    cout << "buffer push(2) " << endl;
    buffer.push(3);
    cout << "buffer push(3) " << endl;
    buffer.push(4);
    cout << "buffer push(4) " << endl;
    cout << "buffer full() " << buffer.full() << endl;
    buffer.push(5);
    cout << "buffer push(5) " << endl;
    cout << "buffer full() " << buffer.full() << endl;

#if 0
    cout << "buffer empty() " << buffer.empty() << endl;
    buffer.push(1);
    cout << "buffer push(1) " << endl;
    cout << "buffer empty() " << buffer.empty() << endl;
    buffer.push(2);
    cout << "buffer push(2) " << endl;
    buffer.push(3);
    cout << "buffer push(3) " << endl;
    cout << "buffer top() " << buffer.top() << endl;
    cout << "buffer size() " << buffer.size() << endl;
    buffer.pop();
    cout << "buffer pop() " << endl;
    cout << "buffer top() " << buffer.top() << endl;
    cout << "buffer push(4) " << endl;
    buffer.push(4);
    cout << "buffer top() " << buffer.top() << endl;
    cout << "buffer pop() " << endl;
    buffer.pop();
    cout << "buffer top() " << buffer.top() << endl;
    cout << "buffer pop() " << endl;
    buffer.pop();
    cout << "buffer top() " << buffer.top() << endl;
    cout << "buffer empty() " << buffer.empty() << endl;
    cout << "buffer pop() " << endl;
    buffer.pop();
    cout << "buffer empty() " << buffer.empty() << endl;
#endif
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
