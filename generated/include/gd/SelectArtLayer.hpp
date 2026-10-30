#pragma once

class SelectArtDelegate;
class cocos2d;

class SelectArtLayer : public FLAlertLayer {
public:
    virtual ~SelectArtLayer();
    virtual void keyBackClicked();

    static SelectArtLayer* create(SelectArtType type, int index);

    bool init(SelectArtType type, int index);
    void onClose(cocos2d::CCObject* sender);
    void onSelectCustom(cocos2d::CCObject* sender);
    void selectArt(cocos2d::CCObject* sender);
    void updateSelectedCustom(int idx);

    cocos2d::CCArray* m_artSprites;
    cocos2d::CCArray* m_lineSprites;
    int m_art;
    int m_line;
    SelectArtType m_type;
    SelectArtDelegate* m_delegate;

};
