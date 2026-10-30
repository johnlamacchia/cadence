#pragma once

class GJLocalScore;

class GJLocalLevelScoreCell : public TableViewCell {
public:
    virtual ~GJLocalLevelScoreCell();
    virtual bool init();
    virtual void draw();

    GJLocalLevelScoreCell(char const* identifier, float width, float height);
    void loadFromScore(GJLocalScore* score);
    void updateBGColor(int index);

    GJLocalScore* m_localScore;

};
