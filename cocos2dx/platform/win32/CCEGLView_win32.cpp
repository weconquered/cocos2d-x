/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CCEGLView.h"

#include "EGL/egl.h"
#include "gles/gl.h"

#include "CCSet.h"
#include "ccMacros.h"
#include "CCDirector.h"
#include "CCTouch.h"
#include "CCTouchDispatcher.h"
#include "CCIMEDispatcher.h"
#include "CCKeypadDispatcher.h"
#include "CCApplication.h"
#include "CCLayer.h"
#include "CCGestureRecognizer.h"

NS_CC_BEGIN;

//////////////////////////////////////////////////////////////////////////
// impliment CCEGL
//////////////////////////////////////////////////////////////////////////

class CCEGL
{
public:
	~CCEGL() 
	{
		if (EGL_NO_SURFACE != m_eglSurface)
		{
			eglDestroySurface(m_eglDisplay, m_eglSurface);
		}
		if (EGL_NO_CONTEXT != m_eglContext)
		{
			eglDestroyContext(m_eglDisplay, m_eglContext);
		}
		eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglTerminate(m_eglDisplay);
		if (m_eglNativeDisplay)
		{
			ReleaseDC(m_eglNativeWindow, m_eglNativeDisplay);
		}
	}

	static CCEGL * create(CCEGLView * pWindow)
	{
		CCEGL * pEGL = new CCEGL;
		BOOL bSuccess = FALSE;
		do 
		{
			CC_BREAK_IF(! pEGL);

			pEGL->m_eglNativeWindow = pWindow->getHWnd();

			pEGL->m_eglNativeDisplay = GetDC(pEGL->m_eglNativeWindow);

			EGLDisplay eglDisplay;
			CC_BREAK_IF(EGL_NO_DISPLAY == (eglDisplay = eglGetDisplay(pEGL->m_eglNativeDisplay)));

			EGLint nMajor, nMinor;
			CC_BREAK_IF(EGL_FALSE == eglInitialize(eglDisplay, &nMajor, &nMinor) || 1 != nMajor);

			const EGLint aConfigAttribs[] =
			{
				EGL_LEVEL,				0,
				EGL_SURFACE_TYPE,		EGL_WINDOW_BIT,
				EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
				EGL_NATIVE_RENDERABLE,	EGL_FALSE,
				EGL_DEPTH_SIZE,			16,
				EGL_NONE,
			};
			EGLint iConfigs;
			EGLConfig eglConfig;
			CC_BREAK_IF(EGL_FALSE == eglChooseConfig(eglDisplay, aConfigAttribs, &eglConfig, 1, &iConfigs) 
				|| (iConfigs != 1));

			EGLContext eglContext;
			eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, NULL);
			CC_BREAK_IF(EGL_NO_CONTEXT == eglContext);

			EGLSurface eglSurface;
			eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, pEGL->m_eglNativeWindow, NULL);
			CC_BREAK_IF(EGL_NO_SURFACE == eglSurface);

			CC_BREAK_IF(EGL_FALSE == eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext));

			pEGL->m_eglDisplay = eglDisplay;
			pEGL->m_eglConfig  = eglConfig;
			pEGL->m_eglContext = eglContext;
			pEGL->m_eglSurface = eglSurface;
			bSuccess = TRUE;
		} while (0);

		if (! bSuccess)
		{
			CC_SAFE_DELETE(pEGL);  
		}

		return pEGL;
	}

	void resizeSurface()
	{
//  		if (! m_eglNativeWindow || EGL_NO_DISPLAY == m_eglDisplay)
//  		{
//  			return;
//  		}
//  
//  		// release old surface
//  		if (EGL_NO_SURFACE != m_eglSurface)
//  		{
//  			eglDestroySurface(m_eglDisplay, m_eglSurface);
//  			m_eglSurface = EGL_NO_SURFACE;
//  		}
//  
//  		// create new surface and make current
//  		m_eglSurface = eglCreateWindowSurface(m_eglDisplay, m_eglConfig, m_eglNativeWindow, NULL);
//  		eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);
	}

	void swapBuffers()
	{
		if (EGL_NO_DISPLAY != m_eglDisplay)
		{
			eglSwapBuffers(m_eglDisplay, m_eglSurface);
		}
	}
private:
	CCEGL() 
		: m_eglNativeWindow(NULL)
		, m_eglNativeDisplay(EGL_DEFAULT_DISPLAY)
		, m_eglDisplay(EGL_NO_DISPLAY)
		, m_eglConfig(0)
		, m_eglSurface(EGL_NO_SURFACE)
		, m_eglContext(EGL_NO_CONTEXT)
	{}

	EGLNativeWindowType     m_eglNativeWindow;
	EGLNativeDisplayType    m_eglNativeDisplay;
	EGLDisplay              m_eglDisplay;
	EGLConfig               m_eglConfig;
	EGLSurface              m_eglSurface;
	EGLContext              m_eglContext;
};

//////////////////////////////////////////////////////////////////////////
// impliment CCEGLView
//////////////////////////////////////////////////////////////////////////
static CCEGLView * s_pMainWindow;
static const WCHAR * kWindowClassName = L"Cocos2dxWin32";

static LRESULT CALLBACK _WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (s_pMainWindow && s_pMainWindow->getHWnd() == hWnd)
	{
		return s_pMainWindow->WindowProc(uMsg, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

CCEGLView::CCEGLView()
: m_bCaptured(false)
, m_bOrientationReverted(false)
, m_bOrientationInitVertical(false)
, m_pDelegate(NULL)
, m_pEGL(NULL)
, m_hWnd(NULL)
, m_eInitOrientation(CCDeviceOrientationPortrait)
, m_fScreenScaleFactorX(1.0f)
, m_fScreenScaleFactorY(1.0f)
,m_isFullScreen(false)
, m_lpfnAccelerometerKeyHook(NULL)
{
	enablePause = false;
	m_mouseClic = false;
	m_altPressed = false;
    m_pTouch    = new CCTouch;
    m_pSet      = new CCSet;
    m_tSizeInPoints.cx = m_tSizeInPoints.cy = 0;
    SetRectEmpty(&m_rcViewPort);
	myStack.myTouchStack.clear();
	myStack.myTouchEndCount=0;
}

CCEGLView::~CCEGLView()
{
	delete m_pTouch;
	delete m_pSet;
}
void	CCEGLView::GetScreenCapacity(int& hSize,int& vSize)
{
	HDC screen = GetDC(NULL);
	hSize=GetDeviceCaps(screen,HORZSIZE);
	vSize=GetDeviceCaps(screen,VERTSIZE);
}


bool CCEGLView::Create(LPCTSTR pTitle, int w, int h)
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(m_hWnd);

		HINSTANCE hInstance = GetModuleHandle( NULL );
		WNDCLASS  wc;		// Windows Class Structure

		// Redraw On Size, And Own DC For Window.
		wc.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;  
		wc.lpfnWndProc    = _WindowProc;					// WndProc Handles Messages
		wc.cbClsExtra     = 0;                              // No Extra Window Data
		wc.cbWndExtra     = 0;								// No Extra Window Data
		wc.hInstance      = hInstance;						// Set The Instance
		wc.hIcon          = LoadIcon( NULL, IDI_WINLOGO );	// Load The Default Icon
		wc.hCursor        = LoadCursor( NULL, IDC_ARROW );	// Load The Arrow Pointer
		wc.hbrBackground  = NULL;                           // No Background Required For GL
		wc.lpszMenuName   = NULL;                           // We Don't Want A Menu
		wc.lpszClassName  = kWindowClassName;               // Set The Class Name

		CC_BREAK_IF(! RegisterClass(&wc) && 1410 != GetLastError());		

		// center window position
		RECT rcDesktop;
		GetWindowRect(GetDesktopWindow(), &rcDesktop);

		// create window
		m_hWnd = CreateWindowEx(
			WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,	// Extended Style For The Window
			kWindowClassName,									// Class Name
			pTitle,												// Window Title
			WS_CAPTION | WS_POPUPWINDOW | WS_MINIMIZEBOX,		// Defined Window Style
			0, 0,								                // Window Position
			0,                                                  // Window Width
			0,                                                  // Window Height
			NULL,												// No Parent Window
			NULL,												// No Menu
			hInstance,											// Instance
			NULL );

		CC_BREAK_IF(! m_hWnd);

        m_eInitOrientation = CCDirector::sharedDirector()->getDeviceOrientation();
        m_bOrientationInitVertical = (CCDeviceOrientationPortrait == m_eInitOrientation
            || kCCDeviceOrientationPortraitUpsideDown == m_eInitOrientation) ? true : false;
        m_tSizeInPoints.cx = w;
        m_tSizeInPoints.cy = h;
        resize(w, h);

		// init egl
		m_pEGL = CCEGL::create(this);

		if (! m_pEGL)
		{
			DestroyWindow(m_hWnd);
			m_hWnd = NULL;
			break;
		}

		s_pMainWindow = this;
		bRet = true;
	} while (0);

	return bRet;
}

void	CCEGLView::Update()
{
	std::vector<TouchStruct>::iterator	it=myStack.myTouchStack.begin();

	while(it!=myStack.myTouchStack.end())
	{
		bool canExit=false;
		TouchStruct& current=(*it);

		switch(current.type)
		{
		case 0:
			{
				m_pTouch->SetTouchInfo(0, (float)(current.pos.x - m_rcViewPort.left) / m_fScreenScaleFactorX,
				(float)(current.pos.y - m_rcViewPort.top) / m_fScreenScaleFactorY);
				m_pSet->addObject(m_pTouch);
				m_pDelegate->touchesBegan(m_pSet, NULL);
				break;
			}
		case 1:
			{
				m_pTouch->SetTouchInfo(0, (float)(current.pos.x- m_rcViewPort.left) / m_fScreenScaleFactorX,
					(float)(current.pos.y - m_rcViewPort.top) / m_fScreenScaleFactorY);
				m_pDelegate->touchesMoved(m_pSet, NULL);
				break;
			}
		case 2:
			{
				m_pTouch->SetTouchInfo(0, (float)(current.pos.x- m_rcViewPort.left) / m_fScreenScaleFactorX,
					(float)(current.pos.y - m_rcViewPort.top) / m_fScreenScaleFactorY);
				m_pDelegate->touchesEnded(m_pSet, NULL);
				m_pSet->removeObject(m_pTouch);
				myStack.myTouchEndCount--;
				canExit=true;
				break;
			}
		}
		// one touch end per loop
		it=myStack.myTouchStack.erase(it);
		if(canExit)
		{
			break;
		}
	}

}


void CCEGLView::takeWindowFocus()
{
	//Attach foreground window thread

	//to our thread
	AttachThreadInput(
		GetWindowThreadProcessId(
			::GetForegroundWindow(),NULL),
		GetCurrentThreadId(),TRUE);

	//Do our stuff here ;-)

	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd); //Just playing safe


	//Detach the attached thread

	AttachThreadInput(
		GetWindowThreadProcessId(
			::GetForegroundWindow(),NULL),
		GetCurrentThreadId(),FALSE);
}

LRESULT CCEGLView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	switch (message)
	{

		case WM_SHOWWINDOW:
		{
			this->takeWindowFocus();
			break;
		}

		case WM_SYSKEYDOWN:
		{
			if (!m_mouseClic)
			{
				m_altPressed = true;
			}
			break;
		}
		case WM_SYSKEYUP:
		{
			if (!m_mouseClic)
			{
				m_altPressed = false;
			}
			break;
		}
	case WM_LBUTTONDOWN:
		{
			if (!m_altPressed)
			{
				if (m_pDelegate && m_pTouch && MK_LBUTTON == wParam)
				{
					POINT pt = {(short)LOWORD(lParam), (short)HIWORD(lParam)};
					if (PtInRect(&m_rcViewPort, pt))
					{
						if(myStack.myTouchEndCount<4)
						{
							m_mouseClic = true;
	#ifdef WIN32
							CCDirector::sharedDirector()->ChangeMouseCursor(CURSOR_POINT_PRESSED);
	#endif
							m_bCaptured = true;
							SetCapture(m_hWnd);
	/*						m_pTouch->SetTouchInfo(0, (float)(pt.x - m_rcViewPort.left) / m_fScreenScaleFactorX,
								(float)(pt.y - m_rcViewPort.top) / m_fScreenScaleFactorY);
							m_pSet->addObject(m_pTouch);
							m_pDelegate->touchesBegan(m_pSet, NULL);*/
							TouchStruct	toAdd;
							toAdd.pos.x=(float)LOWORD(lParam);
							toAdd.pos.y=(float)HIWORD(lParam);
							toAdd.type=0;

							myStack.myTouchStack.push_back(toAdd);
						}
					}
				}
			}
			break;
		}

	case WM_MOUSEMOVE:
        {
				POINT pos;
				GetCursorPos(&pos);
				int value = CCDirector::sharedDirector()->GetPos().y - pos.y;

				if ( m_altPressed && ( value > 0) )
				{
					//Up
					CCDirector::sharedDirector()->addInputs(ST_ADD);
				}
				else if ( m_altPressed && ( value < 0) )
				{
					//Down
					CCDirector::sharedDirector()->addInputs(ST_SUBSTRACT);
				}
				CCDirector::sharedDirector()->SetPos(pos);
				
				if (MK_LBUTTON == wParam && m_bCaptured)
				{
/*					m_pTouch->SetTouchInfo(0, (float)((short)LOWORD(lParam)- m_rcViewPort.left) / m_fScreenScaleFactorX,
						(float)((short)HIWORD(lParam) - m_rcViewPort.top) / m_fScreenScaleFactorY);
					m_pDelegate->touchesMoved(m_pSet, NULL);*/
					if(myStack.myTouchEndCount<4)
					{
						TouchStruct	toAdd;
						toAdd.pos.x=(float)LOWORD(lParam);
						toAdd.pos.y=(float)HIWORD(lParam);
						toAdd.type=1;

						myStack.myTouchStack.push_back(toAdd);
					}

				}
			}
			break;

	case WM_LBUTTONUP:
		{
			if (!m_altPressed)
			{
				if (m_bCaptured)
				{
					if(myStack.myTouchEndCount<4)
					{
#ifdef WIN32
						m_mouseClic = false;

						CCDirector::sharedDirector()->ChangeMouseCursor(CURSOR_BACK_TO_POINT);
#endif
	/*					m_pTouch->SetTouchInfo(0, (float)((short)LOWORD(lParam)- m_rcViewPort.left) / m_fScreenScaleFactorX,
							(float)((short)HIWORD(lParam) - m_rcViewPort.top) / m_fScreenScaleFactorY);
						m_pDelegate->touchesEnded(m_pSet, NULL);
						m_pSet->removeObject(m_pTouch);*/
						TouchStruct	toAdd;
						toAdd.pos.x=(float)LOWORD(lParam);
						toAdd.pos.y=(float)HIWORD(lParam);
						toAdd.type=2;

						myStack.myTouchStack.push_back(toAdd);
						myStack.myTouchEndCount++;
					}
					ReleaseCapture();

					m_bCaptured = false;
					
				}
			}
		case SIZE_RESTORED:
			CCApplication::sharedApplication().applicationWillEnterForeground();
			break;
		case SIZE_MINIMIZED:
			CCApplication::sharedApplication().applicationDidEnterBackground();
			break;
		}
		break;
			
		case WM_MOUSEWHEEL:
		{
			if (!m_altPressed && !m_mouseClic)
			{
				short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

				if ( zDelta > 0 )
				{
					CCDirector::sharedDirector()->addInputs(ST_ADD);
				}
				else
				{
					CCDirector::sharedDirector()->addInputs(ST_SUBSTRACT);
				}
			}
			break;
		}
		
	case WM_KEYDOWN:
		if (wParam == VK_F1 || wParam == VK_F2)
		{
			if (GetKeyState(VK_LSHIFT) < 0 ||  GetKeyState(VK_RSHIFT) < 0 || GetKeyState(VK_SHIFT) < 0)
				CCKeypadDispatcher::sharedDispatcher()->dispatchKeypadMSG(wParam == VK_F1 ? kTypeBackClicked : kTypeMenuClicked);
		}
		if ( m_lpfnAccelerometerKeyHook!=NULL )
		{
			(*m_lpfnAccelerometerKeyHook)( message,wParam,lParam );
		}
		break;
	case WM_KEYUP:
		if ( m_lpfnAccelerometerKeyHook!=NULL )
		{
			(*m_lpfnAccelerometerKeyHook)( message,wParam,lParam );
		}
		break;
    case WM_CHAR:
        {
            if (wParam < 0x20)
            {
                if (VK_BACK == wParam)
                {
                    CCIMEDispatcher::sharedDispatcher()->dispatchDeleteBackward();
                }
                else if (VK_RETURN == wParam)
                {
                    CCIMEDispatcher::sharedDispatcher()->dispatchInsertText("\n", 1);
                }
                else if (VK_TAB == wParam)
                {
                    // tab input
                }
                else if (VK_ESCAPE == wParam)
                {
                    // ESC input
					CCDirector::sharedDirector()->end();
                }
            }
            else if (wParam < 128)
            {
                // ascii char
                CCIMEDispatcher::sharedDispatcher()->dispatchInsertText((const char *)&wParam, 1);
            }
            else
            {
                char szUtf8[8] = {0};
                int nLen = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)&wParam, 1, szUtf8, sizeof(szUtf8), NULL, NULL);

                CCIMEDispatcher::sharedDispatcher()->dispatchInsertText(szUtf8, nLen);
            }
			if ( m_lpfnAccelerometerKeyHook!=NULL )
			{
				(*m_lpfnAccelerometerKeyHook)( message,wParam,lParam );
			}
        }
        break;

	case WM_PAINT:
		BeginPaint(m_hWnd, &ps);
		EndPaint(m_hWnd, &ps);
		break;

	case WM_CLOSE:
		CCDirector::sharedDirector()->end();
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
		case WM_SETFOCUS:
			m_altPressed = false;
			CCDirector::sharedDirector()->resume();
			break;
		case WM_KILLFOCUS:
			if (enablePause)
			{
				CCDirector::sharedDirector()->pause();
			}
			m_altPressed = false;
			break;

	default:
		return DefWindowProc(m_hWnd, message, wParam, lParam);
	}
	return 0;
}

void CCEGLView::setAccelerometerKeyHook( LPFN_ACCELEROMETER_KEYHOOK lpfnAccelerometerKeyHook )
{
	m_lpfnAccelerometerKeyHook=lpfnAccelerometerKeyHook;
}

CCSize CCEGLView::getSize()
{
    if (m_bOrientationReverted)
    {
        return CCSize((float)(m_tSizeInPoints.cy), (float)(m_tSizeInPoints.cx));
    }
    return CCSize((float)(m_tSizeInPoints.cx), (float)(m_tSizeInPoints.cy));
}

bool CCEGLView::isOpenGLReady()
{
    return (NULL != m_pEGL);
}

void CCEGLView::release()
{
	if (m_hWnd)
	{
		DestroyWindow(m_hWnd);
		m_hWnd = NULL;
	}
	s_pMainWindow = NULL;
	UnregisterClass(kWindowClassName, GetModuleHandle(NULL));

    CC_SAFE_DELETE(m_pSet);
    CC_SAFE_DELETE(m_pTouch);
    CC_SAFE_DELETE(m_pDelegate);
    CC_SAFE_DELETE(m_pEGL);
    delete this;
}

void CCEGLView::addSubview(CCLayer* _layer)
{
	CCDirector::sharedDirector()->addSubview(_layer);
}

void CCEGLView::setTouchDelegate(EGLTouchDelegate * pDelegate)
{
    m_pDelegate = pDelegate;
}

void CCEGLView::swapBuffers()
{
    if (m_pEGL)
    {
        m_pEGL->swapBuffers();
    }
}

int CCEGLView::setDeviceOrientation(int eOritation)
{
	do 
	{
		bool bVertical = (CCDeviceOrientationPortrait == eOritation
			|| kCCDeviceOrientationPortraitUpsideDown == eOritation) ? true : false;

		CC_BREAK_IF(m_bOrientationReverted && bVertical != m_bOrientationInitVertical);
		CC_BREAK_IF(! m_bOrientationReverted && bVertical == m_bOrientationInitVertical);

        m_bOrientationReverted = (bVertical == m_bOrientationInitVertical) ? false : true;

        // swap width and height
		RECT rc;
		GetClientRect(m_hWnd, &rc);
        resize(rc.bottom - rc.top, rc.right - rc.left);

	} while (0);

	return m_eInitOrientation;
}

void CCEGLView::setViewPortInPoints(float x, float y, float w, float h)
{
    if (m_pEGL)
    {
        float factorx = m_fScreenScaleFactorX / CC_CONTENT_SCALE_FACTOR();
		float factory = m_fScreenScaleFactorY / CC_CONTENT_SCALE_FACTOR();
        glViewport((GLint)(x * factorx) + m_rcViewPort.left,
            (GLint)(y * factory) + m_rcViewPort.top,
            (GLint)(w * factorx),
            (GLint)(h * factory));
    }
}

void CCEGLView::setScissorInPoints(float x, float y, float w, float h)
{
    if (m_pEGL)
    {
        float factorx = m_fScreenScaleFactorX / CC_CONTENT_SCALE_FACTOR();
		float factory = m_fScreenScaleFactorY / CC_CONTENT_SCALE_FACTOR();
        glScissor((GLint)(x * factorx) + m_rcViewPort.left,
            (GLint)(y * factory) + m_rcViewPort.top,
            (GLint)(w * factorx),
            (GLint)(h * factory));
    }
}

void CCEGLView::setIMEKeyboardState(bool /*bOpen*/)
{
}

HWND CCEGLView::getHWnd()
{
    return m_hWnd;
}

void CCEGLView::resize(int width, int height)
{
    if (! m_hWnd)
    {
        return;
    }

    RECT rcClient;
    GetClientRect(m_hWnd, &rcClient);
    if (rcClient.right - rcClient.left == width &&
        rcClient.bottom - rcClient.top == height)
    {
        return;
    }
	if(m_isFullScreen)
	{
		GetWindowRect(GetDesktopWindow(), &rcClient);

		AdjustWindowRectEx(&rcClient, GetWindowLong(m_hWnd, GWL_STYLE), false, GetWindowLong(m_hWnd, GWL_EXSTYLE));
		// change width and height
		SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, rcClient.right - rcClient.left, 
			rcClient.bottom - rcClient.top, SWP_NOCOPYBITS | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
	}
	else
	{
		// calculate new window width and height
		rcClient.right = rcClient.left + width;
		rcClient.bottom = rcClient.top + height;
		AdjustWindowRectEx(&rcClient, GetWindowLong(m_hWnd, GWL_STYLE), false, GetWindowLong(m_hWnd, GWL_EXSTYLE));

		// change width and height
		SetWindowPos(m_hWnd, 0, 0, 0, rcClient.right - rcClient.left, 
			rcClient.bottom - rcClient.top, SWP_NOCOPYBITS | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
	}
    if (m_pEGL)
    {
        m_pEGL->resizeSurface();
    }

	if(m_isFullScreen)
	{
		// ASSORIA m_fScreenScaleFactor can be reset here to match better resolution


		GetWindowRect(GetDesktopWindow(), &rcClient);
		// calculate client new width and height
		int newW = rcClient.right - rcClient.left;
		int newH = rcClient.bottom - rcClient.top;

#ifdef TEST_SCALE_FACTOR


		float scaleX;
		float scaleY;

		if (m_bOrientationReverted)
		{
			scaleX=((float)newW)/((float)m_tSizeInPoints.cy * CC_CONTENT_SCALE_FACTOR());
			scaleY=((float)newH)/((float)m_tSizeInPoints.cx * CC_CONTENT_SCALE_FACTOR());
		}
		else
		{
			scaleX=((float)newW)/((float)m_tSizeInPoints.cy * CC_CONTENT_SCALE_FACTOR());
			scaleY=((float)newH)/((float)m_tSizeInPoints.cx * CC_CONTENT_SCALE_FACTOR());
		}
/*
		if(scaleX<scaleY)
		{
			m_fScreenScaleFactor=scaleX*CC_CONTENT_SCALE_FACTOR();
		}
		else
		{
			m_fScreenScaleFactor=scaleY*CC_CONTENT_SCALE_FACTOR();
		}*/
		m_fScreenScaleFactorX=scaleX*CC_CONTENT_SCALE_FACTOR();
		m_fScreenScaleFactorY=scaleY*CC_CONTENT_SCALE_FACTOR();
#endif
		int viewPortW = (int)(m_tSizeInPoints.cx * m_fScreenScaleFactorX);
		int viewPortH = (int)(m_tSizeInPoints.cy * m_fScreenScaleFactorY);
		if (m_bOrientationReverted)
		{
			viewPortW = (int)(m_tSizeInPoints.cy * m_fScreenScaleFactorX);
			viewPortH = (int)(m_tSizeInPoints.cx * m_fScreenScaleFactorY);
		}


		// calculate new view port
		m_rcViewPort.left   = rcClient.left + (newW - viewPortW) / 2;
		m_rcViewPort.top    = rcClient.top + (newH - viewPortH) / 2;
		m_rcViewPort.right  = m_rcViewPort.left + viewPortW;
		m_rcViewPort.bottom = m_rcViewPort.top + viewPortH;

	}
	else
	{
		// calculate view port in pixels
		int viewPortW = (int)(m_tSizeInPoints.cx * m_fScreenScaleFactorX);
		int viewPortH = (int)(m_tSizeInPoints.cy * m_fScreenScaleFactorY);
		if (m_bOrientationReverted)
		{
			int tmp = viewPortW;
			viewPortW = viewPortH;
			viewPortH = tmp;
		}
		GetClientRect(m_hWnd, &rcClient);

    // calculate client new width and height
    int newW = rcClient.right - rcClient.left;
    int newH = rcClient.bottom - rcClient.top;

		// calculate new view port
		m_rcViewPort.left   = rcClient.left + (newW - viewPortW) / 2;
		m_rcViewPort.top    = rcClient.top + (newH - viewPortH) / 2;
		m_rcViewPort.right  = m_rcViewPort.left + viewPortW;
		m_rcViewPort.bottom = m_rcViewPort.top + viewPortH;
	}
}

void CCEGLView::centerWindow()
{
    if (! m_hWnd)
    {
        return;
    }

    RECT rcDesktop, rcWindow;
    GetWindowRect(GetDesktopWindow(), &rcDesktop);

    // substract the task bar
    HWND hTaskBar = FindWindow(TEXT("Shell_TrayWnd"), NULL);
    if (hTaskBar != NULL)
    {
        APPBARDATA abd;

        abd.cbSize = sizeof(APPBARDATA);
        abd.hWnd = hTaskBar;

        SHAppBarMessage(ABM_GETTASKBARPOS, &abd);
        SubtractRect(&rcDesktop, &rcDesktop, &abd.rc);
    }
    GetWindowRect(m_hWnd, &rcWindow);

    int offsetX = (rcDesktop.right - rcDesktop.left - (rcWindow.right - rcWindow.left)) / 2;
    offsetX = (offsetX > 0) ? offsetX : rcDesktop.left;
    int offsetY = (rcDesktop.bottom - rcDesktop.top - (rcWindow.bottom - rcWindow.top)) / 2;
    offsetY = (offsetY > 0) ? offsetY : rcDesktop.top;

    SetWindowPos(m_hWnd, 0, offsetX, offsetY, 0, 0, SWP_NOCOPYBITS | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER);
}

void CCEGLView::setScreenScale(float factorx,float factory)
{
    m_fScreenScaleFactorX = factorx;
	m_fScreenScaleFactorY = factory;
}

bool CCEGLView::canSetContentScaleFactor()
{
    return true;
}

void CCEGLView::setContentScaleFactor(float contentScaleFactor)
{
    m_fScreenScaleFactorX = contentScaleFactor;
	m_fScreenScaleFactorY = contentScaleFactor;
    if (m_bOrientationReverted)
    {
        resize((int)(m_tSizeInPoints.cy * contentScaleFactor), (int)(m_tSizeInPoints.cx * contentScaleFactor));
    }
    else
    {
        resize((int)(m_tSizeInPoints.cx * contentScaleFactor), (int)(m_tSizeInPoints.cy * contentScaleFactor));
    }
    centerWindow();
}

CCEGLView& CCEGLView::sharedOpenGLView()
{
    CC_ASSERT(s_pMainWindow);
    return *s_pMainWindow;
}

NS_CC_END;
