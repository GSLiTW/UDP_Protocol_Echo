# UDP_Protocol_Echo
## Compile:
```console
gcc client.c -o client
```
```console
gcc server.c -o server
```

## Run:
```console
./server <ip> <port>
```
```console
./client <ip> <port> <message> <mex_retry>
```
The client should print the message echoed from the server (the message it sends to the server originally).

## Retry:
Using the select() function to monitoring the socket. After each timeout, the timeout interval would grow to be two times of the original timeout interval (The exponetial backoff).

## Demonstration Video:

