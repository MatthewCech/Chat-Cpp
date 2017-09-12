/*!****************************************************************\
@brief
  Type-safe UDP Socket Wrapper.
@note
  See page 81 in book.
\******************************************************************/
#pragma once
#include <Defines.hpp>       // AG_SOCKET 
#include "SocketAddress.hpp" // SocketAddress
#include <memory>            // std::shared_ptr



class UDPSocket
{
public:
  // Destructor
  ~UDPSocket();

  // Standard Operations
  int Bind(const SocketAddress &sockAddr);
  int SendTo(const void *dataToSend, int dataLen, const SocketAddress &destination);
  int RecieveFrom(void *recieveBuffer, int bufferLength, SocketAddress &externalSource);
  
  // Settings
  int SetNonBlockingMode(bool isNonBlocking);

private:
  // Variables
  AG_SOCKET socket_;
  
  // Mark SocketUtil as a friend to prevent public instanciation
  friend class SocketUtil;
  UDPSocket(AG_SOCKET insocket);
};

// Define shared pointer type
typedef std::shared_ptr<UDPSocket> UDPSocketPtr;
