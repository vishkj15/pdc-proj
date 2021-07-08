#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<math.h>
#include<time.h>
#include<stdbool.h>
#include <omp.h>

#define CHUNKSIZE 10
int main(void)
{ int INF=99999999;
  omp_set_num_threads(7);
  int  tid, chunk=CHUNKSIZE;
  FILE* file = fopen ("st70.txt", "r");
  int i = 0,j=0 ;
  int a[3000];
  float bestfit;

  fscanf (file, "%d", &i);
  while (!feof (file))
    {
      a[j]=i;
      fscanf (file, "%d", &i);
	  j++;
    }
    int tot=j;
    i=0;
    int cor[tot+1][5];

  int z=0,x=1;
for(i=0;i<tot;i++)
   {
	if(z==3){
		x++;
		z=0;
		cor[x][z]=a[i];
		z=1;}
	else{
		cor[x][z]=a[i];
		z++;}
 }
  int len=x;
   fclose (file);
   int path[len+1],bestpath[len+1],oldpath[len+1];
   float dismat[len+1][len+1];
   for(i=1;i<=len;i++)
   {
   	for(j=1;j<=len;j++)
   	{
   		if (i==j){
   			dismat[i][j]=INF;

		   }
		else{
			int x1=pow((cor[i][1]-cor[j][1]),2);
			int y1=pow((cor[i][2]-cor[j][2]),2);
			dismat[i][j]=pow((x1+y1),0.5);
		}
	   }
   }
   for(i=1;i<=len;i++)
   {
   	path[i]=i;
   }

    void shuffle1(int path[],int len)
   {
   int num,upper=len,lower=1,n1,n2,temp;
   	 n1=(rand()%(upper-lower+1))+1;
   	 n2=(rand()%(upper-lower+1))+1;
   	 temp=path[n1];
   	 path[n1]=path[n2];
   	 path[n2]=temp;}

   	float fitness(int path[],int len)
   	{  float dis=0;

   	   for(i=1;i<=len-1;i++)
   	   {  int x1=path[i],y1=path[i+1];
		  dis=dis+dismat[x1][y1];

		  }
	   float fit=1/dis;
   	   return fit;
	   }
   srand(time(0));
   for(i=1;i<=len;i++)
   {
   shuffle1(path,len);}
   printf("\n");


void twoopt(int route[],int len)
{int best1[len],newroute[len],e,q;
for(i=1;i<=len;i++)
{
best1[i] = route[i];}
     int improved = 1;
     while (improved==1)
     {
          improved = 0;
          for(q=1;q<len-2;q++)
          {
                  for(e=q+1;e<len;e++)
                  {
                          if (e-q==1)
						   continue;
						  for(i=1;i<=len;i++){
                             newroute[i] = route[i];}
                             for(i=q,j=e-1;i<e,j>q-1;i++,j--)
                             {
                             	newroute[i]=route[j];
							 }
                          if (fitness(newroute,len)>fitness(best1,len)){
                          	for(i=1;i<=len;i++)
                          	{
                                  best1[i]= newroute[i];

                                  route[i] = best1[i];}
                           improved = 1;}}}}
                           float bfit=fitness(best1,len);
            printf("\nThe best path is:");
			for(i=1;i<len;i++)
			{

            printf("%d ",newroute[i]);}
            printf("\nbest fit is:%f",1/bfit);
 }
void simulatedannealing(int path[],int len)
{           int n;
	        float temperature;
	        printf("Enter the temperature: ");
	        scanf("%f",&temperature);
	        printf("\n");
	        printf("Enter the no of iterations:");
	        scanf("%d",&n);
            float deltaDistance = 0;
            float coolingRate = 0.9999;
            float absoluteTemperature = 0.00001;
            int currpath[len];float newfit;float oldfit;
            for(i=1;i<=len;i++)
            {
            currpath[i]=path[i];
            bestpath[i]=path[i];}
            bestfit=fitness(path,len);

            while(temperature>absoluteTemperature)
            {
                #pragma omp parallel shared(bestpath,bestfit,currpath,chunk) private(i,tid)

				{tid = omp_get_thread_num();
				printf("Thread %d starting...\n",tid);
                  #pragma omp for schedule(dynamic,chunk)
            	for(i=0;i<n;i++)
            	{
            		int up=len/10,low=len/20;
   	               int r1=(rand()%(up-low+1))+1;
   	               for(i=1;i<=len;i++)
            {
            oldpath[i]=currpath[i];}
   	               oldfit=fitness(oldpath,len);
   	               for(i=0;i<r1;i++)
   	               {
					shuffle1(currpath,len);}
					newfit=fitness(currpath,len);
					if(newfit>bestfit)
					{
						for(i=1;i<=len;i++)
                            {
                            bestpath[i]=currpath[i];}
						bestfit=newfit;
					}
					if(newfit<oldfit)
					{   up=1,low=0;
   	                    int r2=(rand()%(up-low+1))+1;
   	                    float dif=(newfit-oldfit);
   	                    float v=((-1)*dif)/temperature;
   	                     float p=exp(v);
   	                    if(r2<p)
   	                    {
						for(i=1;i<=len;i++)
                            {
                             currpath[i]=oldpath[i];}
									}}
								}}
				temperature=temperature*coolingRate;
			}


}
simulatedannealing(path,len);
twoopt(bestpath,len);


return 0;
}
