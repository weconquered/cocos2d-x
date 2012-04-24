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
#include "CCSet.h"
#include "CCDirector.h"
#include "CCTouch.h"
#include "CCTouchDispatcher.h"
#include "CCAccelerometer_bada.h"
#include "CCIMEDispatcher.h"
#include <FText.h>
#include <string>

using namespace std;
using namespace Osp::Base;
using namespace Osp::Base::Runtime;
using namespace Osp::Base::Collection;
using namespace Osp::Text;
using namespace Osp::System;
using namespace Osp::App;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Graphics;

NS_CC_BEGIN

static CCEGLView * s_pMainWindow = NULL;
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
    }

    static CCEGL * create(CCEGLView * pWindow)
    {
        CCEGL * pEGL = new CCEGL;
        bool bSuccess = false;
        do 
        {
            CC_BREAK_IF(! pEGL);

            pEGL->m_eglNativeWindow = pWindow;

            EGLDisplay eglDisplay;
            CC_BREAK_IF(EGL_NO_DISPLAY == (eglDisplay = eglGetDisplay(pEGL->m_eglNativeDisplay)));

            CC_BREAK_IF(EGL_FALSE == eglInitialize(eglDisplay, null, null));

        	EGLint eglConfigList[] = {
        		EGL_RED_SIZE,	5,
        		EGL_GREEN_SIZE,	6,
        		EGL_BLUE_SIZE,	5,
        		EGL_ALPHA_SIZE,	0,
        		EGL_DEPTH_SIZE, 8,
        		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        		EGL_NONE
        	};
        	EGLint eglContextList[] = {
        		EGL_CONTEXT_CLIENT_VERSION, 2,
        		EGL_NONE
        	};

        	eglBindAPI(EGL_OPENGL_ES_API);
            ///

            EGLint iConfigs;
            EGLConfig eglConfig;
            CC_BREAK_IF(EGL_FALSE == eglChooseConfig(eglDisplay, eglConfigList, &eglConfig, 1, &iConfigs)
                || (iConfigs != 1));

            EGLContext eglContext;
            eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, eglContextList);
            CC_BREAK_IF(EGL_NO_CONTEXT == eglContext);

            EGLSurface eglSurface;
            eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, pEGL->m_eglNativeWindow, NULL);
            CC_BREAK_IF(EGL_NO_SURFACE == eglSurface);

            CC_BREAK_IF(EGL_FALSE == eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext));

            pEGL->m_eglDisplay = eglDisplay;
            pEGL->m_eglConfig  = eglConfig;
            pEGL->m_eglContext = eglContext;
            pEGL->m_eglSurface = eglSurface;
            bSuccess = true;

        } while (0);

        if (! bSuccess)
        {
            CC_SAFE_DELETE(pEGL);
        }

        return pEGL;
    }

    void resizeSurface()
    {
//          if (! m_eglNativeWindow || EGL_NO_DISPLAY == m_eglDisplay)
//          {
//              return;
//          }
//  
//          // release old surface
//          if (EGL_NO_SURFACE != m_eglSurface)
//          {
//              eglDestroySurface(m_eglDisplay, m_eglSurface);
//              m_eglSurface = EGL_NO_SURFACE;
//          }
//  
//          // create new surface and make current
//          m_eglSurface = eglCreateWindowSurface(m_eglDisplay, m_eglConfig, m_eglNativeWindow, NULL);
//          eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);
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

CCEGLView::CCEGLView()
: m_bHVGADevice(false)
, m_pEGL(NULL)
, m_pKeypad(null)
{

}

CCEGLView::~CCEGLView()
{
    end();
    CC_SAFE_DELETE(m_pEGL);
    CC_SAFE_DELETE(m_pKeypad);
}

bool CCEGLView::isOpenGLReady()
{
    return (NULL != m_pEGL);
}

void CCEGLView::end()
{
    Application::GetInstance()->Terminate();
}

void CCEGLView::swapBuffers()
{
    if (m_pEGL)
    {
        m_pEGL->swapBuffers();
    }
}

void CCEGLView::setIMEKeyboardState(bool bOpen)
{
    if (bOpen)
    {
        const char * pszText = CCIMEDispatcher::sharedDispatcher()->getContentText();
        m_pKeypad->SetText(pszText);
        m_pKeypad->SetShowState(bOpen);
        m_pKeypad->Show();
    }
}

bool CCEGLView::Create(Osp::App::Application* pApp, Orientation eOrientation)
{
    Construct(FORM_STYLE_NORMAL);
    Frame *pFrame = Application::GetInstance()->GetAppFrame()->GetFrame();

    pFrame->AddControl(*this);
    pFrame->SetCurrentForm(*this);

    m_pEGL = CCEGL::create(this);
    if (m_pEGL == null)
    {
        return false;
    }

    s_pMainWindow = this;
    Draw();
    Show();

    SetOrientation(eOrientation);

    float width = 0.0f;
    float height = 0.0f;

    if (eOrientation == ORIENTATION_PORTRAIT || eOrientation == ORIENTATION_PORTRAIT_REVERSE)
    {
        width = MIN(m_sSizeInPixel.width, m_sSizeInPixel.height);
        height = MAX(m_sSizeInPixel.width, m_sSizeInPixel.height);
    }
    else
    {
        width = MAX(m_sSizeInPixel.width, m_sSizeInPixel.height);
        height = MIN(m_sSizeInPixel.width, m_sSizeInPixel.height);
    }

    setFrameSize(width, height);

    return true;
}

result CCEGLView::OnInitializing(void)
{
    result r = E_SUCCESS;

    AddTouchEventListener(*this);
    Touch touch;
    touch.SetMultipointEnabled(*this, true);

    m_pKeypad = new Keypad();
    m_pKeypad->Construct(KEYPAD_STYLE_NORMAL, KEYPAD_MODE_ALPHA);
    m_pKeypad->AddTextEventListener(*this);

    Rectangle rc = GetBounds();
    if ((rc.width == 480 && rc.height == 720)
            || (rc.width == 720 && rc.height == 480))
    {
    	m_bHVGADevice = true;
    	setFrameSize(rc.width / 1.5f, rc.height / 1.5f);
    }
    else
    {
    	m_bHVGADevice = false;
    	setFrameSize(rc.width, rc.height);
    }

    return r;
}

result CCEGLView::OnTerminating(void)
{
    result r = E_SUCCESS;

    // TODO: Add your termination code here

    return r;
}

// touch event
void CCEGLView::OnTouchIndicated(const Control& source,
    const Point& startPosition, const Point& currentPoint)
{

}

void CCEGLView::OnTouchPressed(const Control& source,
    const Point& currentPosition, const TouchEventInfo & touchInfo)
{
    int id = (int)touchInfo.GetPointId();
    float x = (float)touchInfo.GetCurrentPosition().x;
    float y = (float)touchInfo.GetCurrentPosition().y;
    CCLOG("OnTouchPressed id = %d,x = %f,y = %f, count = %d", id, x, y, s_mapTouches.GetCount());
    if (m_bHVGADevice)
    {
        x = x * 2 / 3;
        y = y * 2 / 3;
    }

    handleTouchesBegin(1, &id, &x, &y);
}

void CCEGLView::OnTouchLongPressed(const Control& source,
    const Point& currentPosition, const TouchEventInfo & touchInfo)
{

}


void CCEGLView::OnTouchReleased(const Control& source,
    const Point& currentPosition, const TouchEventInfo & touchInfo)
{
    int id = (int)touchInfo.GetPointId();
    float x = (float)touchInfo.GetCurrentPosition().x;
    float y = (float)touchInfo.GetCurrentPosition().y;
    if (m_bHVGADevice)
    {
        x = x * 2 / 3;
        y = y * 2 / 3;
    }

    handleTouchesEnd(1, &id, &x, &y);
    CCLOG("OnTouchReleased id = %d,x = %f,y = %f", id, x, y);
}


void CCEGLView::OnTouchMoved(const Control& source,
    const Point& currentPosition, const TouchEventInfo & touchInfo)
{
    IList *pList = null;
    do
    {
        CC_BREAK_IF(!m_pDelegate);

        int i = 0;
        Touch touch;
        pList = touch.GetTouchInfoListN(source);

        CC_BREAK_IF(pList == NULL);

        int pointerNumber = pList->GetCount();

        CC_BREAK_IF(pointerNumber <= 0);

        int id[30] = {0};
        float x[30] = {0};
        float y[30] = {0};

        for (i = 0; i < pointerNumber; i++)
        {
            TouchInfo *pTouchInfo = static_cast<TouchInfo *>(pList->GetAt(i));
            id[i] = (int)pTouchInfo->id;
            float tmpX = (float)pTouchInfo->position.x;
            float tmpY = (float)pTouchInfo->position.y;
            if (m_bHVGADevice)
            {
                x[i] = tmpX * 2 / 3;
                y[i] = tmpY * 2 / 3;
            }
            else
            {
                x[i] = tmpX;
                y[i] = tmpY;
            }
        }

        handleTouchesMove(pointerNumber, id, x, y);

    } while (0);

    if (pList != null)
    {
        pList->RemoveAll(true);
        delete pList;
    }
}


void CCEGLView::OnTouchDoublePressed(const Control& source,
    const Point& currentPosition, const TouchEventInfo & touchInfo)
{
    CCLOG("double pressed..");
    OnTouchPressed(source, currentPosition, touchInfo);
}


void CCEGLView::OnTouchFocusIn(const Control& source,
    const Point& currentPosition, const TouchEventInfo & touchInfo)
{

}


void CCEGLView::OnTouchFocusOut(const Control& source,
    const Point& currentPosition, const TouchEventInfo & touchInfo)
{

}

result CCEGLView::OnDraw(void)
{
    result r = E_SUCCESS;
    CCDirector * pDirector = CCDirector::sharedDirector();
    pDirector->drawScene();
    return r;
}

// Implement an ITextEventListener
void CCEGLView::OnTextValueChanged(const Control& source)
{
    // clear textinput text
    std::string strOldText = CCIMEDispatcher::sharedDispatcher()->getContentText();

    for (int i = 0; i < strOldText.length(); i++)
    {
        CCIMEDispatcher::sharedDispatcher()->dispatchDeleteBackward();
    }

    String str = m_pKeypad->GetText();
    if (str.GetLength() <= 0)
    {
        CCIMEDispatcher::sharedDispatcher()->dispatchInsertText("\n", 1);
    }
    else
    {
        Utf8Encoding utf8;
        ByteBuffer* pBuffer = utf8.GetBytesN(str);
        if (pBuffer != null)
        {
            const char* pszText = (const char*)pBuffer->GetPointer();
            if (pszText != NULL)
            {
                int len = strlen(pszText);
                if (pszText[len-1] != '\n')
                {
                    std::string strText = pszText;
                    strText.append("\n");
                    CCIMEDispatcher::sharedDispatcher()->dispatchInsertText(strText.c_str(), strText.length());
                }
            }
            delete pBuffer;
        }
    }
}

void CCEGLView::OnTextValueChangeCanceled(const Control& source)
{
    CCIMEDispatcher::sharedDispatcher()->dispatchInsertText("\n", 1);
}

CCEGLView& CCEGLView::sharedOpenGLView()
{
    CC_ASSERT(s_pMainWindow);
    return *s_pMainWindow;
}

NS_CC_END
