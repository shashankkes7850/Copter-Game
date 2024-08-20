#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <cstdlib>

using namespace sf;
using namespace std;

class Copter
{
public:
	Copter();
	~Copter();
	auto& get_sprite();

private:
	Texture copter_texture;
	Sprite copter;

protected:
	int hit_points;
};

Copter::Copter()
{
	if (!copter_texture.loadFromFile("textures/copter.png"))
	{
		cout << "Error" << endl;
	}

	copter.setTexture(copter_texture);
	copter.setScale(Vector2f(0.3, 0.5));
	hit_points = 10;
}

Copter::~Copter()
{
}

auto& Copter::get_sprite() {
	return Copter::copter;
}

class Enemy
{
public:
	Enemy();
	~Enemy();
	auto& get_sprite();

private:
	Texture enemy_texture;
	Sprite enemy;
};

Enemy::Enemy()
{
	if (!enemy_texture.loadFromFile("textures/enemy_jet.png"))
	{
		cout << "Enemy error" << endl;
	}

	enemy.setTexture(enemy_texture);
	enemy.setScale(Vector2f(0.05, 0.05));
}

Enemy::~Enemy()
{
}

auto& Enemy::get_sprite()
{
	return Enemy::enemy;
}

class Missiles
{
public:
	Missiles();
	~Missiles();
	auto& get_sprite();

private:
	Texture missile_texture;
	Sprite missile;
};

Missiles::Missiles()
{
	if (!missile_texture.loadFromFile("textures/missiles.png"))
	{
		cout << "Missile Error" << endl;
	}
	missile.setTexture(missile_texture);
	missile.setScale(Vector2f(0.05, 0.05));
	missile.rotate(90.f);
}

Missiles::~Missiles()
{
}

auto& Missiles::get_sprite() {
	return Missiles::missile;
}

class Health : public Copter
{
public:
	Health();
	~Health();
	auto& get_sprite();
	int& get_hp();

private:
	RectangleShape health;
};

Health::Health()
{
	health.setFillColor(Color::Red);
	health.setOutlineThickness(2);
	health.setOutlineColor(Color::Black);
	health.setSize(Vector2f(200, 10));
}

Health::~Health()
{
}

auto& Health::get_sprite()
{
	return Health::health;
}

int& Health::get_hp()
{
	return Health::hit_points;
}

int main() {
	srand(time(0));
	RenderWindow window(VideoMode(800, 600), "Copter Adventure", Style::Close);
	window.setFramerateLimit(60);

	//COPTER TEXTURE
	Copter copter;
	copter.get_sprite().setPosition(Vector2f(0, window.getSize().y / 2));
	Vector2f copter_front;

	//MISSILE TEXTURE
	Missiles missile;
	vector<Sprite>missiles;

	int shoot_timer = 0;

	//ENEMY TEXTURE

	Enemy enemy;
	vector<Sprite>enemies;

	int spawn_timer = 0;

	//HEALTH BAR

	Health health;
	health.get_sprite().setPosition(Vector2f(window.getSize().x / 2 - health.get_sprite().getSize().x / 2, 10));

	//start button

	Texture start_texture;
	if (!start_texture.loadFromFile("textures/start.png"))
	{
		cout << "Error" << endl;
	}
	Sprite start;
	start.setTexture(start_texture);
	start.setScale(Vector2f(0.25, 0.25));
	start.setPosition(Vector2f(window.getSize().x / 2 - 100, window.getSize().y / 2 - 100));

	//exit button

	Texture end_texture;
	if (!end_texture.loadFromFile("textures/quit.png"))
	{
		cout << "Error" << endl;
	}
	Sprite end;
	end.setTexture(end_texture);
	end.setScale(Vector2f(0.25, 0.25));
	end.setPosition(Vector2f(window.getSize().x / 2 - 100, window.getSize().y / 2 - 30));

	//easy button
	Texture easy_texture;
	if (!easy_texture.loadFromFile("textures/easy.png"))
	{
		cout << "Error" << endl;
	}
	Sprite easy;
	easy.setTexture(easy_texture);
	easy.setScale(Vector2f(0.25, 0.25));
	easy.setPosition(Vector2f(window.getSize().x / 2 - 100, window.getSize().y / 2 - 100));

	//normal button 
	Texture normal_texture;
	if (!normal_texture.loadFromFile("textures/normal.png"))
	{
		cout << "Error" << endl;
	}
	Sprite normal;
	normal.setTexture(normal_texture);
	normal.setScale(Vector2f(0.25, 0.25));
	normal.setPosition(Vector2f(window.getSize().x / 2 - 100, window.getSize().y / 2 - 30));

	//hard button
	Texture hard_texture;
	if (!hard_texture.loadFromFile("textures/hard.png"))
	{
		cout << "Error" << endl;
	}
	Sprite hard;
	hard.setTexture(hard_texture);
	hard.setScale(Vector2f(0.25, 0.25));
	hard.setPosition(Vector2f(window.getSize().x / 2 - 100, window.getSize().y / 2 + 40));
	//BACKGROUND

	Texture Back_tex;
	if (!Back_tex.loadFromFile("textures/Buildings.png"))
		cout << "Background error" << endl;
	Sprite background;
	background.setTexture(Back_tex);
	background.setScale(Vector2f(0.25, 0.25));

	int level = 0;

	// audio

	SoundBuffer buffer;
	if (!buffer.loadFromFile("audios/blast.wav")) {
		cout << "audio error";
	}
	// sound
	Sound sound;
	sound.setBuffer(buffer);

	Texture retry_texture, quit_texture;
	if (!retry_texture.loadFromFile("textures/start.png") || !quit_texture.loadFromFile("textures/quit.png"))
	{
		cout << "Error" << endl;
	}

	Sprite retry, quit;
	retry.setTexture(retry_texture);
	retry.setScale(Vector2f(0.25, 0.25));
	retry.setPosition(Vector2f(window.getSize().x / 2 - 100, window.getSize().y / 2 - 100));

	quit.setTexture(quit_texture);
	quit.setScale(Vector2f(0.25, 0.25));
	quit.setPosition(Vector2f(window.getSize().x / 2 - 100, window.getSize().y / 2 - 30));

	//text
	Font font;
	if (!font.loadFromFile("font/text.ttf"))
	{
		cout << "Error" << endl;
	}
	Text text;
	text.setFont(font);
	text.setCharacterSize(24);
	text.setFillColor(Color::Black);
	text.setStyle(Text::Bold);
	text.setPosition(Vector2f(window.getSize().x - 250, 10));

	int score = 0;

	// score
	Text high_score;
	high_score.setFont(font);
	high_score.setCharacterSize(32);
	high_score.setFillColor(Color::White);
	high_score.setStyle(Text::Bold);

	//MENU LOOP

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
				window.close();

			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Right)
				window.close();
		}

		window.clear(Color::Black);
		window.draw(start);
		window.draw(end);

		if (start.getGlobalBounds().contains(Vector2f(Mouse::getPosition(window))) && Mouse::isButtonPressed(Mouse::Left))
		{
			break;
		}
		if (end.getGlobalBounds().contains(Vector2f(Mouse::getPosition(window))) && Mouse::isButtonPressed(Mouse::Left))
		{
			window.close();
		}
		window.display();
	}
	GameLoop:
	// difficulty level
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
				window.close();

			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Right)
				window.close();
		}

		window.clear(Color::Black);
		window.draw(easy);
		window.draw(normal);
		window.draw(hard);


		if (easy.getGlobalBounds().contains(Vector2f(Mouse::getPosition(window))) && Mouse::isButtonPressed(Mouse::Left))
		{
			level = 30;			
			break;
		}
		if (normal.getGlobalBounds().contains(Vector2f(Mouse::getPosition(window))) && Mouse::isButtonPressed(Mouse::Left))
		{
			level = 20;			
			break;
		}
		if (hard.getGlobalBounds().contains(Vector2f(Mouse::getPosition(window))) && Mouse::isButtonPressed(Mouse::Left))
		{
			level = 12;
			break;
		}
		window.display();
	}
	//GAME LOOP

	while (window.isOpen())
	{
		score++;

		text.setString("Score : " + to_string(score));
		if (health.get_hp() <= 0)
		{
			break;
		}

		health.get_sprite().setSize(Vector2f(health.get_hp() * 20, health.get_sprite().getSize().y));

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
				window.close();

			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Right)
				window.close();
		}
		copter.get_sprite().setPosition(Vector2f(Mouse::getPosition(window)));
		copter_front = Vector2f(copter.get_sprite().getPosition().x + copter.get_sprite().getGlobalBounds().width, copter.get_sprite().getPosition().y + copter.get_sprite().getGlobalBounds().height / 2);

		window.clear(Color::White);

		window.draw(background);
		window.draw(text);

		//to make the copter move

		window.draw(copter.get_sprite());
		if ((Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) && copter.get_sprite().getPosition().y >= 40)
		{
			copter.get_sprite().move(0, -5);
		}
		if ((Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)) && (copter.get_sprite().getPosition().y + copter.get_sprite().getGlobalBounds().height <= window.getSize().y))
		{
			copter.get_sprite().move(0, 5);
		}
		if ((Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)) && (copter.get_sprite().getPosition().x >= 0))
		{
			copter.get_sprite().move(-5, 0);
		}
		if ((Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) && ((copter.get_sprite().getPosition().x + copter.get_sprite().getGlobalBounds().width) <= window.getSize().x))
		{
			copter.get_sprite().move(5, 0);
		}

		// to shoot the missiles

		if ((Mouse::isButtonPressed(Mouse::Left) || Keyboard::isKeyPressed(Keyboard::Space)) && shoot_timer >= 20)
		{
			missile.get_sprite().setPosition(copter_front);
			missiles.push_back(missile.get_sprite());
			shoot_timer = 0;
		}
		shoot_timer++;

		for (int i = 0; i < missiles.size(); i++)
		{
			missiles[i].move(6.f, 0);

			window.draw(missiles[i]);
			if (missiles[i].getPosition().x > window.getSize().x)
				missiles.erase(missiles.begin() + i);
		}

		// TO SPAWN ENEMIES

		if (spawn_timer >= level)
		{
			enemy.get_sprite().setPosition(Vector2f(window.getSize().x, 40 + rand() % (int)(window.getSize().y - enemy.get_sprite().getGlobalBounds().height - 10)));
			enemies.push_back(enemy.get_sprite());
			spawn_timer = 0;
		}
		spawn_timer++;
		for (int i = 0; i < enemies.size(); i++)
		{
			if (level == 12)
				enemies[i].move(-9, 0);
			else
				enemies[i].move(-7, 0);
			window.draw(enemies[i]);
			if (enemies[i].getPosition().x < -enemy.get_sprite().getGlobalBounds().width)
				enemies.erase(enemies.begin() + i);
		}

		//TO DETECT COLLISION OF ENEMIES AND MISSILES

		for (int i = 0; i < enemies.size(); i++)
		{
			for (int j = 0; j < missiles.size(); j++)
			{
				if (enemies[i].getGlobalBounds().intersects(missiles[j].getGlobalBounds()))
				{
					enemies.erase(enemies.begin() + i);
					missiles.erase(missiles.begin() + j);
					score += 50;
					sound.play();
					break;
				}
			}
		}

		// TO GET COLLISION OF COPTER AND ENEMIES

		for (int i = 0; i < enemies.size(); i++)
		{
			if (copter.get_sprite().getGlobalBounds().intersects(enemies[i].getGlobalBounds()))
			{
				enemies.erase(enemies.begin() + i);
				if (health.get_hp() > 0)
					health.get_hp()--;

				
				break;
			}
		}

		// health related 
		window.draw(health.get_sprite());

		window.display();
	}
	// game over loop
	high_score.setString("YOUR SCORE : " + to_string(score));
	high_score.setPosition(Vector2f(window.getSize().x / 2 - high_score.getGlobalBounds().width / 2, 100));
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
				window.close();

			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Right)
				window.close();
		}
			
		if (retry.getGlobalBounds().contains(Vector2f(Mouse::getPosition(window))) && Mouse::isButtonPressed(Mouse::Left))
		{
			window.clear();
			window.display();
			score = 0;
			health.get_hp() = 10;
			window.clear();
			window.display();
			goto GameLoop;
		}
		else if (quit.getGlobalBounds().contains(Vector2f(Mouse::getPosition(window))) && Mouse::isButtonPressed(Mouse::Left))
		{
			window.close();
		}
			
		window.clear(Color::Black);

		window.draw(retry);
		window.draw(quit);
		window.draw(high_score);

		window.display();
	}
	return 0;
}
