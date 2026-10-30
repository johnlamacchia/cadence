#pragma once

class DS_Dictionary;

class GManager : public cocos2d::CCNode {
public:
    virtual bool init();
    virtual void setup();
    virtual void encodeDataTo(DS_Dictionary* dict);
    virtual void didEncodeForSave();
    virtual void saveFinished(bool success);
    virtual void dataLoaded(DS_Dictionary* dict);
    virtual void firstLoad();

    GManager();
    std::string getCompressedSaveString();
    std::string getSaveString();
    void load();
    void loadDataFromFile(std::string const& filename);
    void loadFromCompressedString(std::string& str);
    void loadFromString(std::string& str);
    void save(bool);
    void saveGMTo(std::string filename, bool async);
    bool tryLoadData(DS_Dictionary* dict, std::string const& filename);
    void waitForSave(double seconds);

    std::string m_fileName;
    bool m_setup;
    std::atomic_bool m_isSaving;
    bool m_quickSave;

};
