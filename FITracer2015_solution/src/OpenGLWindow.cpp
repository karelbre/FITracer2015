/*
	Name: OpenGLWindow.cpp
	Desc: Main function.
	Author: -tHE SWINe-, Karel Brezina (xbrezi13)
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
// Get OpenGL 3.0 funcionality.
#include "OpenGL30Drv.h"
// Program context.
#include "SDLGLContext.h"
// Pathtracing module.
#include "RenderEngine.h"

static const char *p_s_window_name = "FITracer2015";
static const char *p_s_class_name = "my_wndclass";

// Resolution of window.
int n_width = 800;
int n_height = 600;
// Note: have to be multiple of 50
#define RESULT_RESOLUTION 500

// Call if come new windows message.
LRESULT CALLBACK WndProc(HWND h_wnd, UINT n_msg, WPARAM n_w_param, LPARAM n_l_param);
// Tell WINAPI to repaint.
void glPostRedisplay();
// OpenGL double buffering.
void glSwapBuffers();
// Destroy for WINAPI.
void Exit();
// Create all buttons in scene.
void InitButtons(SDLGLContext* context);
// Create all texts in scene.
void InitTexts(SDLGLContext* context);
// Function for thread.
int ComputePT(void* gpu_pt);
// Enable/Disable vertical synchronization.
void SetVSync(bool sync);
// Print all OpenGL extensions.
void PrintGLInfo();

// Handle for window.
static HWND h_wnd;
// OpenGL handle.
static CGL3Driver driver;
// Program context.
SDLGLContext context;

// Arguments are not used.
int main(int n_arg_num, const char **p_arg_list)
{
	// Register window's class.
	WNDCLASSEX t_wnd_class;
	t_wnd_class.cbSize = sizeof(WNDCLASSEX);
	t_wnd_class.style = CS_HREDRAW | CS_VREDRAW;
	t_wnd_class.lpfnWndProc = WndProc;
	t_wnd_class.cbClsExtra = 0;
	t_wnd_class.cbWndExtra = 0;
	t_wnd_class.hInstance = GetModuleHandle(NULL);
	t_wnd_class.hIcon = LoadIcon(0, IDI_APPLICATION);
	t_wnd_class.hCursor = LoadCursor(0, IDC_ARROW);
	t_wnd_class.hbrBackground = NULL;
	t_wnd_class.lpszMenuName = 0;
	t_wnd_class.lpszClassName = (LPCSTR)p_s_class_name;
	t_wnd_class.hIconSm = LoadIcon(0, IDI_APPLICATION);
	RegisterClassEx(&t_wnd_class);

	// Compute window's propotions.
	RECT t_rect = {0, 0, n_width, n_height};
	AdjustWindowRectEx(&t_rect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
	int n_wnd_width = t_rect.right - t_rect.left;
	int n_wnd_height = t_rect.bottom - t_rect.top;

	// Create window.
	h_wnd = CreateWindow((LPCSTR)p_s_class_name, (LPCSTR)p_s_window_name, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, n_wnd_width, n_wnd_height, NULL, NULL, GetModuleHandle(NULL), NULL);

	// Set OpenGL to support functionality of new OpenGL (3.0+).
	bool b_forward_compatible = true; 
	// Initialization of OpenGL.
	if(!driver.Init(h_wnd, b_forward_compatible, 3, 0, n_width, n_height, 32, 24, 0, false)) {
		fprintf(stderr, "OpenGL error: driver.Init() initialization failed\n"); // nepodarilo se zinicializovat OpenGL
		return EXIT_FAILURE;
	}

	// Print OpenGL version.
	printf("OpenGL initialized: %s\n", (const char*)glGetString(GL_VERSION));
	
	// Print extensions.
	//PrintGLInfo();
	
	// Check if OpenGL 3.0 extensions is supported.
	if(!GL_VERSION_3_0) {
		fprintf(stderr, "OpenGL error: OpenGL 3.0 not supported\n");
		return EXIT_FAILURE;
	}

	// Turn on vertical synchronization.
	// Decrease CPU load.
	SetVSync(true);

	// My code. Initialization.
	// Create objects and load to context.
	InitButtons(&context);
	InitTexts(&context);
	// Init of program context.
	if (!context.Init(n_width, n_height, RESULT_RESOLUTION, RESULT_RESOLUTION)) {
		fprintf(stderr, "Context init failed.\n");
		return EXIT_FAILURE;
	}
	// Prepare objects to render.
	context.PrepareScene();
	// Init of pathtracing context.
	RenderEnginePT pt_engine; 
	context.SeizeSem(1);
	pt_engine.InitPT(&context);
	// Begin compute of pathtracing.
	SDL_Thread* threadID = SDL_CreateThread(ComputePT, "Pathracing", &pt_engine);
	
	ShowWindow(h_wnd, SW_SHOW);
	UpdateWindow(h_wnd);

	// Forever loop of checking events.
	MSG t_msg;
	while(GetMessage(&t_msg, 0, 0, 0) > 0) { 
		TranslateMessage(&t_msg);
		DispatchMessage(&t_msg);
	}

	SDL_WaitThread(threadID, NULL);

	return EXIT_SUCCESS;
}


// Service message loop.
// h_wnd -> window where message belongs to.
// n_msg -> number of message.
// n_w_param -> 1st param of message.
// n_l_param -> 2nd param of message.
// @return 0 if msg process by app, otherwise process by DefWindowProc();
LRESULT CALLBACK WndProc(HWND h_wnd, UINT n_msg, WPARAM n_w_param, LPARAM n_l_param)
{
	// Resolve type of message.
	switch(n_msg) {
	case WM_PAINT:
		{
			PAINTSTRUCT t_ps;
			HDC h_dc = BeginPaint(h_wnd, &t_ps);
			EndPaint(h_wnd, &t_ps);
		}
		glClearColor(.5f, .5f, .5f, .5f);
		glClear(GL_COLOR_BUFFER_BIT);
		context.Draw();
		glSwapBuffers();
		glPostRedisplay();
		return 0;

	case WM_DESTROY:
		driver.Shutdown();
		PostQuitMessage(0);
		context.Destroy();
		return 0;

	case WM_SIZE:
		context.Draw();
		return 0;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		{
			POINT pointOfClick;
			GetCursorPos(&pointOfClick);
			ScreenToClient(h_wnd, &pointOfClick);
			context.CheckButtons(pointOfClick.x, pointOfClick.y, true);
		}
		return 0;

	case WM_MOUSEMOVE:
		{
			POINT pointOfClick;
			GetCursorPos(&pointOfClick);
			ScreenToClient(h_wnd, &pointOfClick);
			context.CheckButtons(pointOfClick.x, pointOfClick.y, false);
		}
		return 0;

	case WM_KEYDOWN:
		context.CheckHotkeys(int(n_w_param));
		return 0;

	case WM_ERASEBKGND:
		return 1;
	}

	return DefWindowProc(h_wnd, n_msg, n_w_param, n_l_param);
}

// Enable/Disable vertical synchronization if available.
// sync -> Enable/Disable
void SetVSync(bool sync)
{
	typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALPROC)(int);
	PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;

	bool goAhead = false;
	int n_extension_num;
	glGetIntegerv(GL_NUM_EXTENSIONS, &n_extension_num);
	for (int i = 0; i < n_extension_num; ++i) {
		const char *extensions = (const char*)glGetStringi(GL_EXTENSIONS, i);
		if (strstr(extensions, "WGL_EXT_swap_control") != 0)
			goAhead = true;
	}
	if (!goAhead)
		return;

	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)wglGetProcAddress("wglSwapIntervalEXT");

	if (wglSwapIntervalEXT)
		wglSwapIntervalEXT(sync);
}

// Swaps OpenGL front and back buffer, effectively making back buffer contents visible.
void glSwapBuffers()
{
	driver.Blit();
}

// Forces the window to repaint.
void glPostRedisplay()
{
	RedrawWindow(h_wnd, NULL, NULL, RDW_INVALIDATE);
}

// Use this function to end the application properly.
void Exit()
{
	DestroyWindow(h_wnd);
}

// Create all buttons.
void InitButtons(SDLGLContext* context)
{
	unsigned int wpadding = 30;
	unsigned int hpadding = 0;

	GLButton* button1 = new GLButton(40 + wpadding, 510 + hpadding, 100, 30);
	button1->SetColor(0.7f, 0.7f, 1.0f, 1.0f);
	button1->SetText("CPU rendering");
	button1->SetID(0);

	GLButton* button2 = new GLButton(40 + wpadding, 470 + hpadding, 100, 30);
	button2->SetColor(0.7f, 0.7f, 1.0f, 1.0f);
	button2->SetText("GPU rendering");
	button2->SetID(1);

	button1->AddConnection(button2);
	button2->AddConnection(button1);

	GLButton* button3 = new GLButton(40 + wpadding, 360 + hpadding, 100, 30);
	button3->SetColor(0.7f, 0.7f, 1.0f, 1.0f);
	button3->SetText("List");
	button3->SetID(2);

	GLButton* button4 = new GLButton(40 + wpadding, 320 + hpadding, 100, 30);
	button4->SetColor(0.7f, 0.7f, 1.0f, 1.0f);
	button4->SetText("Octree");
	button4->SetID(3);

	GLButton* button5 = new GLButton(40 + wpadding, 280 + hpadding, 100, 30);
	button5->SetColor(0.7f, 0.7f, 1.0f, 1.0f);
	button5->SetText("Uniform grid");
	button5->SetID(4);

	GLButton* button6 = new GLButton(40 + wpadding, 240 + hpadding, 100, 30);
	button6->SetColor(0.7f, 0.7f, 1.0f, 1.0f);
	button6->SetText("BVH");
	button6->SetID(5);

	button3->AddConnection(button4);
	button3->AddConnection(button5);
	button3->AddConnection(button6);

	button4->AddConnection(button3);
	button4->AddConnection(button5);
	button4->AddConnection(button6);

	button5->AddConnection(button3);
	button5->AddConnection(button4);
	button5->AddConnection(button6);

	button6->AddConnection(button3);
	button6->AddConnection(button4);
	button6->AddConnection(button5);

	GLButton* button7 = new GLButton(760, 560, 30, 30);
	button7->SetColor(1.0f, 0.0f, 0.0f, 1.0f);
	button7->Press();
	button7->SetText("||");

	GLButton* button8 = new GLButton(690, 560, 70, 30);
	button8->SetColor(0.7f, 0.7f, 1.0f, 1.0f);
	button8->SetText("Save");

	context->AddButton(button1);
	context->AddButton(button2);
	context->AddButton(button3);
	context->AddButton(button4);
	context->AddButton(button5);
	context->AddButton(button6);
	context->AddButton(button7);
	context->AddButton(button8);
}

// Create all texts.
void InitTexts(SDLGLContext* context)
{
	GLText* text1 = new GLText(540, 545, 100, 30, "Samples: ");
	text1->SetColor(0.5f, 0.0f, 0.5f, 1.0f);
	GLText* text2 = new GLText(40, 560, 170, 25, "Choose rendering mode");
	text2->SetColor(0.0f, 0.5f, 0.5f, 1.0f);
	GLText* text3 = new GLText(30, 410, 180, 25, "Choose accelerate structure");
	text3->SetColor(0.0f, 0.5f, 0.5f, 1.0f);
	GLText* text4 = new GLText(280, 545, 150, 30, "Rendering time: ");
	text4->SetColor(0.5f, 0.0f, 0.5f, 1.0f);
	GLText* text5 = new GLText(30, 180, 180, 25, "Progress of computation");
	text5->SetColor(0.5f, 0.0f, 0.5f, 1.0f);

	GLText* text6 = new GLText(440, 545, 80, 30, "0.000 sec");
	text6->SetColor(0.5f, 0.0f, 0.5f, 1.0f);
	GLText* text7 = new GLText(645, 545, 35, 30, "000");
	text7->SetColor(0.5f, 0.0f, 0.5f, 1.0f);

	context->AddText(text1);
	context->AddText(text2);
	context->AddText(text3);
	context->AddText(text4);
	context->AddText(text5);
	context->AddText(text6);
	context->AddText(text7);
}

// For thread. Start pathtarcing computation
int ComputePT(void* _gpu_pt)
{
	RenderEnginePT* gpu_pt = (RenderEnginePT*)_gpu_pt;
	gpu_pt->ComputePT();
	gpu_pt->Destroy();
	return 0;
}

// Print OpenGL extensions.
void PrintGLInfo()
{
	int n_extension_num;
	glGetIntegerv(GL_NUM_EXTENSIONS, &n_extension_num);
	printf("\n");
	for (int i = 0; i < n_extension_num; ++i) {
		const char *p_s_ext_name = (const char*)glGetStringi(GL_EXTENSIONS, i); // glGetString(GL_EXTENSIONS) uz v OpenGL 3 nefrci, protoze aplikace mely historicky problemy s prilis dlouhymi stringy. ted se extensions zjistuji po jedne.
		printf("%s\n", p_s_ext_name);
	}
	printf("\n");
}