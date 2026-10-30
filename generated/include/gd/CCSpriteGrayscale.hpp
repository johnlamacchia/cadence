#pragma once

class cocos2d;

class CCSpriteGrayscale : public CCSpriteWithHue {
public:
    virtual ~CCSpriteGrayscale();
    virtual std::string getShaderName();
    virtual const char* shaderBody();

    static CCSpriteGrayscale* create(std::string const& file);
    static CCSpriteGrayscale* create(std::string const& file, cocos2d::CCRect const& rect);
    static CCSpriteGrayscale* createWithSpriteFrame(cocos2d::CCSpriteFrame* frame);
    static CCSpriteGrayscale* createWithSpriteFrameName(std::string const& frameName);
    static CCSpriteGrayscale* createWithTexture(cocos2d::CCTexture2D* texture);
    static CCSpriteGrayscale* createWithTexture(cocos2d::CCTexture2D* texture, cocos2d::CCRect const& rect, bool rotated);

    CCSpriteGrayscale();

};
