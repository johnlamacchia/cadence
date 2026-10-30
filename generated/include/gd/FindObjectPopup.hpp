#pragma once

class EditorUI;
class cocos2d;

class FindObjectPopup : public EditorWindowPopup {
public:
    virtual ~FindObjectPopup();
    virtual void valueDidChange(int, float);
    virtual void onBuildWindowContent();
    virtual void onFieldEnter();

    static FindObjectPopup* create(EditorUI*);

    FindObjectPopup();
    bool init(EditorUI*);
    void onFindStep(cocos2d::CCObject* sender);
    void onOptions(cocos2d::CCObject* sender);
    void onSelect(cocos2d::CCObject* sender);
    void /* TodoReturn */ runFind(int);

    int m_groupID;

};
