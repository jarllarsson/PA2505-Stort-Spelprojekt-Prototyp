#include "RenderWindow.h"
#include <AntTweakBar.h>

RenderWindow::RenderWindow()
{
	g_hInst					= NULL;
	g_hWnd					= NULL;

	g_SwapChain				= NULL;
	g_Device				= NULL;
	g_DeviceContext			= NULL;

	mLevel					= NULL;
	mBox					= NULL;

	mLightShader			= NULL;
	mPointLight				= NULL;
	m_secondPL				= NULL;

	mBox=mBox2				= NULL;
	mAABB					= NULL;
	m_viewDepthBuffer		= NULL;
	m_viewNormalBuffer		= NULL;
	m_viewShadowMap			= NULL;
	m_viewSSAO				= NULL;
	m_viewLightPass			= NULL;

	m_shadowMap				= NULL;

	mWindowHasFocus			= false;
	m_useCollisionFrustum	= false;
	m_useVSYNC				= true;
	m_drawDebug				= false;
}
RenderWindow::~RenderWindow()
{
	SAFE_RELEASE(g_Device);
	SAFE_RELEASE(g_DeviceContext);
	SAFE_RELEASE(g_SwapChain);

	SAFE_DELETE(mShaderFactory);
	SAFE_DELETE(mObjectFactory);
	SAFE_DELETE(mBox);
	SAFE_DELETE(mBox2);
	SAFE_DELETE(techniqueDeferred);
	SAFE_DELETE(mLevel);
	SAFE_DELETE(gameCamera);
	SAFE_DELETE(mMovementListener);
	
	SAFE_DELETE(mPointLight);
	SAFE_DELETE(m_secondPL);

	for(unsigned int i = 0; i < m_debugDataQuadTree.size();i++)
		SAFE_DELETE(m_debugDataQuadTree[i]);

	SAFE_DELETE(mAABB);
	SAFE_DELETE(m_frustum);
	SAFE_DELETE(m_quadTree);
	SAFE_DELETE(m_SSAO);
	SAFE_DELETE(m_viewDepthBuffer);
	SAFE_DELETE(m_viewNormalBuffer);
	SAFE_DELETE(m_viewShadowMap);
	SAFE_DELETE(m_viewSSAO);
	SAFE_DELETE(m_viewLightPass);

	SAFE_DELETE(m_shadowMap);
}

HRESULT RenderWindow::Init( HINSTANCE hInstance, int nCmdShow)
{
	HRESULT hr = S_OK;

	mMovementListener = new MovementEventListener();
	CameraSettings settings;
	settings.fieldOfView	= 45;
	settings.aspectRatio	= (float)(SCREEN_WIDTH)/(float)(SCREEN_HEIGHT);
	settings.nearPlane		= 0.5f;
	settings.farPlane		= 500.0f;
	settings.pitch			= 1.0f;
	settings.heading		= 0;
	settings.pos			= D3DXVECTOR3(30,40,-40);

	gameCamera = new Camera(mMovementListener,settings);

	if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
		return E_FAIL;

	if( FAILED( InitHardware() ) )
		return E_FAIL;
	
	mShaderFactory = new ShaderFactory(g_Device,g_DeviceContext);
	
	mObjectFactory = new ObjectFactory();
	mObjectFactory->Init(g_Device,g_DeviceContext,gameCamera);
	
	if( FAILED( InitBuffers() ) )
		return E_FAIL;
	
	if( FAILED( InitObjects() ) )
		return E_FAIL;

	InitAntTweakBar();

	mMovementListener->AddKeyListener('K',WHENPRESSED,m_viewDepthBuffer->getSwitchRenderSE());
	mMovementListener->AddKeyListener('K',WHENPRESSED,m_viewNormalBuffer->getSwitchRenderSE());
	mMovementListener->AddKeyListener('K',WHENPRESSED,m_viewShadowMap->getSwitchRenderSE());
	mMovementListener->AddKeyListener('K',WHENPRESSED,m_viewSSAO->getSwitchRenderSE());
	mMovementListener->AddKeyListener('K',WHENPRESSED,m_viewLightPass->getSwitchRenderSE());

	mMovementListener->AddKeyListener('F',WHENPRESSED,gameCamera->GetFpsListener());
	return hr;
}

HRESULT RenderWindow::InitWindow( HINSTANCE hInstance, int nCmdShow )
{
	HRESULT hr = S_OK;

	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = StaticWndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = 0;
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName  = "BTH_D3D_Template";
	wcex.hIconSm        = 0;
	if( !RegisterClassEx(&wcex) )
		return E_FAIL;

	// Create window
	g_hInst = hInstance; 
	RECT rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

	if(!(g_hWnd = CreateWindow(
		"BTH_D3D_Template",
		"BTH - Direct3D 11.0 Template",
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		this)))
	{
		return E_FAIL;
	}

	ShowWindow( g_hWnd, nCmdShow );
	ShowCursor(	true );
	SetCursorPos((int)HALF_SCREEN_WIDTH,(int)(HALF_SCREEN_HEIGHT));

	return S_OK;
}

HRESULT RenderWindow::InitHardware()
{
	HRESULT hr = S_OK;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverType;

	D3D_DRIVER_TYPE driverTypes[] = 
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof(sd) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width = SCREEN_WIDTH;
	sd.BufferDesc.Height = SCREEN_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = WINDOWED;

	D3D_FEATURE_LEVEL featureLevelsToTry[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	D3D_FEATURE_LEVEL initiatedFeatureLevel;

	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
	{
		driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			driverType,
			NULL,
			createDeviceFlags,
			featureLevelsToTry,
			ARRAYSIZE(featureLevelsToTry),
			D3D11_SDK_VERSION,
			&sd,
			&g_SwapChain,
			&g_Device,
			&initiatedFeatureLevel,
			&g_DeviceContext);

		if( SUCCEEDED( hr ) )
		{
			char title[256];
			sprintf_s(
				title,
				sizeof(title),
				"We gather together | Direct3D 11.0 device initiated with Direct3D %s feature level",
				FeatureLevelToString(initiatedFeatureLevel)
				);
			SetWindowText(g_hWnd, title);

			break;
		}

	}
	if( FAILED(hr) )
		return hr;

	return hr;
}

HRESULT RenderWindow::InitBuffers()
{
	HRESULT hr = S_OK;

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (float)SCREEN_WIDTH;
	vp.Height = (float)SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_DeviceContext->RSSetViewports( 1, &vp );

	techniqueDeferred = new Deferred();
	techniqueDeferred->Init(g_Device,g_DeviceContext,g_SwapChain,mShaderFactory);

	return hr;
}
HRESULT RenderWindow::InitObjects()
{
	HRESULT hr = S_OK;
	float xOffset = 30;

	m_quadTree = new Quadtree(mObjectFactory,D3DXVECTOR3(0+xOffset,0,0),D3DXVECTOR3(20,5,20));

	mBox = new Box(D3DXVECTOR3(xOffset,0,0));
	mBox->Init(g_Device,g_DeviceContext,mShaderFactory->CreateAdvanceDeferredShader());

	mBox2 = new Box(D3DXVECTOR3(xOffset,-4.75f,10));
	mBox2->Init(g_Device,g_DeviceContext,mShaderFactory->CreateAdvanceDeferredShader());

	mLevel = new Heightmap();
	mLevel->init(g_Device,g_DeviceContext,mShaderFactory->CreateBasicDeferredShader(),"../Terrain/height512x512.raw",512,512,1.0f);
	
	mLightShader = mShaderFactory->CreatePointLightShader();
	mPointLight = mObjectFactory->CreateAPointLight(mLightShader,D3DXVECTOR3(0+xOffset,10,0),WHITE,50);
	m_secondPL = mObjectFactory->CreateAPointLight(mLightShader,D3DXVECTOR3(0,0,0),RED,10);

	m_debugDataQuadTree.push_back(mObjectFactory->CreateAMeshFromObj(D3DXVECTOR3(5+xOffset	,-2,5),"bth.obj",RED));
	m_debugDataQuadTree.push_back(mObjectFactory->CreateAMeshFromObj(D3DXVECTOR3(15+xOffset	,-2,5),"bth.obj",GREEN));
	m_debugDataQuadTree.push_back(mObjectFactory->CreateAMeshFromObj(D3DXVECTOR3(15+xOffset	,-2,15),"bth.obj",WHITE));
	m_debugDataQuadTree.push_back(mObjectFactory->CreateAMeshFromObj(D3DXVECTOR3(5+xOffset	,-2,15),"bth.obj",BLACK));
	
//	m_debugDataQuadTree.push_back(mObjectFactory->CreateAMeshFromObj(D3DXVECTOR3(-5+xOffset	,-2,15),"bth.obj",BLACK));
//	m_debugDataQuadTree.push_back(mObjectFactory->CreateAMeshFromObj(D3DXVECTOR3(-15+xOffset,-2,15),"bth.obj",WHITE));
//	m_debugDataQuadTree.push_back(mObjectFactory->CreateAMeshFromObj(D3DXVECTOR3(-5+xOffset	,-2,5),"bth.obj",RED));
//	m_debugDataQuadTree.push_back(mObjectFactory->CreateAMeshFromObj(D3DXVECTOR3(-15+xOffset,-2,5),"bth.obj",GREEN));
//	
//	m_debugDataQuadTree.push_back(mObjectFactory->CreateAMeshFromObj(D3DXVECTOR3(-5+xOffset	,-2,-15),"bth.obj",DARKBROWN));
//	m_debugDataQuadTree.push_back(mObjectFactory->CreateAMeshFromObj(D3DXVECTOR3(-15+xOffset,-2,-15),"bth.obj",DARK_YELLOW_GREEN));
//	m_debugDataQuadTree.push_back(mObjectFactory->CreateAMeshFromObj(D3DXVECTOR3(-5+xOffset	,-2,-5),"bth.obj",LIGHT_YELLOW_GREEN));
//	m_debugDataQuadTree.push_back(mObjectFactory->CreateAMeshFromObj(D3DXVECTOR3(-15+xOffset,-2,-5),"bth.obj",BEACH_SAND));
//	
//	m_debugDataQuadTree.push_back(mObjectFactory->CreateAMeshFromObj(D3DXVECTOR3(5+xOffset	,-2,-5),"bth.obj",DARKBROWN));
//	m_debugDataQuadTree.push_back(mObjectFactory->CreateAMeshFromObj(D3DXVECTOR3(15+xOffset	,-2,-5),"bth.obj",DARK_YELLOW_GREEN));
//	m_debugDataQuadTree.push_back(mObjectFactory->CreateAMeshFromObj(D3DXVECTOR3(15+xOffset	,-2,-15),"bth.obj",LIGHT_YELLOW_GREEN));
//	m_debugDataQuadTree.push_back(mObjectFactory->CreateAMeshFromObj(D3DXVECTOR3(5+xOffset	,-2,-15),"bth.obj",BEACH_SAND));
	m_quadTree->PlaceObjects(m_debugDataQuadTree);

	CameraSettings settings;
	settings.aspectRatio	= SCREEN_WIDTH / SCREEN_HEIGHT;
	settings.fieldOfView	= 45.0f;
	settings.nearPlane		= 1.0f;
	settings.farPlane		= 30.0f;
	settings.pitch			= 0;
	settings.heading		= 0;
	settings.pos			= D3DXVECTOR3(xOffset,0,0);
	m_frustum = mObjectFactory->CreateAFrustum(	settings,mShaderFactory->CreateBoundBoxShader(),RED);

	CameraSettings shadowMapSettings;
	shadowMapSettings.aspectRatio	= 1;
	shadowMapSettings.fieldOfView	= 45.0f;
	shadowMapSettings.nearPlane		= 50.0f;
	shadowMapSettings.farPlane		= 100.0f;
	shadowMapSettings.pitch			= HALFPI-0.3f;
	shadowMapSettings.heading		= 0;
	shadowMapSettings.pos			= D3DXVECTOR3(xOffset,55,-20);

	Camera* shadow = new Camera(NULL,shadowMapSettings);
	Frustum* newFrustum = mObjectFactory->CreateAFrustum(	shadowMapSettings,mShaderFactory->CreateBoundBoxShader(),
															WHITE);
	m_shadowMap = new ShadowMaps(g_Device,g_DeviceContext,mShaderFactory->CreateShadowMapShader(),newFrustum,1024);

	m_SSAO = new SSAO(g_Device,g_DeviceContext,mShaderFactory->CreateSSAOShader());

	D3DXVECTOR2 min=D3DXVECTOR2(1,-1);
	D3DXVECTOR2 max=D3DXVECTOR2(0.5f,-0.5f);
	D3DXVECTOR4 activeChannels=D3DXVECTOR4(1,0,0,0);
	m_viewDepthBuffer = mObjectFactory->CreateAScreenAlignedQuad(min,max,techniqueDeferred->GetDepthBuffer(), activeChannels);

	min=D3DXVECTOR2(0.5f,-1);
	max=D3DXVECTOR2(0.0f,-0.5f);
	activeChannels=D3DXVECTOR4(1,1,1,0);
	m_viewNormalBuffer = mObjectFactory->CreateAScreenAlignedQuad(min,max,techniqueDeferred->GetNormalBuffer(), activeChannels);

	min=D3DXVECTOR2(0,-1);
	max=D3DXVECTOR2(-0.5f,-0.5f);
	activeChannels=D3DXVECTOR4(1,0,0,0);
	m_viewShadowMap = mObjectFactory->CreateAScreenAlignedQuad(min,max,m_shadowMap->GetShadowResource(),activeChannels);

	min=D3DXVECTOR2(-0.5f,-1);
	max=D3DXVECTOR2(-1, -0.5f);
	activeChannels=D3DXVECTOR4(1,0,0,0);
	m_viewSSAO = mObjectFactory->CreateAScreenAlignedQuad(min,max,techniqueDeferred->GetSSAOBuffer(),activeChannels,1);

	min=D3DXVECTOR2(1,-0.5f);
	max=D3DXVECTOR2(0.5f,0);
	activeChannels=D3DXVECTOR4(1,1,1,1);
	m_viewLightPass = mObjectFactory->CreateAScreenAlignedQuad(min,max,techniqueDeferred->GetLightBuffer(),activeChannels);


	SoundManager::getInstance();

	return hr;
}

void RenderWindow::InitAntTweakBar()
{
	TwInit(TW_DIRECT3D11, g_Device);
	TwWindowSize(SCREEN_WIDTH,SCREEN_HEIGHT);

	TwBar *myBar;
	myBar = TwNewBar("AntTweakBar");
	TwAddVarRO(myBar, "Delta Time",TW_TYPE_FLOAT,&m_deltaTime,"");
	TwAddVarRW(myBar, "DrawDebug", TW_TYPE_BOOL8, &m_drawDebug, "group=Overall");
	TwAddVarRW(myBar, "Scale", TW_TYPE_FLOAT, &m_SSAO->GetSSAOParms()->gScale, "group=SSAO min=0 max=10 step=0.01");
	TwAddVarRW(myBar, "Bias", TW_TYPE_FLOAT, &m_SSAO->GetSSAOParms()->gBias, "group=SSAO step=0.01");
	TwAddVarRW(myBar, "Intensity", TW_TYPE_FLOAT, &m_SSAO->GetSSAOParms()->gIntensity, "group=SSAO step=0.01");
	TwAddVarRW(myBar, "SampleRadius", TW_TYPE_FLOAT, &m_SSAO->GetSSAOParms()->gSampleRadius, "group=SSAO step=0.01");
	TwAddVarRW(myBar, "Shadow Size", TW_TYPE_INT32, m_shadowMap->GetMapSizeRef(), "group=Shadow step=2");
	TwAddVarRW(myBar, "Use DFrustum", TW_TYPE_BOOL8, &m_useCollisionFrustum, "group=QuadTree");
	TwAddVarRW(myBar, "Use VSYNC", TW_TYPE_BOOL8, &m_useVSYNC, "group=Overall");
	TwAddVarRW(myBar, "Frustum Rot", TW_TYPE_FLOAT, m_frustum->GetRotation(), "group=QuadTree step=0.0001");
	TwAddVarRW(myBar, "LightColor", TW_TYPE_COLOR4F, mPointLight->GetColor(), "group=Light");

}

HRESULT RenderWindow::Update( float deltaTime )
{
	gameCamera->Update(deltaTime);
	mBox->Update(deltaTime);
	mPointLight->Update(deltaTime);
	for(unsigned int i = 0; i < m_debugDataQuadTree.size();i++)
		m_debugDataQuadTree[i]->Update(deltaTime);
	m_frustum->Update(deltaTime);
	if(m_shadowMap->CheckIfShadowMapHasChanged())
		m_viewShadowMap->UpdateShaderResource(m_shadowMap->GetShadowResource());
	m_shadowMap->Update(deltaTime);

	if (mWindowHasFocus && gameCamera->CheckIfFPSEnabled())
		SetCursorPos((int)HALF_SCREEN_WIDTH,(int)(HALF_SCREEN_HEIGHT));

	m_quadTree->CheckNodesAgainstFrustum(m_frustum);
	return S_OK;
}

HRESULT RenderWindow::Render(float deltaTime)
{
	D3DXMATRIX vp;
	if(m_useCollisionFrustum)
		vp = m_frustum->GetFrustumCamera()->getViewProjMatrix();
	else
		vp = gameCamera->getViewProjMatrix();

	techniqueDeferred->ClearBuffers();

	m_shadowMap->ShadowBeginRender();
	for (unsigned int i = 0; i <  m_debugDataQuadTree.size(); i++)
	{
		if (m_debugDataQuadTree[i]->GetIsVisible())
		{
			m_debugDataQuadTree[i]->RenderShadowMap(m_shadowMap->GetShadowShader(),m_shadowMap->GetViewProjection());
		}
	}
	m_shadowMap->EndShadowRender();

	techniqueDeferred->RenderToGBuffers();
	mLevel->Render(vp);
	mBox->Render(vp);
	mBox2->Render(vp);

	for(unsigned int i = 0; i < m_debugDataQuadTree.size();i++)
	{
		if (m_debugDataQuadTree[i]->GetIsVisible())
			m_debugDataQuadTree[i]->Render(vp);
	}

	D3DXMATRIX viewProjectionInverse;
	D3DXMatrixInverse(&viewProjectionInverse, 0, &vp);

	techniqueDeferred->RenderLightPrePass(mLightShader);
	mPointLight->Draw(vp,viewProjectionInverse);
	m_secondPL->Draw(vp,viewProjectionInverse);

	g_DeviceContext->OMSetBlendState(NULL,0,0xffffffff );
	techniqueDeferred->RenderUnLitObjects();

	if(m_drawDebug)
	{	
		m_quadTree->Render(vp);
		m_frustum->Render(vp);
		m_shadowMap->RenderDebug(vp);
		mPointLight->RenderDebug(vp);
		m_secondPL->RenderDebug(vp);
	}
	techniqueDeferred->RenderSSAOPass();

	m_SSAO->RenderPostProcessing(	vp,viewProjectionInverse,
									techniqueDeferred->GetNormalBuffer(),
									techniqueDeferred->GetDepthBuffer());

	techniqueDeferred->RenderFrame(	viewProjectionInverse,m_shadowMap->GetViewProjection(),
									m_shadowMap->GetShadowResource(),m_shadowMap->GetMapSize());

	if(m_drawDebug)
	{
		m_viewDepthBuffer->Render();
		m_viewNormalBuffer->Render();
		m_viewShadowMap->Render();
		m_viewSSAO->Render();
		m_viewLightPass->Render();
	}
	
	TwDraw();
	if(FAILED(g_SwapChain->Present( m_useVSYNC, 0 )))
		return E_FAIL;
	return S_OK;
}

void RenderWindow::Run()
{
	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	float secsPerCnt = 1.0f / (float)cntsPerSec;

	__int64 prevTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);

	// Main message loop
	MSG msg = {0};
	while(WM_QUIT != msg.message)
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			__int64 currTimeStamp = 0;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
			m_deltaTime = (currTimeStamp - prevTimeStamp) * secsPerCnt;

			if (m_deltaTime>=1)
				m_deltaTime=1;
			/*
			 *	THE MAIN LOOP
			 */
			Update(m_deltaTime);
			Render(m_deltaTime);

			prevTimeStamp = currTimeStamp;
		}
	}
	TwTerminate();
}

char* RenderWindow::FeatureLevelToString(D3D_FEATURE_LEVEL featureLevel)
{
	if(featureLevel == D3D_FEATURE_LEVEL_11_0)
		return "11.0";
	if(featureLevel == D3D_FEATURE_LEVEL_10_1)
		return "10.1";
	if(featureLevel == D3D_FEATURE_LEVEL_10_0)
		return "10.0";

	return "Unknown";
}

LRESULT CALLBACK RenderWindow::StaticWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	RenderWindow* parent;

	if (message ==  WM_CREATE )
	{
		parent = (RenderWindow*) ((LPCREATESTRUCT)lParam)->lpCreateParams;
		SetWindowLongPtr(hWnd,-21,(LONG_PTR)parent);
	}
	else
	{
		parent = (RenderWindow*)GetWindowLongPtr(hWnd,-21);
		if (!parent)
			return DefWindowProc(hWnd,message,wParam,lParam);
	}

	parent->g_hWnd = hWnd;

	return parent->WndProc(message,wParam,lParam);
}


LRESULT RenderWindow::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	if (TwEventWin(g_hWnd, message, wParam, lParam))
		return 0; // Event has been handled by AntTweakBar

	switch (message) 
	{
	case WM_PAINT:
		hdc = BeginPaint(g_hWnd, &ps);
		EndPaint(g_hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_ACTIVATE:
		if(wParam == WA_INACTIVE)
			mWindowHasFocus = false;
		else
			mWindowHasFocus = true;
		break;
	default:
		break;
	}
	if (mWindowHasFocus)
	{
		IOInput(message,wParam);
	}
	return DefWindowProc(g_hWnd, message, wParam, lParam);
}

void RenderWindow::IOInput(UINT message, WPARAM wParam)
{
	switch(message)
	{
	case WM_MOUSEMOVE:
		mMovementListener->NewMouseMovement();
		break;
	case WM_KEYUP:
		mMovementListener->NewKeyEvent(WHENRELEASED,wParam);
		switch(wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		default:
			break;
		}
		break;
	case WM_KEYDOWN:
		mMovementListener->NewKeyEvent(WHENPRESSED,wParam);
		break;
	default:
		break;
	}
}