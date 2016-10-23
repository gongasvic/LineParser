# LineParser

C project works like a small server. school project December 2013

Consists of a client and server side project
we implement the server who waits for a connection and the clients can connect in multi-threaded environment. He can request for files containing a certain line or a certain file alone.

it uses mutex and semaphores to prevent writting in the same file simultaneously, the server allows multiple reading of a file.

It works based on a shared list structure. The client posts request to the list with multi-threads and the server reads the request and executes also in multi-thread then the server posts response in the list and client reads.

On file side uses mem functions to make it as fast as possible.
