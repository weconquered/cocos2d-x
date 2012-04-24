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

#ifndef __CCX_EGLVIEW_BADA_H__
#define __CCX_EGLVIEW_BADA_H__

#include <FApp.h>
#include <FBase.h>
#include <FUi.h>
#include <FSystem.h>
#include "CCGeometry.h"
#include "CCGL.h"
#include "CCEGLViewProtocol.h"

NS_CC_BEGIN

class CCEGL;

class CC_DLL CCEGLView
    : public Osp::Ui::Controls::Form
    , public CCEGLViewProtocol
    , public Osp::Ui::ITouchEventListener
    , public Osp::Ui::ITextEventListener
{
public:
    CCEGLView();
    virtual ~CCEGLView();

    /* override functions */
    bool    isOpenGLReady();
    void    end();
    void    swapBuffers();
    void    setIMEKeyboardState(bool bOpen);

    // static function
    /**
    @brief    get the shared main open gl window
    */
    static CCEGLView& sharedOpenGLView();
    
    bool Create(Osp::App::Application* pApp, Osp::Ui::Orientation eOrientation);

    virtual result OnInitializing(void);
    virtual result OnTerminating(void);

    // touch event
    virtual void OnTouchIndicated(const Osp::Ui::Control& source,
        const Osp::Graphics::Point& startPosition, const Osp::Graphics::Point& currentPoint);

    virtual void OnTouchPressed(const Osp::Ui::Control& source,
        const Osp::Graphics::Point& currentPosition, const Osp::Ui::TouchEventInfo & touchInfo);

    virtual void OnTouchLongPressed(const Osp::Ui::Control& source,
        const Osp::Graphics::Point& currentPosition, const Osp::Ui::TouchEventInfo & touchInfo);

    virtual void OnTouchReleased(const Osp::Ui::Control& source,
        const Osp::Graphics::Point& currentPosition, const Osp::Ui::TouchEventInfo & touchInfo);

    virtual void OnTouchMoved(const Osp::Ui::Control& source,
        const Osp::Graphics::Point& currentPosition, const Osp::Ui::TouchEventInfo & touchInfo);

    virtual void OnTouchDoublePressed(const Osp::Ui::Control& source,
        const Osp::Graphics::Point& currentPosition, const Osp::Ui::TouchEventInfo & touchInfo);

    virtual void OnTouchFocusIn(const Osp::Ui::Control& source,
        const Osp::Graphics::Point& currentPosition, const Osp::Ui::TouchEventInfo & touchInfo);

    virtual void OnTouchFocusOut(const Osp::Ui::Control& source,
        const Osp::Graphics::Point& currentPosition, const Osp::Ui::TouchEventInfo & touchInfo);

    virtual result OnDraw(void);

    virtual void OnTextValueChanged(const Osp::Ui::Control& source);
    virtual void OnTextValueChangeCanceled(const Osp::Ui::Control& source);

private:
    bool m_bHVGADevice;
    CCEGL* m_pEGL;
    Osp::Ui::Controls::Keypad* m_pKeypad;
};

NS_CC_END

#endif    // end of __CCX_EGLVIEW_WIN32_H__
