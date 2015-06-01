#include "Window.h"

void Window::Start()
{
	m_state = State::Playing;

	m_mainWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32), "SAT Test");

	std::vector<sf::Vector2f> points = 
	{
		sf::Vector2f(20, 30),
		sf::Vector2f(20, 60),
		sf::Vector2f(40, 60),
		sf::Vector2f(100, 10),
	};

	Polygon newPolygon(points, sf::Color::Blue, sf::Vector2f(200, 200));

	m_polygons.push_back(newPolygon);

	MainLoop();
}

void Window::MainLoop()
{

	m_clock.restart();
	while (m_state != Exiting)
	{
		m_residualTime += m_clock.restart().asSeconds();
		Update();
		Draw();
		m_mainWindow.display();
		//std::cout << m_remainingTime << std::endl;
	}

}

void Window::Update()
{
	sf::Event event;

	int frames;
	frames = 0;

	while ((m_residualTime > m_minTimestep) && (frames < m_maxFrames))
	{
		while (m_mainWindow.pollEvent(event))
		{	
			if (event.type == sf::Event::Closed)
			{
				m_state = Exiting;
			}
		}
		m_residualTime -= m_minTimestep;
		frames++;
	}


}

void Window::Draw()
{
	m_mainWindow.clear(sf::Color::White);
	for (int i = 0; i < m_polygons.size(); ++i)
		m_polygons[i].Draw(m_mainWindow);
}
