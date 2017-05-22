// Movement_Agent.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Agent.h"
#include "Updater.h"
#include "formation.h"
#include <list>

#define PI 3.14159265

const int nagents = 4;
const int npoints = 5;
const int nobstacles = 6;
bool showBehavior = false;

int main()
{
	// Create Render Windows
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Steering - Agent");

	// Path creation ***********************************************
	std::vector<Vector2D> tempPath;
	std::vector<Vector2D> wedgePath;
	std::vector<Vector2D> oppoPath[3];

	sf::CircleShape anker(10);
	anker.setOrigin(10, 10);

	sf::CircleShape slot[nagents];
	for (int i = 0; i < nagents; i++)
	{
		slot[i] = sf::CircleShape(5, 5);
		slot[i].setOrigin(5, 5);
		slot[i].setFillColor(sf::Color::Green);
	}

	float x = 0;
	float y = 0;

	for (int i = 0; i < npoints; i++)
	{
		x = 300 + cos(i) * 200;
		y = 200 + sin(i) * 200;
		tempPath.push_back(Vector2D(x, y));
	}
	//Drawable lines 
	sf::VertexArray lines(sf::LinesStrip, tempPath.size());
	int i = 0;
	for each (Vector2D index in tempPath)
	{
		lines[i].position = sf::Vector2f(index.x, index.y);
		lines[i].color = sf::Color(0, 153, 204);
		i++;
	}

	// Create Agents *************************************************
	// Sprite
	sf::Texture spaceship_texture;
	if (!spaceship_texture.loadFromFile("./Sprites/Spaceship.png"))
	{
		std::cout << "Error loading Texture";
	}
	spaceship_texture.setSmooth(true);

	sf::Texture falcon_texture;
	if (!falcon_texture.loadFromFile("./Sprites/Falcon.png"))
	{
		std::cout << "Error loading Texture";
	}
	falcon_texture.setSmooth(true);

	sf::Texture opponent_texture;
	if (!opponent_texture.loadFromFile("./Sprites/Opponent.png"))
	{
		std::cout << "Error loading Texture";
	}
	opponent_texture.setSmooth(true);

	sf::Texture background_texture;
	if (!background_texture.loadFromFile("./Sprites/background.jpg"))
	{
		std::cout << "Error loading Texture";
	}
	background_texture.setSmooth(true);
	
	sf::Sprite background;
	background.setTexture(background_texture);
	background.setScale(0.8f, 0.8f);

	sf::Sprite spaceship[nagents];
	sf::Sprite opponent[3];

	// create agents **********************************************
	std::vector<Agent> agent;

	Formation wedge(60);
	wedge.anker.setpostion(Vector2D(50.f, 50.f));
	wedge.anker.setState(followpathy, 1, 1);
	// wedge path
	wedgePath.push_back(Vector2D(100.f,	100.f));
	wedgePath.push_back(Vector2D(150.f, 120.f));
	wedgePath.push_back(Vector2D(200.f, 150.f));
	wedgePath.push_back(Vector2D(300.f, 200.f));
	wedgePath.push_back(Vector2D(250.f, 400.f));
	wedgePath.push_back(Vector2D(300.f, 500.f));
	wedgePath.push_back(Vector2D(500.f, 450.f));
	wedgePath.push_back(Vector2D(600.f, 400.f));
	wedgePath.push_back(Vector2D(800.f, 500.f));
	wedgePath.push_back(Vector2D(900.f, 650.f));
	wedgePath.push_back(Vector2D(1000.f, 600.f));
	
	wedge.anker.setPath(wedgePath);

	//Drawable wedge path 
	sf::VertexArray linesWedge(sf::LinesStrip, wedgePath.size());
	i = 0;
	for each (Vector2D index in wedgePath)
	{
		linesWedge[i].position = sf::Vector2f(index.x, index.y);
		linesWedge[i].color = sf::Color(153, 204, 102);
		i++;
	}

	Agent behaviorAgent(30, 10, 5, 2);
	behaviorAgent.setangularvelocity(0.f);
	behaviorAgent.setPath(tempPath);
	behaviorAgent.setpostion(wedge.GetTargetSlot(i).position);
	behaviorAgent.setGroup(obstacley, 3, 3);
	behaviorAgent.setGroup(collisionfovy, 3, 2);
	//agent[i].setGroup(collisionpredicty,3,2);
	behaviorAgent.setGroup(separationy, 2, 2);
	behaviorAgent.setGroup(seeky, 1, 1);

	sf::Sprite behaveAgent;
	behaveAgent.setTexture(falcon_texture);
	behaveAgent.setScale(sf::Vector2f(0.08f, 0.08f)); // absolute scale factor
	behaveAgent.setOrigin(sf::Vector2f(falcon_texture.getSize().x / 2, falcon_texture.getSize().y / 2));

	for (int i = 0; i < nagents; i++)
	{
		agent.push_back(Agent(30, 10, 5, 2));

		spaceship[i].setTexture(spaceship_texture);
		spaceship[i].setScale(sf::Vector2f(0.08f, 0.08f)); // absolute scale factor
		spaceship[i].setOrigin(sf::Vector2f(spaceship_texture.getSize().x / 2, spaceship_texture.getSize().y / 2));
		
		agent[i].setangularvelocity(0.f);
		agent[i].setPath(tempPath);
		agent[i].setpostion(wedge.GetTargetSlot(i).position);
		agent[i].setGroup(obstacley,3,3);
		agent[i].setGroup(collisionfovy,3,2);
		//agent[i].setGroup(collisionpredicty,3,2);
		agent[i].setGroup(separationy,2,2);
		agent[i].setGroup(arrivey,1,1);
	}

	for (int i = 0; i < 3; i++)
	{
		agent.push_back(Agent(20, 5, 5, 2));

		oppoPath[i].push_back(Vector2D(250 * ((i + 2)), -100));
		oppoPath[i].push_back(Vector2D(250 * ((i + 2)), 100));
		oppoPath[i].push_back(Vector2D(250 * ((i + 2)), 600));
		oppoPath[i].push_back(Vector2D(250 * ((i + 2)), 1000));

		opponent[i].setTexture(opponent_texture);
		opponent[i].setScale(sf::Vector2f(0.6f, 0.6f)); // absolute scale factor
		opponent[i].setOrigin(sf::Vector2f(opponent_texture.getSize().x / 2, opponent_texture.getSize().y / 2));

		agent[i+nagents].setangularvelocity(0.f);
		agent[i+nagents].setpostion(Vector2D(250 * ((i+2)),0));
		agent[i+nagents].setPath(oppoPath[i]);
		agent[i+nagents].setGroup(followpathy, 1, 1);
		//agent[i+nagents].setvelocity(Vector2D(0, 20));
	}
	
	//Agent agent(20,15,1,1,'s');
	//agent.setangularvelocity(0.f);
	Updater kinematic;

	// target Radius
	sf::CircleShape TargetRadius(120);
	TargetRadius.setOrigin(120, 120);
	TargetRadius.setFillColor(sf::Color::Transparent);
	TargetRadius.setOutlineThickness(2);

	// collision Radius
	sf::CircleShape collision(10,10);
	collision.setOrigin(10, 10);
	collision.setFillColor(sf::Color::Blue);

	// texture for obstacles
	sf::Texture obstacle_texture;
	if (!obstacle_texture.loadFromFile("./Sprites/Meteor.png"))
	{
		std::cout << "Error loading Texture";
	}
	obstacle_texture.setSmooth(true);

	// target **************************************************************
	//sf::CircleShape target(20);
	//target.setOrigin(20, 20);
	sf::Texture target_texture;
	if (!target_texture.loadFromFile("./Sprites/Spaceship_2.png"))
	{
		std::cout << "Error loading Texture";
	}
	target_texture.setSmooth(true);
	sf::Sprite target;
	target.setTexture(target_texture);
	target.setScale(sf::Vector2f(0.25f, 0.25f)); // absolute scale factor
	target.setOrigin(sf::Vector2f(target_texture.getSize().x / 2, target_texture.getSize().y / 2));
	target.setRotation(90);

	TargetTrans targetStruct;
	targetStruct.angvelocity = 0.f;
	targetStruct.orientation.angle = 0.f;
	targetStruct.velocity = Vector2D(20, 0);
	targetStruct.position = Vector2D(1000, 600);

	float trackupdate = 0.f;
	std::list<Vector2D> trackpos;

	// obstacles *************************************************************
	Vector2D position;
	Vector2D dimension;

	std::vector<Obstacle> obstacle;

	std::vector<sf::RectangleShape> rectangle;

	// draw nobstacles obstacles
	// 1.
	position = Vector2D(100,400);
	dimension = Vector2D(200,200);
	obstacle.push_back(Obstacle(dimension, position));

	sf::RectangleShape rect(sf::Vector2f(dimension.x, dimension.y));
	rect.setOrigin(dimension.x / 2, dimension.y / 2);
	rect.setPosition(position.x, position.y);
	rect.scale(0.8, 0.8); // draw rectangle a little bit smaller (collision)
	rect.setTexture(&obstacle_texture);
	rectangle.push_back(rect);

	// 2.
	position = Vector2D(400, 200);
	dimension = Vector2D(100, 100);
	obstacle.push_back(Obstacle(dimension, position));

	rect.setSize(sf::Vector2f(dimension.x, dimension.y));
	rect.setOrigin(dimension.x / 2, dimension.y / 2);
	rect.setPosition(position.x, position.y);
	rectangle.push_back(rect);

	// 3.
	position = Vector2D(420, 300);
	dimension = Vector2D(100, 100);
	obstacle.push_back(Obstacle(dimension, position));

	rect.setSize(sf::Vector2f(dimension.x, dimension.y));
	rect.setOrigin(dimension.x / 2, dimension.y / 2);
	rect.setPosition(position.x, position.y);
	rectangle.push_back(rect);

	// 4.
	position = Vector2D(600, 520);
	dimension = Vector2D(150, 150);
	obstacle.push_back(Obstacle(dimension, position));

	rect.setSize(sf::Vector2f(dimension.x, dimension.y));
	rect.setOrigin(dimension.x / 2, dimension.y / 2);
	rect.setPosition(position.x, position.y);
	rectangle.push_back(rect);

	// 5.
	position = Vector2D(900, 200);
	dimension = Vector2D(100, 100);
	obstacle.push_back(Obstacle(dimension, position));

	rect.setSize(sf::Vector2f(dimension.x, dimension.y));
	rect.setOrigin(dimension.x / 2, dimension.y / 2);
	rect.setPosition(position.x, position.y);
	rectangle.push_back(rect);

	// 6.
	position = Vector2D(1100, 500);
	dimension = Vector2D(200, 200);
	obstacle.push_back(Obstacle(dimension, position));

	rect.setSize(sf::Vector2f(dimension.x, dimension.y));
	rect.setOrigin(dimension.x / 2, dimension.y / 2);
	rect.setPosition(position.x, position.y);
	rectangle.push_back(rect);

	/*
	for (int i = 0; i < nobstacles; i++)
	{
		position.x = std::rand() % (window.getSize().x - 100) + 50;
		position.y = std::rand() % (window.getSize().y - 100) + 50;

		obstacle.push_back(Obstacle(dimension,position));

		sf::RectangleShape rect(sf::Vector2f(dimension.x,dimension.y));
		rect.setOrigin(dimension.x / 2, dimension.y / 2);
		rect.setPosition(position.x,position.y);
		rect.scale(0.7, 0.7); // draw rectangle a little bit smaller (collision)
		rect.setTexture(&obstacle_texture);
		rectangle.push_back(rect);
	}
	*/
	sf::Font font;
	if (!font.loadFromFile("arial.ttf"))
	{
		// error...
	}
	// draw text
	sf::Text text;
	// select the font
	text.setFont(font); // font is a sf::Font
	text.setString("Arrive");

	// draw track
	sf::CircleShape track(2);
	track.setFillColor(sf::Color::White);

	// MousePosition vector
	sf::Vector2i MousePosition;

	sf::Clock clock;
	sf::Time dt;

	// run the program as long as the window is open
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
			// set target
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				// get global mouse position
				MousePosition = sf::Mouse::getPosition(window);
				target.setPosition(MousePosition.x, MousePosition.y);

				// ****** Target *****************************
				targetStruct.position = Vector2D(MousePosition.x, MousePosition.y);
				//std::cout << position.x << " " << position.y << std::endl;
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::F)
				{
					behaviorAgent.setState(fleey,1,1);
					text.setString("Flee");
					showBehavior = true;
				}
				else if (event.key.code == sf::Keyboard::S)
				{
					behaviorAgent.setState(seeky,1,1);
					text.setString("Seek");
					showBehavior = true;
				}
				else if (event.key.code == sf::Keyboard::A)
				{
					behaviorAgent.setState(arrivey,1,1);
					text.setString("Arrive");
					showBehavior = true;
				}
				else if (event.key.code == sf::Keyboard::W)
				{
					behaviorAgent.setState(wandery,1,1);
					text.setString("Wander");
					showBehavior = true;
				}
				else if (event.key.code == sf::Keyboard::P)
				{
					behaviorAgent.setState(pursuey,1,1);
					text.setString("Pursue");
					showBehavior = true;
				}
				else if (event.key.code == sf::Keyboard::E)
				{
					behaviorAgent.setState(evadey,1,1);
					text.setString("Evade");
					showBehavior = true;
				}
				else if (event.key.code == sf::Keyboard::B)
				{
					behaviorAgent.setState(followpathy,1,1);
					text.setString("Follow Path");
					showBehavior = true;
				}
			}
		}
		// clear the window with black color
		window.clear(sf::Color::Black);

		window.draw(background);

		for each (sf::RectangleShape rect in rectangle)
		{
			window.draw(rect);
		}

		// update trackpos every 0.5sec
		if (trackupdate > .8f)
		{
			trackupdate = 0.f;
			for (int i = 0; i < nagents; i++)
			{
				trackpos.push_back(agent[i].getposition());
			}
		}
		// draw track positions in list every frame
		for each (Vector2D vec in trackpos)
		{							   
			track.setPosition(sf::Vector2f(vec.x, (vec.y))); 
			window.draw(track);
		}

		// ****** Target *****************************
		//targetStruct.position = Vector2D(targetStruct.position.x, targetStruct.position.y);
		// set for SFML
		target.setPosition(targetStruct.position.x, targetStruct.position.y);

		//targetStruct.position += targetStruct.velocity * dt.asSeconds();
		//targetStruct.position.x = fmodf(targetStruct.position.x, window.getSize().x);
		//targetStruct.position.y = fmodf(targetStruct.position.y, window.getSize().y);

		// ********* Opponent ***********************
		for (int i = nagents; i < nagents + 3; i++)
		{
			kinematic.update(agent[i], targetStruct, agent, obstacle, dt.asSeconds());

			// check if out of windows size
			agent[i].setpostion(Vector2D(fmodf(agent[i].getposition().x, window.getSize().x), fmodf(agent[i].getposition().y, window.getSize().y)));
		
			opponent[i - nagents].setPosition(agent[i].getposition().x, agent[i].getposition().y);
			opponent[i - nagents].setRotation(agent[i].getorientation().angle * 180 / PI);
			opponent[i - nagents].rotate(-180);  // because of Spaceship Sprite is flipped...or start orientation is 0 -> does not match with Sprite...

			window.draw(opponent[i-nagents]);
		}

		// wedge Target
		wedge.setTarget(targetStruct);

		TargetRadius.setPosition(targetStruct.position.x, targetStruct.position.y);

		anker.setPosition(sf::Vector2f(wedge.anker.getposition().x, wedge.anker.getposition().y));

		window.draw(anker);

		// draw everything here...
		//window.draw(triangle);
		window.draw(target);
		window.draw(TargetRadius);
		window.draw(text);
		window.draw(lines);
		window.draw(linesWedge);
		
		// update kinematic with target pos = mousePos ************************************************
		for (int i = 0; i < nagents; i++)
		{
			// group dynamic *****************************************************************
			kinematic.update(wedge.anker, wedge.getTarget(), agent, obstacle, dt.asSeconds());
			// agents ************************************************************************
			kinematic.update(agent[i], wedge.GetTargetSlot(i), agent, obstacle, dt.asSeconds());

			// check if out of windows size
			agent[i].setpostion(Vector2D(fmodf(agent[i].getposition().x, window.getSize().x), fmodf(agent[i].getposition().y, window.getSize().y)));
			// update position of triangle = agent
			spaceship[i].setPosition(agent[i].getposition().x, agent[i].getposition().y);
			spaceship[i].setRotation(agent[i].getorientation().angle * 180 / PI);
			spaceship[i].rotate(-180);  // because of Spaceship Sprite is flipped...or start orientation is 0 -> does not match with Sprite...

			window.draw(spaceship[i]);
			
			//std::cout << wedge.getSlotPosition(i).x << " " << wedge.getSlotPosition(i).y << std::endl;
			slot[i].setPosition(wedge.GetTargetSlot(i).position.x, wedge.GetTargetSlot(i).position.y);
			window.draw(slot[i]);

			// ******* Draw Rays ********************************************************
			Vector2D vel = Vector2D(targetStruct.position) - agent[i].getposition();

			// agent velocity (= orientation)
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(agent[i].getposition().x, agent[i].getposition().y),sf::Color::Cyan),
				sf::Vertex(sf::Vector2f(agent[i].getposition().x + agent[i].getvelocity().x * 3.5f,agent[i].getposition().y + agent[i].getvelocity().y * 3.5f),sf::Color::Cyan)
			};
			window.draw(line, 2, sf::Lines);
			/*
			// agent x 
			sf::Vertex line2[] =
			{
				sf::Vertex(sf::Vector2f(agent[i].getposition().x, agent[i].getposition().y),sf::Color::Blue),
				sf::Vertex(sf::Vector2f(agent[i].getposition().x + agent[i].getvelocity().x * 10,agent[i].getposition().y),sf::Color::Blue)
			};
			window.draw(line2, 2, sf::Lines);
			// agent y
			sf::Vertex line3[] =
			{
				sf::Vertex(sf::Vector2f(agent[i].getposition().x, agent[i].getposition().y),sf::Color::Red),
				sf::Vertex(sf::Vector2f(agent[i].getposition().x, agent[i].getposition().y + agent[i].getvelocity().y * 10),sf::Color::Red)
			};
			window.draw(line3, 2, sf::Lines);
			*/
		}

		// wedge velocity (= orientation)
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(wedge.anker.getposition().x, wedge.anker.getposition().y),sf::Color::Cyan),
			sf::Vertex(sf::Vector2f(wedge.anker.getposition().x + wedge.anker.getvelocity().x * 3.5f,wedge.anker.getposition().y + wedge.anker.getvelocity().y * 3.5f),sf::Color::Cyan)
		};
		window.draw(line, 2, sf::Lines);

		if (showBehavior == true)
		{

			// ****** Target *****************************
			targetStruct.position = Vector2D(targetStruct.position.x, targetStruct.position.y);
			// set for SFML
			target.setPosition(targetStruct.position.x, targetStruct.position.y);

			targetStruct.position += targetStruct.velocity * dt.asSeconds();
			targetStruct.position.x = fmodf(targetStruct.position.x, window.getSize().x);
			targetStruct.position.y = fmodf(targetStruct.position.y, window.getSize().y);

			// agent ************************************************************************
			kinematic.update(behaviorAgent, targetStruct, agent, obstacle, dt.asSeconds());

			// check if out of windows size
			behaviorAgent.setpostion(Vector2D(fmodf(behaviorAgent.getposition().x, window.getSize().x), fmodf(behaviorAgent.getposition().y, window.getSize().y)));
			// update position of triangle = agent
			behaveAgent.setPosition(behaviorAgent.getposition().x, behaviorAgent.getposition().y);
			behaveAgent.setRotation(behaviorAgent.getorientation().angle * 180 / PI);
			behaveAgent.rotate(-180);  // because of Spaceship Sprite is flipped...or start orientation is 0 -> does not match with Sprite...

			window.draw(behaveAgent);

			// ******* Draw Rays ********************************************************
			Vector2D vel = Vector2D(targetStruct.position) - behaviorAgent.getposition();

			// agent velocity (= orientation)
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(behaviorAgent.getposition().x, behaviorAgent.getposition().y),sf::Color::Cyan),
				sf::Vertex(sf::Vector2f(behaviorAgent.getposition().x + behaviorAgent.getvelocity().x * 3.5f,behaviorAgent.getposition().y + behaviorAgent.getvelocity().y * 3.5f),sf::Color::Cyan)
			};
			window.draw(line, 2, sf::Lines);
			/*
			// agent x 
			sf::Vertex line2[] =
			{
				sf::Vertex(sf::Vector2f(behaviorAgent.getposition().x, behaviorAgent.getposition().y),sf::Color::Blue),
				sf::Vertex(sf::Vector2f(behaviorAgent.getposition().x + behaviorAgent.getvelocity().x * 10,behaviorAgent.getposition().y),sf::Color::Blue)
			};
			window.draw(line2, 2, sf::Lines);
			// agent y
			sf::Vertex line3[] =
			{
				sf::Vertex(sf::Vector2f(behaviorAgent.getposition().x, behaviorAgent.getposition().y),sf::Color::Red),
				sf::Vertex(sf::Vector2f(behaviorAgent.getposition().x, behaviorAgent.getposition().y + behaviorAgent.getvelocity().y * 10),sf::Color::Red)
			};
			window.draw(line3, 2, sf::Lines);
			*/
		}

		// end the current frame
		window.display();

		trackupdate += dt.asSeconds();

		dt = clock.restart();
	}

	return 0;

}

