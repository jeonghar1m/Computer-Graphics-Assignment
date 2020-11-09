// include the basic windows header files and the Direct3D header file

#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>

// define the screen resolution and keyboard macros
#define SCREEN_WIDTH  1920
#define SCREEN_HEIGHT 1080
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

// global declarations
LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class
LPD3DXSPRITE d3dspt;    // the pointer to our Direct3D Sprite interface

// sprite declarations
LPDIRECT3DTEXTURE9 sprite;    // the pointer to the sprite

// function prototypes
void initD3D(HWND hWnd); // sets up and initializes Direct3D
void render_frame(void); // renders a single frame
void cleanD3D(void); // closes Direct3D and releases memory

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
D3DXVECTOR3 position(550.0f, 350.0f, 0.0f);    // position at 50, 50 with no depth

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    HWND hWnd;
    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"WindowClass1";

    RegisterClassEx(&wc);

    hWnd = CreateWindowEx(NULL,
        L"WindowClass1",
        L"Our Direct3D Program",
        WS_EX_TOPMOST | WS_POPUP,
        0, 0,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        NULL,
        NULL,
        hInstance,
        NULL);

    ShowWindow(hWnd, nCmdShow);

    // set up and initialize Direct3D
    initD3D(hWnd);

    // enter the main loop:
    MSG msg;

    while (TRUE)
    {
        DWORD starting_point = GetTickCount();

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        render_frame();

        // check the 'escape' key
        if (KEY_DOWN(VK_ESCAPE))
            PostMessage(hWnd, WM_DESTROY, 0, 0);

        while ((GetTickCount() - starting_point) < 25);
    }

    // clean up DirectX and COM
    cleanD3D();
    return msg.wParam;
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        } break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}





// this function initializes and prepares Direct3D for use
void initD3D(HWND hWnd)
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION);
    D3DPRESENT_PARAMETERS d3dpp;

    ZeroMemory(&d3dpp, sizeof(d3dpp));

    d3dpp.Windowed = FALSE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hWnd;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferWidth = SCREEN_WIDTH;
    d3dpp.BackBufferHeight = SCREEN_HEIGHT;

    // create a device class using this information and the info from the d3dpp stuct
    d3d->CreateDevice(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp,
        &d3ddev);

    D3DXCreateSprite(d3ddev, &d3dspt);    // create the Direct3D Sprite object
    D3DXCreateTextureFromFile(d3ddev, L"nyan_cat.png", &sprite);
    return;

}

// this is the function used to render a single frame
void render_frame(void)
{
    int speed = 5;
    // clear the window to a deep blue
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    d3ddev->BeginScene();    // begins the 3D scene
    d3dspt->Begin(NULL);    // begin sprite drawing

    // draw the sprite
    D3DXVECTOR3 center(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
    if(KEY_DOWN(VK_SHIFT))  //DASH
        speed = 10;

    if (KEY_DOWN(VK_LEFT))
        position.x -= speed;
    if (KEY_DOWN(VK_RIGHT))
        position.x += speed;
    if (KEY_DOWN(VK_UP))
        position.y -= speed;
    if (KEY_DOWN(VK_DOWN))
        position.y += speed;

    if (KEY_DOWN(VK_SPACE)) {}

    d3dspt->Draw(sprite, NULL, &center, &position, D3DCOLOR_XRGB(255, 255, 255));
    d3dspt->End();    // end sprite drawing
    d3ddev->EndScene();    // ends the 3D scene
    d3ddev->Present(NULL, NULL, NULL, NULL);
    return;
}

// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
    sprite->Release();
    d3dspt->Release();
    d3ddev->Release();
    d3d->Release();
    return;
}