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

#ifndef __CCARMATURE_DATAS_H__
#define __CCARMATURE_DATAS_H__

#include "ccTypes.h"
#include "CCAffineTransform.h"
#include "cocostudio/CCArmatureDefine.h"
#include "CCTweenFunction.h"

#include <vector>
#include <unordered_map>

namespace cocostudio {
    
/**
 * The base node include a lot of attributes.
 * @js NA
 * @lua NA
 */
struct BaseData
{
	/**
     * @js ctor
     */
    BaseData();
    
    /*
    * Calculate two BaseData's between value(to - from) and set to self
    *
    * @param  from   from BaseData
    * @param  to     to BaseData
    */
    void subtract(const BaseData& from, const BaseData& to, bool limit);

    void setColor(const cocos2d::Color4B &color);
    cocos2d::Color4B getColor();

    float x;					//! position x attribute
    float y;					//! position y attribute
    int zOrder;			//! zorder attribute, used to order the Bone's depth order

    /**
    * x y skewX skewY scaleX scaleY used to calculate transform matrix
    * skewX, skewY can have rotation effect
    * To get more matrix information, you can have a look at this pape : http://www.senocular.com/flash/tutorials/transformmatrix/
    */
    float skewX;
    float skewY;
    float scaleX;
    float scaleY;

    float tweenRotate;       //! SkewX, SkewY, and TweenRotate effect the rotation

    bool isUseColorInfo;    //! Whether or not this frame have the color changed Info
    int a, r, g, b;

};


/**
* DisplayType distinguish which type your display is.
*/
enum DisplayType
{
    CS_DISPLAY_SPRITE,                //! display is a single Sprite
    CS_DISPLAY_ARMATURE,         //! display is a Armature
    CS_DISPLAY_PARTICLE,            //! display is a CCParticle.

    CS_DISPLAY_MAX
};
/**
 *  @js NA
 *  @lua NA
 */
struct DisplayData
{
    static const std::string changeDisplayToTexture(const std::string& displayName);
    
	/**
     * @js ctor
     */
    DisplayData();

    DisplayType displayType;	//! mark which type your display is
    std::string displayName;
};


/**
 *  @js NA
 *  @lua NA
 */
struct SpriteDisplayData : public DisplayData
{
    SpriteDisplayData();
    
    BaseData skinData;
};

/**
 *  @js NA
 *  @lua NA
 */
struct ArmatureDisplayData  : public DisplayData
{
    ArmatureDisplayData();
};

/**
 *  @js NA
 *  @lua NA
 */
struct ParticleDisplayData : public DisplayData
{
    ParticleDisplayData();
};



/**
* BoneData used to init a Bone.
* BoneData keeps a DisplayData list, a Bone can have many display to change.
* The display information saved in the DisplayData
* @js NA
* @lua NA
*/
struct BoneData : public BaseData
{
    BoneData(void);
    
    void addDisplayData(const DisplayData& displayData);
    const DisplayData& getDisplayData(int index) const;

    std::string name;                //! the bone's name
    std::string parentName;     //! the bone parent's name
    std::vector<DisplayData> displayDataList;    //! save DisplayData informations for the Bone
    cocos2d::AffineTransform boneDataTransform;
};


/**
* ArmatureData saved the Armature name and Bonedata needed for the CCBones in this Armature
* When we create a Armature, we need to get each Bone's BoneData as it's init information.
* So we can get a BoneData from the Dictionary saved in the ArmatureData.
* @js NA
* @lua NA
*/
struct ArmatureData
{
    ArmatureData();
    
    void addBoneData(const BoneData& boneData);
    const BoneData& getBoneData(const std::string& boneName) const;

    std::string name;
    std::unordered_map<std::string, BoneData> boneDataDic;
    float dataVersion;
};

enum BlendType
{
    BLEND_NORMAL,
    BLEND_LAYER,
    BLEND_DARKEN,
    BLEND_MULTIPLY,
    BLEND_LIGHTEN,
    BLEND_SCREEN,
    BLEND_OVERLAY,
    BLEND_HARD_LIGHT,
    BLEND_ADD,
    BLEND_SUBSTRACT,
    BLEND_DIFFERENCE,
    BLEND_INVERT,
    BLEND_ALPHA,
    BLEND_ERASE
};

/**
 *  @js NA
 *  @lua NA
 */
struct FrameData : public BaseData
{
    FrameData();
    
    int frameID;
    int duration;                //! The frame will last duration frames

    cocos2d::tweenfunc::TweenType tweenEasing;     //! Every frame's tween easing effect
    int easingParamNumber;
    float *easingParams;

    bool isTween;                //! Whether it's a tween key frame

    /**
    * The current display index when change to this frame.
    * If value is -1, then display will not be shown.
    */
    int displayIndex;

    cocos2d::BlendFunc blendFunc;

    std::string strEvent;
    /**
    * strMovement, strEvent, strSound, strSoundEffect do not support yet
    */
    std::string strMovement;
    std::string strSound;
    std::string strSoundEffect;
};

/**
 *  @js NA
 *  @lua NA
 */
struct MovementBoneData
{
    MovementBoneData();

    bool init();
    
    void addFrameData(const FrameData& frameData);
    const FrameData& getFrameData(int index) const;

    float delay;             //! movement delay percent, this value can produce a delay effect
    float scale;             //! scale this movement
    float duration;        //! this Bone in this movement will last m_iDuration frames
    std::string name;    //! bone name

    std::vector<FrameData> frameList;
};

/**
 *  @js NA
 *  @lua NA
 */
struct MovementData
{
    MovementData();
    
    void addMovementBoneData(const MovementBoneData& movBoneData);
    const MovementBoneData& getMovementBoneData(const std::string& boneName) const;

    std::string name;
    int duration;        //! the frames this movement will last
    float scale;		  //! scale this movement

    /**
    * Change to this movement will last durationTo frames. Use this effect can avoid too suddenly changing.
    *
    * Example : current movement is "stand", we want to change to "run", then we fill durationTo frames before
    * change to "run" instead of changing to "run" directly.
    */
    int durationTo;

    /*
    * This is different from duration, durationTween contain tween effect.
    * duration is the raw time that the animation will last, it's the same with the time you edit in the Action Editor.
    * durationTween is the actual time you want this animation last.
    * Example : If we edit 10 frames in the flash, then duration is 10. When we set durationTween to 50, the movement will last 50 frames, the extra 40 frames will auto filled with tween effect
    */
    int durationTween;

    bool loop;           //! whether the movement was looped

    /**
    * Which tween easing effect the movement use
    * TWEEN_EASING_MAX : use the value from MovementData get from flash design panel
    */
    cocos2d::tweenfunc::TweenType tweenEasing;

    /**
    * @brief	save movment bone data
    * @key	const std::string& 
    * @value	MovementBoneData *
    */
    std::unordered_map<std::string, MovementBoneData> movBoneDataDic;
};


/**
*  AnimationData include all movement infomation for the Armature
*  The struct is AnimationData -> MovementData -> MovementBoneData -> FrameData
*                                              -> MovementFrameData
*  @js NA
*  @lua NA
*/
struct AnimationData
{
    void addMovement(const MovementData& movData);
    const MovementData& getMovement(const std::string& movementName) const;
    ssize_t getMovementCount() const;

    std::string name;
    std::unordered_map<std::string, MovementData> movementDataDic;
    std::vector<std::string> movementNames;
};



/*
* ContourData include a contour vertex information
* @js NA
* @lua NA
*/
struct ContourData
{
    void addVertex(const cocos2d::Point &vertex);

    std::vector<cocos2d::Point> vertexList;	//! Save contour vertex info, vertex saved in a Point
};




/*
* TextureData include a texture's information
* @js NA
* @lua NA
*/
struct TextureData
{
    TextureData();
    
    void addContourData(const ContourData& contourData);
    const ContourData& getContourData(int index) const;


    float height;		//! The texture's width, height
    float width;

    float pivotX;		//! The texture's anchor point
    float pivotY;

    std::string name;	//! The texture's name

    std::vector<ContourData> contourDataList;
};


}

#endif /*__CCARMATURE_DATAS_H__*/
