#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
//#include <math.h>
#define SEED 35791246
 
int main(int argc, char* argv[])
{
    long niter = 100000;
    int myid;                       //holds process's rank id
    double x,y;                     //x,y value for the random coordinate
    int i, count=0;                 //Count holds all the number of how many good coordinates
    double z;                       //Used to check if x^2+y^2<=1
    double pi;                      //holds approx value of pi
    int nodenum;
    if(argc > 1){
       niter = atol(argv[1]);
    }
 
    MPI_Init(&argc, &argv);                 //Start MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);           //get rank of node's process
    MPI_Comm_size(MPI_COMM_WORLD, &nodenum);
    int recieved[nodenum];
    long recvniter[nodenum];
    srand(SEED+myid);                       //Give rand() a seed value. Needs to be different on each node
 
    if(myid != 0)
    {
        niter = niter / nodenum;
        for (i=0; i<niter; ++i)                  //main loop
        {
            x= ((double)rand())/RAND_MAX;           //gets a random x coordinate
            y =((double)rand())/RAND_MAX;           //gets a random y coordinate
            z = x*x+y*y;                  //Checks to see if number in inside unit circle
            if (z<=1)
            {
                count++;                //if it is, consider it a valid random point
            }
        }
        for(i=0; i<nodenum; ++i)
        {
            MPI_Send(&count, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
            MPI_Send(&niter, 1, MPI_LONG, 0, 2, MPI_COMM_WORLD);
        }
    }
    else if (myid == 0)
    {
        printf("Computing PI on %d nodes, each with %lu iterations\n", nodenum, niter);
        for(i=0; i<nodenum; ++i)
        {
            MPI_Recv(&recieved[i], nodenum, MPI_INT,MPI_ANY_SOURCE,1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&recvniter[i],nodenum, MPI_LONG, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
 
    //if root process
    if (myid == 0)                     
    {
        int finalcount = 0;
        long finalniter = 0;
        for(i = 0; i<nodenum; ++i)
        {
            finalcount += recieved[i];
            finalniter += recvniter[i];
        }
 
        //p = 4(m/n)
        pi = ((double)finalcount/(double)finalniter)*4.0;               
        printf("Pi computed with %d/%lu points\n", finalcount, finalniter);

        //Print the calculated value of pi   
        printf("Pi: %f\n", pi);             
 
    }
 
    //Close the MPI instance
    MPI_Finalize();
    return 0;
}

