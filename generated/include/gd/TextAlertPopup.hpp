#pragma once

class cocos2d;

class TextAlertPopup : public cocos2d::CCNode {
public:
    virtual ~TextAlertPopup();

    static TextAlertPopup* create(std::string text, float delay, float scale, int opacity, std::string font);

    void /* TodoReturn */ inheritOpacity(TextAlertPopup*);
    bool init(std::string text, float delay, float scale, int opacity, std::string font);
    void /* TodoReturn */ replaceIn(cocos2d::CCNode*, int, TextAlertPopup*, int);
    void setAlertPosition(cocos2d::CCPoint windowOffset, cocos2d::CCPoint pointOffset);
    void setLabelColor(cocos2d::ccColor3B color);

    float m_width;
    float m_height;
    cocos2d::CCLabelBMFont* m_label;
    cocos2d::extension::CCScale9Sprite* m_background;

};
