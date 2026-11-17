#pragma once

#include <SFML/Audio.hpp>

using namespace sf;

enum class SoundTypes {
    UFO_MOVEMENT,
    ENEMY_MOVEMENT,
    ENEMY_DEATH,
    PLAYER_SHOOT,
    PLAYER_DEATH,
};

class SoundSystem
{
private:
    SoundBuffer ufoHighBuffer;
    SoundBuffer ufoLowBuffer;

    SoundBuffer enemyMovementBuffer1;
    SoundBuffer enemyMovementBuffer2;
    SoundBuffer enemyMovementBuffer3;
    SoundBuffer enemyMovementBuffer4;
    SoundBuffer enemyDeathBuffer;

    SoundBuffer playerShootBuffer;
    SoundBuffer playerDeathBuffer;

    Sound ufoHigh;
    Sound ufoLow;

    Sound enemyMovement1;
    Sound enemyMovement2;
    Sound enemyMovement3;
    Sound enemyMovement4;
    Sound enemyDeath;

    Sound playerShoot;
    Sound playerDeath;

    int currentEnemyMovement = 0;
    bool ufoPlayingHigh = true;
    float masterVolume = 1.0f;

    sf::Clock enemySoundClock;
    float minEnemySoundDelay = 0.05f;

public:
    /**
     * @brief Initializes the sound system.
     */
    SoundSystem()
    {
        loadSounds();
        initializeSounds();
        setVolume(masterVolume);
    }

private:
    /**
     * @brief Loads in all the sounds into their respective buffers.
     */
    void loadSounds()
    {
        if (!ufoHighBuffer.loadFromFile("Assets/Sounds/ufo_highpitch.wav")) {
            exit(5);
        }
        if (!ufoLowBuffer.loadFromFile("Assets/Sounds/ufo_lowpitch.wav")) {
            exit(5);
        }
        if (!enemyMovementBuffer1.loadFromFile("Assets/Sounds/fastinvader1.wav")) {
            exit(5);
        }
        if (!enemyMovementBuffer2.loadFromFile("Assets/Sounds/fastinvader2.wav")) {
            exit(5);
        }
        if (!enemyMovementBuffer3.loadFromFile("Assets/Sounds/fastinvader3.wav")) {
            exit(5);
        }
        if (!enemyMovementBuffer4.loadFromFile("Assets/Sounds/fastinvader4.wav")) {
            exit(5);
        }
        if (!enemyDeathBuffer.loadFromFile("Assets/Sounds/invaderkilled.wav")) {
            exit(5);
        }
        if (!playerShootBuffer.loadFromFile("Assets/Sounds/shoot.wav")) {
            exit(5);
        }
        if (!playerDeathBuffer.loadFromFile("Assets/Sounds/explosion.wav")) {
            exit(5);
        }
    }

    /**
     * @brief Initializes all the sounds.
     */
    void initializeSounds()
    {
        ufoHigh.setBuffer(ufoHighBuffer);
        ufoLow.setBuffer(ufoLowBuffer);
        enemyMovement1.setBuffer(enemyMovementBuffer1);
        enemyMovement2.setBuffer(enemyMovementBuffer2);
        enemyMovement3.setBuffer(enemyMovementBuffer3);
        enemyMovement4.setBuffer(enemyMovementBuffer4);
        enemyDeath.setBuffer(enemyDeathBuffer);
        playerShoot.setBuffer(playerShootBuffer);
        playerDeath.setBuffer(playerDeathBuffer);
    }

    /**
     * @brief Set the volume of the sounds.
     */
    void setVolume(float volume)
    {
        ufoHigh.setVolume(volume);
        ufoLow.setVolume(volume);
        enemyMovement1.setVolume(volume);
        enemyMovement2.setVolume(volume);
        enemyMovement3.setVolume(volume);
        enemyMovement4.setVolume(volume);
        enemyDeath.setVolume(volume);
        playerShoot.setVolume(volume);
        playerDeath.setVolume(volume);
    }

    /**
     * @brief Plays the UFO movement sound.
     */
    void playUFOSound()
    {
        if (
            ufoHigh.getStatus() == SoundSource::Status::Stopped && 
            ufoLow.getStatus() == SoundSource::Status::Stopped
        ) {
            if (ufoPlayingHigh) ufoHigh.play();
            else ufoLow.play();

            ufoPlayingHigh = !ufoPlayingHigh;
        }
    }

    /**
     * @brief Plays the enemy movement sound.
     */
    void playEnemyMovementSound()
    {
        if (enemySoundClock.getElapsedTime().asSeconds() < minEnemySoundDelay) return;
        enemySoundClock.restart();

        switch (currentEnemyMovement)
        {
            case 0: enemyMovement1.play(); break;
            case 1: enemyMovement2.play(); break;
            case 2: enemyMovement3.play(); break;
            case 3: enemyMovement4.play(); break;
        }

        currentEnemyMovement = (currentEnemyMovement + 1) % 4;
    }


public:
    /**
     * @brief Plays a game sound.
     * 
     * @param type The sound type to play.
     */
    void playSound(SoundTypes type)
    {
        switch (type)
        {
            case SoundTypes::UFO_MOVEMENT: playUFOSound(); break;
            case SoundTypes::ENEMY_MOVEMENT: playEnemyMovementSound(); break;
            case SoundTypes::ENEMY_DEATH: enemyDeath.play(); break;
            case SoundTypes::PLAYER_SHOOT: playerShoot.play(); break;
            case SoundTypes::PLAYER_DEATH: playerDeath.play(); break;
        }
    }

    /**
     * @brief Stops a game sound.
     *
     * @param type The sound type to stop.
     */
    void stopSound(SoundTypes type)
    {
        switch (type)
        {
        case SoundTypes::UFO_MOVEMENT:
            ufoHigh.stop();
            ufoLow.stop();
            break;

        case SoundTypes::ENEMY_MOVEMENT:
            enemyMovement1.stop();
            enemyMovement2.stop();
            enemyMovement3.stop();
            enemyMovement4.stop();
            break;

        case SoundTypes::ENEMY_DEATH:
            enemyDeath.stop();
            break;

        case SoundTypes::PLAYER_SHOOT:
            playerShoot.stop();
            break;

        case SoundTypes::PLAYER_DEATH:
            playerDeath.stop();
            break;
        }
    }
};