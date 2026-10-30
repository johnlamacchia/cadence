#pragma once

class GJBaseGameLayer;
class GameObject;
class cocos2d;

class LabelGameObject : public EffectGameObject {
public:
    virtual ~LabelGameObject();
    virtual bool init();
    virtual void setOpacity(unsigned char opacity);
    virtual void setupCustomSprites(std::string frameName);
    virtual void addMainSpriteToParent(bool reorder);
    virtual void resetObject();
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);
    virtual void setObjectColor(cocos2d::ccColor3B const& color);
    virtual void updateTextKerning(int kerning);
    virtual int getTextKerning();

    static LabelGameObject* create();

    LabelGameObject();
    void createLabel(std::string text);
    void queueUpdateLabel(std::string text);
    void removeLabel();
    void unlockLabelColor();
    void updateLabel(float value);
    void updateLabel(std::string text);
    void updateLabelAlign(int alignment);
    void updateLabelIfDirty();
    void updatePreviewLabel();

    cocos2d::CCLabelBMFont* m_label;
    bool m_labelDirty;
    std::string m_labelString;
    bool m_labelColorLocked;
    int m_alignment;
    bool m_showSecondsOnly;
    int m_shownSpecial;
    bool m_isTimeCounter;
    int m_kerning;
    bool m_updateLabel;

};
