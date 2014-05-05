/*
 * C++ Program to Implement shortest pair 2D
 */
#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <stdio.h>
#include <algorithm>
#include <cmath>
#include <climits>
#include <vector>
#include <time.h>
using namespace std;
//#include <conio>
struct Point{
	float x,y;
};
void merge(Point *,int, int , int, Point*);
void mergey(Point *,int, int , int, Point*);
double closest_pair_dnq(Point *b,Point *bc,Point* bcy,int low,int high,int arrayln){
	int mid=floor((low+high)/2);
	double totalmin;
	double minl;
	double minr;
//cout<<"low: "<<low<<"high: "<<high<<"medium"<<mid<<"\n";
	
if(high-low<=20){
		double dist= INT_MAX;;
		for(int j=low;j<=high;j++){
			for(int k=(j+1) ;k<=high;k++){
				if(j !=k){
				double d=sqrt((b[j].y-b[k].y)*(b[j].y-b[k].y)+(b[j].x-b[k].x)*(b[j].x-b[k].x));
				//cout<<"the distance from d complex"<<d<<"the j val"<<j<<"the k val"<<k<<"distance"<<d<<"\n";
				if(d<dist){
					dist=d;
					
				}
				}
				
			}
		}
		return dist;
	}
	else{
		
		minl=closest_pair_dnq(b,bc,bcy,low,mid,arrayln);	


        minr=closest_pair_dnq(b,bc,bcy,mid,high,arrayln);


	totalmin=min(minl,minr);
	vector<Point> finalarr;
	for(int m=0;m<arrayln;m++){
		if(bcy[m].x >= bc[mid].x-totalmin && bcy[m].x <= bc[mid].x+totalmin){
			finalarr.push_back(bcy[m]);
			}
	}
for( int i = 0; i < finalarr.size(); i++ ) {
	if(finalarr[i].x <=bc[mid].x){
	for(int j= i-8; j<=i+8; j++ ) {
		if((i!=j) && (j>=0) && (j<finalarr.size())){
			double d=sqrt((finalarr[i].y-finalarr[j].y)*(finalarr[i].y-finalarr[j].y)+(finalarr[i].x-finalarr[j].x)*(finalarr[i].x-finalarr[j].x));
			if(d < totalmin){
				totalmin=d;
			}
		}
	}
}
}
return totalmin;

		
		
}
	
}
double closest_pair_dnqp(Point *b,Point *bc,Point* bcy,int low,int high,int arrayln){
	int tid;
	int mid=floor((low+high)/2);
	double totalmin;
	double minl;
	double minr;
//cout<<"low: "<<low<<"high: "<<high<<"medium"<<mid<<"\n";
	
if(high-low<=40){
		double dist= INT_MAX;;
		for(int j=low;j<=high;j++){
			for(int k=(j+1) ;k<=high;k++){
				double d=sqrt((b[j].y-b[k].y)*(b[j].y-b[k].y)+(b[j].x-b[k].x)*(b[j].x-b[k].x));
				//cout<<"the distance from d complex"<<d<<"the j val"<<j<<"the k val"<<k<<"distance"<<d<<"\n";
				if(d<dist){
					dist=d;
					
				}
				
			}
		}
		return dist;
	}
	else{
#pragma omp task untied shared(b,bc,bcy,low,mid,arrayln,minl)
{
	//int tid= omp_get_thread_num();
	//cout<<"the thread id"<<tid<<"\n";
		minl=closest_pair_dnqp(b,bc,bcy,low,mid,arrayln);
	}
#pragma omp task untied shared(b,bc,bcy,low,mid,arrayln,minr)
{
		minr=closest_pair_dnqp(b,bc,bcy,mid,high,arrayln);
	}
		#pragma omp taskwait
{
	
		totalmin=min(minl,minr);
		vector<Point> finalarr;
		for(int m=0;m<arrayln;m++){
			if(bcy[m].x >= bc[mid].x-totalmin && bcy[m].x <= bc[mid].x+totalmin){
				finalarr.push_back(bcy[m]);
				}
		}
	for( int i = 0; i < finalarr.size(); i++ ) {
		if(finalarr[i].x <=bc[mid].x){
		for(int j= i-8; j<=i+8; j++ ) {
			if((i!=j) && (j>=0) && (j<finalarr.size())){
				double d=sqrt((finalarr[i].y-finalarr[j].y)*(finalarr[i].y-finalarr[j].y)+(finalarr[i].x-finalarr[j].x)*(finalarr[i].x-finalarr[j].x));
				if(d < totalmin){
					totalmin=d;
				}
			}
		}
	}
	}
	return totalmin;
}
}

	
}
int fib(int n)
{
  int i, j;
  if (n<2)
    return n;
  else
    {
       #pragma omp task untied shared(i) firstprivate(n)
      {
      	i=fib(n-1);
		int k=omp_get_thread_num();
		//printf("Thread %d doing task 1\n",k);
		//cout<<"the thread num is "<<k<<"\n";
      } 
	   

       #pragma omp task untied shared(j) firstprivate(n)
       j=fib(n-2);

       #pragma omp taskwait
       return i+j;
    }
}
void mergesorty(Point *a, int low, int high, Point* sc)
{
    int mid;
    if (low < high)
    {
        mid=(low+high)/2;
        mergesorty(a,low,mid,sc);
        mergesorty(a,mid+1,high,sc);
        mergey(a,low,high,mid,sc);
    }
    return;
}
void mergey(Point *a, int low, int high, int mid,Point* sc)
{
    int i, j, k;
    i = low;
    k = low;
    j = mid + 1;
    while (i <= mid && j <= high)
    {
        if (a[i].y < a[j].y)
        {
            sc[k].y = a[i].y;
			sc[k].x=a[i].x;
            k++;
            i++;
        }
        else
        {
            sc[k].y = a[j].y;
			sc[k].x=a[j].x;
            k++;
            j++;
        }
    }
    while (i <= mid)
    {
        sc[k].y = a[i].y;
		sc[k].x = a[i].x;
        k++;
        i++;
    }
    while (j <= high)
    {
        sc[k].y = a[j].y;
		sc[k].x = a[j].x;
        k++;
        j++;
    }
    for (i = low; i < k; i++)
    {
        a[i].y = sc[i].y;
		a[i].x = sc[i].x;
    }
}
void mergesort(Point *a, int low, int high, Point* sc)
{
    int mid;
    if (low < high)
    {
        mid=(low+high)/2;
        mergesort(a,low,mid,sc);
        mergesort(a,mid+1,high,sc);
        merge(a,low,high,mid,sc);
    }
    return;
}
void merge(Point *a, int low, int high,int mid,Point *sc)
{
    int i, j, k;
    i = low;
    k = low;
    j = mid + 1;
    while (i <= mid && j <= high)
    {
        if (a[i].x < a[j].x)
        {
            sc[k].x = a[i].x;
			sc[k].y = a[i].y;
            k++;
            i++;
        }
        else
        {
            sc[k].x = a[j].x;
			sc[k].y = a[j].y;
            k++;
            j++;
        }
    }
    while (i <= mid)
    {
        sc[k].x = a[i].x;
		sc[k].y = a[i].y;
        k++;
        i++;
    }
    while (j <= high)
    {
        sc[k].x = a[j].x;
		sc[k].y = a[j].y;
        k++;
        j++;
    }
    for (i = low; i < k; i++)
    {
        a[i].y = sc[i].y;
		a[i].x = sc[i].x;
    }

}
int main()
{
	 int nfib = 10;
    int i,num;
    cout<<"enter number of the points\n";
	cin>>num;
	Point *a;
	Point *ac;
	Point *acy;
	ac=new Point[num];
	a=new Point[num];
	acy=new Point[num];
	
    for (i = 0; i < num; i++)
    {
        //a[i]=(rand() % 1024)+1;
		acy[i].x=ac[i].x=a[i].x= static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1024.12));
		acy[i].y=ac[i].y=a[i].y= static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/906.12));
		//cout<<a[i].x<<"and"<<a[i].y<<"the i valie is"<<i<<"\n";
    }
	//cout<<"allocation has been done";

    //mergesort(a, 0, (num-1));
	clock_t start, end;
	     double cpu_time_used;
		 Point *sc;
		sc=new Point[num];
	   start = clock();
	    
	double dist= INT_MAX;
	for(int j=0;j<num;j++){
		for(int k=(j+1);k<num;k++){
			
					double d=sqrt((a[j].y-a[k].y)*(a[j].y-a[k].y)+(a[j].x-a[k].x)*(a[j].x-a[k].x));
					
				
					//cout<<"the dis simple is x"<<d<<"j value is"<<j<<"k value is"<<k<<"\n";
			if(d<dist){
				dist=d;
			}
			
		}
	}
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	
	cout<<"Closest pair normal method"<<dist<<"time taken by this method"<<cpu_time_used<<"\n";
	
	sc=new Point[num];
start = clock();
	mergesort(ac, 0, (num-1),sc);
    for (i = 0; i < num; i++)
    {
		//cout<<ac[i].x<< "and" <<ac[i].y<<"the i value is"<<i<<"\n";
    }
	
	sc=new Point[num];
mergesorty(acy,0,(num-1),sc);
    for (i = 0; i < num; i++)
    {
        //cout<<acy[i].x<< "and y arranged" <<acy[i].y<<"\n";
    }
	
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	cout<<"Time taken to sort"<<cpu_time_used<<"\n";
	start = clock();
	double dis=closest_pair_dnq(a,ac,acy,0,num-1,num);
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	cout<<"Min distance is divide and conquer method"<< dis<<"time taken by this func"<<cpu_time_used<<"\n";
    omp_set_dynamic(0);
    omp_set_num_threads(4);
	
#pragma omp parallel shared(a,ac,acy,num)
	{
#pragma omp single
		{
			double start=omp_get_wtime();
			double dis=closest_pair_dnqp(a,ac,acy,0,num-1,num);
			double end=omp_get_wtime();
			double prtime=(double(end-start));
			cout<<"the procressing time for parallel distance"<<dis<<"time"<<prtime<<"\n";
			
			 //printf ("fib(%d) = %d\n", nfib, fib(nfib));
	}

}
	
}
