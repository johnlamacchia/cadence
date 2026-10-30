#pragma once

class CCTextInputNode;
class Dropdown;
class DropdownListPopup;
class DropdownListRow;
class FLAlertLayer;
class IDNameBrowserPopup;
class cocos2d;

class IDNameInput : public cocos2d::CCNode, public TextInputDelegate, public IDNameBrowserDelegate, public DropdownListPopupOwner {
public:
    virtual ~IDNameInput();
    virtual void idNameBrowserClosed(IDNameBrowserPopup*, int);
    virtual void textChanged(CCTextInputNode*);
    virtual void textInputOpened(CCTextInputNode*);
    virtual void textInputClosed(CCTextInputNode*);
    virtual void textInputShouldOffset(CCTextInputNode*, float);
    virtual void textInputReturn(CCTextInputNode*);
    virtual bool allowTextInput(CCTextInputNode*);
    virtual void enterPressed(CCTextInputNode*);
    virtual void dropdownListPopupClosed(DropdownListPopup*);

    static IDNameInput* create(FLAlertLayer*, TextInputDelegate*, int, int, std::string const&, cocos2d::CCPoint, float, int, int, int, bool);

    void /* TodoReturn */ collectMatches(std::string const&, std::vector<DropdownListRow>&);
    void /* TodoReturn */ commitID(int);
    void /* TodoReturn */ commitTypedName();
    void /* TodoReturn */ dismissAutocomplete();
    void /* TodoReturn */ idNameTypeForItemValueType(int);
    void /* TodoReturn */ initCore(FLAlertLayer*, TextInputDelegate*, int, int, std::string const&, cocos2d::CCPoint, float, int, int, int, bool);
    void /* TodoReturn */ nameLess(std::string const&, int, std::string const&, int);
    void onArrowLeft(cocos2d::CCObject* sender);
    void onArrowRight(cocos2d::CCObject* sender);
    void onAutocompleteRow(cocos2d::CCObject* sender);
    void onBrowser(cocos2d::CCObject* sender);
    void onNextFree(cocos2d::CCObject* sender);
    void /* TodoReturn */ refreshLabel();
    void /* TodoReturn */ revertToLastID();
    void setType(int);
    void /* TodoReturn */ stepArrow(int);
    void /* TodoReturn */ textIsPureInt(std::string const&);
    void /* TodoReturn */ updateAutocomplete();
    void /* TodoReturn */ useRank(int);
    void /* TodoReturn */ useRankLess(std::unordered_map<int, int> const&, int, int);

};
