#include "Window.h"

void Window::Start()
{
	m_state = State::Playing;

	m_mainWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32), "SAT Test");
	m_mainWindow.setMouseCursorVisible(false);
	std::vector<sf::Vector2f> points =
	{
		sf::Vector2f(20, 30),
		sf::Vector2f(20, 100),
		sf::Vector2f(100, 70),
		sf::Vector2f(70, 30),
		sf::Vector2f(50, 20),
	};

	m_userPolygon = std::shared_ptr<Polygon>(new Polygon(points, sf::Color::Blue, sf::Vector2f(50, 50)));

	points =
	{
		sf::Vector2f(50, 30),
		sf::Vector2f(50, 80),
		sf::Vector2f(100, 80),
		sf::Vector2f(100, 30),
	};

	m_polygons.push_back(Polygon(points, sf::Color::Green, sf::Vector2f(200, 200)));

	points =
	{
		sf::Vector2f(50, 30),
		sf::Vector2f(30, 50),
		sf::Vector2f(70, 100),
		sf::Vector2f(100, 70),
	};

	m_polygons.push_back(Polygon(points, sf::Color::Green, sf::Vector2f(200, 400)));

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
			if (event.type == sf::Event::MouseMoved)
			{
				HandleMouseMove(event);
				for (int i = 0; i < m_polygons.size(); ++i)
					collM.CheckCollision(*m_userPolygon, m_polygons[i]);
			}
			if (event.type == sf::Event::KeyPressed)
				for (int i = 0; i < m_polygons.size(); ++i)
					collM.CheckCollision(*m_userPolygon, m_polygons[i]);
		}
		m_residualTime -= m_minTimestep;
		frames++;
	}


}

void Window::Draw()
{
	m_mainWindow.clear(sf::Color::White);
	m_userPolygon->Draw(m_mainWindow);
	for (int i = 0; i < m_polygons.size(); ++i)
		m_polygons[i].Draw(m_mainWindow);
}

void Window::HandleMouseMove(sf::Event)
{
	if (!m_userPolygon)
		return;

	sf::Vector2f userPolygonPos = m_userPolygon->GetPosition();
	sf::Vector2i mousePos = sf::Mouse::getPosition(m_mainWindow);

	m_userPolygon->SetPosition(sf::Vector2f(mousePos.x, mousePos.y));

}