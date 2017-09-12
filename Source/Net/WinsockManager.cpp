#include "WinsockManager.hpp"

#ifdef OS_WINDOWS

// Windows specific socket manager static object for library init
WinsockManager WinsockManager::wsm_ = WinsockManager();


// Starts app-specific windows networking.
WinsockManager::WinsockManager()
{
  WSADATA wsa;
  int res = WSAStartup(MAKEWORD(2, 2), &wsa);
  //! todo: handle result value
}


// Shuts down app-specific windows networking.
WinsockManager::~WinsockManager()
{
  WSACleanup();
}

#endif // OS_WINDOWS
