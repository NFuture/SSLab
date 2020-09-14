#include<stdio.h>
#include<stdlib.h>
#define q 3




//Function for FCFS scheduling
void findFcfsTime(int n, int *at, int *bt, int *tat, int *wt){
    int cum=0,i;
    for (i=0; i < n; i++){
        *(tat+i) = abs(cum - *(at+i))+ *(bt + i);
        *(wt+i) = *(tat+i) - *(bt+i);
        cum += *(bt + i);
    }
}





//Function for priority scheduling
void findPrioTime(int n, int *at, int *bt, int *tat, int *wt, int *prio){
    int i=0,j=0,min,p,k=0;
    while(k<n){
    	p=-1;
    	min=1000;
    	for (i=0; i < n; i++){
    		
    		if(*(at+i)<=j && *(wt+i)<0){
    			if(*(prio+i)<min){
    				p=i;
    				min=*(prio+i);
				}
			}
		}
		if(p<0){
			j++;
			continue;
		}
	
		*(tat+p) = abs(j - *(at+p))+ *(bt + p);
        *(wt+p) = *(tat+p) - *(bt+p);
        j += *(bt + p);
        k++;
	}
}







//Function for round robin  scheduling
void findRRTime(int n, int *at, int *bt, int *tat, int *wt, int *prio){
    int *temp = (int *)malloc(n*sizeof(int));
    int cnt=0,j=0,i;
    for (i=0; i<n; i++){
        *(temp+i) = *(bt+i);
    }
    while(cnt<n){
    	for (i=0; i < n; i++){
    		if (*(at+i)<=j && *(at+i)>=0) {
                if (*(temp+i) <= q){
                    j += *(temp+i);
                    *(tat+i) = j - *(at+i);
                    *(wt+i)=*(tat+i)-*(bt+i);
                    *(at+i) = -1;
                    cnt++;
                } else {
                    j += q;
                    *(temp+i) -= q;
                }
            }
        }
        if (cnt >= n)
            break;
	}	
}










//Function for SJF scheduling
void findSjfTime(int n, int *at, int *bt, int *tat, int *wt){
    int i=0,j=0,min,p,k=0;
    while(k<n){
    	p=-1;
    	min=1000;
    	for (i=0; i < n; i++){
    	
    		if(*(at+i)<=j && *(wt+i)<0){
    			if(*(bt+i)<min){
    				p=i;
    				min=*(bt+i);
				}
			}
		}
		if(p<0){
			j++;
			continue;
		}
	
		*(tat+p) = abs(j - *(at+p))+ *(bt + p);
        *(wt+p) = *(tat+p) - *(bt+p);
        j += *(bt + p);
        k++;
	}
}







void main(){
    int n=0, *at, *bt, *tat, *wt, *prio;
    int a, b , c, i = 0;

	 // opening output.txt
	 FILE *fout;
    fout = fopen("output.txt", "w");
    if (fout == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    
	//opening input.txt
    FILE *fptr;
    if ((fptr = fopen("input.txt", "r")) == NULL) {
        printf("Error! opening file");
        exit(1);
    }
    while (!feof(fptr)){
        fscanf(fptr, "%d\t%d\t%d", &a, &b, &c);
        n++;
    }
    rewind(fptr);
	printf("Input\n");
	//allocating pointers
    at = (int*) malloc(n * sizeof(int));
    bt = (int*) malloc(n * sizeof(int));
    tat = (int*) malloc(n * sizeof(int));
    wt = (int*) malloc(n * sizeof(int));
    prio = (int*) malloc(n * sizeof(int));
    
    printf("Arrival Time\tBurst Time\tPriority\n");
    fprintf(fout,"Arrival Time\t\tBurst Time\t\tPriority\n");
    while (!feof(fptr)){
        fscanf(fptr, "%d\t%d\t%d", at+i, bt+i, prio+i);
        printf("%d\t\t%d\t\t%d\n",*(at+i),*(bt+i),*(prio+i));
        fprintf(fout, "%d\t\t\t\t%d\t\t\t\t%d\n", *(at+i),*(bt+i),*(prio+i));
        i++;        
    }
    printf("\n");
    for (i=0; i < n; i++){
        *(wt + i)=-1;
    }
    
    //calling fcfs schedulin g function
    findFcfsTime(n, at, bt, tat,wt);
    fprintf(fout,"FCFS\n");
    fprintf(fout,"Turn Arount Time\tWaiting Time\n");
    for (i=0; i < n; i++){
        fprintf(fout,"%d\t\t\t%d\n", *(tat + i), *(wt + i));
        *(wt + i)=-1;
    }
    fprintf(fout,"\n");
    
    //calling sjf scheduling function
    findSjfTime(n, at, bt, tat,wt);
    fprintf(fout,"SJF\n");
    fprintf(fout,"Turn Arount Time\tWaiting Time\n");
    for (i=0; i < n; i++){
        fprintf(fout,"%d\t\t\t%d\n", *(tat + i), *(wt + i));
        *(wt + i)=-1;
    }
    fprintf(fout,"\n");
    
    //calling priority scheduling function
    findPrioTime(n, at, bt, tat,wt,prio);
    fprintf(fout,"Priroty \n");
    fprintf(fout,"Turn Arount Time\tWaiting Time\n");
    for (i=0; i < n; i++){
        fprintf(fout,"%d\t\t\t%d\n", *(tat + i), *(wt + i));
        *(wt + i)=-1;
    }
    fprintf(fout,"\n");
    
    //calling round robin scheduling function
    findRRTime(n, at, bt, tat,wt,prio);
    fprintf(fout,"Round Robin\n");
    fprintf(fout,"Turn Arount Time\tWaiting Time\n");
    for (i=0; i < n; i++){
        fprintf(fout,"%d\t\t\t%d\n", *(tat + i), *(wt + i));
        *(wt + i)=-1;
    }
    
    
    
    //freeing allocated pointers
    free(at);
    free(bt);
    free(tat);
    free(wt);
    fclose(fptr);
}
