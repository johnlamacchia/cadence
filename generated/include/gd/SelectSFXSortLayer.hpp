#pragma once

class SelectSFXSortDelegate;
class cocos2d;

class SelectSFXSortLayer : public FLAlertLayer {
public:
    virtual ~SelectSFXSortLayer();
    virtual void keyBackClicked();

    static SelectSFXSortLayer* create(AudioSortType sortType);

    SelectSFXSortLayer();
    bool init(AudioSortType sortType);
    void onClose(cocos2d::CCObject* sender);
    void onSelect(cocos2d::CCObject* sender);

    AudioSortType m_sortType;
    SelectSFXSortDelegate* m_delegate;

};
