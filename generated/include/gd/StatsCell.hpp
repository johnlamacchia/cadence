#pragma once

class StatsObject;

class StatsCell : public TableViewCell {
public:
    virtual ~StatsCell();
    virtual bool init();
    virtual void draw();

    StatsCell(char const* identifier, float width, float height);
    const char* getTitleFromKey(char const* key);
    void loadFromObject(StatsObject* object);
    void updateBGColor(int index);

};
