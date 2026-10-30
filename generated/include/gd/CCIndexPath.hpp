#pragma once

class CCIndexPath : public cocos2d::CCObject {
public:
    virtual ~CCIndexPath();

    static CCIndexPath* CCIndexPathWithSectionRow(int section, int row);

    CCIndexPath();

    int m_row;
    int m_section;

};
