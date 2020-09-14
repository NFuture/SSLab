#include<stdio.h>
#include<stdlib.h>
#define m 3
#define n 10
FILE *fout;
int bankAlg(int k,int max[m][n],int all[m][n]){//Bankers algorithm
    int ma=0,mb=0,mc=0,i,j;
    int p[n];
    int temp[m][n];

    for (int j = 0; j <= k; j++){
        for (i = 0; i < m; i++)
        {
            temp[i][j]=max[i][j]-all[i][j];
            if(!j)
                temp[i][j]*=-1;
        }
        ma+=all[0][j];
        mb+=all[1][j];
        mc+=all[2][j];
    }
   // printf("%d %d %d\n",ma,mb,mc);
    printf("\tNeed\nA\tB\tC");
    fprintf(fout,"\tNeed\nA\tB\tC");
    for (int j = 1; j <= k; j++){
        printf("\n");
        fprintf(fout,"\n");
        for (int i = 0; i < m; i++)
        {
            printf("%d\t",temp[i][j]);
            fprintf(fout,"%d\t",temp[i][j]);
            
        }
        
    }
   
    i=1;j=0;
    int fl=1;
    while (1){
        if(temp[0][i]<0) i++;
       // printf("%d %d %d\n",temp[0][i],temp[1][i],temp[2][i]);
       if(temp[0][i]<=temp[0][0] && temp[1][i]<=temp[1][0] && temp[2][i]<=temp[2][0]){
           temp[0][0]+=all[0][i];
           temp[1][0]+=all[1][i];
           temp[2][0]+=all[2][i];
           temp[0][i]=-1;temp[1][i]=-1;temp[2][i]=-1;
           p[j]=i-1;j++;
           fl=0;
       }
       if(j==5){fl=0;break;}
        i++;
        if(i==6){
            if(fl){ break;}
            i=1;
            fl=1;
        }
    }
    
    if(fl){
        printf("\n\nNo Safe Sequence\n");
        fprintf(fout,"\n\nNo Safe Sequence\n");
        return 1;
    }
    printf("\n\nSafe sequence:\t");
    fprintf(fout,"\n\nSafe sequence:\t");
    for(i=0;i<k;i++){
        printf("->P%d",p[i]);
        fprintf(fout,"->P%d",p[i]);
    }
    return 0;

}

void reqPro(int k,int max[m][n],int all[m][n]){//Function requesting allocation
    int a,b,c,p,i;
    printf("\n------------------------------------------\n");
    fprintf(fout,"\n------------------------------------------\n");
    printf("Select Process:\n");
    for(i=1;i<=k;i++)
        printf("%d.P%d\n",i,i-1);
    scanf("%d",&p);
    printf("Request allocation:\n");
    scanf("%d%d%d",&a,&b,&c);
    if(all[0][0]<a || all[1][0]<b || all[2][0]<c || (max[0][p]-all[0][p])<a || (max[1][p]-all[1][p])<b || (max[2][p]-all[2][p])<c ){
        printf("Request Denied\n");
        fprintf(fout,"Request Denied\n");
        return;
    }
    all[0][p]+=a;
    all[1][p]+=b;
    all[2][p]+=c;
    all[0][0]-=a;
    all[1][0]-=b;
    all[2][0]-=c;
    if(bankAlg(k,max,all)){
    printf("Request Denied\n");
    fprintf(fout,"Request Denied\n");
    }
}

void main(){
    FILE *fptr;
    int max[m][n],all[m][n];
    int k=0,a,b,c,ma=0,mb=0,mc=0,i;
    if ((fout = fopen("output.txt", "w")) == NULL) {
        printf("Error! opening file");
        exit(1);
    }
    if ((fptr = fopen("input.txt", "r")) == NULL) {
        printf("Error! opening file");
        exit(1);
    }

    i=0;
    while (!feof(fptr))
    {
       
        fscanf(fptr,"%d\t%d\t%d\t%d\t%d\t%d",&max[0][i],&max[1][i],&max[2][i],&all[0][i],&all[1][i],&all[2][i]);
        ma+=all[0][i];
        mb+=all[1][i];
        mc+=all[2][i];
        i++;
    }
    k=i-1;
    printf("------------------------------------------\n");
    fprintf(fout,"------------------------------------------\n");
    bankAlg(k,max,all);
    while(i){
        reqPro(k,max,all);
        printf("\nRequest again 1.Yes\t0.No:");
        scanf("%d",&i);
    }
}

//printf("%d%d%d\n",ma,mb,mc);
  /*printf("%d %d\t",max[0][i],all[0][i]);
    printf("%d %d\t",max[1][i],all[1][i]);
    printf("%d %d\t",max[2][i],all[2][i]);
    for (int j = 0; j < k; j++){
        printf("\n");
        for (int i = 0; i < m; i++)
        {
            printf("%d %d\t",max[i][j],all[i][j]);
        }
        
    }*/