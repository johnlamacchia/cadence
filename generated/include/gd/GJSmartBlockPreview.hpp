#pragma once

class LevelEditorLayer;
class SmartPrefabResult;
class cocos2d;

class GJSmartBlockPreview : public cocos2d::CCNode {
public:
    virtual ~GJSmartBlockPreview();

    static GJSmartBlockPreview* create(std::string key);

    void addChance(int count, int total);
    void addCount(int count);
    void addPreview(std::string key, std::string data, LevelEditorLayer* layer);
    cocos2d::CCArray* addTemplateGuide(SmartPrefabResult result, float opacity);
    void addTemplateGuide(std::string key, int type);
    void createInfoLabel();
    bool init(std::string key);
    void toggleSelectItem(bool selected);

    std::string m_prefabKey;
    cocos2d::CCLabelBMFont* m_infoLabel;
    cocos2d::CCSprite* m_selectSprite;
    bool m_selected;

};
