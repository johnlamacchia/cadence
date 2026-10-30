#pragma once

class ReviewChoiceDelegate;
class cocos2d;

class ReviewChoiceSheet : public SetupTriggerPopup {
public:
    virtual ~ReviewChoiceSheet();
    virtual void keyBackClicked();
    virtual void onClose(cocos2d::CCObject* sender);

    static ReviewChoiceSheet* create(ReviewChoiceDelegate*, int, char const*, std::string const&, std::vector<std::string> const&, std::vector<char const*> const&, bool, char const*);

    bool init(ReviewChoiceDelegate*, int, char const*, std::string const&, std::vector<std::string> const&, std::vector<char const*> const&, bool, char const*);
    void onChoice(cocos2d::CCObject* sender);

};
