#include "CameraController.h"
#include "Camera.h"

CameraController::CameraController(Camera* parent)
{
	mParent			= parent;
	keyListener		= false;
}

CameraController::CameraController(Camera* parent,D3DXVECTOR3 moveDirection)
{
	mMoveDirection	= moveDirection;
	mParent			= parent;
	keyListener		= true;
}

CameraController::~CameraController()
{
}

void CameraController::PerformAction()
{
	MoveCamera();
}

void CameraController::MoveCamera()
{
	if(keyListener)
		mParent->addAcceleration(mMoveDirection);
	else
	{
		if(mParent->CheckIfFPSEnabled())
		{
			POINT point;
			GetCursorPos(&point);

			float turn = -(HALF_SCREEN_WIDTH - point.x);
			float pitch = -(HALF_SCREEN_HEIGHT - point.y);

			mParent->adjustHeadinPitch(turn*0.001f,pitch*0.001f);
		}
	}
}
