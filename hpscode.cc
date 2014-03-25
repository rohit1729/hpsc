#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <stdio.h>

using namespace std;

void create(int );
int**subsum(int ,int **,int**,char);
int **cal(int , int **,int**);
int main()
{
	int n;
	std::cout<<"please enter size matrice :";
	std::cin>>n;
	create(n);
	return 0;
}
//***************create**********
void create(int n)
{
	int i,j;
	int **s,**v,**c,**ki;
	s=new int*[n];
	for(i=0;i<n;i++)
		s[i]=new int[n];
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			s[i][j]=(rand() % 5)+1;
	v=new int*[n];
	for(i=0;i<n;i++)
		v[i]=new int [n];
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			v[i][j]=(rand() % 5)+1;

	c=new int*[n];
	ki=new int*[n];
	for(i=0;i<n;i++){
		c[i]=new int [n];
		ki[i]=new int [n];
	}
		
	for(i=0;i<n;i++){
	for(j=0;j<n;j++){
		c[i][j]=0;
		ki[i][j]=0;	
	}	
	}
    
	double start=omp_get_wtime();
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			for(int k=0;k<n;k++){
				ki[i][j]=ki[i][j]+(s[i][k]*v[k][j]);
			}
			//std::cout<<ki[i][j]<<"     ";;
		}
		//std::cout<<"\n";
	}
	double end=omp_get_wtime();
	double prtime=(double(end-start));
	std::cout<<"the procressing time for linear one     "<<prtime<<"\n";
	
		 
#pragma omp parallel
{
#pragma omp single
  {
	start=omp_get_wtime();
	c= cal(n,s,v);
	end=omp_get_wtime();
	prtime=(double(end-start));
	std::cout<<"the procressing time for parallel strassen part  "<<prtime<<"\n";
}
}
	/*for(i=0;i<n;i++) {
		std::cout<<"\n";
		for(j=0;j<n;j++)
			std::cout<<c[i][j]<<"     ";
	}
	*/
		
	}

	//**********************cal*******************
	int** cal(int n,int **s,int**v)
	{
			

		int i,j,k,**c,t,**a11,**a12,**a21,**a22,**b11,**b12,**b21,**b22;
		int **m1,**m2,**m3,**m4,**m5,**m6,**m7,**w,**p1,**p2;
		int tid,nthreads=0;
		c=new int*[n];
		for(i=0;i<n;i++)
			c[i]=new int [n];


		if(n<=64){
			for(i=0;i<n;i++)
			for(j=0;j<n;j++) {
				c[i][j]=0 ;
				for(k=0;k<n;k++)
					c[i][j]+=s[i][k]*v[k][j]; }
				return c;     }
				else {
					t=n/2;
					//*****************************a and b****************
					a11=new int*[t];
					for(i=0;i<t;i++)
						a11[i]=new int [t];
					for(i=0;i<t;i++)
						for(j=0;j<t;j++)
							a11[i][j]=s[i][j];

					a12=new int*[t];
					for(i=0;i<t;i++)
						a12[i]=new int [t];
					for(i=0;i<t;i++)
						for(j=0;j<t;j++)
							a12[i][j]=s[i][j+t];

					a21=new int*[t];
					for(i=0;i<t;i++)
						a21[i]=new int [t];
					for(i=0;i<t;i++)
						for(j=0;j<t;j++)
							a21[i][j]=s[i+t][j];

					a22=new int*[t];
					for(i=0;i<t;i++)
						a22[i]=new int [t];
					for(i=0;i<t;i++)
						for(j=0;j<t;j++)
							a22[i][j]=s[t+i][j+t];
					//*************************************************

					b11=new int*[t];
					for(i=0;i<t;i++)
						b11[i]=new int [t];
					for(i=0;i<t;i++)
						for(j=0;j<t;j++)
							b11[i][j]=v[i][j];

					b12=new int*[t];
					for(i=0;i<t;i++)
						b12[i]=new int [t];
					for(i=0;i<t;i++)
						for(j=0;j<t;j++)
							b12[i][j]=v[i][j+t];

					b21=new int*[t];
					for(i=0;i<t;i++)
						b21[i]=new int [t];
					for(i=0;i<t;i++)
						for(j=0;j<t;j++)
							b21[i][j]=v[i+t][j];

					b22=new int*[t];
					for(i=0;i<t;i++)
						b22[i]=new int [t];
					for(i=0;i<t;i++)
						for(j=0;j<t;j++)
							b22[i][j]=v[t+i][j+t];


					//*****************************m1-m7************************


					m1=new int*[t];
					for(i=0;i<t;i++)
						m1[i]=new int[t];
					for(i=0;i<t;i++)
						for(j=0;j<t;j++)
							m1[i][j]=0;

					m2=new int*[t];
					for(i=0;i<t;i++)
						m2[i]=new int[t];
					for(i=0;i<t;i++)
						for(j=0;j<t;j++)
							m2[i][j]=0;

					m3=new int*[t];
					for(i=0;i<t;i++)
						m3[i]=new int[t];
					for(i=0;i<t;i++)
						for(j=0;j<t;j++)
							m3[i][j]=0;

					m4=new int*[t];
					for(i=0;i<t;i++)
						m4[i]=new int[t];
					for(i=0;i<t;i++)
						for(j=0;j<t;j++)
							m4[i][j]=0;

					m5=new int*[t];
					for(i=0;i<t;i++)
						m5[i]=new int[t];
					for(i=0;i<t;i++)
						for(j=0;j<t;j++)
							m5[i][j]=0;

					m6=new int*[t];
					for(i=0;i<t;i++)
						m6[i]=new int[t];
					for(i=0;i<t;i++)
						for(j=0;j<t;j++)
							m6[i][j]=0;

					m7=new int*[t];
					for(i=0;i<t;i++)
						m7[i]=new int[t];
					for(i=0;i<t;i++)
						for(j=0;j<t;j++)
							m7[i][j]=0;


					p1=new int*[t];
					for(i=0;i<t;i++)
						p1[i]=new int[t];
					for(i=0;i<t;i++)
						for(j=0;j<t;j++)
							p1[i][j]=0;

					p2=new int*[t];
					for(i=0;i<t;i++)
						p1[i]=new int[t];
					for(i=0;i<t;i++)
						for(j=0;j<t;j++)
							p1[i][j]=0;
				
#pragma omp task untied private(p1,p2,tid) shared(a11,a22,b11,b22,m1,t)
						{
							//tid = omp_get_thread_num();
							//printf("Thread %d doing section 1\n",tid);
							p1=subsum(t,a11,a22,'d');
							p2=subsum(t,b11,b22,'d');
							m1=cal(t,p1,p2);
						}
#pragma omp task untied private(p1,p2,tid) shared(a21,a22,b11,m2,t)
						{
							//tid = omp_get_thread_num();
							//printf("Thread %d doing section 2\n",tid);
							p1=subsum(t,a21,a22,'d');
							m2=cal(t,p1,b11);
						}
#pragma omp task untied private(p1,p2,tid) shared(b12,b22,a11,m3,t)
						{
							//tid = omp_get_thread_num();
							//printf("Thread %d doing section 3\n",tid);
							p1=subsum(t,b12,b22,'k');
							m3=cal(t,a11,p1);
						}
#pragma omp task untied private(p1,p2,tid) shared(b21,b11,a22,t,m4)
						{
							//tid = omp_get_thread_num();
							//printf("Thread %d doing section 4\n",tid);
							p1=subsum(t,b21,b11,'k');
							m4=cal(t,a22,p1);
						}
#pragma omp task untied private(p1,p2,tid) shared(a11,a12,b22,t,m5)
						{
							//tid = omp_get_thread_num();
							//printf("Thread %d doing section 5\n",tid);
							p1=subsum(t,a11,a12,'d');
							m5=cal(t,p1,b22);
						}
#pragma omp task untied private(p1,p2,tid) shared(a21,a11,b11,b12,t,m6)
						{
							//tid = omp_get_thread_num();
							//printf("Thread %d doing section 6\n",tid);
							p1=subsum(t,a21,a11,'k');
							p2=subsum(t,b11,b12,'d');
							m6=cal(t,p1,p2);
						}
#pragma omp task untied private(p1,p2,tid) shared(a12,a22,b21,b22,t,m7)
						{
							//tid = omp_get_thread_num();
							//printf("Thread %d doing section 7\n",tid);
							p1=subsum(t,a12,a22,'k');
							p2=subsum(t,b21,b22,'d');
							m7=cal(t,p1,p2);
						}
#pragma omp taskwait
						{
					w=new int*[n];
					for(i=0;i<n;i++)
						w[i]=new int[n];
					p1=subsum(t,m1,m4,'d');
					p2=subsum(t,p1,m5,'k');
					p2=subsum(t,p2,m7,'d');
					for(i=0;i<t;i++)
					for(j=0;j<t;j++){
						w[i][j]=p2[i][j];}

						p1=subsum(t,m3,m5,'d');
						for(i=0;i<t;i++)
						for(j=0;j<t;j++){
							w[i][j+t]=p1[i][j];
						}

						p1=subsum(t,m2,m4,'d');
						for(i=0;i<t;i++)
							for(j=0;j<t;j++)
								w[i+t][j]=p1[i][j];

						p1=subsum(t,m1,m3,'d');
						p2=subsum(t,p1,m2,'k');
						p2=subsum(t,p2,m6,'d');
						for(i=0;i<t;i++)
							for(j=0;j<t;j++)
								w[i+t][j+t]=p2[i][j];

						return w;
					}
					}
				}


				//*********************sub and sum***************
				int**subsum(int n,int **s,int **v,char ch)
				{

					int **m,i,j;
					m=new int*[n];
					for(i=0;i<n;i++)
						m[i]=new int [n];
					for(i=0;i<n;i++)
						for(j=0;j<n;j++)
							m[i][j]=0;
					if(ch=='d'){
						for(i=0;i<n;i++)
							for(j=0;j<n;j++)
								m[i][j]=s[i][j]+v[i][j];
					}
					else{
						for(i=0;i<n;i++)
							for(j=0;j<n;j++)
								m[i][j]=s[i][j]-v[i][j];
					}
					return m;

				}
