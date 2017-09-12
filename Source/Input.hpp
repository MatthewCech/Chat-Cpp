#pragma once
#include "Defines.hpp"
#include <string>
#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <thread>

#ifdef OS_WINDOWS
#include <conio.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif



#define MAX_INPUT_LINE_SIZE 1250


// Checks to see if enter was pressed, dumping input.
inline bool KeyboardHit();

// Gets the last line of input on the command line.
inline std::string GetLastLine();



#ifdef OS_WINDOWS
std::string line;
inline bool KeyboardHit()
{
  if (_kbhit())
    return true;
  return false;
}
#else
bool KeyboardHit()
{
  struct timeval tv;
  fd_set fds;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
  select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
  return FD_ISSET(STDIN_FILENO, &fds);
}
#endif

inline std::string GetLastLine()
{
  char iCh[MAX_INPUT_LINE_SIZE];
  return fgets(iCh, MAX_INPUT_LINE_SIZE, stdin);
}


