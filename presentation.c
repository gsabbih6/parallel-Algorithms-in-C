#include <stdio.h>
#include <omp.h>
#include <unistd.h>
#include <math.h>
#include <gmp.h>
#include <stdlib.h>

#define N 2

//int N =2;ßß
#define Tn 1200 //actually N+1
#define K 10 // order of precision for the use with GMP


int main(int argc, char *argv[])
{
    printf("started%d\n",1);
     double time_s = omp_get_wtime();
    
    int Numt=omp_get_num_threads();
    mpf_t tempv[Numt];
    mpf_t sum[Numt];
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
    
    mpf_t* x=(mpf_t *) malloc(N*sizeof(mpf_t));
    mpf_t* y=(mpf_t *) malloc(N*sizeof(mpf_t));
    mpf_t* z=(mpf_t *) malloc(N*sizeof(mpf_t));
    
    mpf_inits(R,Sigma,timestep,b,tpow,x_red,X,y_red,Y,z_red,Z,s1,s2,temp,c,count,tempv,sum,divs);
    
    printf("completed s%d\n",1);
     for (int i = 0; i <N; ++i)
     {
         mpf_init2(x[i],K);
         mpf_init2(y[i],K);
         mpf_init2(z[i],K);
     }
    mpf_set_d(R,28);
    mpf_set_d(Sigma,10);
    mpf_set_d(b,8/3);
    mpf_set_d(timestep,0.01);
    printf("initialised  completed %d\n",1);
    
    //  set initial conditions where at time, t=0;
     mpf_set_d(X, - 15.8);
     mpf_set_d(Y, - 17.48);
     mpf_set_d(Z, 35.64);
     printf("initialised  completed %d\n",1);

     for (int t = 1; t < Tn; t++) // start iterations from t=1 to t=Tn; we know t0
     {

         mpf_set(x[0], X);
//         x[0]=X[t-1];
         mpf_set(y[0], Y);
//         y[0]=Y[t-1];
         mpf_set(z[0], Z);
//         z[0]=Z[t-1];
        printf("hi %d\n",t);
        for (int i = 0; i<N; i++)
        {
            //printf("hi %d\n", i);
                 // mpf_set_d(s1,0.00);
                 // mpf_set_d(s2,0.00);

            #pragma omp parallel private(k,tid)
            {
                tid = omp_get_thread_num();

                #pragma omp for
                for (k=0; k<=i; k++)
                {
                // s1=s1+x[i-k]*z[k];
//                 mpf_mul(temp,x[i-k],z[k]);
                // // gmp_printf("temp is %Ff\n",temp);
//                 mpf_add(s1,s1,temp);
                // // s2=s2+x[i-k]*y[k];
//                 mpf_mul(temp,x[i-k],y[k]);
//                 mpf_add(s2,s2,temp);
                
                    mpf_mul(tempv[tid],x[i-k],z[k]);
                    mpf_add(sum[2*tid],sum[2*tid],tempv[tid]);
                    mpf_mul(tempv[tid],x[i-k],y[k]);
                    mpf_add(sum[2*tid+1],sum[2*tid+1],tempv[tid]);
                }

            }
            
            for (j=0; j<Numt; j++)
            {
            mpf_add(s1,s1,sum[2*j]);
            mpf_add(s2,s2,sum[2*j+1]);
            }
            
            
            // compute the
            mpf_set_ui(c,1.0);
            mpf_set_ui(count,i+1);


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
            // gmp_printf("tau is %Ff\n",t);

            // x_red=x_red+(x[i]*t);
            mpf_mul(temp,x[i],tpow);
            mpf_add(x_red,x_red,temp);

            // y_red=y_red+(y[i]*t);
            mpf_mul(temp,y[i],tpow);
            mpf_add(y_red,y_red,temp);

            // z_red=z_red+(z[i]*t);
            mpf_mul(temp,z[i],tpow);
            mpf_add(z_red,z_red,temp);

            // mpf_clears(timestep,tpow);
        }


    // double X=x[0]+x_red;
    mpf_add(X,X,x_red);
    // double Y=x[0]+(y_red);
    mpf_add(Y,Y,y_red);
    // double Z=[0]+(z_red);
    mpf_add(Z,Z,z_red);
    printf("completed%d\n",t);

    }


     gmp_printf("X at t = %d is %.5Ff\n",t,X);
     gmp_printf("Y t = %d is %.5Ff\n",t,Y);
     gmp_printf("Z t = %d is %.5Ff\n",t,Z);
     time_s = omp_get_wtime()-time_s;
     printf("Time elapsed is %fsecs\n", time_s);



    return 0;
}

