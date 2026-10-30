#pragma once

class cocos2d;

class ButtonSprite : public cocos2d::CCSprite {
public:
    virtual ~ButtonSprite();

    static ButtonSprite* create(char const* caption);
    static ButtonSprite* create(cocos2d::CCSprite* topSprite);
    static ButtonSprite* create(char const* caption, float scale);
    static ButtonSprite* create(char const* caption, const char* font, const char* texture);
    static ButtonSprite* create(char const* caption, const char* font, const char* texture, float scale);
    static ButtonSprite* create(char const* caption, int width, int minWidth, float scale, bool absolute);
    static ButtonSprite* create(cocos2d::CCSprite* topSprite, int width, bool absolute, float height, const char* texture, float scale);
    static ButtonSprite* create(cocos2d::CCSprite* topSprite, int width, int minWidth, float height, float scale, bool absolute);
    static ButtonSprite* create(char const* caption, int width, int minWidth, float scale, bool absolute, char const* font, char const* bg);
    static ButtonSprite* create(const char* caption, int width, bool absolute, const char* font, const char* texture, float height, float scale);
    static ButtonSprite* create(char const* caption, int width, int minWidth, float scale, bool absolute, char const* font, char const* bg, float height);
    static ButtonSprite* create(cocos2d::CCSprite* topSprite, int width, int minWidth, float height, float scale, bool absolute, char const* bgSprite, bool noScaleSpriteForBG, float unk);

    ButtonSprite();
    bool init(char const* caption, int width, int minWidth, float scale, bool absolute, char const* font, char const* bg, float height);
    bool init(cocos2d::CCSprite* topSprite, int width, int minWidth, float height, float scale, bool absolute, char const* bgSprite, bool noScaleSpriteForBG, float unk);
    void setColor(cocos2d::ccColor3B color);
    void setString(char const* text);
    void updateBGImage(char const* filename);
    void updateSpriteBGSize();
    void updateSpriteOffset(cocos2d::CCPoint offset);

    int m_mode;
    float m_width;
    float m_minWidth;
    float m_scale;
    float m_height;
    float m_unk26c;
    bool m_absolute;
    cocos2d::CCLabelBMFont* m_label;
    cocos2d::CCSprite* m_subSprite;
    cocos2d::CCSprite* m_subBGSprite;
    cocos2d::extension::CCScale9Sprite* m_BGSprite;
    cocos2d::CCPoint m_textOffset;
    cocos2d::CCPoint m_spritePosition;
    std::string m_caption;

};
