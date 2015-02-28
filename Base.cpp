#include "maths.h"
#include "matrix.h"
#include "vector3.h"

static CBase *g_pD3DApp = NULL;
HWND g_hWnd = NULL;
int g_USE_SSE = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return g_pD3DApp->MsgProc(hWnd, uMsg, wParam, lParam);
}

CBase::CBase(BOOL FullScreen, int Width, int Height, const char *Title)
{
	Log.LogFile << "Starting CBase Constructor...\n\n";
	
	g_pD3DApp = this;

	m_IsFullScreen = FullScreen;
	m_Width = Width;
	m_Height = Height;
	m_Bpp = 32;
	m_Title = Title;
	m_hWnd = NULL;
	m_pD3D = NULL;
	m_pd3dDevice = NULL;

	// Set the FPU status to single precision
	_controlfp(_PC_24, MCW_PC);

	// Check for SSE
	CheckForSSE();

	// Assign the function pointers
	AssignPointers();

	Log.LogFile << "OK.\n\n";
}

CBase::~CBase()
{
	CBase::SafeRelease(m_pd3dDevice, "Releasing D3D Device");
	CBase::SafeRelease(m_pD3D, "Releasing D3D Interface");

	// Restore FPU status
	_controlfp(_CW_DEFAULT, 0xFFFF);
}

void CBase::CheckForSSE()
{
	Log.LogFile << "Checking for SSE support...\n";

	__asm
	{
		mov eax, 1
		cpuid
		and edx, 0x02000000
		shr edx, 25
		mov [g_USE_SSE], edx
	}

	if(g_USE_SSE)
		Log.LogFile << "SSE available, using...\n\n";
	else
		Log.LogFile << "BOO!, no SSE available, defaulting to FPU\n\n";
}

void CBase::AssignPointers()
{	
	Log.LogFile << "Assigning function pointers...\n";

	if(g_USE_SSE)
	{
		CMaths::RSqrt =				&CMaths::SSE_RSqrt;
		CMaths::Sqrt =				&CMaths::SSE_Sqrt;
		CMaths::ASin =				&CMaths::SSE_ASin;
		CMaths::ACos =				&CMaths::SSE_ACos;
		CMaths::Rcp =				&CMaths::SSE_Rcp;

		CMatrix::ZeroMatrix =		&CMatrix::SSE_ZeroMatrix;
		CMatrix::Transpose =		&CMatrix::SSE_Transpose;
		CMatrix::Multiply =			&CMatrix::SSE_Multiply;

		CVector::Negate =			&CVector::SSE_Negate;
		CVector::Multiply =			&CVector::SSE_Multiply;
		CVector::Divide =			&CVector::SSE_Divide;
		CVector::Add =				&CVector::SSE_Add;
		CVector::Subtract =			&CVector::SSE_Subtract;
		CVector::Cross =			&CVector::SSE_Cross;
		CVector::Dot =				&CVector::SSE_Dot;
		CVector::DotW =				&CVector::SSE_DotW;
		CVector::SqrMagnitude =		&CVector::SSE_SqrMagnitude;
		CVector::Magnitude =		&CVector::SSE_Magnitude;

		Log.LogFile << "All SSE enabled functions assigned...\n";
	}
	else
	{
		CMaths::RSqrt =				&CMaths::FPU_RSqrt;
		CMaths::Sqrt =				&CMaths::FPU_Sqrt;
		CMaths::ASin =				&CMaths::FPU_ASin;
		CMaths::ACos =				&CMaths::FPU_ACos;
		CMaths::Rcp =				&CMaths::FPU_Rcp;

		CMatrix::ZeroMatrix =		&CMatrix::FPU_ZeroMatrix;
		CMatrix::Transpose =		&CMatrix::FPU_Transpose;
		CMatrix::Multiply =			&CMatrix::FPU_Multiply;

		CVector::Negate =			&CVector::FPU_Negate;
		CVector::Multiply =			&CVector::FPU_Multiply;
		CVector::Divide =			&CVector::FPU_Divide;
		CVector::Add =				&CVector::FPU_Add;
		CVector::Subtract =			&CVector::FPU_Subtract;
		CVector::Cross =			&CVector::FPU_Cross;
		CVector::Dot =				&CVector::FPU_Dot;
		CVector::DotW =				&CVector::FPU_DotW;
		CVector::SqrMagnitude =		&CVector::FPU_SqrMagnitude;
		CVector::Magnitude =		&CVector::FPU_Magnitude;

		Log.LogFile << "All FPU enabled functions assigned...\n";
	}

	Log.LogFile << "Done.\n\n";
}

void CBase::SetTitle(const char *Title) 
{ 
	Log.LogFile << "Setting Window's Title...";

	m_Title = Title;
	if(m_hWnd)
		SetWindowText(m_hWnd, m_Title);

	Log.LogFile << m_Title << "...OK.\n\n";
}

HRESULT CBase::Create()
{
	Log.LogFile << "Creating Window...";

    HRESULT hr;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	DWORD dwStyle, dwExStyle;

	if (m_IsFullScreen)                 
	{
        dwExStyle = WS_EX_APPWINDOW;
        dwStyle = WS_POPUP;         
	}
	else
	{
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        dwStyle = WS_OVERLAPPEDWINDOW;         
	}	
	
    WNDCLASSEX wc = {sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0, 0, hInstance, 
						NULL, LoadCursor(NULL,IDC_ARROW), NULL, NULL, "DX", NULL};

	RegisterClassEx(&wc);

	m_hWnd = CreateWindowEx(dwExStyle, "DX", m_Title, dwStyle, 0, 0, m_Width, m_Height, 0, 0, 0, 0);
	assert(m_hWnd != NULL);
	g_hWnd = m_hWnd;

	Log.LogFile << "INSTANCE = " << hInstance << "...HWND = " << m_hWnd << "...OK.\n\n";

	if(FAILED(hr = InitD3D9()))
	{
		return E_FAIL;
	}
	
	ShowWindow(m_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(m_hWnd);

	return S_OK;
}

void CBase::CentreWindow() const
{
	Log.LogFile << "Centreing Window...";
	SetWindowPos(m_hWnd, HWND_TOP, (GetSystemMetrics(SM_CXSCREEN) - m_Width) / 2,
				(GetSystemMetrics(SM_CYSCREEN) - m_Height) / 2, 0, 0, SWP_NOSIZE);
	Log.LogFile << "OK.\n\n";
}
                      
HRESULT CBase::InitD3D9()
{
	HRESULT hr;

	Log.LogFile << "Initialising Direct3D...\n";
	
	Log.LogFile << "Creating D3D interface...";
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(m_pD3D == NULL)
	{
		Log.LogFile << "Error creating D3D interface!\n\n";
		return E_FAIL;
	}
	Log.LogFile << "OK.\n\n";

	Log.LogFile << "Setting up Display Mode...";
	D3DDISPLAYMODE d3ddm;
	if(FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		Log.LogFile << "Error Getting Adaptor Display Mode!\n\n";
		return E_FAIL;
	}

	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));

	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.hDeviceWindow = m_hWnd;
	m_d3dpp.BackBufferCount = 1;
	m_d3dpp.EnableAutoDepthStencil = true;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	if(m_IsFullScreen)
	{
		ShowCursor(false);
		m_d3dpp.Windowed          = false;
		m_d3dpp.BackBufferWidth   = m_Width;
		m_d3dpp.BackBufferHeight  = m_Height;
		m_d3dpp.BackBufferFormat  = D3DFMT_A8R8G8B8;
		m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
		m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		Log.LogFile << "Full Screen...Width = " << m_Width << ", Height = " << m_Height << "...OK.\n\n";
	}
	else
	{
		m_d3dpp.Windowed          = true;
		m_d3dpp.BackBufferFormat  = D3DFMT_UNKNOWN;
		Log.LogFile << "Windowed...Width = " << m_Width << ", Height = " << m_Height << "...OK.\n\n";
	}

	D3DCAPS9 d3dcaps;

	Log.LogFile << "Creating D3D Device...";
	
	m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dcaps);

	if(d3dcaps.DevCaps & D3DDEVCAPS_PUREDEVICE)
	{
		hr = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd, 
									D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE, 
									&m_d3dpp, &m_pd3dDevice);
		Log.LogFile << "Using Pure Device...";
	}
	else if(d3dcaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		hr = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd, 
									D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_d3dpp, &m_pd3dDevice);
		Log.LogFile << "Using Hardware T&L Device...";
	}
	else
	{
		hr = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd, 
									D3DCREATE_SOFTWARE_VERTEXPROCESSING, &m_d3dpp, &m_pd3dDevice);
		Log.LogFile << "Using Software T&L Device...";
	}

	if(FAILED(hr))
	{	
		Log.LogFile << "Error Creating Device!...\n\n";
		return E_FAIL;
	}

	Log.LogFile << "OK.\n\nFinished Initialising Direct3D.\n\n";
	
	if(FAILED(InitScene()))
		return E_FAIL;
	
	return hr;
}

int CBase::Run()
{
	MSG msg;
	BOOL bMsg;

	ZeroMemory(&msg, sizeof(msg));

	do
	{													
		if(m_hWnd == GetForegroundWindow())
			bMsg = PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE);
		else
			bMsg = GetMessage(&msg, NULL, 0U, 0U);

		if(bMsg)
		{
			TranslateMessage(&msg);						
			DispatchMessage(&msg);						
		}

		else
		{
			MoveFrame();
			Render();
		}
	}
	while(msg.message != WM_QUIT);											

	return (int)msg.wParam;
}

LRESULT CBase::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}