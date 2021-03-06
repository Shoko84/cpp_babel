/*
** CmdCallJoin.cpp for Server in /home/enguerrand/delivery/Server/CmdCallJoin.cpp
**
** Made by Enguerrand Allamel
** Login   <enguerrand.allamel@epitech.eu>
**
** Started on  mer. sept. 27 16:43:05 2017 Enguerrand Allamel
** Last update mer. sept. 27 16:43:05 2017 Enguerrand Allamel
*/

#include "CmdCallJoin.hpp"
#include "Server.hpp"


babel::CmdCallJoin::CmdCallJoin(babel::Server &server):
	_server(server)
{
}

babel::CmdCallJoin::~CmdCallJoin()
{

}

bool babel::CmdCallJoin::run(size_t tunnelId, babel::NetworkData & data)
{
  NetworkDataCSJoin networkDataCSJoin = {0};
  std::copy_n(data.data.data(), sizeof(NetworkDataCSJoin), reinterpret_cast<char *>(&networkDataCSJoin));

  if (networkDataCSJoin.idCall != 0)
    {
      if (!this->_server.getCallManager().convIsExist(networkDataCSJoin.idCall) &&
	      !this->_server.getCallManager().isAllowToJoin(networkDataCSJoin.idCall,
							    this->_server.getNetworkManager().get()->getTunnelInfoByTunnelId(tunnelId).login))
	{
	  this->_server.getNetworkManager().get()->write(tunnelId, NetworkData(506, 0, {}));
	  return false;
	}

      CLIENT_CALL_STRUCT client = {0};
      NetworkDataCSJoinSuccess networkDataCSJoinSuccess = {0};
      networkDataCSJoinSuccess.idCall = networkDataCSJoin.idCall;


      this->_server.getCallManager().add(networkDataCSJoin.idCall,
					 this->_server.getNetworkManager().get()->getTunnelInfoByTunnelId(tunnelId).login,
					 this->_server.getNetworkManager().get()->getTunnelInfoByTunnelId(tunnelId).ip,
					 networkDataCSJoin.port);

      auto participant = (*this->_server.getCallManager().getCalls().find(networkDataCSJoin.idCall)).second.getParticipants();
      auto it = participant.begin();
      for (auto i = 0 ; i < 8 ; i++)
	{
	  if (it != participant.end())
	    {
	      (*it).second.login.copy(networkDataCSJoinSuccess.client[i].login, 32);
	      (*it).second.ip.copy(networkDataCSJoinSuccess.client[i].ip, 16);
	      networkDataCSJoinSuccess.client[i].port = (*it).second.port;
	      it++;
	    }
	  else
	    {
	      networkDataCSJoinSuccess.client[i].login[0] = '\0';
	      networkDataCSJoinSuccess.client[i].ip[0] = '\0';
	      networkDataCSJoinSuccess.client[i].port = 0;
	    }
	}
      std::array<char, 2048> dataSend = {0};
      std::copy_n(reinterpret_cast<char *>(&networkDataCSJoinSuccess), sizeof(NetworkDataCSJoinSuccess), dataSend.begin());

      this->_server.getNetworkManager().get()->write(tunnelId, NetworkData(43, sizeof(NetworkDataCSJoinSuccess), dataSend));

      NetworkDataSCJoin networkDataSCJoin = {0};
      this->_server.getNetworkManager().get()->getTunnelInfoByTunnelId(tunnelId).login.copy(networkDataSCJoin.client.login, 32);
      this->_server.getNetworkManager().get()->getTunnelInfoByTunnelId(tunnelId).ip.copy(networkDataSCJoin.client.ip, 16);
      networkDataSCJoin.client.port = networkDataCSJoin.port;

      networkDataSCJoin.idCall = networkDataCSJoin.idCall;

      dataSend.fill(0);
      std::copy_n(reinterpret_cast<char *>(&networkDataSCJoin), sizeof(NetworkDataSCJoin), dataSend.begin());

      for (auto ite : participant)
	{
	  try
	    {
	      if (ite.second.login.compare(this->_server.getNetworkManager().get()->getTunnelInfoByTunnelId(tunnelId).login) != 0)
		this->_server.getNetworkManager().get()->write(ite.first, NetworkData(9, sizeof(NetworkDataSCJoin), dataSend));
	    }
	  catch (NetworkManagerException)
	    {
	      continue;
	    }
	}
      return true;
    }
  else
    {
      auto call = this->_server.getCallManager().create();

      std::array<char, 2048> dataSend = {0};

      if (!this->_server.getCallManager().add(call.getId(),
					      this->_server.getNetworkManager().get()->getTunnelInfoByTunnelId(tunnelId).login,
					      this->_server.getNetworkManager().get()->getTunnelInfoByTunnelId(tunnelId).ip,
					      networkDataCSJoin.port))
	{
	  this->_server.getNetworkManager().get()->write(tunnelId, NetworkData(501, 0, {}));
	  return false;
	}

      NetworkDataCSJoinSuccess networkDataCSJoinSuccess = {0};

      networkDataCSJoinSuccess.idCall = call.getId();

      this->_server.getNetworkManager().get()->getTunnelInfoByTunnelId(tunnelId).login.copy(networkDataCSJoinSuccess.client[0].login, 32);
      this->_server.getNetworkManager().get()->getTunnelInfoByTunnelId(tunnelId).login.copy(networkDataCSJoinSuccess.client[0].ip, 16);
      networkDataCSJoinSuccess.client[0].port = networkDataCSJoin.port;

      std::copy_n(reinterpret_cast<char *>(&networkDataCSJoinSuccess), sizeof(NetworkDataCSJoinSuccess), dataSend.begin());
      this->_server.getNetworkManager().get()->write(tunnelId, NetworkData(43, sizeof(NetworkDataCSJoinSuccess), dataSend));
      return true;
    }
}
