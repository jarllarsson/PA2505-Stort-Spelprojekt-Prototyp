#include <windows.h>
#include <windowsx.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <iostream>
#include <time.h>
#include "Camera.h"
#include "Heightmap.h"
#include "GameLogic.h"
#include "Globals.h"
#include "DeferredRender.h"
#include "HUD.h"
#include "Box.h"

// include the Direct3D Library file
#pragma comment (lib, "d3d10.lib")
#pragma comment (lib, "d3dx10.lib")

// global declarations
ID3D10Device* device;
IDXGISwapChain* swapchain;    // the pointer to the swap chain class

GameLogic* gameLogic;

ID3D10RasterizerState* mRSState;

HUD* hud;

Heightmap* mapTerrain;
DeferredRender* deferredRender;

static float deltaTime = 0.0f;

// function prototypes
void initD3D(HWND hWnd);
void initViewPort();
void render_frame(void);
void cleanD3D(void);
void init_geometry(void);
void initCamera(void);
void initHUD(void);
void update(float pDeltaTime);

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	HWND hWnd;
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL,
		L"WindowClass",
		L"We gather together",
		WS_BORDER,
		0, 0,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);

	// set up and initialize Direct3D
	initD3D(hWnd);
	initViewPort();
	deferredRender = new DeferredRender(device,swapchain);
	deferredRender->init();
	init_geometry();
	initCamera();
	initHUD();

	gameLogic = new GameLogic(device);

	// enter the main loop:
	MSG msg;

	clock_t startTime = 0;
	clock_t endTime = 0;

	int i = 0;

	while(TRUE)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if(msg.message == WM_QUIT)
				break;
		}
		deltaTime = (endTime-startTime)/(float)CLOCKS_PER_SEC;
		startTime = clock();
		update(deltaTime);
		render_frame();
		endTime = clock();

	}

	// clean up DirectX and COM
	cleanD3D();

	return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_ACTIVATE:
		if( LOWORD(wParam) == WA_INACTIVE )
		{
			gameLogic->setIsActive(false);
		}
		else
		{
			gameLogic->setIsActive(true);
		}
		break;
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		} break;
	}

	return DefWindowProc (hWnd, message, wParam, lParam);
}

void initViewPort()
{
	D3D10_VIEWPORT viewport;    // create a struct to hold the viewport data
	ZeroMemory(&viewport, sizeof(D3D10_VIEWPORT));    // clear out the struct for use

	viewport.TopLeftX = 0;    // set the left to 0
	viewport.TopLeftY = 0;    // set the top to 0
	viewport.Width = SCREEN_WIDTH;    // set the width to the window's width
	viewport.Height = SCREEN_HEIGHT;    // set the height to the window's height
	viewport.MinDepth = 0.0f;    // the closest an object can be on the depth buffer is 0.0
	viewport.MaxDepth = 1.0f;    // the farthest an object can be on the depth buffer is 1.0

	device->RSSetViewports(1, &viewport);    // set the viewport
}
void initRasterizerType()
{
	D3D10_RASTERIZER_DESC rsDec;

	ZeroMemory(&rsDec,sizeof(D3D10_RASTERIZER_DESC));
	rsDec.FillMode = D3D10_FILL_SOLID;
	rsDec.CullMode = D3D10_CULL_BACK;

	device->CreateRasterizerState(&rsDec,&mRSState);
}

void initHeightMap()
{
	mapTerrain = new Heightmap();
	mapTerrain->init(device,512,512,1.0f);
}
// this function initializes and prepares Direct3D for use
void initD3D(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC scd;

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Width = SCREEN_WIDTH;
	scd.BufferDesc.Height = SCREEN_HEIGHT;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.Windowed = TRUE;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D_FEATURE_LEVEL level;

	// create a device class and swap chain class using the information in the scd struct
	D3D10CreateDeviceAndSwapChain(NULL,
		D3D10_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		D3D10_SDK_VERSION,
		&scd,
		&swapchain,
		&device);
}

void initCamera()
{
	Camera::getInstance()->setPerspectiveProjection(45,SCREEN_WIDTH/SCREEN_HEIGHT,0.5f,1000.0f);
}
void initHUD()
{
	hud = new HUD(device);
}
void update(float pDeltaTime)
{
	gameLogic->update(pDeltaTime);
	Camera::getInstance()->update(pDeltaTime);
}

// this is the function used to render a single frame
void render_frame(void)
{
	gameLogic->draw();
	mapTerrain->draw();
	deferredRender->draw();
	//hud->draw();
}

// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
	swapchain->SetFullscreenState(FALSE,NULL);
	Camera::destroy();

	delete deferredRender;
	delete mapTerrain;
	delete gameLogic;

	device->Release();    // close and release the 3D device
	swapchain->Release();    // close and release the swap chain
}

// this is the function that creates the geometry to render
void init_geometry(void)
{	
	initHeightMap();
}