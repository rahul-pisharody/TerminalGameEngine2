#include "GameObject.hpp"



//GameObject::GameObject(){}
GameObject::GameObject(std::string name)
{
	setName(name);
	setParent(nullptr);
}

GameObject::~GameObject()
{
	_vec_components.clear();
	_vec_children.clear();
}

GameObject * GameObject::getParent()
{
	return _gob_parent;
}

void GameObject::setParent(GameObject * parent)
{
	_gob_parent = parent;
}

GameObject * GameObject::getChild(std::string name)
{
	for (auto& child : _vec_children)
	{
		if (child->getName() == name)
		{
			return child.get();
		}
	}
	return nullptr;
}

int GameObject::addChild(std::unique_ptr<GameObject> child)
{
	if (getChild(child->getName()) != nullptr)
	{
		return 1;//FAILED
	}
	child->setParent(this);
	_vec_children.push_back(std::move(child));
	return 0;
}

GameComponent * GameObject::getComponent(std::string name)
{
	for (auto& component : _vec_components)
	{
		if (component->getName() == name)
		{
			return component.get();
		}
	}
	return nullptr;
}

int GameObject::addGameComponent(std::unique_ptr<GameComponent> component)
{
	if (getComponent(component->getName()) != nullptr)
	{
		return 1;//Failed
	}
	component->setParentGameObject(this);
	_vec_components.push_back(std::move(component));
	return 0;
}

void GameObject::update(float delta)
{
	for (auto& component : _vec_components)
	{
		component->update(delta);
	}
	for (auto& child : _vec_children)
	{
		child->update(delta);
	}
}

void GameObject::renderTree(Mat4x4 mat_parent_transform)
{
	Mat4x4 mat_local_transform = getLocalTransform().getTransformMatrix().multMatrix(mat_parent_transform);

	for (auto& component : _vec_components)
	{
		component->render(mat_local_transform);
	}
	for (auto& child : _vec_children)
	{
		child->renderTree(mat_local_transform);
	}
}


Transform& GameObject::getLocalTransform()
{
	return _trf_transform;
}

Transform GameObject::getGlobalTransform()
{
	if (getParent() == nullptr)
	{
		return getLocalTransform();
	}
	return getLocalTransform()*(getParent()->getGlobalTransform());
}

std::string GameObject::getName()
{
	return _name;
}
void GameObject::setName(std::string name)
{
	_name = name;
}

