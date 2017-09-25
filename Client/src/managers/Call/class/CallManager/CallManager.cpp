#include "CallManager.hpp"

babel::CallManager::CallManager(babel::BabelClientManager& ancestor)
	:
	_root(ancestor)
{
	std::cout << "Call manager created" << std::endl;
}

babel::CallManager::~CallManager()
{
	std::cout << "Call manager destructed" << std::endl;
}

babel::BabelClientManager& babel::CallManager::getRoot(void)
{
	return this->_root;
}

const babel::BabelClientManager& babel::CallManager::getRoot(void) const
{
	return this->_root;
}

const babel::Call & babel::CallManager::getCurrentcall(void) const
{
	return this->_currentCall;
}

babel::Call & babel::CallManager::getCurrentCall(void)
{
	return this->_currentCall;
}

const std::vector<babel::Call>& babel::CallManager::getCallHistory(void) const
{
	return this->_callHistory;
}

std::vector<babel::Call>& babel::CallManager::getCallHistory(void)
{
	return this->_callHistory;
}

const babel::Status babel::CallManager::addNewParticipant(const std::string &)
{
	// TODO: network call to add a new participant to the call
	return babel::Status(0, "New participant added");
}

const babel::Status babel::CallManager::leaveCall(void)
{
	// TODO: Encapsulate this in network call (handlecmd)
	this->archiveCurrentCall();
	this->resetCurrentCall();
	// __________________________________________________
	return babel::Status(0, "Call ended");
}

const babel::Status babel::CallManager::joinCall(const std::vector<std::string>& list)
{
	// TODO: Encapsulate this in network call
	this->_currentCall.updateParticipantsList(list);
	this->_currentCall.setActivity(true);
	// _________________________________________
	return babel::Status(0, "Call joined");
}

const std::vector<std::string>& babel::CallManager::updateCurrentCallParticipants(void)
{
	// TODO : Netork call 'server.getParticipantForCall()'
	return this->_currentCall.getParticipants();
}

void babel::CallManager::archiveCurrentCall(void)
{
	this->_callHistory.push_back(babel::Call(this->_currentCall));
	this->resetCurrentCall();
}

void babel::CallManager::resetCurrentCall(void)
{
	this->_currentCall.reset();
}