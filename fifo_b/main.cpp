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
  FifoThreadSafe<testStruct>* buf = reinterpret_cast<FifoThreadSafe<testStruct>*>(arg); 
  testStruct outStruct;
  for (int i=0; i < 100000; i++){
    buf->pop(outStruct);
    cout << "pop " << outStruct.a << endl;
  }
}

void * writer(void * arg){
  cout << "entering writer thread" << endl;
  testStruct myTestStruct = { 1, 2, 3 };
  FifoThreadSafe<testStruct>* buf = reinterpret_cast<FifoThreadSafe<testStruct>*>(arg); 
  for (int i=0; i < 10000; i++){
    myTestStruct.a++;
    if (buf->push(myTestStruct)){
      cout << "push " << myTestStruct.a << endl;
    }
  }
}

int main(){

  cout << "main function" << endl; 
  FifoThreadSafe<testStruct>* buf = new FifoThreadSafe<testStruct>{100};

  pthread_t writer_thread, reader_thread;

  void *status;

  pthread_create(&writer_thread, NULL, &writer, reinterpret_cast<void*>(buf));
  pthread_create(&reader_thread, NULL, &reader, reinterpret_cast<void*>(buf));

  pthread_join(writer_thread, &status);
  pthread_join(reader_thread, &status);

  pthread_exit(NULL);
  return 0;
}
