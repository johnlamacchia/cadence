#pragma once

class GJSmartTemplate;
class cocos2d;

class BrowseSmartKeyLayer : public BrowseSmartTemplateLayer {
public:
    virtual ~BrowseSmartKeyLayer();
    virtual void onBack(cocos2d::CCObject* sender);

    static BrowseSmartKeyLayer* create(GJSmartTemplate* smartTemplate, std::string prefabKey);

    BrowseSmartKeyLayer();
    void addChanceToSelected(int chance, bool zero);
    void createTemplateObjects();
    void deletedSelectedItems();
    cocos2d::CCArray* getAllSelectedBlocks();
    bool init(GJSmartTemplate* smartTemplate, std::string prefabKey);
    void onButton(cocos2d::CCObject* sender);
    void onPrefabObject(cocos2d::CCObject* sender);
    void updateChanceValues();

    std::string m_prefabKey;
    int m_templatePage;

};
