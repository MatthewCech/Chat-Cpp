#pragma once
#include <string>
#include "Defines.hpp"



namespace NetTest
{
  const std::string TEST_PORT("36390");
  const std::string LOCAL_IP("127.0.0.1");
  const std::string LOCAL_ADDR(LOCAL_IP + ":" + TEST_PORT);

  int TCPConnect(bool *isRunning, int argc, const char** argv);
  int AddressTest(int argc, char const *argv[]);
  int PrimaryUDPLoop(bool *isRunning);
  int KeyboardInputNonblockingTest(bool *isRunning);
  int PrimaryUDPLoopWithInput(bool *isRunning);
  int AddrSpecified(bool *isRunning, int argc, const char *argv[]);
  int TCPAddrSpecified(bool *isRunning, int argc, const char *argv[]);
  int JSONTest();
  int MessageObjectTest();

  // Server pair
  int UDPFarEnd(bool *isRunning, int argc, const char* argv[]);
  int UDPTestServer(bool *isRunning, int argc, const char *argv[]);

}
