#pragma once

class EmoteDef;
class cocos2d;

class TextStyleSection : public cocos2d::CCObject {
public:
    virtual ~TextStyleSection();

    static TextStyleSection* create(int start, int end, TextStyleType type);
    static TextStyleSection* createColoredSection(cocos2d::ccColor3B color, int start, int end);
    static TextStyleSection* createDelaySection(int start, float delay);
    static TextStyleSection* createInstantSection(int start, int end, float duration);
    static TextStyleSection* createShakeSection(int start, int end, int intensity, int perSecond);
    static TextStyleSection* createEmoteSection(int, EmoteDef const*);

    bool init(int start, int end, TextStyleType type);

    TextStyleType m_styleType;
    int m_startIndex;
    int m_endIndex;
    cocos2d::ccColor3B m_color;
    float m_instantTime;
    float m_delay;
    int m_shakeIntensity;
    int m_shakesPerSecond;
    EmoteDef* m_emoteDefinition;

};
