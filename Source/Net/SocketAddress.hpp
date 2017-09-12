/*!****************************************************************\
@brief
  Type-safe Socket Address Wrapper.
@note
  See page 74 in book for starting point.
\******************************************************************/
#pragma once
// General includes
#include <iostream>       // Printing
#include <memory>         // std::shared_ptr
#include <string>         // String formatting
#include <Defines.hpp>    // OS deduction

#ifdef OS_WINDOWS

// Windows specific
#include "WinsockManager.hpp"

#else

// Linux/Mac specific
#include <sys/types.h>  // ? - 
#include <sys/socket.h> // sockets
#include <arpa/inet.h>  // inet_ntop
#include <netinet/in.h> // ? - IPv4 functionality
#include <netinet/ip.h> // ? - 
#include <netdb.h>      // addrinfo

#endif



class SocketAddress
{
public:
  // Address creation
  //! todo: IPv6 Constructor
  SocketAddress(uint32_t inAddress, uint16_t inPort);
  SocketAddress(const sockaddr& inSockAddr);
  SocketAddress();

  // Accessors
  sockaddr& GetAsSockAddr();
  sockaddr* GetAsSockAddrPtr();
  const sockaddr &GetAsSockAddr() const;
  size_t GetSize() const;
  uint16_t GetPort() const;
  std::string GetIP() const;

  // Overloaded Operators
  friend std::ostream& operator<<(std::ostream& os, const SocketAddress& addr);

private:
  // Private Member Functions
  sockaddr_in* getAsSockAddrIn();
  const sockaddr_in* getAsSockAddrInConst() const;
  
  // Variables
  sockaddr sockAddr_;
};

typedef std::shared_ptr<SocketAddress> SocketAddressPtr;
