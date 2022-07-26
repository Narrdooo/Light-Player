#include "PlayerUI.h"

PlayerUI::PlayerUI()
{
}

PlayerUI::~PlayerUI()
{
}

void PlayerUI::drawDocumentWindowTitleBar(DocumentWindow& window, Graphics& g,
                                     int w, int h, int titleSpaceX, int titleSpaceW,
                                     const Image* icon, bool drawTitleTextOnLeft)
{
    if (w * h == 0)
        return;

    auto isActive = window.isActiveWindow();

    // ------------- draw shadow -------------------
    g.setColour(Colours::lightgrey);

    g.fillRoundedRectangle(0 , 
                           shadowSize, 
                           w - shadowSize, 
                           h + 2 * cornerSize, 
                           cornerSize);

    // ------------- grey roundline ------------------
    g.setColour(Colours::lightgrey);

    g.drawRoundedRectangle(shadowSize,
                           0,
                           w - shadowSize - playListWidth + 2 * cornerSize,
                           h + 2 * cornerSize,
                           cornerSize,
                           1);

    // ------------- grey line ------------------
    g.drawLine(0,
               cornerSize - 3,
               0,
               h,
               3);

    // ------------- blue roundline -------------------
    g.setColour(backgroundColour);

    g.drawRoundedRectangle(w - playListWidth,
                           0,
                           playListWidth,
                           h + 2 * cornerSize,
                           cornerSize,
                           1);

    // ------------- blue roundrect -------------------
    g.setColour(backgroundColour);

    g.fillRoundedRectangle(w - playListWidth - 2 * cornerSize,
                           0,
                           playListWidth + 2 * cornerSize,
                           h + 2 * cornerSize,
                           cornerSize);

    // ------------- white roundrect -------------------
    g.setColour(Colours::white);

    g.fillRoundedRectangle(shadowSize, 
                           0, 
                           w - shadowSize - playListWidth,
                           h + 2 * cornerSize,
                           cornerSize);

}

void PlayerUI::setParameter(int _cornerSize, 
                            int _shadowSize, 
                            int _playListWidth, 
                            Colour _backgroundColour)
{
    cornerSize = _cornerSize;
    shadowSize = _shadowSize;
    playListWidth = _playListWidth;
    backgroundColour = _backgroundColour;
}

