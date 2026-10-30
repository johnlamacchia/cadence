#pragma once

class CCURLObject;
class cocos2d;

class URLCell : public TableViewCell {
public:
    virtual ~URLCell();
    virtual bool init();
    virtual void draw();

    URLCell(char const* identifier, float width, float height);
    void loadFromObject(CCURLObject* object);
    void onURL(cocos2d::CCObject* sender);
    void updateBGColor(int index);

    CCURLObject* m_urlObject;

};
