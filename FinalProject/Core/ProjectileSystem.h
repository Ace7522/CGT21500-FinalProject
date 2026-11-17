#pragma once

#include <iostream>
#include <vector>
#include <SFPhysics.h>
#include "../Game/Projectile.h"

using namespace sfp;

class ProjectileSystem
{
private:
    World& world;
    std::vector<Projectile> projectiles;

public:
    /**
     * @brief Initializes the projectile system.
     *
     * @param world Reference to the physics world.
     */
    ProjectileSystem(World& world)
        : world(world), projectiles(100)
    {
        initializeProjectiles();
    }

private:
    /**
     * @brief Initializes all the projectiles by setting their velocity to 0 and placing them offscreen.
     */
    void initializeProjectiles()
    {
        for (Projectile& projectile : projectiles)
        {
            projectile.setCenter(Vector2f(-10, -10));
            projectile.setVelocity(Vector2f(0, 0));

            world.AddPhysicsBody(projectile);
        }
    }

public:
    /**
     * @brief Spawns a projectile.
     *
     * @param position The position to spawn the projectile at.
     * @param velocity The velocity to give the projectile.
     * @param type The projectile type.
     */
    void spawnProjectile(const Vector2f& position, const Vector2f& velocity, ProjectileType type)
    {
        for (Projectile& projectile : projectiles)
        {
            if (!projectile.isActive())
            {
                projectile.setCenter(position);
                projectile.setVelocity(velocity);
                projectile.setActive(true);
                projectile.setProjectileType(type);

                return;
            }
        }
    }

    /**
     * @brief Removes a projectile.
     *
     * @param projectile The projectile to remove.
     */
    void removeProjectile(Projectile& projectile)
    {
        projectile.setVelocity(Vector2f(0, 0));
        projectile.setCenter(Vector2f(-10, -10));
        projectile.setActive(false);
    }

    /**
     * @brief Returns a non const reference to the projectiles vector.
     *
     * @return The non const reference.
     */
    std::vector<Projectile>& getProjectiles()
    {
        return projectiles;
    }

    /**
     * @brief Returns a const reference to the projectiles vector.
     *
     * @return The const reference.
     */
    const std::vector<Projectile>& getProjectiles() const
    {
        return projectiles;
    }
};