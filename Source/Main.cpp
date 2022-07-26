#include <JuceHeader.h>
#include "PlayerPanel.h"
#include "AudioPlayer.h"
#include "PlayerUI.h"

//==============================================================================
class LightPlayerApplication  : public JUCEApplication
{
public:
    //==============================================================================
    LightPlayerApplication() = default;

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override             { return true; }

    //==============================================================================
    void initialise (const String& commandLine) override
    {
        mainWindow.reset (new MainWindow (getApplicationName(), *this));
    }

    void shutdown() override
    {

        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        quit();
    }


    class MainWindow    : public DocumentWindow
    {
    public:
        MainWindow(const String& name, JUCEApplication& a)
            : DocumentWindow(name, 
                             Desktop::getInstance().getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId),
                             0),
            app(a)
        {
            centreWithSize(100, 100);
            playerPanel.reset(new PlayerPanel());
            setUsingNativeTitleBar(false);
            setContentOwned(playerPanel.get(), true);
            setResizable(false, false);
            setOpaque(false);
            setDropShadowEnabled(false);
            setBackgroundColour(Colours::transparentWhite);


            playerUI.setParameter(playerPanel->cornerSize,
                                  playerPanel->shadowSize,
                                  playerPanel->playList->getWidth(),
                                  playerPanel->playList->backgroundColour);
            setLookAndFeel(&playerUI);

        //#if JUCE_ANDROID || JUCE_IOS
        //    setFullScreen(true);
        //#else
        //    setResizable(true, false);
        //    setResizeLimits(300, 250, 10000, 10000);
        //    centreWithSize(getWidth(), getHeight());
        //#endif
            setVisible(true);
            playerPanel->hideButton.onClick = [this] { minimiseButtonPressed(); };
            playerPanel->closeButton.onClick = [this] { closeButtonPressed(); };

        }

        void closeButtonPressed() override
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCEApplication& app;
        PlayerUI playerUI;
        std::unique_ptr<PlayerPanel> playerPanel;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };



private:
    std::unique_ptr<MainWindow> mainWindow;
    
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (LightPlayerApplication)
