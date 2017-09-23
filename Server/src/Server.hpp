/*
** Server.hpp for Server in /home/enguerrand/delivery/Server/Server.hpp
**
** Made by Enguerrand Allamel
** Login   <enguerrand.allamel@epitech.eu>
**
** Started on  mer. sept. 20 18:51:53 2017 Enguerrand Allamel
** Last update mer. sept. 20 18:51:53 2017 Enguerrand Allamel
*/

#ifndef SERVER_SERVER_HPP
#define SERVER_SERVER_HPP

#include <iostream>
#include <unordered_map>
#include <memory>
#include "BabelNetworkMacro.hpp"
#include "Network/Boost/NetworkTcpServerBoost.hpp"
#include "Log/LogInTerm.hpp"
#include "Cmd/HandleCmd.hpp"

namespace babel {
  class Server {
   protected:
    unsigned int								_port;
    std::shared_ptr<NetworkTcpServerBoost>					_tcpServer;
    std::unordered_map<std::string, std::shared_ptr<INetworkTcpServerTunnel>>	_tcpTunnel;
    LogInTerm									_logInTerm;
    HandleCmd									_handleCmd;
   public:
    Server(unsigned int port = 4242);
    ~Server();

    void run();

    const LogInTerm getLogInTerm() const;
  };
}

#endif //SERVER_SERVER_HPP
