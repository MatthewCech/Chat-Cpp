/*!****************************************************************\
@brief
  Static, windows-specific class designed to staticially initalize
  itself in order to allow windows networking libraries to start up.
\******************************************************************/
#pragma once
#include <Defines.hpp>

#ifdef OS_WINDOWS
#include <WinSock2.h> // Windows networking, the experience.
#include <Ws2tcpip.h> // Winsock extensions and additional windows functionality



class WinsockManager
{
public:
  // Constructor/Destructor
  WinsockManager();
  ~WinsockManager();

private:
  // Variables
  static WinsockManager wsm_;
};


#endif // OS_WINDOWS
