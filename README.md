
# libknx

Doc ToDo:
- What is it
- What can and can't it do
- Intended use
- Programming language
- How to build
- How does it work
- What is the design / idea


Libknx is an open source C++ library to interact with a KNX network. Becasue this project is very much work in progress (as is this Readme...), the first focus is client side KNX over IP. In other words to connect to a KNX IP router. A client still can send and receive group messages like any other KNX device, but it act as an IP router itself.

A knx ip client needs to open an UDP socket and send messages to UDP sockets. Libknx uses asio with coroutines for that.

## Design principles
Libknx is an object oriented library. Every object has it's specific responsiblilty, in order to keeps stuff tidy, testable and reusable as much as possible. It is a build up of groups of objects (packages).

## Requirements
### Build
- C++ compiler that can do C++ 23 (recent gcc or clang)
- CMake
- Ninja
- GoogleTest
### Runtime
- Asio

## High level overview

In order to interact with an IP router the clients needs to have a data UDP port and a control UDP port. Frames over this line are as follows:

Top level is a KNX IP header, than a Response or a Request.

## General source Setup
### src/bytes
Contains functionality to parse and create network frames. The objects here focus on translation of bytes to types like uint16, utf-8 strings,  boolean etc.

### src/cemi
Contains classes to parse and create cemi frames.

### src/connection
User side (API) objects for discovery and create a connection to a IP router.

### src/headers
Headers / frames that are used in the protocol. The objects in this directory can parse and create them. The objects in this directory use ByteBuffer from src/bytes.

### src/ip
Asio UDP connection wrappers for the libknx.

### src/requests & src/responses
Contains objects that are send between libknx and a KNX IP router. Theses objects can parse and convert bytes / frames. Because the main focus of the project is to connect TO a KNX IP router, requests can't parse (would be used for incomming) and responses can't convert to bytes.




## ToDo list
### Functional
- Mechanism that keeps track of the requests send:
  - Match incomming responses to requests
  - Time-out requests without a response
