#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "ZombieArena.h"
#include "Player.h"
#include "TextureHolder.h"
#include "SoundHolder.h"
#include "Zombie.h"
#include "Bullet.h"
#include "Missile.h"
#include "Pickup.h"

int main()
{
    TextureHolder holder;
    SoundHolder soundHolder;

    enum class State
    {
        PAUSED,
        PLAYING,
        LEVELING_UP,
        GAME_OVER
    };

    State state = State::GAME_OVER;

    sf::Vector2f resolution;

    resolution.x = sf::VideoMode::getDesktopMode().width;
    resolution.y = sf::VideoMode::getDesktopMode().height;

    sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y),
                            "zambie arena game", sf::Style::Fullscreen);

    window.setMouseCursorVisible(false);
    sf::Sprite crossHair;
    crossHair = sf::Sprite(TextureHolder::getTexture("graphics/crosshair.png"));
    crossHair.setOrigin(55/2, 55/2);

    sf::View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

    sf::Clock clock;

    sf::Time gameTimeTotal;

    sf::Vector2f mouseWorldPosition;

    sf::Vector2i mouseScreenPosition;

    Player player;

    sf::IntRect arena;

    sf::VertexArray background;

    sf::Texture textureBackground = TextureHolder::getTexture("graphics/background_sheet.png");

    int numZombies = 10;
    int numZombiesAlive = 10;
    Zombie* zombies = nullptr;

    //my new stuff
    sf::Font font;
    font.loadFromFile("fonts/Aban.ttf");
    //font.loadFromFile("fonts/arial.ttf");

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(50);
    text.setColor(sf::Color::Blue);
    text.setPosition(sf::Vector2f(370, 335));

    sf::String str = L"!kÄk¶A x¼i ÂL¶Hp ¾£õ¹¶ ÁpIM ¾M";
    //sf::String str = "Wolcome to Zombie Arena!";
    text.setString("");

    sf::String str1;

    Bullet bullets[100];
    int currentBullet = 0;
    int bulletsSpare = 24;
    int bulletsInClip = 6;
    int clipSize = 6;
    float fireRate = 1;
    sf::Time lastPressed;

    int selectedZombie = 0;

    //Musics
    sf::Music gameMusic;
    gameMusic.openFromFile("sound/ZombiesAreComing.ogg");
    gameMusic.setVolume(25);

    bool gameMusicPlaying = false;

    //add a couple of pickup
    Pickup healthPickup(1);
    Pickup ammoPickup(2);

    int score = 0;
    int hiScore = 0;

    sf::Sprite spriteGameOver = sf::Sprite(TextureHolder::getTexture("graphics/background.png"));
    spriteGameOver.setPosition(sf::Vector2f(0, 0));

    sf::View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));

    sf::Sprite spriteAmmoIcon = sf::Sprite(TextureHolder::getTexture("graphics/ammo_icon.png"));
    spriteAmmoIcon.setPosition(sf::Vector2f(28, 620));

    sf::Text pausedText;
    pausedText.setFont(font);
    pausedText.setColor(sf::Color::White);
    pausedText.setCharacterSize(85);
    pausedText.setPosition(sf::Vector2f(310, 210));
    sf::String strPaused = L"·qM »n oT¹ÄH ¾µ¨j ¾¶HjH ÁHoM";
    pausedText.setString(strPaused);

    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setColor(sf::Color::White);
    gameOverText.setCharacterSize(80);
    gameOverText.setPosition(sf::Vector2f(380, 440));
    sf::String strGameOver = L"·qM »n oT¹ÄH ÁpIM ÁHoM";
    gameOverText.setString(strGameOver);

    // Levelling up
	sf::Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(50);
	levelUpText.setFillColor(sf::Color::White);
	levelUpText.setPosition(450, 80);

    sf::String strLevelUp = L"¦Ã±{ foº yÄHqÎH .1 \nJIzi qÄIw yÄHqÎH .2 \nÂT¶°w oX¨Hke yÄHqÎH .3 \n·kÄ»j Søow yÄHqÎH .4 \noT¿M » oTzÃM ÂT¶°w ¾L÷] .5 \noT¿M » oTzÃM RIµ¿¶ ¾L÷] .6";

	levelUpText.setString(strLevelUp);

	// Ammo
	sf::Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(55);
	ammoText.setFillColor(sf::Color::White);
	ammoText.setPosition(200, 600);
	sf::String strAmmo = L"0/0";
	ammoText.setString(strAmmo);

	// Score
	sf::Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(55);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(20, 0);

	std::ifstream inputFile("gamedata/HiScore.txt");

	if(inputFile.is_open())
    {
        inputFile >> hiScore;
        inputFile.close();
        //std::cout << "file inputed" << std::endl;
    }

    sf::Text hiScoreText;
    hiScoreText.setFont(font);
    hiScoreText.setColor(sf::Color::White);
    hiScoreText.setCharacterSize(50);
    hiScoreText.setPosition(sf::Vector2f(980, 0));
    std::stringstream sHiS;
    sHiS << hiScore;
    sf::String strHiScore = L":pIÃT¶H ¸ÄoU¯IM";
    hiScoreText.setString(sHiS.str() + strHiScore);

    // Zombies remaining
	sf::Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(55);
	zombiesRemainingText.setFillColor(sf::Color::White);
	zombiesRemainingText.setPosition(1050, 600);
	sf::String strZombiesAlive = L" :IÀ ÂL¶Hp";
	zombiesRemainingText.setString(strZombiesAlive);

	// Wave number
	int wave = 0;
	sf::Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(55);
	waveNumberText.setFillColor(sf::Color::White);
	waveNumberText.setPosition(850, 600);
	sf::String strWave = L" :Z¼¶";
	waveNumberText.setString(strWave);

    sf::RectangleShape healthBar;
    healthBar.setFillColor(sf::Color::Red);
    healthBar.setPosition(sf::Vector2f(300, 620));

    int framesSicnceLastHUDUpdate = 0;

    int fpsMeasurementFrameInterval = 100;

    sf::SoundBuffer reloadBuffer;
    reloadBuffer.loadFromFile("sound/reload.wav");
    sf::Sound reloadSound;
    reloadSound.setBuffer(reloadBuffer);

    sf::SoundBuffer reloadFailedBuffer;
    reloadFailedBuffer.loadFromFile("sound/reload_failed.wav");
    sf::Sound reloadFailedSound;
    reloadFailedSound.setBuffer(reloadFailedBuffer);

    sf::SoundBuffer splatBuffer;
    splatBuffer.loadFromFile("sound/splat.wav");
    sf::Sound splatSound;
    splatSound.setBuffer(splatBuffer);

    sf::SoundBuffer pickupBuffer;
    pickupBuffer.loadFromFile("sound/pickup.wav");
    sf::Sound pickupSound;
    pickupSound.setBuffer(pickupBuffer);

    sf::SoundBuffer hitBuffer;
    hitBuffer.loadFromFile("sound/pickup.wav");
    sf::Sound hitSound;
    hitSound.setBuffer(hitBuffer);

    // Prepare the powerup sound
	sf::SoundBuffer powerupBuffer;
	powerupBuffer.loadFromFile("sound/powerup.wav");
	sf::Sound powerupSound;
	powerupSound.setBuffer(powerupBuffer);

    while(window.isOpen())
    {
        /*
        ************
        handle input
        ************
        */

        sf::Event event;

        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Return && state == State::PLAYING)
                {
                    state = State::PAUSED;
                    gameMusic.pause();
                }

                else if(event.key.code == sf::Keyboard::Return && state == State::PAUSED)
                {
                    state = State::PLAYING;

                    gameMusic.play();

                    clock.restart();
                }

                else if(event.key.code == sf::Keyboard::Return && state == State::GAME_OVER)
                {
                    wave = 0;
                    score = 0;
                    currentBullet = 0;
                    bulletsSpare = 24;
                    bulletsInClip = 6;
                    clipSize = 6;
                    fireRate = 1;

                    player.resetPlayerState();

                    state = State::LEVELING_UP;
                }

                if(state == State::PLAYING)
                {
                    if(event.key.code == sf::Keyboard::R)
                    {
                        if(bulletsSpare >= clipSize)
                        {
                            bulletsInClip = clipSize;
                            bulletsSpare -= clipSize;
                            reloadSound.play();
                        }
                        else if(bulletsSpare > 0)
                        {
                            bulletsInClip = bulletsSpare;
                            bulletsSpare = 0;
                            reloadSound.play();
                        }
                        else
                            {
                                reloadFailedSound.play();
                            }
                    }

                }
            }
        } //end polling event

        // handle player quitting
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close();
        }

        //handle controlls while playing

        if(state == State::PLAYING)
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                player.moveUp();
            }
            else
            {
                player.stopUp();
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                player.moveDown();
            }
            else
            {
                player.stopDown();
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                player.moveLeft();
            }
            else
            {
                player.stopLeft();
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                player.moveRight();
            }
            else
            {
                player.stopRight();
            }

            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
               {
                   if(gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds()
                       > 1000 / fireRate && bulletsInClip > 0)
                    {

                        bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y,
                                                     mouseWorldPosition.x, mouseWorldPosition.y,
                                                     player.getRotation());

                        currentBullet++;

                        if(currentBullet > 99)
                        {
                            currentBullet = 0;
                        }

                        lastPressed = gameTimeTotal;

                        bulletsInClip--;
                    }
               }

               if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
               {
                   if(gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds()
                       > 1000 / fireRate && bulletsInClip > 0)
                    {

                        for(int i = 0; i < numZombies; i++)
                        {
                            if(!zombies[i].isAlive())
                            {
                                selectedZombie = -1;
                                continue;
                            }

                            srand(time(0) * i);

                            selectedZombie = rand() % numZombies;

                            if(!zombies[selectedZombie].isAlive())
                            {
                                selectedZombie = -1;
                            }
                        }

                        if(selectedZombie > 0)
                            bullets[currentBullet].homingShoot(player.getCenter().x, player.getCenter().y,
                                                     zombies[selectedZombie].getCenter().x, zombies[selectedZombie].getCenter().y,
                                                     player.getRotation());
                        currentBullet++;

                        if(currentBullet > 99)
                        {
                            currentBullet = 0;
                        }

                        lastPressed = gameTimeTotal;

                        bulletsInClip--;
                    }
               }


        } //end wasd while playing

        //Handle the leveling up state
        if(state == State::LEVELING_UP)
        {
            if(event.key.code == sf::Keyboard::Num1)
            {
                fireRate++;
                state = State::PLAYING;
            }

            if(event.key.code == sf::Keyboard::Num2)
            {
                clipSize += clipSize;
                state = State::PLAYING;
            }

            if(event.key.code == sf::Keyboard::Num3)
            {
                player.upgradeHealth();
                state = State::PLAYING;
            }

            if(event.key.code == sf::Keyboard::Num4)
            {
                player.upgradeSpeed();
                state = State::PLAYING;
            }

            if(event.key.code == sf::Keyboard::Num5)
            {
                healthPickup.upgrade();
                state = State::PLAYING;
            }

            if(event.key.code == sf::Keyboard::Num6)
            {
                ammoPickup.upgrade();
                state = State::PLAYING;
            }

            if(state == State::PLAYING)
            {
                if(gameMusic.Stopped)
                    gameMusic.play();

                wave++;

                arena.top = 0;
                arena.left = 0;
                arena.width = 500 * wave;
                arena.height = 500 * wave;

                //int tileSize = 50;
                int tileSize = createBackground(background, arena);

                player.spawn(arena, resolution, tileSize);

                //configure pickup
                healthPickup.setArena(arena);
                ammoPickup.setArena(arena);

                numZombies = 5 * wave;

                delete[] zombies;
                zombies = createHorde(numZombies, arena);

                numZombiesAlive = numZombies;

                powerupSound.play();

                clock.restart();
            }
        }// end leveling up state

        /*
        ****************
        update the frame
        ****************
        */

        if(state == State::GAME_OVER)
        {
            if(gameMusic.Playing)
                gameMusic.stop();

            if(clock.getElapsedTime().asSeconds() >= 0.1f)
            {
                static float totalTime = 0;
                totalTime += clock.getElapsedTime().asSeconds();

                bool b_CompleteString = false;


                static int i = 0;

                if(i >= str.getSize())
                {
                    b_CompleteString = true;
                }

                if(!b_CompleteString)
                {
                    str1 += str[i];

                    text.setString(str1);

                    i++;
                }

                if(totalTime >= 0.4f && b_CompleteString)
                    {
                        if(text.getColor()== sf::Color::Blue)
                            text.setColor(sf::Color::Black);
                        else
                            text.setColor(sf::Color::Blue);

                      /*  if(text.getColor()== sf::Color::Blue)
                            text.setColor(sf::Color::Black);
                        else if(text.getColor()== sf::Color::Yellow)
                            text.setColor(sf::Color::Green);
                        else if(text.getColor()== sf::Color::Green)
                            text.setColor(sf::Color::Cyan);
                        else if(text.getColor()== sf::Color::Cyan)
                            text.setColor(sf::Color::Magenta);
                        else
                            text.setColor(sf::Color::Blue); */

                        totalTime = 0;
                    }

                clock.restart();
            }

            //clock.restart();

        }


        if(state == State::PLAYING)
        {
            gameMusicPlaying = true;

            if(gameMusicPlaying && gameMusic.getStatus() != sf::SoundSource::Playing)
            {
                gameMusic.play();
            }

            sf::Time dt = clock.restart();

            gameTimeTotal += dt;

            float dtAsSeconds = dt.asSeconds();

            mouseScreenPosition = sf::Mouse::getPosition();

            mouseWorldPosition = window.mapPixelToCoords(sf::Mouse::getPosition(), mainView);

            player.update(dtAsSeconds, sf::Mouse::getPosition());

            sf::Vector2f playerPosition(player.getCenter());

            mainView.setCenter(player.getCenter());

            crossHair.setPosition(mouseWorldPosition.x, mouseWorldPosition.y);

            for(int i = 0; i < numZombies; i++)
            {
                if(zombies[i].isAlive())
                {
                    zombies[i].update(dt.asSeconds(), playerPosition);
                }
            }

            for(int i = 0; i < 100; i++)
            {
                if(bullets[i].isInFlight() && !bullets[i].isHomingMissile())
                {
                    bullets[i].update(dtAsSeconds, 0, 0);
                }
            }
            for(int i = 0; i < 100; i++)
            {
                if(bullets[i].isInFlight() && bullets[i].isHomingMissile())
                {
                    if(selectedZombie > 0)
                        bullets[i].update(dtAsSeconds, zombies[selectedZombie].getCenter().x, zombies[selectedZombie].getCenter().y);
                }
            }

            healthPickup.update(dtAsSeconds);
            ammoPickup.update(dtAsSeconds);

            for(int i = 0; i < 100; i++)
            {
                for(int j = 0; j < numZombies; j++)
                {
                    if(bullets[i].isInFlight() && zombies[j].isAlive())
                    {
                        if(bullets[i].getPosition().intersects(zombies[j].getPosition()))
                        {
                            bullets[i].stop();

                            if(zombies[j].hit())
                            {
                                splatSound.play();

                                score += 10;

                                if(score > hiScore)
                                {
                                    hiScore = score;
                                }

                                numZombiesAlive--;

                                //std::cout << "num is " << numZombiesAlive << std::endl;

                                if(numZombiesAlive <= 0)
                                {
                                    state = State::LEVELING_UP;
                                }
                            }
                        }
                    }
                }
            }

            for(int i = 0; i < numZombies; i++)
            {
                if(player.getPosition().intersects(zombies[i].getPosition())
                   && zombies[i].isAlive())
                {
                    if(player.hit(gameTimeTotal))
                    {
                        hitSound.play();
                    }

                    if(player.getHealth() <= 0)
                    {
                        state = State::GAME_OVER;

                        std::ofstream outputFile("gamedata/HiScore.txt");
                        outputFile << hiScore;
                        outputFile.close();
                    }
                }
            }

            if(player.getPosition().intersects(healthPickup.getPosition())
               && healthPickup.isSpawned())
            {
                pickupSound.play();
                player.increaseHealthLevel(healthPickup.gotIt());
            }

            if(player.getPosition().intersects(ammoPickup.getPosition())
               && ammoPickup.isSpawned())
            {
                reloadSound.play();
                bulletsSpare += ammoPickup.gotIt();
            }

            healthBar.setSize(sf::Vector2f(player.getHealth() * 2.5, 70));

            framesSicnceLastHUDUpdate++;

            if(framesSicnceLastHUDUpdate > fpsMeasurementFrameInterval)
            {
                std::stringstream ssAmmo;
                std::stringstream ssScore;
                std::stringstream ssHiScore;
                std::stringstream ssWave;
                std::stringstream ssZombiesAlive;

                ssAmmo << bulletsInClip << "/" << bulletsSpare;
                ammoText.setString(ssAmmo.str());

                // Update the high score text
				ssHiScore << hiScore;
				hiScoreText.setString(ssHiScore.str() + strHiScore);

				// Update the wave
				ssWave << wave;
				waveNumberText.setString(ssWave.str() + strWave);

				// Update the high score text
				ssZombiesAlive << numZombiesAlive;
				zombiesRemainingText.setString(ssZombiesAlive.str() + strZombiesAlive);

				framesSicnceLastHUDUpdate = 0;
            }

        } //end updating the scene

        /*
        **************
        draw the scene
        **************
        */

        if(state == State::PLAYING)
        {
            window.clear(sf::Color::Blue);

            window.setView(mainView);

            window.draw(background, &textureBackground);

            if(ammoPickup.isSpawned())
            {
                window.draw(ammoPickup.getSprite());
            }

            if(healthPickup.isSpawned())
            {
                window.draw(healthPickup.getSprite());
            }

            for(int i = 0; i < numZombies; i++)
            {
                window.draw(zombies[i].getSprite());
            }

            window.draw(player.getSprite());

            for(int i = 0; i < 100; i++)
            {
                if(bullets[i].isInFlight())
                {
                    window.draw(bullets[i].getShape());
                    window.draw(bullets[i].getSprite());
                }
            }

            for(int i = 0; i < 100; i++)
            {
                if(bullets[i].isInFlight() && bullets[i].isHomingMissile())
                {
                    window.draw(bullets[i].getSprite());
                }
            }

            window.draw(crossHair);

            window.setView(hudView);

            // Draw all the HUD elements
			window.draw(spriteAmmoIcon);
			window.draw(ammoText);
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(zombiesRemainingText);
        }

        if(state == State::PAUSED)
        {
			window.draw(pausedText);
        }

        if(state == State::LEVELING_UP)
        {
            window.draw(spriteGameOver);
			window.draw(levelUpText);

        }

        if(state == State::GAME_OVER)
        {
            window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(hiScoreText);

            window.draw(text);
        }

        window.display();


    } //end game loop

    delete[] zombies;
    delete[] bullets;

    return 0;
}
