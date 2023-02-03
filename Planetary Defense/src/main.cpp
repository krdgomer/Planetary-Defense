#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cstdlib>
#include "Menu.h"
#include "Player.h"
#include "Asteroid.h"
#include <vector>
#include <random>
#include <string>

using namespace sf;
RenderWindow window(VideoMode(1024, 768), "Planetary Defense");

int main()
{
    //INITIALIZE WINDOW

    window.setFramerateLimit(60);

    //NECESSAY VARIABLES
    bool gameStarted = false;
    bool muted = false;
    bool gameEnded = false;
    srand((unsigned)time(NULL));
    int scoreint = 0;
    bool soundplayed = false;
  


    //RANDOM NUMBER GENERATOR
    std::random_device rd;
    std::mt19937 e2(rd());
    int min = 100, max = 900;
    std::uniform_int_distribution<int> dist(min, max);
 
    //FONT AND TEXT
    Font dogica;
    dogica.loadFromFile("media/dogica.ttf");
    Text score;
    score.setFont(dogica);
    score.setString("0");
    score.setPosition(Vector2f(window.getSize().x / 2-20, window.getSize().y / 4));
    score.setCharacterSize(40);
    score.setFillColor(Color::White);
    Text destroyed;
    destroyed.setFont(dogica);
    destroyed.setCharacterSize(20);
    destroyed.setFillColor(Color::White);
    

    


    //MENU CLASS
    Menu MainMenu;
    MainMenu.menu_music.openFromFile("media/bgmusic.wav");
    MainMenu.menu_tex.loadFromFile("media/menubg.png");
    MainMenu.game_tex.loadFromFile("media/bg.png");
    MainMenu.menu_sprite.setTexture(MainMenu.menu_tex);

    MainMenu.menu_music.play();
    MainMenu.menu_music.setLoop(true);

    MainMenu.startButton.setSize(Vector2f(150.f, 50.f));
    MainMenu.startButton.setPosition(Vector2f(423.f, 331.f));
    MainMenu.startButton.setFillColor(Color::Transparent);

    MainMenu.exitButton.setSize(Vector2f(150.f, 50.f));
    MainMenu.exitButton.setPosition(Vector2f(423.f, 444.f));
    MainMenu.exitButton.setFillColor(Color::Transparent);

    MainMenu.muteButton_tex.loadFromFile("media/unmute.png");
    MainMenu.unmuteButton_tex.loadFromFile("media/mute.png");
    MainMenu.muteButton.setTexture(MainMenu.muteButton_tex);
    MainMenu.muteButton.setPosition(Vector2f(881.f, 664.f));

    Texture infoT;
    infoT.loadFromFile("media/info.png");
    Sprite info;
    info.setTexture(infoT);
    bool hideinfo = false;
    

    //PLAYER
    Player player;
    player.playerTex.loadFromFile("media/ship.png");
    player.playerSprite.setTexture(player.playerTex);
    player.speed = 3.f;
    player.playerSprite.setPosition(Vector2f(512, 700));
    player.playerSprite.setScale(Vector2f(2.f, 2.f));
   
    
    //ASTEROID
    Asteroid asteroid;
    asteroid.asteroidTex.loadFromFile("media/asteroid.png");
    asteroid.asteroidSprite.setTexture(asteroid.asteroidTex);
    asteroid.asteroidSprite.setOrigin(Vector2f(asteroid.asteroidTex.getSize().x / 2, asteroid.asteroidTex.getSize().y / 2));
    std::vector<Asteroid> asteroids;
    asteroids.push_back(Asteroid(asteroid));
    int asteroidTimer = 0;

    //MISSILES
    RectangleShape missile;
    missile.setFillColor(Color::Yellow);
    missile.setSize(Vector2f(4.f, 15.f));
    std::vector<RectangleShape> missiles;
    missiles.push_back(RectangleShape(missile));
    int shootTimer=0;
    SoundBuffer shoot;
    shoot.loadFromFile("media/shoot.wav");
    Sound shootSound;
    shootSound.setBuffer(shoot);

    SoundBuffer end;
    end.loadFromFile("media/thud.wav");
    Sound endSound;
    endSound.setBuffer(end);


    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            //BUTTON CLICK HANDLING
            if (event.type == Event::MouseButtonPressed) {
               
                if (event.mouseButton.button == Mouse::Left ) {
                 
                    if (!gameStarted) {
                        
                        if ((MainMenu.startButton.getGlobalBounds().contains(window.mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y))))) {
                            gameStarted = true;
                            MainMenu.menu_sprite.setTexture(MainMenu.game_tex);
                        }
                        else if ((MainMenu.exitButton.getGlobalBounds().contains(window.mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y))))) {
                            window.close();
                        }
                        else if ((MainMenu.muteButton.getGlobalBounds().contains(window.mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y)))) && !muted) {
                            MainMenu.muteButton.setTexture(MainMenu.unmuteButton_tex);
                            muted = true;
                        }
                        else if ((MainMenu.muteButton.getGlobalBounds().contains(window.mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y)))) && muted) {
                            MainMenu.muteButton.setTexture(MainMenu.muteButton_tex);
                            muted = false;
                        }
                    }

                    if (gameStarted) {
                        if ((info.getGlobalBounds().contains(window.mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y))))) 
                            hideinfo = true;
                            
                        

                    }
                }
            }
        }

        //UPDATE
        if (muted)
            MainMenu.menu_music.setVolume(0.f);
        else
            MainMenu.menu_music.setVolume(100.f);


        //AFTER THE GAME STARTED
        if (gameStarted && !gameEnded) {

            if(!hideinfo)
                info.setPosition(Vector2f(401.f, 251.f));
            else 
            {
                player.playerSprite.setOrigin(Vector2f(player.playerTex.getSize().x / 2, player.playerTex.getSize().y / 2));

                //PLAYER MOVEMENT
                if (Keyboard::isKeyPressed(Keyboard::A) && player.playerSprite.getPosition().x>0)
                    player.playerSprite.move(Vector2f(-player.speed, 0.f));
                if (Keyboard::isKeyPressed(Keyboard::D) && player.playerSprite.getPosition().x<1010)
                    player.playerSprite.move(Vector2f(player.speed, 0.f));

                //SHOOTING MISSILES

                if (shootTimer < 100)
           			shootTimer++;

                if (Mouse::isButtonPressed(Mouse::Left) && shootTimer >= 50) 
                    {
                   		missile.setPosition(Vector2f(player.playerSprite.getPosition().x, player.playerSprite.getPosition().y));
              			missiles.push_back(RectangleShape(missile));
                        shootSound.play();
                        
               			shootTimer = 0;
                    }
                    
                for (size_t i = 0; i < missiles.size(); i++)
              		{
               			missiles[i].move(0.f, -10.f);
                        if(missiles[i].getPosition().y <= 0)
                			missiles.erase(missiles.begin() + i);
             		}
                //ASTEROID SPAWNING

                if (asteroidTimer < 200)
                    asteroidTimer++;
                    
                if (asteroidTimer >= 100)
                    {
                        asteroid.asteroidSprite.setPosition(dist(e2), 0.f);
                    	asteroids.push_back(Asteroid(asteroid));
                  
              			asteroidTimer = 0;
              		}

                for (size_t i = 0; i < asteroids.size(); i++) {
                    asteroids[i].asteroidSprite.move(0.f, 1.f);
                    if (asteroids[i].asteroidSprite.getPosition().y >= 758) {
                        asteroids.erase(asteroids.begin() + i);
                        gameEnded = true;

                    }

                }

                //MISSILE-ASTEROID COLLISION
                if (!asteroids.empty() && !missiles.empty())
                    {
                    for (size_t i = 0; i < missiles.size(); i++)
                    	{
                    		for (size_t k = 0; k < asteroids.size(); k++)
                   				{
                   					if (missiles[i].getGlobalBounds().intersects(asteroids[k].asteroidSprite.getGlobalBounds()))
                   					    {
                    						missiles.erase(missiles.begin() + i);
                    						asteroids.erase(asteroids.begin() + k);
                                            scoreint++;
                    						break;
                    					}
                    				}
                    			}
                    		}
                //WRITE SCORE ON SCREEN
                score.setString(std::to_string(scoreint));
            }

        }

        if (gameStarted && gameEnded) {

            if (!soundplayed) {
                endSound.play();
                soundplayed = true;
            }
            
            destroyed.setString("Asteroids destroyed: " + std::to_string(scoreint));
            destroyed.setPosition(Vector2f(window.getSize().x / 2-100, window.getSize().y / 2));

        }
            


        window.clear();

        //DRAW
        window.draw(MainMenu.menu_sprite);
        if (!gameStarted) {
            window.draw(MainMenu.startButton);
            window.draw(MainMenu.exitButton);
            window.draw(MainMenu.muteButton);
        }
        
        if (gameStarted && !gameEnded) {

            if (!hideinfo)
                window.draw(info);
            else {

                window.draw(player.playerSprite);

                
                for (size_t i = 0; i < missiles.size(); i++)
                    window.draw(missiles[i]);
                for (size_t i = 0; i < asteroids.size(); i++) {
                    window.draw(asteroids[i].asteroidSprite);
                    if (asteroids[0].asteroidSprite.getPosition().x == 0.f)
                        asteroids[0].asteroidSprite.move(Vector2f(500.f, 0.f));
                }
                    
                window.draw(score);



            }
            
        }
        if (gameStarted && gameEnded) {
            window.draw(destroyed);
        }


        window.display();
        

        
    }
    return 0;
}