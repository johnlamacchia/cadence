#pragma once

class cocos2d;

class DialogObject : public cocos2d::CCObject {
public:
    virtual ~DialogObject();

    static DialogObject* create(std::string character, std::string text, int characterFrame, float textScale, bool unskippable, cocos2d::ccColor3B nameColor);

    DialogObject();
    bool init(std::string character, std::string text, int characterFrame, float textScale, bool unskippable, cocos2d::ccColor3B nameColor);

    std::string m_text;
    std::string m_character;
    int m_characterFrame;
    cocos2d::ccColor3B m_nameColor;
    float m_textScale;
    bool m_unskippable;

};
