#pragma once

class SFXInfoObject : public cocos2d::CCObject {
public:
    virtual ~SFXInfoObject();

    static SFXInfoObject* create(int id, std::string name, int folderID, int size, int duration);

    SFXInfoObject();
    std::string getLowerCaseName();
    bool init(int id, std::string name, int folderID, int size, int duration);

    int m_sfxID;
    int m_folderID;
    int m_size;
    int m_duration;
    bool m_folder;
    bool m_unk049;
    int m_unk048;
    std::string m_name;
    std::string m_lowerCaseName;
    std::string m_unk090;

};
