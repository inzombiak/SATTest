#ifndef WINDOW_H
#define WINDOW_H

#include "Polygon.h"

class Window
{
public:
	void Start();

private:
	//Enum to identify state of the window
	enum State
	{
		Playing,
		Exiting,
	};

	//Holds current window state
	State m_state;

	//Main loop
	void MainLoop();

	//Draw window
	void Draw();

	//Update window
	void Update();

	//Window dimensions
	const int WINDOW_HEIGHT = 480;
	const int WINDOW_WIDTH = 320;

	//SFML window
	sf::RenderWindow m_mainWindow;

	//Fixed framerate
	const float m_minTimestep = sf::seconds(1.f / 60.f).asSeconds();
	const int m_maxFrames = 5;
	float m_residualTime = 0;

	//Game clock
	sf::Clock m_clock;

	std::vector<Polygon> m_polygons;

};

#endif