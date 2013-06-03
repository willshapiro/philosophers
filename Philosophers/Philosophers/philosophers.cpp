// Will Shapiro: Dining Philosophers
#include <iostream>
#include <vector>
#include <chrono>
#include <stdio.h>      
#include <stdlib.h>     
#include <time.h>  
using namespace std;

//declare the ints storing the number of philosophers and available forks
int num_philosophers;
int available_forks;
//create a vector to store all the forks as booleans (true=available, false=in use)
vector<bool> forks;


//Philosopher Class
class Philosopher {
    int hungry, eating, thinking, status; //status switches between hungry=1, eating=2, thinking=3
	int state, state_change; //state, state_change track how long a philosopher has been doing an activity
	
public:
    int number; //each philosopher is given a unique number
    Philosopher(int); //standard constructor
    Philosopher(); //default constructor
    void start_eating();
    void update();
    bool isHungry();
    
};



bool Philosopher::isHungry(){
	if(status==hungry){
		return true;
	}else{
		return false;
	}
}


void Philosopher::start_eating(){
	state_change = rand() % 7+3; //philosopher eats for between 3 and 9 minutes
	cout<<"philosopher "<<number<<" started eating for " <<state_change<<" seconds \n";
	state = 0;
	status = eating;
	
}

void Philosopher::update(){
	if(status!=hungry){
		state = state + 1; //philosophers state is incremented if eating or thinking
	}
	if(state>=state_change){
		
		state = 0;
		if(status==eating){
			state_change = rand() % 7+3; //philosopher thinks for between 3 and 9 minutes
            cout<<"philosopher "<<number<<" started thinking for " <<state_change<<" seconds \n";
            
            //available forks are increased by 2, and their booleans are set to true, indicating they
            //are ready to be used
			available_forks=available_forks+2; 
			forks[number] = true;
			forks[(number + 1)%num_philosophers] = true;
            
			status = thinking;
            
		}else if(status ==thinking){
			status = hungry;
			cout<<"philosopher "<<number<<" is hungry \n";
		}
	}
}

Philosopher::Philosopher(int num){
	number = num;
	hungry = 1;
	eating = 2;
	thinking = 3;
	status = 1;
	state= 0;
}

Philosopher::Philosopher(){
	number = 0;
	hungry = 1;
	eating = 2;
	thinking = 3;
	status = 1;
	
}



vector<Philosopher> philosophers; //initialize a vector of Philsophers
void wait ( int seconds); //wait sets the pace of the program 
bool num_philosophers_entered = false;

int main () {

	cout<<"Please entered the desired number of philosophers and forks \n";
	
	int t_num=0;
	while(!num_philosophers_entered){
		cin>>t_num;
		if(t_num<=0){ //user must enter a positive number of philosophers
            
		}else{
			t_num = abs(t_num);
			cout<<t_num<<" philosophers entered \n";
			num_philosophers = t_num;
			vector<Philosopher> philosophers(num_philosophers); //initialize philosophers vector
			available_forks = num_philosophers;
			vector<bool> forks(num_philosophers);
            
			num_philosophers_entered=true;
		}
        
	}

	
    //fill vectors storing philsophers (Philosophers) and forks (as booleans)
	for(int i = 0;i<num_philosophers;i++){
		philosophers.push_back(Philosopher(i));
		forks.push_back(true);
	}
    
	cout<<"there are "<<num_philosophers<<" num_philosophers and "<<available_forks<<" available forks \n";
    
    int t =0;
    while(t<100000){ //program runs for 100000 seconds
        
        
        wait(1); //pace the loop to run once a second
        srand (time(NULL)); //seed random variable with the current time
        int choice = rand() % philosophers.size(); //randomly choose first philosopher
        
        //o_choice and loop_complete ensure that each philosopher is offered forks at most once per second
        int o_choice = choice;
        bool loop_complete = false;
        
        
        while((available_forks>1)&&(!loop_complete)){
            //see if the randomly chosen philosopher is hungry and has both forks available
            if(forks[choice]&&forks[(choice+1)%forks.size()]&&philosophers[choice].isHungry()){
                forks[choice] = false;
                forks[(choice+1)%forks.size()] = false;
                philosophers[choice].start_eating();
                available_forks = available_forks - 2;
            }
            //if there are still at least 2 forks available, look at the next philosopher
            choice = (choice + 1)%philosophers.size();
            
            //if all philosophers have been looked at once and there are still >1 fork available, exit loop
            if(choice==o_choice){
                loop_complete = true;
                cout<<"loop complete \n";
            }
            
            
        }
        
        
        
        //update philosophers
        for(int i = 0;i<philosophers.size();i++){
            philosophers[i].update();
        }
        
        
        t++; //increment the time
        
    }
    return 0;
}


//waits for the desired number of seconds
void wait ( int seconds )
{
    clock_t endwait;
    endwait = clock () + seconds * CLOCKS_PER_SEC ;
    while (clock() < endwait) {}
}
