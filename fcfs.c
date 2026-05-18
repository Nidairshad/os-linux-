//fcfs 
//non preemptive scheduling algorithm
//tat =  ct -at
//wt = tat - bt

#include <stdio.h>

int main(){
   int n,i;
   printf("Enter number of processes");
   scanf("%d",&n);
  
   int p[n],at[n],bt[n],ct[n],tat[n],wt[n];
   float avgTAT=0,avgWT =0;
//initliaze process id
for(i = 0; i < n; i++) {
    p[i] = i + 1;
}   
   //input arrival time
   for( i = 0 ; i < n ; i ++){
       printf("enter arrival time of process P%d : ",i+1);
       scanf("%d",&at[i]);
  } 

   //input burst time
   for( i = 0 ; i < n ; i ++){
       printf("enter burst time of process P%d : ",i+1);
       scanf("%d",&bt[i]);
   }

   //check which will execute first
//sort according to arrival time of processes
   for( i = 0 ; i < n ; i ++){
       for(int j = i+1 ; j<n ; j++){
         if (at[i] > at[j]){
   //swap arrival time
    int temp = at[i];
    at[i] = at[j];
at[j] = temp;

//swap burst time
 int temp2 = bt[i];
bt[i] =bt[j];
bt[j] = temp2;

//swap process numbers
int temp3 = p[i];
p[i] = p[j];
p[j] = temp3;

}
   }
   }
int current_time = 0;

for(i = 0; i < n; i++) {
    if(current_time < at[i]) {
        current_time = at[i]; // CPU idle
    }

    ct[i] = current_time + bt[i];
    current_time = ct[i];

    tat[i] = ct[i] - at[i];
    wt[i] = tat[i] - bt[i];
}
//calculate tat
int totalTAT=0,totalWT = 0;


for(int i = 0 ; i <n ; i ++){
tat[i] = ct[i] - at[i];
totalTAT+=tat[i];
}
//avg
avgTAT = totalTAT/n;
//calculate waiting time
   for(int i = 0 ; i  <  n ; i ++){
      wt[i] =  tat[i] - bt[i];
      totalWT+=wt[i];
    }
avgWT=totalWT/n;
 
//display results:
printf("\nProcess\tAT\tBT\tCT\tTAT\tWT\n");

for(i = 0 ; i < n ; i++){
printf("P%d\t%d\t%d\t%d\t%d\t%d\n",p[i],at[i],bt[i],ct[i],wt[i],tat[i]);
}
return 0;
}
