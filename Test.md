# Tunneling Connection request

## Start connection should send ConnectRequest
## ConnectStateResponse should esteblish connection
## No response on first ConnectRequest should resend ConnectRequest
## Second response on ConnectRequest should esteblish a connection

# Tunneling Connection state

## Connection should send a ConnectStateRequest periodically
## No response on ConnectStateRequest resends ConnectStateRequest
## No response on second ConnectStateRequest should disconnect
## Response on only second ConnectStateRequest should reconnect
## Handling of a disconnect request
Client sends a disconnectresponse message and closes the connection.
## Socket disconnect should close the connection
Because knx ip Tunneling is based on UDP there is no such thing as a lost connection. But the port could be closed on os level, e.g. when a physical connection is lost.
## After a socket disconnect should be able to reconnect
## After a disconnect because of DisconnectRequest should be able to reconnect
## After a disconnect because of no reaction to ConnectStateRequest should be able to reconnect
## After a disconnect because of wrong response to ConnectStateRequest should be able to reconnect

# Tunneling request from server
## Correct handling of a tunneling request from the server
Should send an ack and inform it's listeners

# Cemi

## Ack timeout should result in error to user of API
A cemi Ldata request should result in a cemu L.con (confirm).

