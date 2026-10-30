#pragma once

class EditorUI;
class cocos2d;

class EditorCommandsPopup : public EditorWindowPopup {
public:
    virtual ~EditorCommandsPopup();
    virtual void onBuildWindowContent();
    virtual void onWindowResized();

    static EditorCommandsPopup* create(EditorUI*);

    void /* TodoReturn */ addCommandButton(char const*, EditorCommand);
    bool init(EditorUI*);
    void /* TodoReturn */ layoutCommandButtons();
    void onCommand(cocos2d::CCObject* sender);

};
