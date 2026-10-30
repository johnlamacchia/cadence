#pragma once

class MusicDownloadDelegate;

class MusicDelegateHandler : public cocos2d::CCNode {
public:
    virtual ~MusicDelegateHandler();

    static MusicDelegateHandler* create(MusicDownloadDelegate* delegate);

    MusicDelegateHandler();
    bool init(MusicDownloadDelegate* delegate);

    MusicDownloadDelegate* m_delegate;

};
