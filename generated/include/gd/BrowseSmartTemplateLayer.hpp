#pragma once

class GJSmartTemplate;
class SmartPrefabResult;
class cocos2d;

class BrowseSmartTemplateLayer : public FLAlertLayer {
public:
    virtual void keyBackClicked();
    virtual void onBack(cocos2d::CCObject* sender);

    static BrowseSmartTemplateLayer* create(GJSmartTemplate* smartTemplate, SmartBrowseFilter browseFilter);

    BrowseSmartTemplateLayer();
    ~BrowseSmartTemplateLayer();
    void addObjectToPage(cocos2d::CCObject* object, int page);
    void addPrefabMenuItem(SmartPrefabResult result, int index);
    void baseSetup();
    void createDots();
    void createPrefab(std::string key, int id);
    void goToPage(int page);
    bool init(GJSmartTemplate* smartTemplate, SmartBrowseFilter browseFilter);
    void onClick(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);
    void onTemplateObject(cocos2d::CCObject* sender);
    void updateDots();

    GJSmartTemplate* m_template;
    cocos2d::CCArray* m_pages;
    int m_page;
    cocos2d::CCNode* m_nextPageBtn;
    cocos2d::CCNode* m_prevPageBtn;
    cocos2d::CCArray* m_dotsArray;

};
