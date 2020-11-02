#pragma once
#include "GameComponent.hpp"
#include "../RenderPipeline/Mesh.hpp"
#include "../RenderPipeline/RenderSystem3D.hpp"

/*
Component that stores and renders a mesh
Responsible for calling the renderMesh method on the RenderSystem
*/
class MeshRendererComponent : public GameComponent
{
public:
	MeshRendererComponent();
	MeshRendererComponent(std::string str_name);
	
	int setMesh(std::unique_ptr<Mesh> uptr_mesh);
	int resetMesh();

	//void update(int delta);
	void render(Mat4x4 mat_transform);

private:
	std::unique_ptr<Mesh> _uptr_mesh;
};
