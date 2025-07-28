# libknx
Libknx is an open source C++ library to interact with a KNX network. Becasue this project is very much work in progress (as is this Readme...), the first focus is client side KNX over IP. In other words to connect to a KNX IP router. A client still can send and receive group messages like any other KNX device, but it act as an IP router itself.

A knx ip client needs to open an UDP socket and send messages to UDP sockets. Libknx uses asio with coroutines for that. At a later stage I might try to use the lib in an embedded system, together with [modm.io],

## Design principles
Libknx is an object oriented library. Every object has it's specific responsiblilty, in order to keeps stuff tidy, testable and reusable as much as possible. It is a build up of groups of objects (packages).

KNX has a bunch of types, like percentage, absolute value, on/off, etc. But most of them boil down to standard types e.g. an uint8 (like percentage and value both do). The first setup will only focus on parsing and encoding the basic types. The user / client of this lib must know what kind of types belong to a group value. This depends on the setup of the KNX system. There is not a import of ETS in this lib, as that is a seperate thing. 

### Modern C++
libknx is on C++23.

### Memory safety
Modern C++ can be mem safe in the sence that you don't reference to unallocated objects. You can still cause memory leaks, just like in a language with a garbage collector at runtime. Libknx doesn't use the new keyword and only references objects as a getter (not owner). If shared ownership is needed, shared pointers  should be used.


## Requirements
### Operating System
- FreeBSD
- Linux
- MacOs 15.5
### Build
- gcc or clang
- CMake
- Ninja
- GoogleTest
### Runtime
- Asio 1.34.2

Operating system and devtools should be up to date. I'm not supporting old stuff, as this is a new project.

I'm developing against asio 1.34.2. On FreeBSD you need to install the port instead of the binary package for now, works fine. I can't test MacOs throughly, I own a Intel Mac from 2017 and it has a patched Mac Os 15.5 on it.

# How to build
Just a basic cmake build:
```
mkdir build
cd build
cmake -G Ninja ..
ninja
```

# Development setup
The CMakeLists.txt or cmake config of this project forms the basis. From there I use neovim with clangd as a LSP. It is also possible to use Clion, it imports the CMakeLists.txt perfectly. I use Clion for the debugger and nice code suggestions. I just don't like to use the mouse a lot, so Clion is not my preffered IDE.

My setup runs on Arch Linux, I don't have a automated setup (yet). So I just recompile on FreeBSD by hand and run the tests on both a Intel and a arm (Raspberry Pi), so I know big / little endian conversions work as intended. FreeBSD is my target platform, as I like it as a server.

## High level overview KNX IP protocol
In order to interact with an IP router the clients needs to have a data UDP port and a control UDP port. Frames over this line are as follows:

Top level is a KNX IP header, than a Response or a Request. 

### Connection
Connection interaction goes like this:
Client sends a ConnectRequest to wich the server responds with a ConnectRequestResponse. This contains the info needed to communicate (ip, port etc) and it containts a channelId. This is a number that corresponds with this connection, a handle if you like.

The client than has to responds to the Requests it receives. If not, the server will send a DisconnectRequest and can close the connection.

Likewise, the client can send a DisconnectRequest.

Periodically, the client sends a ConnectStateRequest and the server responds with a ConnectStateResponse. This is to check the connection is still active. Without this, the server can close the connection, to free up resources.

### Tunneling
When a tunneling connection is established, actual data is encapsulated in a TunnelingRequest. Again, the client must respond with a acknowlegde, a AcknowlegdeTunnelingResponse. This TunnelingRequest contains a Cemi packet, which is the packet that would also go over a TP link line (not sure exactly). A Cemi packet doesn't have any dependencies to the TunnelingRequest, but of course the TunnelingRequest is dependend on the Cemi packet as a association. Cemi over Tunneling doesn't use the full types of cemi's, because they are not used in IP tunneling.

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
### Code
- Deal with KnxIpHeader consistenly and use KnxIpHeader or embed into Request Response
- KnxClientConnection as an fascade to an implementation to:
  - allow a different config / implementation, being allow nat and routing
  - Maybe is it possible to exlude asio awaitable dependeny from KnxClientConnection
- KnxIpConnection construction with a builder or Config object.
- I would like to use Cpp Modules, need to see how that works with cmake install
### Functional
First focus points:
- DataPointType parsing
  - length data in Cemi is 2 bytes fixed, should be a vector
  - bool type
  - string type 
  - uint8 
- Requests, Response as bytes: dynamic length, not the current hard coded value
- Time-out on (ConnectState)Request without a reponse
  - Resend
  - if no reply / second time-out: disconnect
- cmake install working, preferably with:
  - freebsd port
  - ArchLinux AUR package
