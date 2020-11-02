#include "MainLoop.hpp"

#include <numeric>


MainLoop::MainLoop()
{
	ticker = Timer();
	//Initialize certain values
	_startUp();
	m_is_term_init = false;
}

MainLoop::~MainLoop()
{
}


void MainLoop::_startUp()
{
	is_running = true;
	_display = std::make_unique<Terminal>();
	InputManager::getInstance();
	RenderSystem3D::getInstance().setDisplay(getDisplay());
	_tree_root = std::make_unique<GameObject>("Root");
}
void MainLoop::_shutDown()
{
	is_running = false;
}

//Initializes the terminal with height, width and pixel size
void MainLoop::initTerminal(int term_h, int term_w, int term_res_size)
{
	m_is_term_init = getDisplay()->initDisplay(term_h, term_w, term_res_size);
}
void MainLoop::initCamera(float fov, float aspect_ratio, float near_plane, float far_plane)
{
	_gob_camera = std::make_unique<Camera>();
	std::unique_ptr<CameraViewComponent> cam_comp = std::make_unique<CameraViewComponent>(fov, aspect_ratio, near_plane, far_plane);
	_gob_camera->addGameComponent(std::move(cam_comp));
	dynamic_cast<CameraViewComponent*>(_gob_camera->getComponent("CameraView"))->setActive(true);

	_tree_root->addChild(std::move(_gob_camera));
	Camera* sdf = static_cast<Camera*>(_tree_root->getChild("Camera"));
}

//Game specific Init. Load resources, etc.
void MainLoop::gameInit()
{
}

//Runs the Core Loop
void MainLoop::run()
{
	if (!m_is_term_init)
		return;
	std::thread t = std::thread(&MainLoop::_loopThread, this);
	t.join();
}

void MainLoop::_loopThread()
{
	//std::vector<float> vec_fps;
	float fps_avg = 0.0f;
	int fps_ct = 1;
	
	int delta;
	gameInit();
	ticker.start();
	while (is_running)
	{
		delta = ticker.getDelta();
		ticker.start();

		
		update(delta);
		getSceneTree()->update(delta);
		getSceneTree()->renderTree(MatTransform::getIdentityMat());
		_draw();
		getDisplay()->clear();
		/*ASGE::WindowSystem::getInstance().refreshWindows();
		AsciiRenderer::getInstance().clear();*/
		InputManager::getInstance().updateInput();
		
		//vec_fps.push_back(1000.0f / (float)delta);
		
		if (delta == 0)
		{
		}
		else
		{
			fps_avg = (1000.0f / (float)delta);// *(1.0f / (float)fps_ct) + fps_avg * (float((fps_ct - 1) / (float)fps_ct));
			//fps_ct += 1;
		}

		char s[256];
		sprintf_s(s, 256, "TGE - FPS:%3.2f", fps_avg);
		getDisplay()->setTerminalTitle(s);

		delta = (int)(1000 / target_fps) - ticker.getDelta();
		//if (delta>0) std::this_thread::sleep_for(std::chrono::milliseconds(delta));
	}
}


//Per-Frame Update
void MainLoop::update(int delta)
{
	
}

Camera * MainLoop::getActiveCamera()
{
	return RenderSystem3D::getInstance().getActiveCamera();
}

//Load buffer onto screen display
void MainLoop::_draw()
{
	getDisplay()->update();
}


int Timer::getDelta()
{
	_now = std::chrono::system_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(_now - _start).count();
}

void Timer::start()
{
	_start = std::chrono::system_clock::now();
}
