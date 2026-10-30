#pragma once

class CCMenuItemToggler;
class GJUserScore;
class cocos2d;

class GJRequestCell : public TableViewCell {
public:
    virtual bool init();
    virtual void draw();

    GJRequestCell(char const* identifier, float width, float height);
    ~GJRequestCell();
    void loadFromScore(GJUserScore* score);
    void markAsRead();
    void onToggle(cocos2d::CCObject* sender);
    void onViewFriendRequest(cocos2d::CCObject* sender);
    void onViewProfile(cocos2d::CCObject* sender);
    void updateBGColor(int index);
    void updateToggle();

    GJUserScore* m_score;
    CCMenuItemToggler* m_toggler;
    bool m_groupRequest;

};
