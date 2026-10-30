#pragma once

class OptionsObjectDelegate;
class StartPosObject;

class OptionsObject : public cocos2d::CCObject {
public:
    virtual ~OptionsObject();

    static OptionsObject* create(int id, bool enabled, std::string name, OptionsObjectDelegate* delegate);

    bool init(int id, bool enabled, std::string name, OptionsObjectDelegate* delegate);
    void toggleState();

    int m_optionID;
    bool m_enabled;
    std::string m_name;
    OptionsObjectDelegate* m_delegate;
    int m_type;
    int m_count;
    StartPosObject* m_startPosObject;

};
