#pragma once

class cocos2d;

class AchievementCell : public TableViewCell {
public:
    virtual ~AchievementCell();
    virtual bool init();
    virtual void draw();

    AchievementCell(char const* identifier, float width, float height);
    void loadFromDict(cocos2d::CCDictionary* dict);
    void updateBGColor(int index);

};
