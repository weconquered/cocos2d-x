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

#ifndef __3D_MODEL_MD2_H__
#define __3D_MODEL_MD2_H__

#include "platform/CCPlatformMacros.h"
#include "base_nodes/CCNode.h"
#include "textures/CCTexture2D.h"

NS_CC_BEGIN

#ifdef _MSC_VER
#pragma pack(push,1)
#define PACK_STRUCT
#else
#define PACK_STRUCT	__attribute__((packed))
#endif

/* MD2 files are stored in little-endian form.
 */

typedef struct
{
    int magicNum;       // magic number, should be 844121161
    int versionNum;     // version number, should be 8
    int skinWidth;      // texture width
    int skinHeight;     // texture height
    int frameSize;      // keyframe size in bytes
    int numSkins;       // the number of textures
    int numVertices;    // the number of vertices per frame
    int numTexcoords;   // the number of texture coordinates
    int numTriangles;   // the number of triangles
    // the number of special commands that can be used to optimize the MD2's
    // mesh into trianbles fans and strips for the renderer
    int numGLCommands;
    int numFrames;          // the number of frames
    int offsetSkins;        // offset in bytes of textures in file
    int offsetTexcoords;    // offset in bytes of texture coordinates in file
    int offsetTriangles;    // offset in bytes of triangles in file
    int offsetFrames;       // offset in bytes of frames in file
    int offsetGLCommands;   // offset in bytes of OpenGL commands in file
    int offsetEnd;          // file size from the header to the end of the file in bytes
} PACK_STRUCT MD2Header;

typedef struct
{
    unsigned char vertex[3];    // the value are compressed, should be recompress them when parsing
    unsigned char lightNormalIndex;
} PACK_STRUCT MD2Vert;

typedef struct
{
    short s;
    short t;
} PACK_STRUCT MD2Texcoord;

typedef struct
{
    char skin[64];
} PACK_STRUCT MD2Skin;

typedef struct
{
    unsigned short vertIndices[3];  // vertex indices
    unsigned short texIndices[3];   // texture coordinate indices
} PACK_STRUCT MD2Mesh;

typedef struct
{
    float   scale[3];      // scale factor
    float   translate[3];  // translation vector
    char    name[16];      // frame name
    MD2Vert verts[1];        // list of frame's vertices
} PACK_STRUCT MD2Frame;

#ifdef _MSC_VER
#pack(pop)
#endif

#undef PACK_STRUCT

class CC_DLL MD2Model : public CCNode
{
public:
    static MD2Model* create(const char *MD2FileName, const char *textureFileName);
    
    MD2Model();
    virtual ~MD2Model();
    
    void draw();
    
private:
    bool loadMD2File(const char *MD2FileName, const char *textureFileName);
    
private:
    MD2Header *m_pHeader;
    MD2Mesh *m_pMeshes;
    MD2Texcoord *m_pTexcoords;
    MD2Vert *m_pVerts;
    CCTexture2D *m_pTexture;
    
    int m_nCurrentFrame;
    int m_nNextFrame;
    
    // percent through current frame
    float m_fInterpol;
    
    float *m_pVertices;
    float *m_pUV;
};

NS_CC_END

#endif // __3D_MODEL_MD2_H__
