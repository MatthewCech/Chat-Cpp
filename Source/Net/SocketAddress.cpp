#include "SocketAddress.hpp"
#include <string.h> // memset, memcpy



// Takes 4-byte IPv4 address and port
SocketAddress::SocketAddress(uint32_t inAddress, uint16_t inPort)
{
  memset(&sockAddr_, 0, sizeof(sockaddr));

  sockaddr_in *my_addr_in = getAsSockAddrIn();
  my_addr_in->sin_family = AF_INET;
  my_addr_in->sin_port   = htons(inPort);

  #ifdef SIN_ADDR_IS_UNION
    my_addr_in->sin_addr.S_un.S_addr = htonl(inAddress);
  #else
    my_addr_in->sin_addr.s_addr = htonl(inAddress);
  #endif

  #ifdef SIN_ADDR_CONTAINS_SIZE
    // Mac-specific addr_in size variable
    my_addr_in->sin_len = sizeof(sockaddr_in);
  #endif
}


// Takes existing native sockaddr and wraps copy
SocketAddress::SocketAddress(const sockaddr& inSockAddr)
{
  memcpy(&sockAddr_, &inSockAddr, sizeof(sockaddr));
}


// Default constructor
SocketAddress::SocketAddress()
{
  memset(&sockAddr_, 0, sizeof(sockaddr));

  sockaddr_in *my_addr_in = getAsSockAddrIn();
  my_addr_in->sin_family = AF_INET;
  my_addr_in->sin_port = 0;

#ifdef SIN_ADDR_IS_UNION
  my_addr_in->sin_addr.S_un.S_addr = htonl(INADDR_ANY);
#else
  my_addr_in->sin_addr.s_addr = htonl(INADDR_ANY);
#endif

#ifdef SIN_ADDR_CONTAINS_SIZE
  // Mac-specific addr_in size variable
  my_addr_in->sin_len = sizeof(sockaddr_in);
#endif
}

// Returns size_t of the sockaddr underlying object
size_t SocketAddress::GetSize() const 
{ 
  return sizeof(sockaddr);
}


// Returns 2-byte port of the socketaddress
uint16_t SocketAddress::GetPort() const
{
  const sockaddr_in* sai = getAsSockAddrInConst();
  return ntohs(sai->sin_port);
}


// Returns the IP as a human readable string.
std::string SocketAddress::GetIP() const
{
  const sockaddr_in* sai = getAsSockAddrInConst();
  char addr[INET_ADDRSTRLEN];

  // grab address IP
  #ifdef SIN_ADDR_IS_UNION
    inet_ntop(sai->sin_family, &(sai->sin_addr.S_un.S_addr), addr, INET_ADDRSTRLEN);
  #else
    #ifdef OS_WINDOWS
      inet_ntop(sai->sin_family, (sockaddr*)(&sai->sin_addr), addr, INET_ADDRSTRLEN);
    #else
      inet_ntop(sai->sin_family, &(sai->sin_addr), addr, INET_ADDRSTRLEN);
    #endif
  #endif

  return std::string(addr);
}


// Return a pointer to the sockaddr cast to a sockaddr_in
sockaddr_in* SocketAddress::getAsSockAddrIn()
{
  return reinterpret_cast<sockaddr_in*>(&sockAddr_);
}


// Return a pointer to the sockaddr constant sockaddrin.
const sockaddr_in* SocketAddress::getAsSockAddrInConst() const
{
  return reinterpret_cast<const sockaddr_in*>(&sockAddr_);
}


// Returns the sock address as modifiable reference.
sockaddr &SocketAddress::GetAsSockAddr() 
{
  return sockAddr_;
}


// Returns the socketaddr pointer
sockaddr* SocketAddress::GetAsSockAddrPtr()
{
  return &sockAddr_;
}


// Returns the sockaddr private variable as a constant.
const sockaddr &SocketAddress::GetAsSockAddr() const
{
  return sockAddr_;
}


// Overloaded shift operator for printing, returns a reference to the ostream passed.
std::ostream& operator<<(std::ostream& os, const SocketAddress& sa)
{
  const sockaddr_in* sai = sa.getAsSockAddrInConst();

  std::string family;

  // grab address family
  switch(sai->sin_family)
  {
    case AF_INET:
      family = "IPV4";
      break;
    case AF_INET6:
      family = "IPV6";
      break;
    case AF_IPX:
    case AF_APPLETALK:
    case AF_UNSPEC:
    default:
      family = "UNDEF";
  }

  os << family << ":[" << sa.GetIP() << ":" << sa.GetPort() << "]";

  return os;
}
