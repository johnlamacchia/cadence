#pragma once

class GJSmartTemplate;
class cocos2d;

class SmartTemplateCell : public TableViewCell {
public:
    virtual ~SmartTemplateCell();
    virtual bool init();
    virtual void draw();

    SmartTemplateCell(char const* identifier, float width, float height);
    void loadFromObject(GJSmartTemplate* smartTemplate);
    void onClick(cocos2d::CCObject* sender);
    void updateBGColor(int index);

    GJSmartTemplate* m_smartTemplate;

};
