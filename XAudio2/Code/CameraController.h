/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		6/12/2012 												*/
/************************************************************************/

#ifndef CAMERACONTROLLER_H__
#define CAMERACONTROLLER_H__

#include "EventInterface.h"
#include "stdafx.h"

class Camera;

class CameraController : public EventInterface
{
private:
	D3DXVECTOR3			mMoveDirection;
	Camera*				mParent;
	bool				keyListener;
public:
	CameraController(Camera* parent);
	CameraController(Camera* parent,D3DXVECTOR3 moveDirection);
	~CameraController();
	void PerformAction();
	void MoveCamera();
};
#endif // CAMERACONTROLLER_H__
