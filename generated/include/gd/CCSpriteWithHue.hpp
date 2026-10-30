#pragma once

class cocos2d;

class CCSpriteWithHue : public cocos2d::CCSprite {
public:
    virtual ~CCSpriteWithHue();
    virtual void draw();
    virtual bool initWithTexture(cocos2d::CCTexture2D* texture);
    virtual bool initWithTexture(cocos2d::CCTexture2D* texture, cocos2d::CCRect const& rect);
    virtual bool initWithTexture(cocos2d::CCTexture2D* texture, cocos2d::CCRect const& rect, bool rotated);
    virtual bool initWithSpriteFrame(cocos2d::CCSpriteFrame* frame);
    virtual std::string getShaderName();
    virtual const char* shaderBody();
    virtual void updateColor();

    static CCSpriteWithHue* create(std::string const& file);
    static CCSpriteWithHue* create(std::string const& file, cocos2d::CCRect const& rect);
    static CCSpriteWithHue* createWithSpriteFrame(cocos2d::CCSpriteFrame* frame);
    static CCSpriteWithHue* createWithSpriteFrameName(std::string const& frameName);
    static CCSpriteWithHue* createWithTexture(cocos2d::CCTexture2D* texture);
    static CCSpriteWithHue* createWithTexture(cocos2d::CCTexture2D* texture, cocos2d::CCRect const& rect, bool rotated);

    CCSpriteWithHue();
    float getAlpha();
    float getHue();
    void getUniformLocations();
    void initShader();
    void setCustomLuminance(float r, float g, float b);
    void setEvenLuminance(float luminance);
    void setHue(float hue);
    void setHueDegrees(float degrees);
    void setLuminance(float luminance);
    void setupDefaultSettings();
    void updateColorMatrix();
    void updateHue(float unused);

    float m_hue;
    std::array<float, 3> m_luminance;
    std::array<std::array<float, 3>, 3> m_colorMatrix;
    int m_uHueLoc;
    int m_uAlphaLoc;
    int m_uLumLoc;

};
