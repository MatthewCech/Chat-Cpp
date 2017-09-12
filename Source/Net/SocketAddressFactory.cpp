#include "SocketAddressFactory.hpp"
#include <string.h> // memset


/*!****************************************************************\
@brief
Creates SocketAddress from string.
@note:
Is BLOCKING because of getaddrinfo call.
@param      inString
The String
@param      inDefaultPort
The Optional Default Port
@return
Pointer to SocketAddress

@todo
Better error reporting/handling.
\******************************************************************/
SocketAddressPtr SocketAddressFactory::CreateIPv4FromString(const std::string& inString, const std::string& inDefaultPort)
{
  // Extract port ("service"), if exists.
  auto pos = inString.find_last_of(':');
  std::string host, service;

  if (pos != std::string::npos)
  {
    host = inString.substr(0, pos);
    service = inString.substr(pos + 1);
    DEBUG_PRINT("PORT FOUND: " << service);
  }
  else
  {
    host = inString;

    // Use default port
    service = inDefaultPort;
    DEBUG_PRINT("NO PORT FOUND!\n");
  }

  // We now have the host and port, commence dns lookup
  addrinfo hint;
  memset(&hint, 0, sizeof(hint));
  hint.ai_family = AF_INET;

  addrinfo* result = nullptr;
  int error = getaddrinfo(host.c_str(), service.c_str(), &hint, &result);

  if (error != 0)
  {
    if (result != nullptr)
      freeaddrinfo(result);

    return nullptr;
  }


  // Walk the result list until we find a valid address, or last element
  addrinfo* cur_info = result;
  while (!cur_info->ai_addr && cur_info->ai_next)
  {
    cur_info = cur_info->ai_next;
  }

  if (!cur_info->ai_addr)
  {
    freeaddrinfo(result);
    return nullptr;
  }

  SocketAddressPtr toRet = std::make_shared<SocketAddress>(*result->ai_addr);

  //! todo: establish validity of line. I noticed it was omitted, added it.
  freeaddrinfo(result);

  return toRet;
};
