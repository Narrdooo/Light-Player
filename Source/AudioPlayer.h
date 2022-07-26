#pragma once
#include <JuceHeader.h>

    
class AudioPlayer : public AudioAppComponent,
                    public ChangeListener,
                    public Timer
{
public:
    AudioPlayer();
    ~AudioPlayer() override;

    void playOrPause();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void resized() override;
    void changeListenerCallback(ChangeBroadcaster* source) override;
    void timerCallback() override;
    void updateLoopState(bool shouldLoop);
    void addSongtoPlay(File file);
    String getSongLength(File file);

    int processSliderWidth = 460;
    int processSliderHeight = 20;
    int playButtonSize = 40;
    int labelWidth = 100;
    int labelHeight = 20;
    int width = 530;
    int height = 100;

private:
    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Stopping
    };


    void loopButtonChanged();

    //==========================================================================
    ImageButton playButton;
    ImageButton prevButton;
    ImageButton nextButton;

    Image playImage;
    Image pauseImage;

    Slider volumeSlider;
    Slider processSlider;

    ToggleButton loopingToggle;
    Label currentPositionLabel;

    std::unique_ptr<FileChooser> chooser;

    AudioFormatManager formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
    TransportState state;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPlayer)
};
