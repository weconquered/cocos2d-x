/****************************************************************************

Copyright (c) 2012 cocos2d-x.org

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

#include "CCSprite3D.h"
#include "CCDirector.h"
#include "3d_model_support/model.h"
#include "kazmath/GL/matrix.h"
#include "shaders/CCShaderCache.h"
#include "effects/CCGrid.h"

NS_CC_BEGIN

CCSprite3D::CCSprite3D()
: m_pModel(NULL)
, m_modelRotation(0.0f)
{
    kmVec3Fill(&m_modelPosition, 0, 0, 0);
    kmVec3Fill(&m_modelScale, 0, 0, 0);
}

CCSprite3D::~CCSprite3D()
{

}

CCSprite3D* CCSprite3D::create(CCModel* pModel)
{
    CCSprite3D* pRet = new CCSprite3D();
    if (pRet && pRet->init()) {
        pRet->m_pModel = pModel;
        pRet->autorelease();
    }
    else {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool CCSprite3D::init()
{
    CCGLProgram* program = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture);
    setShaderProgram(program);

    return true;
}

void CCSprite3D::setPosition(float x,float y,float z)
{
    kmVec3Fill(&m_modelPosition, x, y, z);
}
    
kmVec3 CCSprite3D::getPosition() const
{
    return m_modelPosition;
}

void CCSprite3D::setScale(float x,float y,float z)
{
    kmVec3Fill(&m_modelScale, x, y, z);
}

kmVec3 CCSprite3D::getScale() const
{
    return m_modelScale;
}

void CCSprite3D::setRotation(float angle)
{
    m_modelRotation = angle;
}
    
float CCSprite3D::getRotation() const
{
    return m_modelRotation;
}

void CCSprite3D::setVisible(bool visible)
{
    CCNode::setVisible(visible);
}

bool CCSprite3D::isVisible()
{
    return CCNode::isVisible();
}

void CCSprite3D::showBoundingBox(bool show)
{
    // TODO
}

void CCSprite3D::draw()
{
    m_pModel->render();
}

void CCSprite3D::transform(void)
{
    CCDirector::sharedDirector()->setDepthTest(true);

    CC_NODE_DRAW_SETUP();

    kmMat4 matrixP;
    kmMat4 matrixMV;
    kmMat4 matrixMVP;

    kmGLGetMatrix(KM_GL_PROJECTION, &matrixP );
    kmGLGetMatrix(KM_GL_MODELVIEW, &matrixMV );

    kmMat4 matPos;
    kmMat4Translation(&matPos, m_modelPosition.x, m_modelPosition.y, m_modelPosition.z);
    kmMat4 matScale;
    kmMat4Scaling(&matScale, m_modelScale.x, m_modelScale.y, m_modelScale.z);
    //     kmMat4 matRotation;
    //     kmMat4RotationX(&matRotation, m_modelScale.x, m_modelScale.y, m_modelScale.z);
    kmMat4 matOut;
    kmMat4Multiply(&matOut, &matPos, &matScale);

    kmMat4Multiply(&matrixMVP, &matrixP, &matrixMV);
    //kmMat4Multiply(&matrixMVP, &matrixMVP, &rotationAndMove);				// apply rotation and translation to the matrix
    kmMat4Multiply(&matrixMVP, &matrixMVP, &matOut);

    GLuint matrixId = glGetUniformLocation(m_pShaderProgram->getProgram(), kCCUniformMVPMatrix_s);
    m_pShaderProgram->setUniformLocationWithMatrix4fv(matrixId, matrixMVP.mat, 1);
}

void CCSprite3D::visit()
{
    // quick return if not visible. children won't be drawn.
    if (!m_bVisible)
    {
        return;
    }
    kmGLPushMatrix();

    if (m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->beforeDraw();
    }

    this->transform();

    CCNode* pNode = NULL;
    unsigned int i = 0;

    if(m_pChildren && m_pChildren->count() > 0)
    {
        sortAllChildren();
        // draw children zOrder < 0
        ccArray *arrayData = m_pChildren->data;
        for( ; i < arrayData->num; i++ )
        {
            pNode = (CCNode*) arrayData->arr[i];

            //if ( pNode && pNode->m_nZOrder < 0 ) 
            {
                pNode->visit();
            }
//             else
//             {
//                 break;
//             }
        }
        // self draw
        this->draw();

        for( ; i < arrayData->num; i++ )
        {
            pNode = (CCNode*) arrayData->arr[i];
            if (pNode)
            {
                pNode->visit();
            }
        }        
    }
    else
    {
        this->draw();
    }

    // reset for next frame
    m_uOrderOfArrival = 0;

    if (m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->afterDraw(this);
    }

    kmGLPopMatrix();
}

NS_CC_END
