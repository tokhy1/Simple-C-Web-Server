# Simple TCP Web Server in C

This repository contains a minimal HTTP web server written in **C** using **POSIX/Winsock sockets**.  
The goal of this project is **educational**, not production readiness.

It is designed to demonstrate how web servers work **under the hood**, without frameworks, abstractions, or libraries hiding the details.

---

## Why this project exists

Most backend developers use high-level frameworks (Node.js, Spring, Django, etc.) that hide how networking actually works.

This project was built to understand:

- How TCP servers are created at the OS level
    
- How sockets, file descriptors, and kernel buffers work
    
- How HTTP requests are received and responses are sent
    
- Why blocking I/O causes scalability problems
    
- How backend servers really interact with the operating system

---

## What this server does

- Listens on port **8080**
    
- Accepts incoming TCP connections
    
- Reads a raw HTTP request
    
- Sends a fixed HTTP response (`Hello world!`)
    
- Closes the connection
    

The server is:

- Single-process
    
- Single-threaded
    
- Blocking
    
- TCP-based
    
- IPv4-based
    

This is intentional to keep the behavior visible and understandable.

---

## Limitations (intentional)

This server is **not** production-ready.

It does not handle:

- Multiple clients concurrently
    
- Non-blocking or asynchronous I/O
    
- Large requests
    
- Persistent connections
    
- Proper HTTP parsing
    
- Security concerns
    
---

## How it works (high-level flow)

1. The server creates a TCP socket
    
2. The socket is bound to port 8080
    
3. The server starts listening (kernel creates queues)
    
4. The server blocks waiting for a connection
    
5. A client connects
    
6. The kernel completes the TCP handshake
    
7. The server accepts the connection
    
8. The server reads the HTTP request
    
9. The server writes an HTTP response
    
10. The connection is closed
    

Every step maps directly to kernel data structures and system calls.

---

## How to build and run

### Compile

`gcc server.c -o server`

### Run

`./server`

### Test

Open a browser or use curl:

`curl http://localhost:8080`

You should receive:

`Hello world!`

---
