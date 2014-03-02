/****************************************************************************
Copyright (c) 2013-2014 Chukong Technologies Inc.

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

#include "cocostudio/CCDatas.h"
#include "cocostudio/CCUtilMath.h"
#include "cocostudio/CCTransformHelp.h"

using namespace cocos2d;

namespace cocostudio {


BaseData::BaseData()
    : x(0.0f)
    , y(0.0f)
    , zOrder(0)

    , skewX(0.0f)
    , skewY(0.0f)
    , scaleX(1.0f)
    , scaleY(1.0f)

    , tweenRotate(0.0f)

    , isUseColorInfo(false)
    , a(255)
    , r(255)
    , g(255)
    , b(255)
{
}
    
void BaseData::subtract(const BaseData& from, const BaseData& to, bool limit)
{
    x = to.x - from.x;
    y = to.y - from.y;
    scaleX = to.scaleX - from.scaleX;
    scaleY = to.scaleY - from.scaleY;
    skewX = to.skewX - from.skewX;
    skewY = to.skewY - from.skewY;

    if(isUseColorInfo || from.isUseColorInfo || to.isUseColorInfo)
    {
        a = to.a - from.a;
        r = to.r - from.r;
        g = to.g - from.g;
        b = to.b - from.b;

        isUseColorInfo = true;
    }
    else
    {
        a = r = g = b = 0;
        isUseColorInfo = false;
    }

	if (limit)
	{
        if (skewX > M_PI)
        {
            skewX -= (float)CC_DOUBLE_PI;
        }
        if (skewX < -M_PI)
        {
            skewX += (float)CC_DOUBLE_PI;
        }

        if (skewY > M_PI)
        {
            skewY -= (float)CC_DOUBLE_PI;
        }
        if (skewY < -M_PI)
        {
            skewY += (float)CC_DOUBLE_PI;
        }
    }

    if (to.tweenRotate)
    {
        skewX += to.tweenRotate * M_PI * 2;
        skewY -= to.tweenRotate * M_PI * 2;
    }
	
}

void BaseData::setColor(const Color4B &color)
{
    r = color.r;
    g = color.g;
    b = color.b;
    a = color.a;
}

Color4B BaseData::getColor()
{
    return Color4B(r, g, b, a);
}

//
    
const std::string DisplayData::changeDisplayToTexture(const std::string& displayName)
{
    // remove .xxx
    std::string textureName = displayName;
    size_t startPos = textureName.find_last_of(".");

    if(startPos != std::string::npos)
    {
        textureName = textureName.erase(startPos);
    }

    return textureName;
}

DisplayData::DisplayData(void)
    : displayType(CS_DISPLAY_MAX)
    , displayName("")
{
}

SpriteDisplayData::SpriteDisplayData()
{
    displayType = CS_DISPLAY_SPRITE;
}


ArmatureDisplayData::ArmatureDisplayData(void)
{
    displayType = CS_DISPLAY_ARMATURE;
}

ParticleDisplayData::ParticleDisplayData(void)
{
    displayType = CS_DISPLAY_PARTICLE;
}

BoneData::BoneData(void)
    : name("")
    , parentName("")
{
}


void BoneData::addDisplayData(const DisplayData& displayData)
{
    displayDataList.push_back(displayData);
}

const DisplayData& BoneData::getDisplayData(int index) const
{
    return displayDataList.at(index);
}


ArmatureData::ArmatureData()
    : dataVersion(0.1f)
{
}

void ArmatureData::addBoneData(const BoneData& boneData)
{
    boneDataDic[boneData.name] = boneData;
}

const BoneData& ArmatureData::getBoneData(const std::string& boneName) const
{
    return boneDataDic.at(boneName);
}

FrameData::FrameData(void)
    : frameID(0)
    , duration(1)
    , tweenEasing(cocos2d::tweenfunc::Linear)
    , easingParamNumber(0)
    , easingParams(NULL)
    , isTween(true)
    , displayIndex(0)
    , blendFunc(BlendFunc::ALPHA_NON_PREMULTIPLIED)

    , strEvent("")
    , strMovement("")
    , strSound("")
    , strSoundEffect("")
{
}

MovementBoneData::MovementBoneData()
    : delay(0.0f)
    , scale(1.0f)
    , duration(0)
    , name("")
{
}

void MovementBoneData::addFrameData(const FrameData& frameData)
{
    frameList.push_back(frameData);
}

const FrameData& MovementBoneData::getFrameData(int index) const
{
    return frameList.at(index);
}

MovementData::MovementData(void)
    : name("")
    , duration(0)
    , scale(1.0f)
    , durationTo(0)
    , durationTween(0)
    , loop(true)
    , tweenEasing(cocos2d::tweenfunc::Linear)
{
}

void MovementData::addMovementBoneData(const MovementBoneData& movBoneData)
{
    movBoneDataDic[movBoneData.name] = movBoneData;
}

const MovementBoneData& MovementData::getMovementBoneData(const std::string& boneName) const
{
    return movBoneDataDic.at(boneName);
}


void AnimationData::addMovement(const MovementData& movData)
{
    movementDataDic[movData.name] = movData;
    movementNames.push_back(movData.name);
}

const MovementData& AnimationData::getMovement(const std::string& movementName) const
{
    return movementDataDic.at(movementName);
}

ssize_t AnimationData::getMovementCount() const
{
    return movementDataDic.size();
}


void ContourData::addVertex(const Point &vertex)
{
    vertexList.push_back(vertex);
}

TextureData::TextureData()
    : height(0.0f)
    , width(0.0f)
    , pivotX(0.5f)
    , pivotY(0.5f)
    , name("")
{
}

void TextureData::addContourData(const ContourData& contourData)
{
    contourDataList.push_back(contourData);
}

const ContourData& TextureData::getContourData(int index) const
{
    return contourDataList.at(index);
}


}
