#ifndef RenderWindow_h__
#define RenderWindow_h__

#include "stdafx.h"
#include "Deferred.h"
#include "Heightmap.h"
#include "Triangle.h"
#include "Camera.h"
#include "ObjectHandler.h"
#include "ShaderFactory.h"
#include "Box.h"
#include "MovementEventListener.h"
#include "ObjectFactory.h"
#include "PointLight.h"
#include "Shader.h"
#include "LightHandler.h"
#include "Mesh.h"
#include "AABB.h"
#include "Frustum.h"
#include "Quadtree.h"
#include "SSAO.h"
#include "ScreenAligned.h"
#include "CameraSettings.h"
#include "ShadowMaps.h"
#include"SSAO_parms.h"
#include <vector>
#include "SoundManager.h"

using namespace std;

class RenderWindow
{
private:
	HINSTANCE				g_hInst;  
	HWND					g_hWnd;

	IDXGISwapChain*         g_SwapChain;
	ID3D11Device*			g_Device;
	ID3D11DeviceContext*	g_DeviceContext;

	ShaderFactory*			mShaderFactory;
	ObjectFactory*			mObjectFactory;
	Box*					mBox;
	Box*					mBox2;
	AABB*					mAABB;
	Deferred*				techniqueDeferred;
	Heightmap*				mLevel;
	Camera*					gameCamera;
	MovementEventListener*	mMovementListener;

	Shader*					mLightShader;
	PointLight*				mPointLight,
							*m_secondPL;

	vector<Mesh*>			m_debugDataQuadTree;
	Quadtree*				m_quadTree;

	Frustum*				m_frustum;

	SSAO*					m_SSAO;
	ScreenAligned*			m_viewDepthBuffer;
	ScreenAligned*			m_viewNormalBuffer;
	ScreenAligned*			m_viewShadowMap;
	ScreenAligned*			m_viewSSAO;
	ScreenAligned*			m_viewLightPass;

	ShadowMaps*				m_shadowMap;

	bool					mWindowHasFocus;
	bool					m_drawDebug;
	bool					m_useVSYNC;
	bool					m_useCollisionFrustum;
	float					m_deltaTime;
protected:
	static LRESULT CALLBACK StaticWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	LRESULT				WndProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	RenderWindow();
	~RenderWindow();
	HRESULT				Init( HINSTANCE hInstance, int nCmdShow);
	HRESULT				InitWindow( HINSTANCE hInstance, int nCmdShow );
	HRESULT				InitHardware();
	HRESULT				InitBuffers();
	HRESULT				InitObjects();
	HRESULT				Update( float deltaTime );
	HRESULT				Render( float deltaTime );
	char*				FeatureLevelToString( D3D_FEATURE_LEVEL featureLevel );
	void				Run();
	void				IOInput(UINT message, WPARAM wParam);
	void				InitAntTweakBar();
	void				SwitchCamera();
};
#endif // RenderWindow_h__
