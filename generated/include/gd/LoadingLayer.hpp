#pragma once

class TextArea;
class cocos2d;

class LoadingLayer : public cocos2d::CCLayer {
public:
    static LoadingLayer* create(bool refresh);
    static cocos2d::CCScene* scene(bool refresh);

    LoadingLayer();
    ~LoadingLayer();
    const char* getLoadingString();
    bool init(bool refresh);
    void loadAssets();
    void loadingFinished();
    void updateProgress(int progress);

    bool m_unknown;
    bool m_unknown2;
    int m_loadStep;
    cocos2d::CCLabelBMFont* m_caption;
    TextArea* m_textArea;
    cocos2d::CCSprite* m_sliderBar;
    float m_sliderGrooveXPos;
    float m_sliderGrooveHeight;
    bool m_fromRefresh;

};
