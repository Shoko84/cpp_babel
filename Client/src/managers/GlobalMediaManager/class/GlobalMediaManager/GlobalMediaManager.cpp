#include <iostream>
#include "GlobalMediaManager.hpp"
#include "BabelClientManager.hpp"

babel::GlobalMediaManager::GlobalMediaManager(babel::BabelClientManager& ancestor)
:
	_root(ancestor),
	_audioManager(*this)
{
	
}

babel::GlobalMediaManager::~GlobalMediaManager() {
	
}

babel::BabelClientManager& babel::GlobalMediaManager::getRoot(void) const
{
	return this->_root;
}

// SOUND
bool 		babel::GlobalMediaManager::playSound(B_SAMPLE sample)
{
	return this->_audioManager.playSound(sample);
}

B_SAMPLE 	babel::GlobalMediaManager::recordSound()
{
	return this->_audioManager.recordSound();
}

bool 	babel::GlobalMediaManager::encodeSound()
{
	return this->_audioManager.encodeSound();
}

bool 	babel::GlobalMediaManager::decodeSound()
{
	return this->_audioManager.decodeSound();
}

void 	babel::GlobalMediaManager::setAudioStreamState(bool state)
{
	this->_audioManager.setStreamState(state);
}