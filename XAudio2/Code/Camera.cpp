#include "Camera.h"
#include "CA_Event.h"

Camera::Camera(MovementEventListener* p_movementEvent,CameraSettings p_settings) : Object(p_settings.pos)
{
	lookAt			= D3DXVECTOR3(0,0,0);
	up				= D3DXVECTOR3(0,1,0);
	forward			= D3DXVECTOR3(0,0,1);
	strafeRight		= D3DXVECTOR3(1,0,0);
	dV				= D3DXVECTOR3(0,0,1);
	dU				= D3DXVECTOR3(0,1,0);
	mAcceleration	= D3DXVECTOR3(0,0,0);

	m_fpsListener = new CA_Event(this);
	m_fpsEnabled = true;

	//Rotation Y will count as heading
	//Rotation Z will count as pitch
	rotY = p_settings.heading;
	rotZ = p_settings.pitch;
	speed = 10;

	//Create the projection matrix
	D3DXMatrixPerspectiveFovLH(&projectionMatrix,p_settings.fieldOfView * (float)DEG_TO_RAD,
							p_settings.aspectRatio,p_settings.nearPlane, p_settings.farPlane);

	if(p_movementEvent != NULL)
		AddMovement(p_movementEvent);
	else
		mForward = mBackward = mStrafeLeft = mStrafeRight = mMouseMove = NULL;
	


	updateView();
}
Camera::~Camera()
{
	SAFE_DELETE(mForward);
	SAFE_DELETE(mBackward);
	SAFE_DELETE(mStrafeLeft);
	SAFE_DELETE(mStrafeRight);
	SAFE_DELETE(m_fpsListener);
}

void Camera::AddMovement(MovementEventListener* movementEventListener)
{
	mForward = new CameraController(this,D3DXVECTOR3(0,0,speed));
	movementEventListener->AddKeyListener('W',WHENPRESSED,mForward);

	mBackward = new CameraController(this,D3DXVECTOR3(0,0,-speed));
	movementEventListener->AddKeyListener('S',WHENPRESSED,mBackward);

	mStrafeLeft = new CameraController(this,D3DXVECTOR3(-speed,0,0));
	movementEventListener->AddKeyListener('A',WHENPRESSED,mStrafeLeft);

	mStrafeRight = new CameraController(this,D3DXVECTOR3(speed,0,0));
	movementEventListener->AddKeyListener('D',WHENPRESSED,mStrafeRight);

	mMouseMove = new CameraController(this);
	movementEventListener->AddMouseListener(mMouseMove);
}
void Camera::adjustHeadinPitch(float hRadians, float pRadians)
{
	//Rotation Y will count as heading
	//Rotation Z will count as pitch
	rotY += hRadians;
	rotZ += pRadians;

	if (rotY>TWO_PI)
		rotY -= (float)TWO_PI;
	else if (rotY<0)
		rotY = (float)TWO_PI + rotY;

	if(rotZ>HALFPI)
		rotZ = HALFPI;
	else if(rotZ<-HALFPI)
		rotZ = -HALFPI;
}

void Camera::updateView()
{
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix,rotY,rotZ,0);

	D3DXVec3TransformCoord(&lookAt,&dV,&rotationMatrix);
	D3DXVec3TransformCoord(&up,&dU,&rotationMatrix);
	/*
	D3DXVECTOR4 tempDirection;
	D3DXVec4Transform(&tempDirection,&D3DXVECTOR4(0,0,1,0),&rotationMatrix);

	D3DXVECTOR3 direction = D3DXVECTOR3(tempDirection);
	*/
	D3DXVec3Normalize(&forward,&lookAt);
	D3DXVec3Cross(&strafeRight,&up,&lookAt);

	lookAt = pos + forward;

	D3DXMatrixLookAtLH(&viewMatrix, &pos, &lookAt, &up);
}

void Camera::setPositionAndView(float x, float y, float z, float hDegree, float pDegree)
{
	//set eye coordinates
	pos.x = x;
	pos.y = y;
	pos.z = z;
	D3DXMatrixTranslation(&translationMatrix,pos.x,pos.y,pos.z);

	//set heading and pitch
	rotY	= hDegree * (float) DEG_TO_RAD;
	rotZ	= pDegree * (float) DEG_TO_RAD;

	//update view
	updateView();
}

HRESULT Camera::Update(float deltaTime)
{
	pos +=	deltaTime * ( mAcceleration.z ) * forward +
			deltaTime * ( mAcceleration.x ) * strafeRight;

	
	mAcceleration = mAcceleration - (mAcceleration*3*deltaTime);
	if (abs(mAcceleration.x) < 1)
		mAcceleration.x = 0;
	if (abs(mAcceleration.z) < 1)
		mAcceleration.z = 0;

	updateView();

	return S_OK;
}

HRESULT Camera::Render(D3DXMATRIX p_viewProjection)
{
	return S_OK;
}
void Camera::addAcceleration(D3DXVECTOR3 acceleration)
{
	mAcceleration += acceleration;
}

void Camera::ChangeFPSCameraState()
{
	if (m_fpsEnabled)
		m_fpsEnabled = false;
	else
		m_fpsEnabled = true;
}

bool Camera::CheckIfFPSEnabled()
{
	return m_fpsEnabled;
}

EventInterface* Camera::GetFpsListener()
{
	return m_fpsListener;
}

D3DXMATRIX Camera::GetWorldMatrix()
{
	D3DXMATRIX world;
	D3DXMatrixTranslation(&translationMatrix,pos.x,pos.y,pos.z);

	world = viewMatrix * translationMatrix;
	//D3DXMatrixInverse(&world,0,&viewMatrix);

	return world;
}
