#pragma once

class InfoAlertButton : public CCMenuItemSpriteExtra {
public:
    virtual ~InfoAlertButton();
    virtual void activate();

    static InfoAlertButton* create(std::string title, std::string desc, float spriteScale);

    bool init(std::string title, std::string desc, float spriteScale);

    std::string m_title;
    std::string m_description;
    float m_textScale;
    bool m_scroll;

};
