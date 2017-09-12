#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <Defines.hpp>
#include <Backlog.hpp>
#include "SocketAddress.hpp"
#include "UDPSocket.hpp"
#include "TCPSocket.hpp"



class SocketUtil
{
public:
  // Creation methods
  enum SocketAddressType { IPv4 = AF_INET, IPv6 = AF_INET6 };
  static UDPSocketPtr CreateUDPSocket(SocketAddressType type = IPv4);
  static TCPSocketPtr CreateTCPSocket(SocketAddressType type = IPv4);
  static int SetRawSocketBlockingState(AG_SOCKET socket, bool shouldBeBlocking);

  // Management methods
  static void ReportError(std::string message);
  static int GetLastError();
  static int Select(const std::vector<TCPSocketPtr> *inReadSet, std::vector<TCPSocketPtr> *outReadSet
                  , const std::vector<TCPSocketPtr> *inWriteSet, std::vector<TCPSocketPtr> *outWriteSet
                  , const std::vector<TCPSocketPtr> *inExceptSet, std::vector<TCPSocketPtr> *outExceptSet);
private:
  // Private methods
  static fd_set* fillSetFromVector(fd_set &outSet, const std::vector<TCPSocketPtr> *inSocks, int &ndfs);
  static void fillVectorFromSet(std::vector<TCPSocketPtr>* outSockets, const std::vector<TCPSocketPtr>* inSockets, const fd_set &inSet);
  static int convertMsgToError(const std::string &msg);

  // Variables
  static Backlog<std::string> errorLog_;
};
