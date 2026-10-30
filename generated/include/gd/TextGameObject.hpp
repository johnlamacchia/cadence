#pragma once

class GJBaseGameLayer;
class cocos2d;

class TextGameObject : public GameObject {
public:
    virtual ~TextGameObject();
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);
    virtual void updateTextKerning(int kerning);
    virtual int getTextKerning();

    static TextGameObject* create(cocos2d::CCTexture2D* texture);

    TextGameObject();
    bool init(cocos2d::CCTexture2D* texture);
    void updateTextObject(std::string text, bool defaultFont);

    std::string m_text;
    int m_kerning;

};
