#pragma once

class OptionsObject;
class cocos2d;

class OptionsCell : public TableViewCell {
public:
    virtual ~OptionsCell();

    OptionsCell(char const* identifier, float width, float height);
    void loadFromObject(OptionsObject* object);
    void onToggleOption(cocos2d::CCObject* sender);
    void updateBGColor(int index);

    OptionsObject* m_optionsObject;

};
