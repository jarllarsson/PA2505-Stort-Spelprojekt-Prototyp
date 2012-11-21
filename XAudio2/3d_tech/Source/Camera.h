#ifndef Camera_h__
#define Camera_h__

#include <d3dx10.h>
#include <d3d10.h>

class Camera
{
private:
	float heading, pitch;

	D3DXVECTOR3 velocity;

	D3DXMATRIX viewMatrix;
	D3DXMATRIX projectionMatrix;
	D3DXMATRIX rotationMatrix;
	D3DXMATRIX projViewMatrix;

	const D3DXVECTOR3 dV, dU;
	D3DXVECTOR3 eye,view,up;

	D3DXVECTOR3 forward,strafeRight;

	static Camera* sInstance;
private:
	void updateView();
	Camera();
	virtual ~Camera();
protected:
public:	
	static Camera* getInstance();
	static void destroy();

	void setPerspectiveProjection(float FOV, float aspectRatio, float zNearPlane, float zFarPlane);

	void setPositionAndView(float x, float y, float z, float hDegree, float pDegree);
	void adjustHeadinPitch(float hRadians, float pRadians);
	void setMovementToggle(int i, int v);

	void update(float deltaTime);

	D3DXMATRIX& getViewMatrix(){return viewMatrix;}
	D3DXMATRIX& getProjectionMatrix(){return projectionMatrix;}
	D3DXMATRIX getViewProjMatrix(){return viewMatrix*projectionMatrix;}

	D3DXVECTOR3 getView(){return view;}
	D3DXVECTOR3 getUp(){return up;}
	D3DXVECTOR3 getPos(){return eye;}
	D3DXVECTOR3 getForward(){return forward;}

	void setVelocity(D3DXVECTOR3 pVelocity);
	void setXVelocity(float dx);
	void setYVelocity(float dy);
	void setZVelocity(float dz);

};
#endif // Camera_h__
