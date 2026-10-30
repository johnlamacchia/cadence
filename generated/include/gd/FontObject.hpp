#pragma once

class FontObject : public cocos2d::CCObject {
public:
    virtual ~FontObject();

    static FontObject* createWithConfigFile(char const* font, float scale);

    FontObject();
    void /* TodoReturn */ getFontAdvance(unsigned int);
    int getFontWidth(unsigned int);
    void /* TodoReturn */ hasGlyph(unsigned int);
    bool initWithConfigFile(char const* font, float scale);
    void parseConfigFile(char const* font, float scale);

    std::unordered_map<unsigned int, unsigned int> m_fontWidths;
    std::unordered_map<unsigned int, unsigned int> m_fontAdvances;

};
