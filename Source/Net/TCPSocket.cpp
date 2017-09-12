#include "TCPSocket.hpp"
#include "SocketUtil.hpp"

#define RETURN_ERROR(x) do{             \
    SocketUtil::ReportError(x);         \
    return -SocketUtil::GetLastError(); \
    } while(0)                          \

// Constructor. Takes in a socket object to base TCP socket off of and internally store.
TCPSocket::TCPSocket(AG_SOCKET socket)
  : socket_(socket)
{  }


// Destructor. Cleans up any mess associated with the socket.
TCPSocket::~TCPSocket()
{
  // TODO: handle closing properly
}


// Connects to a remote socket address. Returns an error code if one occured,
// otherwise returns 0 / AG_NO_ERROR
int TCPSocket::Connect(const SocketAddress &addr)
{
  // Connect our internal socket to a specififed address address.
  int res = AG_NO_ERROR;
  res = connect(socket_, &addr.GetAsSockAddr(), addr.GetSize());

  if (res < 0)
  {
    SocketUtil::ReportError("TCPSocket::Connect");
    return SocketUtil::GetLastError();
  }

  return res;
}


// Binds the scoket to a remote address.
int TCPSocket::Bind(const SocketAddress &sockAddr)
{
  int res = AG_NO_ERROR;
  res = bind(socket_, &sockAddr.GetAsSockAddr(), sockAddr.GetSize());

  if (res != AG_NO_ERROR)
  {
    SocketUtil::ReportError("TCPSocket::Bind");
    return SocketUtil::GetLastError();
  }

  return res;
}


// Socket listening setup- binds the current socket to listen,
// accumulating a backlog of info.
int TCPSocket::Listen(int backlog)
{
  int res = AG_NO_ERROR;
  res = listen(socket_, backlog);

  if (res < 0)
  {
    SocketUtil::ReportError("TCPSocket::Listen");
    return SocketUtil::GetLastError();
  }

  return res;
}


// Accept a new socket based on an existing TCP conneciton.
std::shared_ptr<TCPSocket> TCPSocket::Accept(SocketAddress &incomingAddr)
{
  socklen_t len = incomingAddr.GetSize();
  AG_SOCKET sock = accept(socket_, &incomingAddr.GetAsSockAddr(), &len);

  if (sock != INVALID_SOCKET)
    return TCPSocketPtr(new TCPSocket(sock));
  else
  {
    SocketUtil::ReportError("TCPSocket::Accept");
    return nullptr;
  }
}


// Send a buffer of specified length len with the socket.
int TCPSocket::Send(const void *toSend, unsigned int len)
{
  int sentBytes = 0;
  sentBytes = send(socket_, static_cast<const char*>(toSend), len, 0);

  if (sentBytes < 0)
  {
    SocketUtil::ReportError("TCPSocket::Send");
    return SocketUtil::GetLastError();
  }

  // return number of bytes successfully sent.
  return sentBytes;
}


// Recieved a buffer designed to fit in the specified length with the socket.
int TCPSocket::Recieve(void *inbuff, unsigned int len)
{
  int recBytes = 0;
  recBytes = recv(socket_, static_cast<char *>(inbuff), len, 0);

  if (recBytes < 0)
  {
    SocketUtil::ReportError("TCPSocket::Recieve");
    return SocketUtil::GetLastError();
  }

  // Return number of bytes we just recieved.
  return recBytes;
}


// Sets the socket mode to either blocking or non-blocking.
// Returns an error code <0 if the operation fails, reuturns 0
// upon success.
int TCPSocket::SetNonBlockingMode(bool shouldBeNonBlocking)
{
  return SocketUtil::SetRawSocketBlockingState(socket_, shouldBeNonBlocking);
}
