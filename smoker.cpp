#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

pthread_mutex_t mid;
class semaphore {
   int counter;
public:
   semaphore(int num){
   	  counter = num;
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

semaphore lock(1), paper_matches(0), tobacco_matches(0), tobacco_paper(0), agent(0);

void* Agent(void *arg) {
while(true) {
	lock.wait();
	int random = rand() % 3;
	switch(random) {
		case 0:
			paper_matches.signal();
			break;
		case 1:
			tobacco_matches.signal();
			break;
		case 2:
			tobacco_paper.signal();
			break;
		default:
			cout<<"wrong call"<<endl;
	}
	lock.signal();
	agent.wait();
}	
}

void* tobacco(void *arg) {
	while(true) {
		paper_matches.wait();
	    lock.wait();
	    sleep(1); 
	    cout<<"tobacco kills \n";
	    agent.signal();
	    lock.signal();
	}
}

void* paper(void *arg) {
	while(true) {
		tobacco_matches.wait();
	    lock.wait();
	    sleep(1); 
	    cout<<"paper also kills \n";
	    agent.signal();
	    lock.signal();
	}
}

void* matches(void *arg) {
	while(true) {
		tobacco_paper.wait();
	    lock.wait();
	    sleep(1); 
	    cout<<"matches kill \n";
		agent.signal();
	    lock.signal();
	}
}


int main() {
   
   pthread_t id[4]; 
   pthread_create(&id[0], NULL, Agent, NULL);
   pthread_create(&id[1], NULL, matches, NULL);
   pthread_create(&id[2], NULL, paper, NULL);
   pthread_create(&id[3], NULL, tobacco, NULL);
   pthread_join(id[0],NULL); 
   pthread_join(id[1],NULL); 
   pthread_join(id[2],NULL); 
   pthread_join(id[3],NULL); 

}
