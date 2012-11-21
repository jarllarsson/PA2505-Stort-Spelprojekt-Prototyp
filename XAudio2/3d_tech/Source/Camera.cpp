#include "Camera.h"
#include "Globals.h"

Camera* Camera::sInstance = 0;

Camera* Camera::getInstance()
{
	if(sInstance==0)
		sInstance = new Camera();
	return sInstance;
}
void Camera::destroy()
{
	delete sInstance;
}
Camera::Camera():dV(D3DXVECTOR3(0,0,1)),dU(D3DXVECTOR3(0,1,0))
{
	eye =		D3DXVECTOR3(0,0,0);
	view =		D3DXVECTOR3(0,0,1);
	up =		D3DXVECTOR3(0,1,0);
	forward =	D3DXVECTOR3(0,0,1);
	strafeRight=D3DXVECTOR3(1,0,0);
	heading = 0;
	pitch = 0;

	D3DXMatrixIdentity(&viewMatrix);
	D3DXMatrixIdentity(&projectionMatrix);
	
	velocity = D3DXVECTOR3(0.0f,0.0f,0.0f);
}
Camera::~Camera()
{

}

void Camera::setPerspectiveProjection(float FOV, float aspectRatio, float zNearPlane, float zFarPlane)
{
	FOV = FOV * (float)DEG_TO_RAD;

	D3DXMatrixPerspectiveFovLH(&projectionMatrix, FOV, aspectRatio, zNearPlane, zFarPlane);
}

void Camera::adjustHeadinPitch(float hRadians, float pRadians)
{
	heading += hRadians;
	pitch += pRadians;

	if (heading>TWO_PI)
		heading -= (float)TWO_PI;
	else if (heading<0)
		heading = (float)TWO_PI + heading;

	if(pitch>TWO_PI)
		pitch -= (float) TWO_PI;
	else if(pitch<0)
		pitch = (float)TWO_PI+pitch;
}

void Camera::updateView()
{
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix,heading,pitch,0);

	D3DXVec3TransformCoord(&view,&dV,&rotationMatrix);
	D3DXVec3TransformCoord(&up,&dU,&rotationMatrix);

	D3DXVec3Normalize(&forward,&view);
	D3DXVec3Cross(&strafeRight,&up,&view);

	view = eye + view;

	D3DXMatrixLookAtLH(&viewMatrix, &eye,&view,&up);
}

void Camera::setPositionAndView(float x, float y, float z, float hDegree, float pDegree)
{
	//set eye coordinates
	eye.x = x;
	eye.y = y;
	eye.z = z;

	//set heading and pitch
	heading = hDegree * (float) DEG_TO_RAD;
	pitch = pDegree * (float) DEG_TO_RAD;

	//update view
	updateView();
}

void Camera::update(float deltaTime)
{
	float dtInSecond = deltaTime*1000.0f;
	//update position - 1.5 unit per second
	eye +=	deltaTime * ( velocity.z ) * forward +
			deltaTime * ( velocity.x ) * strafeRight;

	velocity.x = 0;
	velocity.y = 0;
	velocity.z = 0;
	//update view
	updateView();
}
void Camera::setVelocity(D3DXVECTOR3 pVelocity)
{
	velocity = pVelocity;
}
void Camera::setXVelocity(float dx)
{
	velocity.x = dx;
}
void Camera::setYVelocity(float dy)
{
	velocity.y = dy;
}
void Camera::setZVelocity(float dz)
{
	velocity.z = dz;
}