
#include <stdio.h>

int main(){
   
int n,i;


printf("enter number of processes:\n");
scanf("%d",&n);

int p[n],at[n],bt[n],ct[n],tat[n],wt[n];
//initilize process id

for(i = 0 ; i  < n; i ++){
p[i]=i+1;
}

//enter arrival times of processes
for(i = 0 ; i < n; i++){
printf("enter at of p%d : ",i+1);
scanf("%d",&at[i]);
}

//enter burst time of processes
for(i = 0 ; i  < n; i ++){
printf("enter bt of p%d : ",i+1);
scanf("%d",&bt[i]);
}

//sort according to burst time of processes

int temp = 0 ;
for(i = 0 ; i < n ; i ++){
for(int j = i+1 ; j < n ; j++){
if(bt[i]>bt[j]){
temp = bt[i];
bt[i] = bt[j];
bt[j] = temp;

//swap  process num
temp = p[i];
p[i] =p[j];
p[j] = temp;

}
}
}
ct[0]= at[0] + bt[0];
//calculate ct
for(i = 1 ; i <  n ; i ++){
ct[i]=ct[i-1]+bt[i];
}
//calculate wt
wt[0] = 0;
for(i = 1 ; i<n ; i++){
wt[i] = wt[i-1] + bt[i-1];
}

//cal tat

for(i = 0 ; i <n ; i++){
tat[i] = wt[i] + bt[i];
}

//print 
printf("P\tAT\tBT\tCT\tTAT\tWT\n");
for(i = 0 ; i < n ; i ++){
printf("%d\t%d\t%d\t%d\t%d\t%d\n",i+1,at[i],bt[i],ct[i],tat[i],wt[i]);
}
return 0;
}


