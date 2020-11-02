
/*
The Core Loop of the Game
*/

#pragma once

#include <chrono>

#include "Terminal.hpp"
#include "InputManager.hpp"
#include "RenderPipeline\RenderSystem3D.hpp"
#include "Core\Camera.hpp"

class Timer
{
private:
	std::chrono::time_point<std::chrono::system_clock> _start;
	std::chrono::time_point<std::chrono::system_clock> _now;

public:
	Timer() {}
	void start();
	int getDelta();
};


class MainLoop
{
private:
	bool is_running;
	Timer ticker;
	int target_fps = 30;

	void _startUp();
	void _shutDown();
	//void _processInput();
	void _draw();

	void _loopThread();
	//void _shutDown();
public:
	MainLoop();
	~MainLoop();
	void initTerminal(int term_h = 50, int term_w = 50, int term_res_size = 5);
	void initCamera(float fov, float aspect_ratio, float near_plane, float far_plane);
	void run();

	virtual void gameInit();
	virtual void update(int delta);

	inline Display* getDisplay() { return _display.get(); }
	inline Camera* getCamera() { return static_cast<Camera*>(_tree_root->getChild("Camera")); }
	Camera* getActiveCamera();
	
	inline GameObject* getSceneTree() { return _tree_root.get(); }//Phase out once scene class made

private:
	std::unique_ptr<Display> _display;
	std::unique_ptr<Camera> _gob_camera;
	std::unique_ptr<GameObject> _tree_root;//TODO: Make scene class

	bool m_is_term_init;
};

