#pragma once

class AdminActionConfirmDelegate;
class cocos2d;

class AdminActionConfirmPopup : public SetupTriggerPopup {
public:
    virtual ~AdminActionConfirmPopup();
    virtual void keyBackClicked();
    virtual void onClose(cocos2d::CCObject* sender);

    static AdminActionConfirmPopup* create(AdminActionConfirmDelegate*, int, char const*, std::string const&, char const*, std::vector<int> const&, std::string const&, bool);

    void /* TodoReturn */ addHelp(char const*, std::string const&);
    void /* TodoReturn */ createContentDelete(AdminActionConfirmDelegate*, int, char const*, std::string const&, int);
    bool init(AdminActionConfirmDelegate*, int, char const*, std::string const&, char const*, std::vector<int> const&, std::string const&, bool);
    void onCategoryToggle(cocos2d::CCObject* sender);
    void onConfirm(cocos2d::CCObject* sender);
    void /* TodoReturn */ selectedCategories();

};
