#pragma once

class MultilineBitmapFont;
class TextAreaDelegate;
class cocos2d;

class TextArea : public cocos2d::CCSprite {
public:
    virtual ~TextArea();
    virtual void update(float dt);
    virtual void draw();
    virtual void setOpacity(unsigned char opacity);

    static TextArea* create(std::string str, char const* font, float scale, float width, cocos2d::CCPoint anchor, float lineHeight, bool disableColor, int maxEmoji);

    void colorAllCharactersTo(cocos2d::ccColor3B color);
    void colorAllLabels(cocos2d::ccColor3B color);
    float fadeIn(float duration, bool unused);
    float fadeInCharacters(float duration, float charDuration, bool fadeOut, TextFadeInStyle style);
    void fadeOut(float duration);
    void fadeOutAndRemove();
    void finishFade();
    void /* TodoReturn */ fitString(std::string, float, float, float, float);
    void hideAll();
    bool init(std::string str, char const* font, float scale, float width, cocos2d::CCPoint anchor, float lineHeight, bool disableColor, int);
    void setEmoteMaxAspect(float);
    void setIgnoreColorCode(bool ignore);
    void setParseEmotes(int);
    void setString(std::string text);
    void showAll();
    void stopAllCharacterActions();

    bool m_disableColor;
    bool m_unk259;
    int m_unk25c;
    float m_unk260;
    MultilineBitmapFont* m_label;
    float m_width;
    float m_unknown;
    std::string m_fontFile;
    float m_height;
    bool m_onTimerUnused;
    cocos2d::CCPoint m_anchorPoint;
    bool m_allShown;
    float m_scale;
    int m_rectHeight;
    int m_rectWidth;
    float m_maxWidth;
    cocos2d::CCPoint m_textPos;
    TextAreaDelegate* m_delegate;
    cocos2d::CCDictionary* m_shakeCharacters;
    float m_shakeElapsed;

};
