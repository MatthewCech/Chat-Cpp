#pragma once
#include "SocketAddress.hpp"
#include <memory>



class TCPSocket
{
  public:
    // Destructor
    ~TCPSocket();

    // Socket setup and general operation
    int Connect(const SocketAddress &addr);
    int Bind(const SocketAddress &remoteAddr);
    int Listen(int backlog = 32);
    std::shared_ptr<TCPSocket> Accept(SocketAddress &incomingAddr);
    int Send(const void *toSend, unsigned int len);
    int Recieve(void *inbuff, unsigned int len);

    // Socket settings
    int SetNonBlockingMode(bool shouldBeNonBlocking);

  private:
    // Mark friends
    friend class SocketUtil;

    // Private constructor
    TCPSocket(AG_SOCKET socket);

    // Variables
    AG_SOCKET socket_;
    SocketAddress info_;
};

typedef std::shared_ptr<TCPSocket> TCPSocketPtr;

