#pragma once

class MusicBrowser;

class MusicBrowserDelegate {
public:
    virtual void musicBrowserClosed(MusicBrowser* browser);

};
