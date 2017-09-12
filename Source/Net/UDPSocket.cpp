#include "UDPSocket.hpp"
#include "SocketUtil.hpp"
#ifndef OS_WINDOWS
#include <unistd.h> // close
#endif

// Closes the socket
UDPSocket::~UDPSocket()
{
  #ifdef OS_WINDOWS
  closesocket(socket_);
  #else
  close(socket_);
  #endif
}


// Binds this socket to a spectici address. Returns error if one occurs, otherwise 0.
int UDPSocket::Bind(const SocketAddress &sockAddr)
{
  int retval = AG_NO_ERROR;
  retval = bind(socket_, &sockAddr.GetAsSockAddr(), sockAddr.GetSize());

  if (retval != AG_NO_ERROR)
  {
    SocketUtil::ReportError("UDPSocket::Bind");
    return retval; //! todo: SocketUtil::GetLastError();
  }

  return retval;
}


// Sends a buffer to the specified SocketAddress.
// Returns the number of bytes sent, or error as a negative number.
int UDPSocket::SendTo(const void *dataToSend, int dataLen, const SocketAddress &destination)
{
  int retval = 0;
  retval = sendto(socket_, static_cast<const char*>(dataToSend), dataLen, 0, &destination.GetAsSockAddr(), destination.GetSize());

  // Check number of bytes sent that is the return value of sendto.
  if (retval >= 0)
    return retval;
  else
  {
    SocketUtil::ReportError("UDPSocket::SendTo");
    return retval; //! todo: SocketUtil::GetLastError();
  }
}


// Recieve from a remote specified socket in a blocking way.
// Return value is number of bytes recieved, or error as a negative number.
int UDPSocket::RecieveFrom(void *recieveBuffer, int bufferLength, SocketAddress &externalSource)
{
  int retval = 0;                      // stored return value
  socklen_t size = externalSource.GetSize(); // size to be updated
  retval = recvfrom(socket_, static_cast<char *>(recieveBuffer), bufferLength, 0, &externalSource.GetAsSockAddr(), &size);

  // If we got 0 or more bytes, awesome.
  if(retval >= 0)
    return retval;
  else
  {
    SocketUtil::ReportError("UDPSocket::RecieveFrom");
    return retval;
  }
}


// Sets the socket mode to either blocking or non-blocking.
// Returns an error code <0 if the operation fails, reuturns 0
// upon success.
int UDPSocket::SetNonBlockingMode(bool isNonBlocking)
{
  return SocketUtil::SetRawSocketBlockingState(socket_, isNonBlocking);
}


// Constructor for the UDP socket object
UDPSocket::UDPSocket(AG_SOCKET insocket)
  : socket_(insocket)
{  }
