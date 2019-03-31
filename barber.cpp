#include <bits/stdc++.h>
#include <pthread.h>
#include <unistd.h>
using namespace std;

pthread_mutex_t mid;
class semaphore {
   int count;
public:
   semaphore(int num){
   	  count = num;
   }
   void wait(){
       pthread_mutex_lock(&mid);         
       while(1){
           if(count>0){                
               count--;                
               break;                    
           }

       }
       pthread_mutex_unlock(&mid);       
   }
   void signal(){
       pthread_mutex_lock(&mid);        
           count++;                   
           pthread_mutex_unlock(&mid); 
       }

};

int num_free_seats = 10;
semaphore customers(0), barber(0), seat(1); 

void* Barber(void *arg) {
	while(true) {
/*
Seat denotes executing the action of sitting on a chair.
The barber waits for customers, on their arrival, he seats then
and obtains mutex on seat and frees is once done servicing them.
*/
		cout<<"Starting......\n";
		sleep(1);
		customers.wait(); 
		
		seat.wait(); 
		num_free_seats++; 
		seat.signal(); 
		
		barber.signal(); 
	}
}

void* Customer(void *arg) {
	while(true) {
		/*
		customer obtains mutex on seat upon arrival and gets added to the customer list
		He gets a chance and when done with the service, exits the shop.
		*/
		seat.wait(); 	
			
		if(num_free_seats > 0) {
			
			num_free_seats--;
			customers.signal(); 	
			seat.signal(); 
			
			barber.wait(); 
			cout<<"Got the cut\n";
			barber.signal();
			return NULL; 
		}	
		else {
			seat.signal();
		}
	}
}


int main() {
   /*
	This solution is for barber customer problem, with 10 customers and 1 barber. 
   */
   pthread_t threads[11];
   pthread_create(&threads[10], NULL, Barber, NULL);
   for(int i=0; i<10; i++)pthread_create(&threads[i], NULL, Customer, NULL);

   for(int i=0;i<11;i++)
   pthread_join(id[i],NULL); 

}
