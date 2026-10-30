#pragma once

class ColorAction;

class InheritanceNode : public cocos2d::CCObject {
public:
    virtual ~InheritanceNode();

    static InheritanceNode* create(int colorID, InheritanceNode* node);

    InheritanceNode();
    bool init(int colorID, InheritanceNode* node);

    int m_colorID;
    InheritanceNode* m_inheritanceNode;
    ColorAction* m_colorAction;
    bool m_unk050;
    bool m_unk051;

};
