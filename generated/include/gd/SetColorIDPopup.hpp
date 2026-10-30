#pragma once

class GJSpecialColorSelect;
class cocos2d;

class SetColorIDPopup : public SetIDPopup, public GJSpecialColorSelectDelegate {
public:
    virtual ~SetColorIDPopup();
    virtual void colorSelectClosed(GJSpecialColorSelect* select, int id);

    static SetColorIDPopup* create(int id);

    SetColorIDPopup();
    bool init(int id);
    void onSelectSpecialColor(cocos2d::CCObject* sender);

};
