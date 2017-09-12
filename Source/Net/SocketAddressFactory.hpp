/*!****************************************************************\
@brief
  Factory for creating SocketAddress objects.
@note
  See page 77 in book.
\******************************************************************/

#include "SocketAddress.hpp"
#include <string>



class SocketAddressFactory
{
public:

  // Creates and returns a pointer to a socket address that was generated with a string address.
  // If unspecified, a port is selected automagically.
  static SocketAddressPtr CreateIPv4FromString(const std::string& inString, const std::string& inDefaultPort = "0");
};
