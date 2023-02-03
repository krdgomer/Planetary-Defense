#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Menu
{
public:
	
	sf::Sprite menu_sprite;
	sf::Texture menu_tex;
	sf::Texture game_tex;

	sf::Music menu_music;

	sf::RectangleShape startButton;
	sf::RectangleShape exitButton;

	sf::Sprite muteButton;
	sf::Texture muteButton_tex;
	sf::Texture unmuteButton_tex;

};

