#pragma once
#include <vector>
#include<memory>

#include "Math\Vec.hpp"
#include "Math\Matrix.hpp"
#include "Transform.hpp"
#include "../GameComponent/GameComponent.hpp"

class GameComponent;

/*
Entity Tree of the Entity-Component(GameComponent) System Architecture
Stores a Local Transform, any number of Components, and pointers to any children

Pointers to Children are uniquely owned by this Object, so deletion will delete any 
GameObjects in this subtree
*/
class GameObject
{
public:
	
	//GameObject();
	GameObject(std::string name);
	~GameObject();

	GameObject * getParent();
	void setParent(GameObject * parent);

	GameObject * getChild(std::string name);
	int addChild(std::unique_ptr<GameObject> child);

	GameComponent * getComponent(std::string name);
	int addGameComponent(std::unique_ptr<GameComponent> component);

	void update(float delta);

	void renderTree(Mat4x4 mat_parent_transform);//Render this and children
	//void render(Mat4x4 mat_local_transform);

	Transform& getLocalTransform();//Use to modify Transform
	Transform getGlobalTransform();

	std::string getName();
	void setName(std::string name);


private:
	GameObject * _gob_parent;
	std::vector<std::unique_ptr<GameObject>> _vec_children;
	std::vector<std::unique_ptr<GameComponent>> _vec_components;

	Transform _trf_transform;
	std::string _name;
};

