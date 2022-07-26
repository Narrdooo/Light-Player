#pragma once

#include <JuceHeader.h>

class PlayerUI : public LookAndFeel_V4
{
public:
    PlayerUI();
    ~PlayerUI();
    void drawDocumentWindowTitleBar(DocumentWindow& window, Graphics& g,
                                    int w, int h, int titleSpaceX, int titleSpaceW,
                                    const Image* icon, bool drawTitleTextOnLeft)    override;

    void setParameter(int _cornerSize,
                      int _shadowSize,
                      int _playListWidth,
                      Colour _backgroundColour);
private:
    int cornerSize;
    int shadowSize;
    int playListWidth;
    Colour backgroundColour;
};