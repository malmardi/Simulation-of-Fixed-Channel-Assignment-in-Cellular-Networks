#include <iostream>
#include <stdlib.h>  // for rand(), srand()
#include <math.h>
using namespace std;

int const END= 3000 ; // simulation end time (msec); 
int const SEED= 1234567;
double clock =0.0;  // simulation clock time
int const N=2;// # of channels
int const users=2; // # of users in the system
void initialize()
{
	srand(SEED); // set seed for random number generator
}
double random()
{
	
	return ( (double) (rand()) /(double) RAND_MAX);

}

double exponential (double rate)
{
	return( (-1.0/rate)*log( random()));
}

void sort(double arival_time[],int users)
{
	users;
	int i,j;
	double temp;
	for(i=0;i<users;i++)
	{		
		for(j=i+1;j<users;j++)
		{
			if(arival_time[i]>arival_time[j])
			{
				temp=arival_time[i];
				arival_time[i]=arival_time[j];
				arival_time[j]=temp;
			}//end if
			
		}//end nested for
	}//end for
		
 }//end function

int emptyCell(double channels[])
{
	int i;
int empty_cell=0;
/*we need to check if there is available channel:
the channel is considered idle, when the channel[i]=0 or channel[i]<=clock (i.e, finish time is less than the clock) */ 
for( i=0;i<N;i++)
{
if(channels[i]==0 || channels[i]<= clock)//if this condition is true, then we need to find any idle channel to allocate it.
{
empty_cell=i;// empty_cell represent any idle channel
break;
}//end if
else
{
continue;
}//end else
}//end for

if (i<N)
return i;
else
return N;

//in the previous loop, if the value of i reaches N, this means that all channels are reserved, and the call should be blocked
}//end function

void main()
{
//int N;//number of channels in the system.
//int users; //number of users in the system.
//cout<<"enter the number of channels"<<endl;
double arrival_rate =0.1; // arrival rate at each user
//cin>>N;
//cin>>users;
for(int k=0;k<50;k++)//this loop is to change the arrival rate and see the changes
{
int served_users=0;// number of calls served during the simulation time.
//int active_calls_in_system=0;
int empty_cell=0;// points to any of the idle channels.
int blocked_cells=0;//number of blocked cells(when all channels are reserved).

double service_rate =1;
double service_time=0.0;
double finish_time=0.0;
//arrays used in the system:
double arrival_time[users]={0.0};// maintain the arrival times for users.
double channels[N]={0.0};//represent the channels in the system and contains the finishing(departure time for calls).

// first step is to initialize the arrival times for the users in the system:
for(int i=0; i<users;i++)
arrival_time[i]=exponential(arrival_rate);

//we need to sort the arrival times in ascending order.
sort(arrival_time,users);
clock=arrival_time[0];/*since the arrival_time array is arranged in ascending order, and the system is discrete-event, then the clock= least 
arrival time.  */
while(clock<END)
{
empty_cell=emptyCell(channels);
if(empty_cell==N)
{
blocked_cells++;
arrival_time[0]+=exponential(arrival_rate);
}//end if
else // when the value of i< N this means that there is an empty cell and we can serve the current call.
{
service_time=exponential(service_rate); // to compute the call duration.
finish_time=arrival_time[0]+service_time;// to compute when the call will be released.
arrival_time[0]=finish_time+exponential(arrival_rate);/* to compute the next arrival time for user i. Note a user can only make another call  
                                                                           when he finishes the first call */
served_users++;
channels[empty_cell]=finish_time;
}//end else
sort(arrival_time,users);/*we need to sort the array again because the next arrival time of one of the users maybe less than the first arrival 
time of other users*/
clock=arrival_time[0];

}//end while


cout<<arrival_rate<<"       "<<served_users<<"        "<<blocked_cells<<"       "<<(double)blocked_cells*100/(double)(blocked_cells+served_users)<<" % "<<endl;

arrival_rate+=0.3;}
}//end main