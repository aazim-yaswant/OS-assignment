#include <bits/stdc++.h>
#include <pthread.h>
#include <unistd.h>
using namespace std;

pthread_mutex_t mid;
class semaphore {
   int counter;
public:
   semaphore(){
   	  counter = 1;
   }
   void wait(){
       pthread_mutex_lock(&mid);        
       while(1){
           if(counter>0){             
               counter--;              
               break;                   
           }

       }
       pthread_mutex_unlock(&mid);      
   }
   void signal(){
       pthread_mutex_lock(&mid);     
           counter++;                
           pthread_mutex_unlock(&mid); 
       }

};
semaphore chopsticks[5], lock;
int phil = 0;

void* philosopher(void *arg) {
    int num;
    lock.wait();
    num = phil;	
    phil++;
    lock.signal();
    
    while(true) {
		sleep(2);
		
		cout<<num<<" thinking"<<endl;
		
		sleep(2);
		
		cout<<num<<" hungry"<<endl;
		
		//We will asymmetrically request the chopsticks for even or odd numbered philosophers
		
		if(num % 2 == 0) {
			chopsticks[(num + 1)%5].wait();
			chopsticks[num%5].wait();
		}
		else {
			chopsticks[num%5].wait();
			chopsticks[(num + 1)%5].wait();
		}
		
		cout<<"num"<<" eating"<<endl;
		
		chopsticks[num%5].signal();
		chopsticks[(num+1) % 5].signal();
		
	}
	
}


int main() {
   
   pthread_t id[5]; //5 philosophers
   pthread_create(&id[1], NULL, philosopher, NULL);
   pthread_create(&id[2], NULL, philosopher, NULL);
   pthread_create(&id[3], NULL, philosopher, NULL);  
   pthread_create(&id[4], NULL, philosopher, NULL);
   pthread_create(&id[0], NULL, philosopher, NULL);
   for(int i=0;i<5;i++)
   pthread_join(id[i],NULL);

}
