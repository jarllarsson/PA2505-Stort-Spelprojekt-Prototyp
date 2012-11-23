#ifndef Camera_H
#define Camera_H

#include "Object.h"
#include "stdafx.h"
#include "MovementEventListener.h"
#include "CameraController.h"
#include "CameraSettings.h"


class CA_Event;

class Camera : public Object
{
private:
	float				speed;

	D3DXMATRIX			viewMatrix,
						projectionMatrix,
						rotationMatrix,
						translationMatrix,
						projViewMatrix;

	D3DXVECTOR3			dV,
						dU;

	D3DXVECTOR3			lookAt,
						up,
						mAcceleration,
						forward,
						strafeRight;

	CameraController	*mForward,
						*mBackward,
						*mStrafeLeft,
						*mStrafeRight,
						*mMouseMove;
	bool				m_fpsEnabled;
	CA_Event			*m_fpsListener;
private:
	void updateView();
public:
	Camera(MovementEventListener* p_movementEvent,CameraSettings p_settings);
	virtual ~Camera();

	void AddMovement(MovementEventListener* movementEventListener);
	void RemoveMovement();

	void setPositionAndView(float x, float y, float z, float hDegree, float pDegree);
	void adjustHeadinPitch(float hRadians, float pRadians);

	HRESULT Update(float deltaTime);
	HRESULT Render(D3DXMATRIX p_viewProjection);

	D3DXMATRIX& getViewMatrix(){return viewMatrix;}
	D3DXMATRIX& getProjectionMatrix(){return projectionMatrix;}
	D3DXMATRIX getRotationMatrix(){return rotationMatrix;}

	D3DXMATRIX getViewProjMatrix(){return viewMatrix*projectionMatrix;}
	D3DXMATRIX GetWorldMatrix();

	D3DXVECTOR3 getView(){return lookAt;}
	D3DXVECTOR3 getForward(){return forward;}
	D3DXVECTOR3 getUp(){return up;}
	D3DXVECTOR3 getPos(){return pos;}
	D3DXVECTOR3 getRight(){return strafeRight;}
	D3DXVECTOR3* getEye(){return &pos;}

	void addAcceleration(D3DXVECTOR3 acceleration);

	void ChangeFPSCameraState();
	bool CheckIfFPSEnabled();
	EventInterface* GetFpsListener();
};
#endif // Camera_H
