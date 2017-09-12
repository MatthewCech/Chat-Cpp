#include "SocketUtil.hpp"

#if defined(OS_POSIX)
#include <fcntl.h>
#endif



// Static variable instantiation.
Backlog<std::string> SocketUtil::errorLog_;


  /////////////////////////////
 // Public member functions //
/////////////////////////////
// Generates a UDP socket with specified address type: default IPv4
UDPSocketPtr SocketUtil::CreateUDPSocket(SocketAddressType type)
{
  AG_SOCKET newSocket = socket(type, SOCK_DGRAM, IPPROTO_UDP);

  if (newSocket == INVALID_SOCKET)
  {
    ReportError("SocketUtil::CreateUDPSocket");
    return nullptr;
  }

  // Generate and return new shared pointer
  return UDPSocketPtr(new UDPSocket(newSocket));
}


// Set the socket as blocking or not.
int SocketUtil::SetRawSocketBlockingState(AG_SOCKET socket, bool shouldBeBlocking)
{
  int result = AG_NO_ERROR;

#ifdef OS_WINDOWS
  u_long arg = 0;
  if (shouldBeBlocking)
    arg = 1;

  result = ioctlsocket(socket, FIONBIO, &arg);
#else
  int flags = fcntl(socket, F_GETFL, 0);
  if (shouldBeBlocking)
    flags = flags | O_NONBLOCK;
  else
    flags = flags & ~O_NONBLOCK;

  result = fcntl(socket, F_SETFL, flags);
#endif

  if (result == SOCKET_ERROR)
  {
    SocketUtil::ReportError("UDPSocket::SetNonBlockingMode");
    return SocketUtil::GetLastError();
  }

  // Explicity return no error if this worked out.
  return AG_NO_ERROR;
}


// Generates and returns a TCP socket with specified address, default being IPv4
TCPSocketPtr SocketUtil::CreateTCPSocket(SocketAddressType type)
{
  AG_SOCKET newSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);//socket(type, SOCK_STREAM, IPPROTO_TCP);

  if (newSocket == INVALID_SOCKET)
  {
    ReportError("SocketUtil::CreatTCPSocket");
    return nullptr;
  }

  // Generate new shared ptr.
  return TCPSocketPtr(new TCPSocket(newSocket));
}


// Reports that an error has occured to the socket utility class.
void SocketUtil::ReportError(std::string message) 
{
  //! todo:  Note: Consider custom data structure that allows you to define a length that rolls off the end when
  // you are exceeding it. Code this up!!! It should be a template and underlying structure can just be
  // a std::vector, or an array if you *really* want it to be.
  errorLog_.Add(message);
  //DEBUG_PRINT(message); 
}


// Returns the last error recieved
int SocketUtil::GetLastError()
{
  return convertMsgToError(errorLog_[0]);
}


// Helper function allowing null to be passed, allowing conversion between vector of sockets and the fd_set.
fd_set* SocketUtil::fillSetFromVector(fd_set &outSet, const std::vector<TCPSocketPtr> *inSocks, int &ndfs)
{
  if (inSocks)
  {
    FD_ZERO(&outSet);

    for (const TCPSocketPtr& s : *inSocks)
    {
#ifdef OS_WINDOWS
      FD_SET(s->socket_, &outSet);
#else
      //ndfs = ( get max of ndfs and socket number and use that)
#endif
    }
    return &outSet;
  }

  return nullptr;
}


// Helpet function to allow a vector out sockets ot be filled as necessary, which allows for null to be an argument.
void SocketUtil::fillVectorFromSet(std::vector<TCPSocketPtr>* outSocks, const std::vector<TCPSocketPtr>* inSocks, const fd_set &inSet)
{
  if (inSocks && outSocks)
  {
    outSocks->clear();
    
    for (const TCPSocketPtr& s : *inSocks)
      if (FD_ISSET(s->socket_, &inSet))
        outSocks->push_back(s);
  }
}


// Allows the functionality of select to be used while running a server. This specifically allows for
// polling of blocking sockets so that you can only get the relevant ones when necessary. While you could
// just have a server of non-blocking sockets, that gets inefficient over time, which for a multiplayer
// game can become relevant.
// Note that select argument 0, nfds, is posix specific. It can be ignored on windows.
int SocketUtil::Select(const std::vector<TCPSocketPtr> *inReadSet, std::vector<TCPSocketPtr> *outReadSet
                     , const std::vector<TCPSocketPtr> *inWriteSet, std::vector<TCPSocketPtr> *outWriteSet
                     , const std::vector<TCPSocketPtr> *inExceptSet, std::vector<TCPSocketPtr> *outExceptSet)
{
  fd_set read;
  fd_set write;
  fd_set except;
  int ndfs = 0;


  fd_set *readPtr = fillSetFromVector(read, inReadSet, ndfs);
  fd_set *writePtr = fillSetFromVector(write, inWriteSet, ndfs);
  fd_set *exceptPtr = fillSetFromVector(except, inExceptSet, ndfs);
  
  int toRet = 0;
#ifdef OS_WINDOWS
  // Note: Last value is a timeout value.
  toRet = select(0, readPtr, writePtr, exceptPtr, nullptr);
#else
  toRet = select(this argument is now relevant so do something about it, readPtr, writePtr, exceptPtr, nullptr);
#endif

  if (toRet > 0)
  {
    fillVectorFromSet(outReadSet, inReadSet, read);
    fillVectorFromSet(outWriteSet, inWriteSet, write);
    fillVectorFromSet(outExceptSet, inExceptSet, except);
  }

  // Returns any error code if relevant.
  return toRet;
}


  //////////////////////////
 // Private member funcs //
//////////////////////////
// Converts the provided string into an error message.
int SocketUtil::convertMsgToError(const std::string &msg)
{
  return 1;
}


