#pragma once

class cocos2d;

class AppDelegate : public cocos2d::CCApplication, public cocos2d::CCSceneDelegate {
public:
    virtual bool applicationDidFinishLaunching();
    virtual void applicationDidEnterBackground();
    virtual void applicationWillEnterForeground();
    virtual void applicationWillBecomeActive();
    virtual void applicationWillResignActive();
    virtual void applicationWillTerminate();
    virtual void trySaveGame(bool force);
    virtual void willSwitchToScene(cocos2d::CCScene* scene);

    static AppDelegate* get();

    float bgScale();
    void checkSound();
    void hideLoadingCircle();
    void loadingIsFinished();
    void pauseGame();
    void pauseSound();
    void platformShutdown();
    void resumeSound();
    void setIdleTimerDisabled(bool disabled);
    void setupGLView();
    void showLoadingCircle(bool unk1, bool unk2, bool unk3);

    bool m_glViewSetup;
    bool m_gamePaused;
    cocos2d::CCScene* m_runningScene;
    bool m_loadingFinished;
    bool m_managersLoadedUnused;
    bool m_ios;
    bool m_unk0eb;
    bool m_unk0ec;
    bool m_needsSafeArea;
    double m_saveTime;

};
