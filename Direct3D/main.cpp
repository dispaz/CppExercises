/*
    This is just a test application for the Direct3D hook proof-of-concept.

    The code for this application is from http://www.directxtutorial.com/
    The only thing I added was 'LoadLibrary("Chapter8_Direct3DHook.dll");'
    to load the POC hook.
*/
// include the basic windows header files and the Direct3D header file
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>

// define the screen resolution
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600


// include the Direct3D Library files
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

// global declarations
LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddev;
LPDIRECT3DVERTEXBUFFER9 cube_v_buffer = NULL;
LPDIRECT3DINDEXBUFFER9 i_buffer = NULL;
LPDIRECT3DVERTEXBUFFER9 tetra_v_buffer = NULL;

// function prototypes
void initD3D(HWND hWnd);
void render_frame(void);
void cleanD3D(void);
void init_graphics(void);
void init_cube();
void init_tetrahedron();
void init_light(void);    // sets up the light and the material

struct CUSTOMVERTEX { FLOAT X, Y, Z; D3DVECTOR NORMAL; };
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_NORMAL)

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


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
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"WindowClass";

    RegisterClassEx(&wc);

    hWnd = CreateWindowEx(NULL, L"WindowClass", L"Our Direct3D Program",
        WS_OVERLAPPEDWINDOW, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);

    // set up and initialize Direct3D
    initD3D(hWnd);

    // enter the main loop:

    MSG msg;

    while (TRUE)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (msg.message == WM_QUIT)
            break;

        render_frame();
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
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hWnd;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferWidth = SCREEN_WIDTH;
    d3dpp.BackBufferHeight = SCREEN_HEIGHT;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    // create a device class using this information and the info from the d3dpp stuct
    d3d->CreateDevice(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp,
        &d3ddev);

    init_graphics();    // call the function to initialize the cube
    init_light();    // call the function to initialize the light and material

    d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);    // turn on the 3D lighting
    d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);    // turn on the z-buffer
    d3ddev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(15, 15, 15));    // ambient light

    LoadLibrary(L"Chapter8_Direct3DDLL.dll");
}

bool shouldRotate = false;
// this is the function used to render a single frame
void render_frame(void)
{
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    d3ddev->BeginScene();

    // select which vertex format we are using
    d3ddev->SetFVF(CUSTOMFVF);

    // set the view transform
    D3DXMATRIX matView;
    D3DXVECTOR3 camPos = D3DXVECTOR3(-10.0f, 8.0f, 25.0f);
	D3DXVECTOR3 lookAtPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 upDirection = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    D3DXMatrixLookAtLH(&matView,
        &camPos, // the camera position
        &lookAtPos,      // the look-at position
        &upDirection);    // the up direction
    d3ddev->SetTransform(D3DTS_VIEW, &matView);

    // set the projection transform
    D3DXMATRIX matProjection;
    D3DXMatrixPerspectiveFovLH(&matProjection,
        D3DXToRadian(45),
        (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT,
        1.0f,    // the near view-plane
        100.0f);    // the far view-plane
    d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);

    // set the world transform
    static float index = 0.0f; index += 0.03f;
    D3DXMATRIX matRotateY;
    if (shouldRotate)
    {
        D3DXMatrixRotationY(&matRotateY, index);
        d3ddev->SetTransform(D3DTS_WORLD, &(matRotateY));
    }

    if(GetAsyncKeyState(VK_F2))
    {
        shouldRotate = !shouldRotate;
        Sleep(200);
	}
    // select the vertex and index buffers to use
    d3ddev->SetStreamSource(0, cube_v_buffer, 0, sizeof(CUSTOMVERTEX));
    d3ddev->SetIndices(i_buffer);

    // draw the cube
    d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);

	d3ddev->SetStreamSource(0, tetra_v_buffer, 0, sizeof(CUSTOMVERTEX));
    d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 4);

    d3ddev->EndScene();

    d3ddev->Present(NULL, NULL, NULL, NULL);
}


// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
    cube_v_buffer->Release();
    tetra_v_buffer->Release();
    i_buffer->Release();
    d3ddev->Release();
    d3d->Release();
}


// this is the function that puts the 3D models into video RAM
void init_graphics(void)
{
    init_cube();
    init_tetrahedron();
}

void init_cube()
{
    // create the vertices using the CUSTOMVERTEX struct
    CUSTOMVERTEX cubeVertices[] =
    {
        { -3.0f, -3.0f, 3.0f, 0.0f, 0.0f, 1.0f, },    // side 1
        { 3.0f, -3.0f, 3.0f, 0.0f, 0.0f, 1.0f, },
        { -3.0f, 3.0f, 3.0f, 0.0f, 0.0f, 1.0f, },
        { 3.0f, 3.0f, 3.0f, 0.0f, 0.0f, 1.0f, },

        { -3.0f, -3.0f, -3.0f, 0.0f, 0.0f, -1.0f, },    // side 2
        { -3.0f, 3.0f, -3.0f, 0.0f, 0.0f, -1.0f, },
        { 3.0f, -3.0f, -3.0f, 0.0f, 0.0f, -1.0f, },
        { 3.0f, 3.0f, -3.0f, 0.0f, 0.0f, -1.0f, },

        { -3.0f, 3.0f, -3.0f, 0.0f, 1.0f, 0.0f, },    // side 3
        { -3.0f, 3.0f, 3.0f, 0.0f, 1.0f, 0.0f, },
        { 3.0f, 3.0f, -3.0f, 0.0f, 1.0f, 0.0f, },
        { 3.0f, 3.0f, 3.0f, 0.0f, 1.0f, 0.0f, },

        { -3.0f, -3.0f, -3.0f, 0.0f, -1.0f, 0.0f, },    // side 4
        { 3.0f, -3.0f, -3.0f, 0.0f, -1.0f, 0.0f, },
        { -3.0f, -3.0f, 3.0f, 0.0f, -1.0f, 0.0f, },
        { 3.0f, -3.0f, 3.0f, 0.0f, -1.0f, 0.0f, },

        { 3.0f, -3.0f, -3.0f, 1.0f, 0.0f, 0.0f, },    // side 5
        { 3.0f, 3.0f, -3.0f, 1.0f, 0.0f, 0.0f, },
        { 3.0f, -3.0f, 3.0f, 1.0f, 0.0f, 0.0f, },
        { 3.0f, 3.0f, 3.0f, 1.0f, 0.0f, 0.0f, },

        { -3.0f, -3.0f, -3.0f, -1.0f, 0.0f, 0.0f, },    // side 6
        { -3.0f, -3.0f, 3.0f, -1.0f, 0.0f, 0.0f, },
        { -3.0f, 3.0f, -3.0f, -1.0f, 0.0f, 0.0f, },
        { -3.0f, 3.0f, 3.0f, -1.0f, 0.0f, 0.0f, },

    };

    // create a vertex buffer interface called v_buffer
    d3ddev->CreateVertexBuffer(24 * sizeof(CUSTOMVERTEX),
        0,
        CUSTOMFVF,
        D3DPOOL_MANAGED,
        &cube_v_buffer,
        NULL);

    VOID* pVoid;    // a void pointer

    // lock v_buffer and load the vertices into it
    cube_v_buffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, cubeVertices, sizeof(cubeVertices));
    cube_v_buffer->Unlock();

    // create the indices using an int array
    short indices[] =
    {
        0, 1, 2,    // side 1
        2, 1, 3,
        4, 5, 6,    // side 2
        6, 5, 7,
        8, 9, 10,    // side 3
        10, 9, 11,
        12, 13, 14,    // side 4
        14, 13, 15,
        16, 17, 18,    // side 5
        18, 17, 19,
        20, 21, 22,    // side 6
        22, 21, 23,
    };

    // create an index buffer interface called i_buffer
    d3ddev->CreateIndexBuffer(36 * sizeof(short),
        0,
        D3DFMT_INDEX16,
        D3DPOOL_MANAGED,
        &i_buffer,
        NULL);

    // lock i_buffer and load the indices into it
    i_buffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, indices, sizeof(indices));
    i_buffer->Unlock();
}

void init_tetrahedron()
{
    CUSTOMVERTEX tetrahedronVertices[] =
    {
        // Face 1 (v0, v1, v2)
        {  1.0f,  7.0f,  1.0f,   0.0f, 0.0f, 1.0f},
        { -1.0f,  5.0f,  1.0f,   0.0f, 0.0f, 1.0f},
        { -1.0f,  7.0f, -1.0f,   0.0f, 0.0f, 1.0f},
        
        // Face 2 (v0, v3, v1)
        {  1.0f,  7.0f,  1.0f,   0.0f, 1.0f, 0.0f},
        {  1.0f,  5.0f, -1.0f,   0.0f, 1.0f, 0.0f},
        { -1.0f,  5.0f,  1.0f,   0.0f, 1.0f, 0.0f},

        // Face 3 (v0, v2, v3)
        {  1.0f,  7.0f,  1.0f,   1.0f, 0.0f, 0.0f},
        { -1.0f,  7.0f, -1.0f,   1.0f, 0.0f, 0.0f},
        {  1.0f,  5.0f, -1.0f,   1.0f, 0.0f, 0.0f},

        // Face 4 (v1, v3, v2)
        { -1.0f,  5.0f,  1.0f,   0.0f,-1.0f, 0.0f },
        {  1.0f,  5.0f, -1.0f,   0.0f,-1.0f, 0.0f },
        { -1.0f,  7.0f, -1.0f,   0.0f,-1.0f, 0.0f },
    };

    d3ddev->CreateVertexBuffer(12 * sizeof(CUSTOMVERTEX),
        0,
		CUSTOMFVF,
        D3DPOOL_MANAGED,
        &tetra_v_buffer,
        NULL);
    
    void* pVertices;
    tetra_v_buffer->Lock(0, sizeof(tetrahedronVertices), (void**)&pVertices, 0);
    memcpy(pVertices, tetrahedronVertices, sizeof(tetrahedronVertices));
    tetra_v_buffer->Unlock();
}

void init_light()
{
    D3DMATERIAL9 material;

    ZeroMemory(&material, sizeof(D3DMATERIAL9));
    material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    d3ddev->SetMaterial(&material);
}
