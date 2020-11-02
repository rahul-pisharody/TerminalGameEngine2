#include "GameComponent.hpp"

GameComponent::GameComponent(std::string str_name) : _str_name(str_name), _gob_parent(nullptr) 
{
}

std::string GameComponent::getName()
{
	return _str_name;
}

GameObject * GameComponent::getParentGameObject()
{
	return _gob_parent;
}

void GameComponent::setParentGameObject(GameObject * gob_par)
{
	_gob_parent = gob_par;
}
