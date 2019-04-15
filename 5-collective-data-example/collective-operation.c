/* This program integrates sin(x) between 0 and pi by computing
* the area of a number of rectangles chosen so as to approximate
* the shape under the curve of the function using MPI.
* 
* The root process acts as a master to a group of child process
* that act as slaves.  The master prompts for the number of 
* interpolations and broadcasts that value to each slave.
* 
* There are num_procs processes all together, and a process 
* computes the area defined by every num_procs-th interval,
* collects a partial sum of those areas, and sends its partial 
* sum to the root process.
**/

#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <stdlib.h>

#define PI 3.1415926535

int main(int argc, char **argv) 
{
    int my_id, root_process, num_procs, ierr, num_intervals, i;
    double rect_width, area, sum, x_middle, partial_sum;
    MPI_Status status;

    /* Let process 0 be the root process. */
    
    root_process = 0;

    /* Now replicate this process to create parallel processes. */

    ierr = MPI_Init(&argc, &argv);

    /* Find out MY process ID, and how many processes were started. */

    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if(my_id == root_process) {

        /* I must be the root process, so I will query the user
        to determine how many interpolation intervals to use. */

        printf("Please enter the number of intervals to interpolate: ");
        scanf("%i", &num_intervals);
    }

    /* Then...no matter which process I am:
    *
    * I engage in a broadcast so that the number of intervals is 
    * sent from the root process to the other processes, and ...
    **/
    ierr = MPI_Bcast(&num_intervals, 1, MPI_INT, root_process, 
        MPI_COMM_WORLD);

    /* calculate the width of a rectangle, and */

    rect_width = PI / num_intervals;

    /* then calculate the sum of the areas of the rectangles for
    * which I am responsible.  Start with the (my_id +1)th
    * interval and process every num_procs-th interval thereafter.
    **/
    partial_sum = 0;
    for(i = my_id + 1; i <num_intervals + 1; i += num_procs) { 
        
        /* Find the middle of the interval on the X-axis. */
        x_middle = (i - 0.5) * rect_width;
        area =  sin(x_middle) * rect_width; 
        partial_sum = partial_sum + area;
    } 
    printf("proc %i computes: %f\n", my_id, (float)partial_sum);

    /* and finally, engage in a reduction in which all partial sums 
    * are combined, and the grand sum appears in variable "sum" in
    * the root process,
    **/
    ierr = MPI_Reduce(&partial_sum, &sum, 1, MPI_DOUBLE,
        MPI_SUM, root_process, MPI_COMM_WORLD);

    /* and, if I am the root process, print the result. */

    if(my_id == root_process) {
        printf("The integral is %f\n", (float)sum);

        /* (yes, we could have summed just the heights, and
        * postponed the multiplication by rect_width til now.) */
    } 

    /* Close down this processes. */

    ierr = MPI_Finalize();
    return 0;
}     
