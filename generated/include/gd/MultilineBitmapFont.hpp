#pragma once

class FontObject;
class cocos2d;

class MultilineBitmapFont : public cocos2d::CCSprite {
public:
    virtual void setOpacity(unsigned char opacity);

    static MultilineBitmapFont* createWithFont(char const* font, std::string text, float scale, float width, cocos2d::CCPoint anchor, int height, bool disableColor, bool, int, float);

    MultilineBitmapFont();
    ~MultilineBitmapFont();
    bool initWithFont(char const* font, std::string text, float scale, float width, cocos2d::CCPoint anchor, int height, bool disableColor, bool, int, float);
    void moveSpecialDescriptors(int index, int size);
    std::string readColorInfo(std::string text);
    void /* TodoReturn */ readEmoteInfo(std::string);
    std::string stringWithMaxWidth(std::string text, float width, float scale);

    FontObject* m_fontObject;
    bool m_unk260;
    int m_unk264;
    float m_unk268;
    cocos2d::CCNode* m_emoteNode;
    cocos2d::CCArray* m_specialDescriptors;
    cocos2d::CCArray* m_characters;
    cocos2d::CCArray* m_lines;
    int m_lineMetric;
    bool m_metricWritten;
    void* m_unkPtr;
    int m_height;
    int m_width;
    cocos2d::CCPoint m_position;
    float m_maxWidth;
    bool m_disableColor;

};
