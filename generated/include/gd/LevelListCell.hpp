#pragma once

class GJLevelList;
class ListCell;
class cocos2d;

class LevelListCell : public TableViewCell {
public:
    virtual bool init();
    virtual void draw();

    static LevelListCell* create(float width, float height);

    LevelListCell(char const* identifier, float width, float height);
    ~LevelListCell();
    void loadFromList(GJLevelList* list);
    void onClick(cocos2d::CCObject* sender);
    void onListInfo(cocos2d::CCObject* sender);
    void onViewProfile(cocos2d::CCObject* sender);
    void updateBGColor(int index);

    GJLevelList* m_levelList;
    bool m_addingLevel;

};
