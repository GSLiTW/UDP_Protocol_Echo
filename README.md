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
./client <ip> <port> <message>
```
The client should print the message echoed from the server (the message it sends to the server originally).