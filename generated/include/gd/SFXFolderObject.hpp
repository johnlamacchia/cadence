#pragma once

class cocos2d;

class SFXFolderObject : public SFXInfoObject {
public:
    static SFXFolderObject* create(int id, std::string name, int folderID);

    SFXFolderObject();
    ~SFXFolderObject();
    bool init(int id, std::string name, int folderID);

    cocos2d::CCArray* m_sfxObjects;

};
