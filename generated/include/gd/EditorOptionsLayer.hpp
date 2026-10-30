#pragma once

class OptionsLayer;
class cocos2d;

class EditorOptionsLayer : public GJOptionsLayer {
public:
    virtual bool init();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void setupOptions();

    static EditorOptionsLayer* create();

    EditorOptionsLayer();
    void onButtonRows(cocos2d::CCObject* sender);
    void onButtonsPerRow(cocos2d::CCObject* sender);

    int m_buttonsPerRow;
    int m_buttonRows;
    cocos2d::CCLabelBMFont* m_buttonsPerRowLabel;
    cocos2d::CCLabelBMFont* m_buttonRowsLabel;

};
