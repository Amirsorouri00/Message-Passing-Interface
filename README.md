# Message-Passing-Interface

1. http://condor.cc.ku.edu/~grobe/docs/intro-MPI-C.shtml    Examples-([1,2]) OK.
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
