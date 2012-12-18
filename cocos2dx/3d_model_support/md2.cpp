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

#include "md2.h"
#include "platform/CCFileUtils.h"
#include "textures/CCTextureCache.h"
#include "shaders/CCShaderCache.h"

#define MD2_MAGIC_NUM   844121161
#define MD2_VERSION     8

NS_CC_BEGIN

MD2Model* MD2Model::create(const char *MD2FileName, const char *textureFileName)
{
    MD2Model *model = new MD2Model();
    
    if (model && model->loadMD2File(MD2FileName, textureFileName))
    {
        model->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(model);
    }
    
    return model;
}

MD2Model::MD2Model()
: m_pHeader(NULL)
, m_pMeshes(NULL)
, m_pTexcoords(NULL)
, m_pVerts(NULL)
, m_pTexture(NULL)
, m_nCurrentFrame(0)
, m_pUV(NULL)
, m_pVertices(NULL)
, m_nNextFrame(1)
{
    CCGLProgram* program = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture);
    setShaderProgram(program);
}

MD2Model::~MD2Model()
{
    CC_SAFE_DELETE(m_pHeader);
    CC_SAFE_DELETE_ARRAY(m_pMeshes);
    CC_SAFE_DELETE_ARRAY(m_pTexcoords);
    CC_SAFE_DELETE_ARRAY(m_pVerts);
    CC_SAFE_RELEASE(m_pTexture);
}

bool MD2Model::loadMD2File(const char *MD2FileName, const char *textureFileName)
{
    bool result = false;
    unsigned char *buffer = NULL;

    do
    {
        unsigned long fileSize;
        buffer = CCFileUtils::sharedFileUtils()->getFileData(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(MD2FileName), "rb", &fileSize);
        if (! buffer)
        {
            CCLOG("can not read file data from %s", MD2FileName);
            break;
        }
        
        // get header data
        m_pHeader = new MD2Header();
        if (! m_pHeader)
        {
            CCLOG("can not allocate memory for header");
            break;
        }
        *m_pHeader = *(MD2Header*)buffer;
        
        // check header
        if (m_pHeader->magicNum != MD2_MAGIC_NUM
            || m_pHeader->versionNum != MD2_VERSION)
        {
            CCLOG("%s is not a valid MD2 file", MD2FileName);
            break;
        }
        
        // load texture
        m_pTexture = CCTextureCache::sharedTextureCache()->addImage(textureFileName);
        if (! m_pTexture)
        {
            CCLOG("can not load texture %s", textureFileName);
            break;
        }
        m_pTexture->retain();
        
        // alloc memories
        m_pVerts = new MD2Vert[m_pHeader->numVertices * m_pHeader->numFrames];
        m_pTexcoords = new MD2Texcoord[m_pHeader->numTexcoords];
        m_pMeshes = new MD2Mesh[m_pHeader->numTriangles];
        if (!m_pVerts || !m_pTexcoords || !m_pMeshes)
        {
            CCLOG("can not allocate enough memories");
            break;
        }
        
        // get vertices
        MD2Frame *frame = NULL;
        MD2Vert *tmpVert = NULL;
        for (int i = 0; i < m_pHeader->numFrames; ++i)
        {
            frame = (MD2Frame*)&buffer[m_pHeader->offsetFrames + m_pHeader->frameSize*i];
            tmpVert = (MD2Vert*)&m_pVerts[m_pHeader->numVertices * i];
            
            for (int j = 0; j < m_pHeader->numVertices; ++j)
            {
                //swap y and z coords to convert to the proper orientation on screen
                tmpVert[j].vertex[0] = frame->scale[0] * frame->verts[j].vertex[0] + frame->translate[0];
                tmpVert[j].vertex[2] = frame->scale[1] * frame->verts[j].vertex[1] + frame->translate[1];
                tmpVert[j].vertex[1] = frame->scale[2] * frame->verts[j].vertex[2] + frame->translate[2];
                
                tmpVert[j].lightNormalIndex = frame->verts[j].lightNormalIndex;
            }
        }

        // get texture coordinates
        
        CCSize winSize = m_pTexture->getContentSize();
        float texWidth = winSize.width;
        float texHeight = winSize.height;
        
        MD2Texcoord *tmpTexPtr = (MD2Texcoord*)&buffer[m_pHeader->offsetTexcoords];
        for (int i = 0; i < m_pHeader->numTexcoords; ++i)
        {
            m_pTexcoords[i].s = (float)tmpTexPtr[i].s / texWidth;
            m_pTexcoords[i].t = (float)tmpTexPtr[i].t / texHeight;
        }
        
        // get mesh data
        
        MD2Mesh *tmpMeshPtr = (MD2Mesh*)&buffer[m_pHeader->offsetTriangles];
        memcpy(m_pMeshes, tmpMeshPtr, sizeof(MD2Mesh) * m_pHeader->numTriangles);
        
        result = true;
    } while(0);
    
    m_pUV = new float[m_pHeader->numTriangles * 6];
    m_pVertices = new float[m_pHeader->numTriangles * 9];
    
    // release memory
    CC_SAFE_DELETE_ARRAY(buffer);
    
    return result;
}

void MD2Model::draw()
{
    ccGLBindTexture2D(m_pTexture->getName());
    
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords );
    
    int uvIdx = 0;
    int vertexIdx = 0;
    for (int i =0; i < m_pHeader->numTriangles; i++)
    {
        m_pVertices[vertexIdx++] = m_pVerts[m_pMeshes[i].vertIndices[0]].vertex[0];
        m_pVertices[vertexIdx++] = m_pVerts[m_pMeshes[i].vertIndices[0]].vertex[1];
        m_pVertices[vertexIdx++] = m_pVerts[m_pMeshes[i].vertIndices[0]].vertex[2];
        
        m_pVertices[vertexIdx++] = m_pVerts[m_pMeshes[i].vertIndices[1]].vertex[0];
        m_pVertices[vertexIdx++] = m_pVerts[m_pMeshes[i].vertIndices[1]].vertex[1];
        m_pVertices[vertexIdx++] = m_pVerts[m_pMeshes[i].vertIndices[1]].vertex[2];
        
        m_pVertices[vertexIdx++] = m_pVerts[m_pMeshes[i].vertIndices[2]].vertex[0];
        m_pVertices[vertexIdx++] = m_pVerts[m_pMeshes[i].vertIndices[2]].vertex[1];
        m_pVertices[vertexIdx++] = m_pVerts[m_pMeshes[i].vertIndices[2]].vertex[2];
        
        m_pUV[uvIdx++] = m_pTexcoords[m_pMeshes[i].texIndices[0]].s;
        m_pUV[uvIdx++] = m_pTexcoords[m_pMeshes[i].texIndices[0]].t;
        
        m_pUV[uvIdx++] = m_pTexcoords[m_pMeshes[i].texIndices[1]].s;
        m_pUV[uvIdx++] = m_pTexcoords[m_pMeshes[i].texIndices[1]].t;
        
        m_pUV[uvIdx++] = m_pTexcoords[m_pMeshes[i].texIndices[2]].s;
        m_pUV[uvIdx++] = m_pTexcoords[m_pMeshes[i].texIndices[2]].t;
    }
    
    glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, m_pVertices);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, m_pUV);
    
    glDrawArrays(GL_TRIANGLES, 0, m_pHeader->numTriangles * 3);
    
    CHECK_GL_ERROR_DEBUG();
}

NS_CC_END
