#include <iostream>
#include <vector>
#include <chrono>
#include "Testing.hpp"
#include "Net/SocketUtil.hpp"
#include "Net/SocketAddressFactory.hpp"
#include "Input.hpp"

// Defines
#define BUFFER_SIZE 1400


void RunClient(const char * serverAddress)
{
  DEBUG_PRINT("Starting TCP client...");

  // Acquire socket connection. We just create a generic socket, and we point it in a direction.
  // In this case, we are providng it with an IP to connect to.
  TCPSocketPtr tcpSocket{ SocketUtil::CreateTCPSocket(SocketUtil::IPv4) };
  SocketAddressPtr addr{ SocketAddressFactory::CreateIPv4FromString(serverAddress) };

  // This is a client socket, and we only have one of them, so setting it to non-blocking so we 
  // can just casually loop over it is a good idea. We wouldn't do this on the server because
  // asking every single socket manually can be a bit of a performance issue, although if you
  // only have a few connecting at a time then you're probably fine to do it anyways.
  tcpSocket->SetNonBlockingMode(true);

  // Wait for that connection tho.
  while (tcpSocket->Connect(*addr) < 0) {};

  // Are we running this loop?
  bool isRunning{ true };

  // Primary Loop.
  while (isRunning)
  {
    // Buffer Buffer, one more than the size because I don't trust myself with basic math.
    char data[BUFFER_SIZE + 1]{ 0 };

    // Check if we have any keyboard input!
    // If we do, we should make sure we strip the ending newline or whatever we want to do it it,
    // then send it on its way to the server.
    if (KeyboardHit())
    {

      std::string input{ GetLastLine() };
      input = input.substr(0, input.size() - 1);
      if (input.size() > 0)
        tcpSocket->Send(input.c_str(), input.length());
    }

    // Recieve and process any data from the server here!
    unsigned int bytesRecieved{ static_cast<unsigned int>(tcpSocket->Recieve(data, BUFFER_SIZE)) };
    if (bytesRecieved > 1)
    {
      std::string toPrint{ data, bytesRecieved };
      std::cout << "[Server]: " << toPrint << '\n';
    }

    // Don't scream eternal, give it a rest for like 16ms, which is approximately 
    // the time between loops in a game running at 60fps.
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
  }
}


void RunServer()
{
  DEBUG_PRINT("Starting TCP server...");

  // Two vectors - one for checking new connections that are blocking,
  // and the other for being populated with sockets that have things 
  // which need to be read from.
  std::vector<TCPSocketPtr> readableBlockingSockets;
  std::vector<TCPSocketPtr> readableSockets;

  // Create our socket, using IPV4
  TCPSocketPtr listenSocket{ SocketUtil::CreateTCPSocket(SocketUtil::IPv4) };

  // Establish we want to listen to any address. We have specified port 27015 for
  // this, although as long as the port is over 10000 you should be fine. This port
  // will have to be specified by the client tho.
  SocketAddress recieveAddr{ INADDR_ANY, 27015 };

  // Init and bind to the recieving address we have. 
  if (listenSocket->Bind(recieveAddr) != AG_NO_ERROR)
    return;

  // Start listening. This is essential, we have to tell the port to actually listen
  // for incoming connections.
  listenSocket->Listen();

  // Now that we are bound, unblocked and listenting, push back to readable.
  readableBlockingSockets.push_back(listenSocket);

  // Are we running this loop?
  bool isRunning{ true };

  // Primary server loop
  while (isRunning)
  {
    // If select tells us that we have some sockets that need attention, lets loop through them.
    // We are using select to parse through our custom vector of blocking sockets and then having
    // it populate another vector with sockets that can be read from as necessary.
    if (SocketUtil::Select(&readableBlockingSockets, &readableSockets, NULL, NULL, NULL, NULL))
      for (const TCPSocketPtr &socket : readableSockets)
      {
        if (socket == listenSocket)
        {
          // In here, we handle a new connection.
          // The socket address here can be used to find the IP of the person connecting,
          // and should probably be saved off to keep track of it if you want it later.
          // The client can be replied to using the specific TCPSocketPtr here,
          // with tcpptr.
          SocketAddress newClientAddr;
          TCPSocketPtr tcpptr{ listenSocket->Accept(newClientAddr) };

          // We don't want to just leave our client in the cold, so it's probably a good idea to
          // take note of their valiant effort and give them a nice pat on the back before
          // they try to do anything else.
          DEBUG_PRINT("New connection from: " << newClientAddr);
          tcpptr->Send("Connected!", 10);
          
          // Oh, and we should add it to the list of sockets we can now listen to.
          readableBlockingSockets.push_back(tcpptr);
        }
        else
        {
          // This is for sockets that have an existing connection associated with them.
          // The socketaddress is not present here, so if any information it holds needs
          // to be accessed, it's best to have a structure linking the sockaddr from earlier
          // to this. The + 1 is optional, but prevents mistakes since we all know i have trouble
          // counting sometimes.
          char segment[BUFFER_SIZE + 1]{ 0 };
          int dataRecieved = socket->Recieve(segment, BUFFER_SIZE);

          // If we recieved a single null byte, we have lost connection.
          if (dataRecieved == 1 && segment[0] == '\0')
          {
            // Look through our list of readable sockets and remove the unfortunately now EX socket. (haha like a parrot or whatever)
            for (unsigned int j = 0; j < readableBlockingSockets.size(); ++j)
              if (readableBlockingSockets[j] == socket)
              {
                // Socket can now be removed.
                readableBlockingSockets.erase(readableBlockingSockets.begin() + j);
                DEBUG_PRINT("Client Disconnected!");
                break;
              }
          }

          // Realistically we can do whatever with the data here, but we only have to do something with it
          // if we actually recieved anything.
          else if (dataRecieved > 0)
            DEBUG_PRINT("Recieved Bytes: " << std::string(segment, dataRecieved));
        }
      }
  }
}


// Application Entry point
int main(int argc, char const *argv[])
{
  if (argc < 2 || argc > 3)
  {
    std::cout << "Incorrect arguments! Please pass 1 for server or 2 for client! <application name> <1 | 2> <if 2: IP:PORT>\n";
    std::cout << "For client, please specify the server address with port number as the third argument!";
    return -1;
  }

  if (argv[1][0] == '1')
    RunServer();
  else if(argv[1][0] == '2')
    RunClient(argv[2]);

  return 0;
}

