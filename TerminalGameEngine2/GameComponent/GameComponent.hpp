#pragma once
#include "../Core/Math/Matrix.hpp"
#include "../Core/GameObject.hpp"
#include <string>
class GameObject;
/*
Base component class of the Entity(GameObject)-Component System architecture
Must be added to a GameObject
*/
class GameComponent
{
public:
	GameComponent(std::string str_name);

	std::string getName();

	GameObject * getParentGameObject();
	void setParentGameObject(GameObject * gob_par);

	virtual void update(int delta) {}
	virtual void render(Mat4x4 mat_transform) {}
private:
	std::string _str_name;
	GameObject *_gob_parent;
};