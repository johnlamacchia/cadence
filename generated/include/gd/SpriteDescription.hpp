#pragma once

class DS_Dictionary;
class cocos2d;

class SpriteDescription : public cocos2d::CCObject {
public:
    static SpriteDescription* createDescription(DS_Dictionary* dict);
    static SpriteDescription* createDescription(cocos2d::CCDictionary* dict);

    SpriteDescription();
    ~SpriteDescription();
    bool initDescription(DS_Dictionary* dict);
    bool initDescription(cocos2d::CCDictionary* dict);

    cocos2d::CCPoint m_position;
    cocos2d::CCPoint m_scale;
    cocos2d::CCPoint m_flipped;
    float m_rotation;
    int m_zValue;
    int m_tag;
    bool m_usesCustomTag;
    cocos2d::CCSpriteFrame* m_texture;

};
