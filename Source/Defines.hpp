/*!****************************************************************\
@brief
  Our own OS defines
\******************************************************************/
#pragma once


  ////////////////
 // OS Defines //
////////////////
// Windows
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define OS_WINDOWS
#endif

// MacOSX
#if defined(__APPLE__)
#define OS_MACOSX
#endif

// Linux
#if defined(__linux__)
#define OS_LINUX
#endif

#if defined(OS_MACOSX) || defined(OS_LINUX)
#define OS_POSIX
#endif


  /////////////////////
 // Network Defines //
/////////////////////
// sockaddr_in->sin_addr.S_un exists
#if !defined(OS_MACOSX)
#define NET_SIN_ADDR_IS_UNION
#endif

// sockaddr_in->sin_len exists and must be set
#if defined(OS_MACOSX)
#define NET_SIN_ADDR_CONTAINS_SIZE
#endif

// Socket is either a custom type or is just an int
#ifdef OS_WINDOWS
#define AG_SOCKET SOCKET
#else
#define AG_SOCKET int
#endif

#if defined(OS_POSIX)
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#endif


  ////////////////////////
 // Additional Defines //
////////////////////////
// Formatting Defines
#define UNUSED(x) (void)(x)
#define DEBUG_PRINT(x) do{if(DEBUG){ std::cout << x << '\n'; }} while(0)
#define AG_NO_ERROR 0

// Server Defines
#define GGSERVER_API_VERSION 1