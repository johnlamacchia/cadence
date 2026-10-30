#pragma once

class GameObject;
class SongTriggerGameObject;

class SongChannelState {
public:
    SongTriggerGameObject* m_songTriggerGameObject1;
    double m_startTime;
    SongTriggerGameObject* m_songTriggerGameObject2;
    double m_prepareTime;

};
