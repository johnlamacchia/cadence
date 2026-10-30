#pragma once

class cocos2d;

class LevelCommentDelegate {
public:
    virtual void loadCommentsFinished(cocos2d::CCArray* comments, char const* key);
    virtual void loadCommentsFailed(char const* key);
    virtual void updateUserScoreFinished();
    virtual void setupPageInfo(std::string info, char const* key);

};
