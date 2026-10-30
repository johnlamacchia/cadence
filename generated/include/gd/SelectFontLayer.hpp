#pragma once

class LevelEditorLayer;
class cocos2d;

class SelectFontLayer : public FLAlertLayer {
public:
    virtual void keyBackClicked();

    static SelectFontLayer* create(LevelEditorLayer* editorLayer);

    SelectFontLayer();
    ~SelectFontLayer();
    bool init(LevelEditorLayer* layer);
    void onChangeFont(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);
    void updateFontLabel();

    int m_font;
    LevelEditorLayer* m_editorLayer;
    cocos2d::CCLabelBMFont* m_fontLabel;

};
