#include "MeshRenderer.hpp"

MeshRendererComponent::MeshRendererComponent() : GameComponent("MeshRenderer")
{
	resetMesh();
}

MeshRendererComponent::MeshRendererComponent(std::string str_name) : GameComponent(str_name)
{
	resetMesh();
}

int MeshRendererComponent::setMesh(std::unique_ptr<Mesh> uptr_mesh)
{
	_uptr_mesh = std::move(uptr_mesh);
	return 0;
}
int MeshRendererComponent::resetMesh()
{
	_uptr_mesh.reset(nullptr);
	return 0;
}

void MeshRendererComponent::render(Mat4x4 mat_transform)
{
	RenderSystem3D::getInstance().renderMesh(*_uptr_mesh, mat_transform);
}