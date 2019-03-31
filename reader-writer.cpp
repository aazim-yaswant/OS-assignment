#include <bits/stdc++.h>
#include <pthread.h>
#include <unistd.h>
using namespace std;

pthread_mutex_t mutexid;

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


int num_read, num_write;

string text = "Considering this as the critical section";
int length; 

semaphore readMutex(1), writeMutex(1), readTry(1), resource(1), printLock(1);

void* Reader(void *arg) {
	while(true) {
		sleep(1);
		readTry.wait(); //indicate reader trying to read		
		readMutex.wait(); //lock entry section 
		
		num_read++;
		if(num_read == 1) {
			resource.wait();
		}
		
		readMutex.signal();
		readTry.signal();
		
		printLock.wait(); //lock for printing multiple lines together
		cout<<"Read the text: "<<text<<"\n";
		printLock.signal();
		
		length = text.length();
		
		printLock.wait();
		cout<<"Length of the text: "<<length<<"\n";
		printLock.signal();
		
		readMutex.wait();
		
		num_read--;
		if(num_read == 0) {
			resource.signal(); 
		}
		// Finally, if the current is the last reader, then locked resource is released
		readMutex.signal();
		
	}
}

void* Writer(void *arg) {
	/*
	The funtion locks out other readers/writers if the current instance is the FIRST writer/reader
	*/
	while(true) {
		sleep(1);
		
		writeMutex.wait();
		
		num_write++;
		
		if( num_write == 1) {
			readTry.wait(); 
		}
		
		writeMutex.signal();
		
		resource.wait();
		//critical section
		int length = text.length();
		int rindex = rand()%length;
		text = text.replace(rindex, 1, " OS_OS ");
		
		resource.signal();
		
		writeMutex.wait();
		
		num_write--;
		
		if( num_write == 0) {
			readTry.signal(); 
		}
		
		writeMutex.signal();
		
	}
}

int main() {
   /*
   The main function creates 2 reader and 1 writer threads using pthread_create and joins them with a terminated thread.
   */
   pthread_t threads[3]; 
   pthread_create(&threads[0], NULL, Reader, NULL);
   pthread_create(&threads[1], NULL, Reader, NULL);
   pthread_create(&threads[2], NULL, Writer, NULL);

   int i=0;
   while(i<3)
   	{
   		pthread_join(threads[i],NULL); 
   		i++;
   	}

}
