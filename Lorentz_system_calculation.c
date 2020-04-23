#include <stdio.h>
#include <omp.h>
#include <unistd.h>
#include <math.h>
#include <gmp.h>
#include <stdlib.h>

//#define N 200000
//
////int N =2;ßß
//#define Tn 5 //actually N+1
//#define K 10 // order of precision for the use with GMP


int main(int argc, char *argv[]){
    
    int N=400;
    int Tn=1200;
     double time_s = omp_get_wtime();
    
    int Numt=omp_get_max_threads();
     printf("Threads sum = %d\n",Numt);
    mpf_t tempv[Numt];
    mpf_t sum[2*Numt];
    mpf_t  X;//=malloc(Tn*sizeof(mpf_t));
    mpf_t  Y;//=malloc(Tn*sizeof(mpf_t));
    mpf_t  Z;//=malloc(Tn*sizeof(mpf_t));
    mpf_t temp;
    mpf_t s1;
    mpf_t s2;
    mpf_t divs;
    mpf_t R;
    mpf_t Sigma;
    mpf_t b;
    int tid=0;

    int k=0;
    int j=0;
    mpf_t x_red;
    mpf_t y_red;
    mpf_t z_red;
    mpf_t tpow;
    mpf_t timestep;
    mpf_t c;
    mpf_t count;
    
    mpf_t x[N+1];
    mpf_t y[N+1];
    mpf_t z[N+1];
    mpf_init(R);mpf_init(Sigma);mpf_init(timestep);mpf_init(b);mpf_init(R);mpf_init(tpow);mpf_init(x_red);mpf_init(X);mpf_init(y_red);mpf_init(Y);mpf_init(z_red);mpf_init(Z);mpf_init(s1);mpf_init(s2);mpf_init(temp);
    
    mpf_init(c);
    mpf_init(count);
    
    for (int i = 0; i <(2*Numt); ++i){
        mpf_init(sum[i]);
    }
    for (int i = 0; i <Numt; ++i){
    mpf_init(tempv[i]);
    }
    
    
    mpf_init(divs);
    
     for (int i = 0; i <N; ++i)
     {
         mpf_init(x[i]);
         mpf_init(y[i]);
         mpf_init(z[i]);
     }
    mpf_set_d(R,28);
    mpf_set_d(Sigma,10);
    mpf_set_d(b,8/3);
    mpf_set_d(timestep,0.01);
    
    //  set initial conditions where at time, t=0;
     mpf_set_d(X, - 15.8);
     mpf_set_d(Y, - 17.48);
     mpf_set_d(Z, 35.64);
     printf("initialised  completed %d\n",1);

     for (int t = 1; t < Tn; t++) // start iterations from t=1 to t=Tn; we know t0
     {

         mpf_set(x[0], X);
         mpf_set(y[0], Y);
         mpf_set(z[0],Z);
        for (int i = 0; i<N; i++)
        {
            #pragma omp parallel private(k,tid)
            {
                tid = omp_get_thread_num();
//                printf("hi %d\n",tid);

                #pragma omp for
                for (k=0; k<=i; k++)
                {
                    mpf_mul(tempv[tid],x[i-k],z[k]);
                    mpf_add(sum[2*tid],sum[2*tid],tempv[tid]);
                    mpf_mul(tempv[tid],x[i-k],y[k]);
                    mpf_add(sum[2*tid+1],sum[2*tid+1],tempv[tid]);
                }

            }
            
            for (j=0; j<Numt; j++)// maual reduction
            {
            mpf_add(s1,s1,sum[2*j]);
            mpf_add(s2,s2,sum[2*j+1]);
            }
            
            
            // compute the
            mpf_set_d(c,1.0);
            mpf_set_d(count,i+1.0);


            mpf_div(divs ,c,count);
            mpf_sub(temp,y[i],x[i]);
            mpf_mul(temp,temp,divs);
            mpf_mul(x[i+1],temp,Sigma);

            mpf_mul(temp,R,x[i]);
            mpf_sub(temp,temp,y[i]);
            mpf_sub(temp,temp,s1);
            mpf_mul(y[i+1],temp,divs);
            mpf_mul(temp,b,z[i]);
            mpf_sub(temp,s2,temp);
            mpf_mul(z[i+1],temp,divs);

        }
         
        // compute the next terms of the series after Xn
        for(int i=0;i<N;i++){

            mpf_pow_ui(tpow,timestep,i);
            mpf_mul(temp,x[i],tpow);
            mpf_add(x_red,x_red,temp);

            mpf_mul(temp,y[i],tpow);
            mpf_add(y_red,y_red,temp);

            mpf_mul(temp,z[i],tpow);
            mpf_add(z_red,z_red,temp);

        }


    mpf_add(X,X,x_red);
    mpf_add(Y,Y,y_red);
    mpf_add(Z,Z,z_red);

    }


//     gmp_printf("X at t = %d is %.5Ff\n",Tn,X);
//     gmp_printf("Y t = %d is %.5Ff\n",Tn,Y);
//     gmp_printf("Z t = %d is %.5Ff\n",Tn,Z);
     time_s = omp_get_wtime()-time_s;
     printf("Time elapsed is %fsecs\n", time_s);



    return 0;
}

