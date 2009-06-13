/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef _CAM_H
#define _CAM_H

#include "../MathLib/flx_vector3.h"
#include "../Core/flx_core.h"
#include "../Core/flx_logger.h"
#include "../MathLib/flx_quaternion.h"

class ENGINE_API Camera
{
public:
    enum CameraBehavior
    {
        CAMERA_BEHAVIOR_FIRST_PERSON,
        CAMERA_BEHAVIOR_SPECTATOR,
        CAMERA_BEHAVIOR_FLIGHT,
        CAMERA_BEHAVIOR_ORBIT
    };

    Camera();
    ~Camera();

    void lookAt(const Vector3 &target);
    void lookAt(const Vector3 &eye, const Vector3 &target, const Vector3 &up);
    void move(float dx, float dy, float dz);
    void move(const Vector3 &direction, const Vector3 &amount);
    void perspective(float fovx, float aspect, float znear, float zfar);
    void rotate(float headingDegrees, float pitchDegrees, float rollDegrees);
    void rotateSmoothly(float headingDegrees, float pitchDegrees, float rollDegrees);
    void undoRoll();
    void updatePosition(const Vector3 &direction, float elapsedTimeSec);
    void zoom(float zoom, float minZoom, float maxZoom);

    // Getter methods.

    const Vector3 &getAcceleration() const;
    CameraBehavior getBehavior() const;
    const Vector3 &getCurrentVelocity() const;
    const Vector3 &getPosition() const;
    float getOrbitMinZoom() const;
    float getOrbitMaxZoom() const;
    float getOrbitOffsetDistance() const;
    float getOrbitPitchMaxDegrees() const;
    float getOrbitPitchMinDegrees() const;
    const Quaternion &getOrientation() const;
    float getRotationSpeed() const;
    const Matrix4x4 &getProjectionMatrix() const;
    const Vector3 &getVelocity() const;
    const Vector3 &getViewDirection() const;
    const Matrix4x4 &getViewMatrix() const;
    const Matrix4x4 &getViewProjectionMatrix() const;
    const Vector3 &getXAxis() const;
    const Vector3 &getYAxis() const;
    const Vector3 &getZAxis() const;
    bool preferTargetYAxisOrbiting() const;
    
    // Setter methods.

    void setAcceleration(const Vector3 &acceleration);
    void setBehavior(CameraBehavior newBehavior);
    void setCurrentVelocity(const Vector3 &currentVelocity);
    void setCurrentVelocity(float x, float y, float z);
    void setOrbitMaxZoom(float orbitMaxZoom);
    void setOrbitMinZoom(float orbitMinZoom);
    void setOrbitOffsetDistance(float orbitOffsetDistance);
    void setOrbitPitchMaxDegrees(float orbitPitchMaxDegrees);
    void setOrbitPitchMinDegrees(float orbitPitchMinDegrees);
    void setOrientation(const Quaternion &newOrientation);
    void setPosition(const Vector3 &newEye);
    void setPreferTargetYAxisOrbiting(bool preferTargetYAxisOrbiting);
    void setRotationSpeed(float rotationSpeed);
    void setVelocity(const Vector3 &velocity);
    void setVelocity(float x, float y, float z);

private:
    void rotateFirstPerson(float headingDegrees, float pitchDegrees);
    void rotateFlight(float headingDegrees, float pitchDegrees, float rollDegrees);
    void rotateOrbit(float headingDegrees, float pitchDegrees, float rollDegrees);
    void updateVelocity(const Vector3 &direction, float elapsedTimeSec);
    void updateViewMatrix();

    static const float DEFAULT_ROTATION_SPEED;
    static const float DEFAULT_FOVX;
    static const float DEFAULT_ZNEAR;
    static const float DEFAULT_ZFAR;
    static const float DEFAULT_ORBIT_MIN_ZOOM;
    static const float DEFAULT_ORBIT_MAX_ZOOM;
    static const float DEFAULT_ORBIT_OFFSET_DISTANCE;
    static const Vector3 WORLD_XAXIS;
    static const Vector3 WORLD_YAXIS;
    static const Vector3 WORLD_ZAXIS;

    CameraBehavior m_behavior;
    bool m_preferTargetYAxisOrbiting;
    float m_accumPitchDegrees;
    float m_savedAccumPitchDegrees;
    float m_rotationSpeed;
    float m_fovx;
    float m_aspectRatio;
    float m_znear;
    float m_zfar;
    float m_orbitMinZoom;
    float m_orbitMaxZoom;
    float m_orbitOffsetDistance;
    float m_firstPersonYOffset;
    Vector3 m_eye;
    Vector3 m_savedEye;
    Vector3 m_target;
    Vector3 m_targetYAxis;
    Vector3 m_xAxis;
    Vector3 m_yAxis;
    Vector3 m_zAxis;
    Vector3 m_viewDir;
    Vector3 m_acceleration;
    Vector3 m_currentVelocity;
    Vector3 m_velocity;
    Quaternion m_orientation;
    Quaternion m_savedOrientation;
    Matrix4x4 m_viewMatrix;
    Matrix4x4 m_projMatrix;
    Matrix4x4 m_viewProjMatrix;
};

//-----------------------------------------------------------------------------

inline const Vector3 &Camera::getAcceleration() const
{ return m_acceleration; }

inline Camera::CameraBehavior Camera::getBehavior() const
{ return m_behavior; }

inline const Vector3 &Camera::getCurrentVelocity() const
{ return m_currentVelocity; }

inline const Vector3 &Camera::getPosition() const
{ return m_eye; }

inline float Camera::getOrbitMinZoom() const
{ return m_orbitMinZoom; }

inline float Camera::getOrbitMaxZoom() const
{ return m_orbitMaxZoom; }

inline float Camera::getOrbitOffsetDistance() const
{ return m_orbitOffsetDistance; }

inline const Quaternion &Camera::getOrientation() const
{ return m_orientation; }

inline float Camera::getRotationSpeed() const
{ return m_rotationSpeed; }

inline const Matrix4x4 &Camera::getProjectionMatrix() const
{ return m_projMatrix; }

inline const Vector3 &Camera::getVelocity() const
{ return m_velocity; }

inline const Vector3 &Camera::getViewDirection() const
{ return m_viewDir; }

inline const Matrix4x4 &Camera::getViewMatrix() const
{ return m_viewMatrix; }

inline const Matrix4x4 &Camera::getViewProjectionMatrix() const
{ return m_viewProjMatrix; }

inline const Vector3 &Camera::getXAxis() const
{ return m_xAxis; }

inline const Vector3 &Camera::getYAxis() const
{ return m_yAxis; }

inline const Vector3 &Camera::getZAxis() const
{ return m_zAxis; }

inline bool Camera::preferTargetYAxisOrbiting() const
{ return m_preferTargetYAxisOrbiting; }

#endif