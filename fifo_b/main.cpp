#include "fifo_boost.hpp"
#include <iostream>

using namespace std;

struct testStruct {
    int a;
    int b;
    int c;
};

void * reader(void * arg){
    cout << "entering reader thread" << endl;
    Fifo_b<testStruct>* buf = reinterpret_cast<Fifo_b<testStruct>*>(arg); 
    testStruct outStruct;
    for (int i=0; i < 1000; i++){
        buf->pop(outStruct);
        cout << "pop " << outStruct.a << endl;
    }
}

void * writer(void * arg){
    cout << "entering writer thread" << endl;
    testStruct myTestStruct = { 1, 2, 3 };
    Fifo_b<testStruct>* buf = reinterpret_cast<Fifo_b<testStruct>*>(arg); 
    for (int i=0; i < 5000; i++){
        myTestStruct.a++;
        if (buf->push(myTestStruct)){
            cout << "push " << myTestStruct.a << endl;
        }
    }
}

int main(){

    cout << "main function" << endl; 
    Fifo_b<testStruct>* buf = new Fifo_b<testStruct>(-10);
    pthread_t writer_thread, reader_thread;

    void *status;

    pthread_create(&writer_thread, NULL, &writer, reinterpret_cast<void*>(buf));
    pthread_create(&reader_thread, NULL, &reader, reinterpret_cast<void*>(buf));

    pthread_join(writer_thread, &status);
    pthread_join(reader_thread, &status);

    pthread_exit(NULL);

    return 0;
}
