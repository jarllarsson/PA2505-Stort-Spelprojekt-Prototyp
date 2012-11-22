/*
 * This source file is part of libRocket, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://www.librocket.com
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <Rocket/Core.h>
#include <Rocket/Controls.h>
#include <Rocket/Debugger.h>
#include <Input.h>
#include <Shell.h>
#include "RenderInterfaceDx11.h"
#include "EventManager.h"
#include "ElementGame.h"
#include "EventInstancer.h"
#include "EventHandlerOptions.h"
#include "EventHandlerStartGame.h"
#include "EventHandler.h"
#include "GameStats.h"

Rocket::Core::Context* context = NULL;
RenderInterfaceDx11* renderDirectX10 = NULL;

const static int width = 1280;
const static int height = 720;

//#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "d3dcompiler.lib")
//#pragma comment (lib,"dxerr.lib")
//
//#ifdef _DEBUG
//#pragma comment(lib, "d3dx11d.lib")
//#pragma comment(lib, "Effects11D.lib")
//#pragma comment(lib, "d3dx10d.lib")
//#else
//#pragma comment(lib, "d3dx11.lib")
//#pragma comment(lib, "Effects11.lib")
//#pragma comment(lib, "d3dx10.lib")
//#endif


void GameLoop()
{
	renderDirectX10->beginFrame();

	context->Update();
	context->Render();

	renderDirectX10->presentFrame();
}

#include <windows.h>
int APIENTRY WinMain(HINSTANCE ROCKET_UNUSED(instance_handle), HINSTANCE ROCKET_UNUSED(previous_instance_handle), 
											char* ROCKET_UNUSED(command_line), int ROCKET_UNUSED(command_show))
{
	// Generic OS initialisation, creates a window and attaches OpenGL.
	if (!Shell::Initialise("./") ||
		!Shell::OpenWindow("The One Game",width,height))
	{
		Shell::Shutdown();
		return -1;
	}

	// Dummy class made to simply count the number of times Start Game has been pressed
	GameStats::init();

	// Install our DirectX render interface into Rocket.
	renderDirectX10 = new RenderInterfaceDx11((HWND)Shell::GetWindowHandle(), 0,0, width, height);
	Rocket::Core::SetRenderInterface(renderDirectX10);

	// Syteminterface handles file reading
	ShellSystemInterface system_interface;
	Rocket::Core::SetSystemInterface(&system_interface);

	// Rocket initialisation.
	Rocket::Core::Initialise();
	// Initialise the Rocket Controls library.
	Rocket::Controls::Initialise();

	// Create the main Rocket context and set it on the shell's input layer.
	context = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(width, height));
	if (context == NULL)
	{
		Rocket::Core::Shutdown();
		Shell::Shutdown();
		return -1;
	}

	// Initialise the Rocket debugger.
	Rocket::Debugger::Initialise(context);
	Rocket::Debugger::SetVisible(true);
	Input::SetContext(context);

	// Load the font faces required for Invaders.
	Shell::LoadFonts("../menu/assets/");

	// Register Invader's custom element and decorator instancers.
	Rocket::Core::ElementInstancer* element_instancer = new Rocket::Core::ElementInstancerGeneric< ElementGame >();
	Rocket::Core::Factory::RegisterElementInstancer("game", element_instancer);
	element_instancer->RemoveReference();

	// Initialise the event instancer and handlers.
	EventInstancer* event_instancer = new EventInstancer();
	Rocket::Core::Factory::RegisterEventListenerInstancer(event_instancer);
	event_instancer->RemoveReference();

	EventManager::RegisterEventHandler("start_game", new EventHandlerStartGame());
	EventManager::RegisterEventHandler("options", new EventHandlerOptions());

	// Start the game.
	if (EventManager::LoadWindow("main_menu"))
		Shell::EventLoop(GameLoop);

	// Release the event handlers.
	EventManager::Shutdown();

	// Shutdown Rocket.
	context->RemoveReference();
	Rocket::Core::Shutdown();

	Shell::CloseWindow();
	Shell::Shutdown();

	return 0;
}
