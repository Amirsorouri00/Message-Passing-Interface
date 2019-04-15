# Message-Passing-Interface

1. http://condor.cc.ku.edu/~grobe/docs/intro-MPI-C.shtml    Examples-([1,2,3,4]) OK.
2. https://www.mcs.anl.gov/research/projects/mpi/tutorial/mpiintro/ppframe.ht  * Just a handy tutorial


##Basic MPI communication routines
It is important to realize that separate processes share no memory variables. They appear to be using the same variables, but they are really using COPIES of any variable defined in the program.
As a result, these programs cannot communicate with each other by exchanging information in memory variables. Instead they may use any of a large number of MPI communication routines. The two basic routines are:

- MPI_Send, to send a message to another process, and
- MPI_Recv, to receive a message from another process.
- The syntax of MPI_Send is:

int MPI_Send(void *data_to_send, int send_count, MPI_Datatype send_type, 
      int destination_ID, int tag, MPI_Comm comm); 
- data_to_send: variable of a C type that corresponds to the send_type supplied below
- send_count: number of data elements to be sent (nonnegative int)
- send_type: datatype of the data to be sent (one of the MPI datatype handles)
- destination_ID: process ID of destination (int)
- tag: message tag (int)
- comm: communicator (handle)
  
Once a program calls MPI_Send, it blocks until the data transfer has taken place and the data_to_send variable can be safely reused. As a result, these routines provide a simple synchronization service along with data exchange.
The syntax of MPI_Recv is:

int MPI_Recv(void *received_data, int receive_count, MPI_Datatype receive_type, 
      int sender_ID, int tag, MPI_Comm comm, MPI_Status *status); 
- received_data: variable of a C type that corresponds to the receive_type supplied below
- receive_count: number of data elements expected (int)
- receive_type: datatype of the data to be received (one of the MPI datatype handles)
- sender_ID: process ID of the sending process (int)
- tag: message tag (int)
- comm: communicator (handle)
- status: status struct (MPI_Status)
  
The receive_count, sender_ID, and tag values may be specified so as to allow messages of unknown length, from several sources (MPI_ANY_SOURCE), or with various tag values (MPI_ANY_TAG).
The amount of information actually received can then be retrieved from the status variable, as with:

count MPI_Get_count(&status, MPI_FLOAT, &true_received_count);
received_source = status.MPI_SOURCE;
received_tag = status.MPI_TAG;
MPI_Recv blocks until the data transfer is complete and the received_data variable is available for use.




##Collective data movement
There are several routines for performing collective data distribution tasks:
####MPI_Bcast
Broadcast data to other processes 
####MPI_Gather, MPI_Gatherv
Gather data from participating processes into a single structure
####MPI_Scatter, MPI_Scatter
Break a structure into portions and distribute those portions to other processes
####MPI_Allgather, MPI_Allgatherv
Gather data from different processes into a single structure that is then sent to all participants (Gather-to-all)
####MPI_Alltoall, MPI_Alltoallv
Gather data and then scatter it to all participants (All-to-all scatter/gather)
The routines with "V" suffixes move variable-sized blocks of data.

The subroutine MPI_Bcast sends a message from one process to all processes in a communicator.

int MPI_Bcast(void *data_to_be_sent, int send_count, MPI_Datatype send_type, 
      int broadcasting_process_ID, MPI_Comm comm);
When processes are ready to share information with other processes as part of a broadcast, ALL of them must execute a call to MPI_BCAST. There is no separate MPI call to receive a broadcast.

MPI_Bcast could have been used in the program sumarray_mpi presented earlier, in place of the MPI_Send loop that distributed data to each process. Doing so would have resulted in excessive data movement, of course. A better solution would be MPI_Scatter or MPI_Scatterv.

The subroutines MPI_Scatter and MPI_Scatterv take an input array, break the input data into separate portions and send a portion to each one of the processes in a communicating group.

int MPI_Scatter(void *send_data, int send_count, MPI_Datatype send_type, 
      void *receive_data, int receive_count, MPI_Datatype receive_type, 
      int sending_process_ID, MPI_Comm comm); 
or

int MPI_Scatterv(void *send_data, int *send_count_array, int *send_start_array, 
      MPI_Datatype send_type, void *receive_data, int receive_count, 
      MPI_Datatype receive_type, int sender_process_ID, MPI_Comm comm);
- data_to_send: variable of a C type that corresponds to the MPI send_type supplied below
- send_count: number of data elements to send (int)
- send_count_array: array with an entry for each participating process containing the number of data elements - to send to that process (int)
- send_start_array: array with an entry for each participating process containing the displacement relative to the start of data_to_send for each data segment to send (int)
- send_type: datatype of elements to send (one of the MPI datatype handles) 

- receive_data: variable of a C type that corresponds to the MPI receive_type supplied below
- receive_count: number of data elements to receive (int)
- receive_type: datatype of elements to receive (one of the MPI datatype handles)
- sender_ID: process ID of the sender (int)
- receive_tag: receive tag (int)
- comm: communicator (handle)
- status: status object (MPI_Status)
The routine MPI_Scatterv could have been used in the program sumarray_mpi presented earlier, in place of the MPI_Send loop that distributed data to each process.

MPI_Bcast, MPI_Scatter, and other collective routines build a communication tree among the participating processes to minimize message traffic. If there are N processes involved, there would normally be N-1 transmissions during a broadcast operation, but if a tree is built so that the broadcasting process sends the broadcast to 2 processes, and they each send it on to 2 other processes, the total number of messages transferred is only O(ln N).