#pragma once

#include <SFML/Graphics.hpp>
#include <SFPhysics.h>
#include "../Utils/SFUtils.h"

using namespace sf;
using namespace sfp;

enum class ProjectileType
{
	PLAYER,
	ENEMY
};

class Projectile : public PhysicsSprite
{
private:
	Texture texture;
	bool active = false;
	ProjectileType projectileType = ProjectileType::PLAYER;

public:
	/**
	 * @brief Creates a projectile.
	 */
	Projectile()
	{
		const float SCALE_FACTOR = 3.0f;
		LoadTex(texture, "Assets/Images/projectile.png");
		setTexture(texture);
		setScale(SCALE_FACTOR, SCALE_FACTOR);
		setOrigin(texture.getSize().x / 2.0f,texture.getSize().y / 2.0f);
	}

	/**
	 * @brief Sets the type of the projectile.
	 *
	 * @param The new type.
	 */
	void setProjectileType(ProjectileType type)
	{
		projectileType = type;
	}

	/**
	 * @brief Returns the type of the projectile.
	 *
	 * @return The projectile type.
	 */
	ProjectileType getProjectileType() const
	{
		return projectileType;
	}

	/**
	 * @brief Checks if the projectile is active.
	 *
	 * @return True if active, otherwise false.
	 */
	bool isActive() const
	{
		return active;
	}

	/**
	 * @brief Sets a projectiles active state.
	 *
	 * @param state The new state.
	 */
	void setActive(bool state)
	{
		active = state;
	}
};