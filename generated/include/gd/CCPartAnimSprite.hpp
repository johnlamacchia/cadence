#pragma once

class CCSpritePart;
class SpriteDescription;
class cocos2d;

class CCPartAnimSprite : public cocos2d::CCSprite {
public:
    virtual void setScaleX(float scaleX);
    virtual void setScaleY(float scaleY);
    virtual void setScale(float scale);
    virtual void setOpacity(unsigned char opacity);
    virtual void setBlendFunc(cocos2d::ccBlendFunc blendFunc);
    virtual void setDisplayFrame(cocos2d::CCSpriteFrame* frame);
    virtual bool isFrameDisplayed(cocos2d::CCSpriteFrame* frame);
    virtual cocos2d::CCSpriteFrame* displayFrame();

    static CCPartAnimSprite* createWithAnimDesc(char const* definition, cocos2d::CCTexture2D* texture, bool useTexture);

    CCPartAnimSprite();
    ~CCPartAnimSprite();
    void changeTextureOfID(char const* key, char const* frame);
    int countParts();
    void dirtify();
    CCSpritePart* getSpriteForKey(char const* key);
    bool initWithAnimDesc(char const* definition, cocos2d::CCTexture2D* texture, bool useTexture);
    void setColor(cocos2d::ccColor3B color);
    void setFlipX(bool flipX);
    void setFlipY(bool flipY);
    void transformSprite(SpriteDescription* description);
    void tweenSpriteTo(SpriteDescription* description, float duration);
    void tweenToFrame(cocos2d::CCSpriteFrame* frame, float duration);

    cocos2d::CCDictionary* m_spritePartIDs;
    cocos2d::CCSpriteFrame* m_spriteFrame;
    bool m_hasChanged;
    cocos2d::CCSize m_unkSize;
    cocos2d::CCArray* m_spriteParts;

};
