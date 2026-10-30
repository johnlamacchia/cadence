#pragma once

class SetTextPopup;
class cocos2d;

class SetFolderPopup : public SetIDPopup, public SetTextPopupDelegate {
public:
    virtual ~SetFolderPopup();
    virtual void valueChanged();
    virtual void setTextPopupClosed(SetTextPopup* popup, std::string text);

    static SetFolderPopup* create(int value, bool isCreated, std::string title);

    bool init(int value, bool isCreated, std::string title);
    void onSetFolderName(cocos2d::CCObject* sender);

    bool m_isCreated;
    cocos2d::CCLabelBMFont* m_titleLabel;

};
