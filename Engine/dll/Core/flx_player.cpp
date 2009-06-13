/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#include "flx_player.h"
#include "flx_engine.h"
#include "flx_core.h"

using namespace Flux;


const float     CAMERA_SPEED_FLIGHT_YAW = 100.0f;
const float     CAMERA_SPEED_ORBIT_ROLL = 100.0f;

Player::Player()
{
	m_bCollides = false;
}

Player::~Player()
{
}

void Player::GetMovementDirection(Vector3 &direction)
{
    static bool moveForwardsPressed = false;
    static bool moveBackwardsPressed = false;
    static bool moveRightPressed = false;
    static bool moveLeftPressed = false;
    static bool moveUpPressed = false;
    static bool moveDownPressed = false;

    Vector3 velocity = getCurrentVelocity();

    direction.set(0.0f, 0.0f, 0.0f);

    if (Flux::Engine::getInstance()->m_bKeys[VK_KEYW])
    {
        if (!moveForwardsPressed)
        {
            moveForwardsPressed = true;
			setCurrentVelocity(velocity.x, velocity.y, 0.0f);
        }

        direction.z += 1.0f;
    }
    else
    {
        moveForwardsPressed = false;
    }

    if (Flux::Engine::getInstance()->m_bKeys[VK_KEYS])
    {
        if (!moveBackwardsPressed)
        {
            moveBackwardsPressed = true;
           setCurrentVelocity(velocity.x, velocity.y, 0.0f);
        }

        direction.z -= 1.0f;
    }
    else
    {
        moveBackwardsPressed = false;
    }

    if (Flux::Engine::getInstance()->m_bKeys[VK_KEYD])
    {
        if (!moveRightPressed)
        {
            moveRightPressed = true;
            setCurrentVelocity(0.0f, velocity.y, velocity.z);
        }

        direction.x += 1.0f;
    }
    else
    {
        moveRightPressed = false;
    }

    if (Flux::Engine::getInstance()->m_bKeys[VK_KEYA])
    {
        if (!moveLeftPressed)
        {
            moveLeftPressed = true;
            setCurrentVelocity(0.0f, velocity.y, velocity.z);
        }

        direction.x -= 1.0f;
    }
    else
    {
        moveLeftPressed = false;
    }

    if (Flux::Engine::getInstance()->m_bKeys[VK_KEYE])
    {
        if (!moveUpPressed)
        {
            moveUpPressed = true;
            setCurrentVelocity(velocity.x, 0.0f, velocity.z);
        }

        direction.y += 1.0f;
    }
    else
    {
        moveUpPressed = false;
    }

    if (Flux::Engine::getInstance()->m_bKeys[VK_KEYQ])
    {
        if (!moveDownPressed)
        {
            moveDownPressed = true;
			setCurrentVelocity(velocity.x, 0.0f, velocity.z);
        }

        direction.y -= 1.0f;
    }
    else
    {
        moveDownPressed = false;
    }
}


void Player::Update(float elapsedTime)
{
	
	float t_heading = 0.0f;
	float t_pitch = 0.0f;
	float t_roll = 0.0f;

	float dx = 0.0f;
	float dy = 0.0f;
	float dz = 0.0f;

	Vector3 direction;
	GetMovementDirection(direction);

	if(Flux::Engine::getInstance()->m_bRButtonDown)
	{
		/*
		switch (getBehavior())
		{
		case Camera::CAMERA_BEHAVIOR_FIRST_PERSON:
			t_heading	= -flx_core->getMouseDistFromCenter().x * Flux::Player::getInstance()->getRotationSpeed();
			t_pitch		= -flx_core->getMouseDistFromCenter().y * Flux::Player::getInstance()->getRotationSpeed();

			rotate(t_heading, t_pitch, t_roll);
			break;

		case Camera::CAMERA_BEHAVIOR_FLIGHT:
			//t_heading	= -m_moveDirection.x * 100.0f * elapsedTime;
			//t_pitch		= -flx_core->getMouseDistFromCenter().y * Flux::Player::getInstance()->getRotationSpeed();
			//t_roll		= -flx_core->getMouseDistFromCenter().x * Flux::Player::getInstance()->getRotationSpeed();

			//rotate(t_heading, t_pitch, t_roll);
			//
			t_heading	= -flx_core->getMouseDistFromCenter().x * Flux::Player::getInstance()->getRotationSpeed();
			t_pitch		= -flx_core->getMouseDistFromCenter().y * Flux::Player::getInstance()->getRotationSpeed();

			rotate(t_heading, t_pitch, t_roll);
			//m_moveDirection.x = 0.0f;
			break;
		}
	*/


		switch (getBehavior())
		{
		default:
			break;

		case Camera::CAMERA_BEHAVIOR_FIRST_PERSON:
		case Camera::CAMERA_BEHAVIOR_SPECTATOR:
			dx = -flx_core->getMouseDistFromCenter().x;
			dy = -flx_core->getMouseDistFromCenter().y;

			rotateSmoothly(dx, dy, 0.0f);
			//updatePosition(direction, elapsedTime);
			break;

		case Camera::CAMERA_BEHAVIOR_FLIGHT:
			dy = flx_core->getMouseDistFromCenter().x;
			dz = -flx_core->getMouseDistFromCenter().y;

			rotateSmoothly(0.0f, dy, dz);

			if ((dx = -direction.x * CAMERA_SPEED_FLIGHT_YAW * elapsedTime) != 0.0f)
				rotate(dx, 0.0f, 0.0f);

			direction.x = 0.0f; // ignore yaw motion when updating camera's velocity
			//updatePosition(direction, elapsedTime);
			
			break;

		case Camera::CAMERA_BEHAVIOR_ORBIT:
			dx = flx_core->getMouseDistFromCenter().x;
			dy = flx_core->getMouseDistFromCenter().y;

			rotateSmoothly(dx, dy, 0.0f);

			if (!preferTargetYAxisOrbiting())
			{
				if ((dz = direction.x * CAMERA_SPEED_ORBIT_ROLL * elapsedTime) != 0.0f)
					rotate(0.0f, 0.0f, dz);
			}

			//if ((dz = -mouse.wheelPos()) != 0.0f)
			//	zoom(dz, getOrbitMinZoom(), getOrbitMaxZoom());

			break;
		}



		SetCursorPos(flx_core->getWindowCenterX(), flx_core->getWindowCenterY());	
	}
	updatePosition(direction, elapsedTime);
}