#include <bits/stdc++.h>
#include <pthread.h>
#include <unistd.h>
using namespace std;

pthread_mutex_t mutexid;

int length = 10;
int array[10] = {1 , 2, 3, 4, 5, 6, 7, 8, 9, 10};

class semaphore {
   int count;
public:
   semaphore(int num){
   	  count = num;
   }
   void wait(){
       pthread_mutex_lock(&mutexid);         
       while(1){
           if(count>0){                
               count--;                
               break;                    
           }

       }
       pthread_mutex_unlock(&mutexid);       
   }
   void signal(){
       pthread_mutex_lock(&mutexid);        
           count++;                   
           pthread_mutex_unlock(&mutexid);  
       }

};

semaphore full(10), empty(0), lock(1);


void* producer(void *id) {
	while(true) {
		sleep(1);
		int output = rand() % 100; 
		
		empty.wait();
		lock.wait();
		
		array[length] = output;
		length++;
	
		lock.signal();
		full.signal();
		
		lock.wait(); 
		cout<<"Produced: "<<output<<" length: "<<length<<endl;
		lock.signal();
		 
	}	
}

void* consumer(void *id) {
	while(true) {
		int input;
		
		full.wait();
		lock.wait();
		
		input = array[length - 1];
		length--;
		
		lock.signal();
		empty.signal();
		
		lock.wait(); 
		cout<<"inputd: "<<input<<" length: "<<length<<endl;
		lock.signal();
		 
	}	
}


int main() {
   
   pthread_t thread[2];
   pthread_create(&thread[1], NULL, consumer, NULL);
   pthread_create(&thread[0], NULL, producer, NULL);

   int i=0;
   while(i<3){
   	   pthread_join(id[i],NULL);
   	   i++;
   	   }
}
