#include "AudioPlayer.h"

AudioPlayer::AudioPlayer()
    : state(Stopped)
{
    setOpaque(false);
    setSize(width, height);
    setVisible(true);
   
    
    addAndMakeVisible(&processSlider);
    processSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    processSlider.setBounds(int((width - processSliderWidth) / 2), 
                            20, 
                            processSliderWidth, 
                            processSliderHeight);
    processSlider.onDragStart = [this] {
        state = Stopped;
        transportSource.stop();
    };
    processSlider.onDragEnd = [this] {
        playOrPause();
    };
    processSlider.onValueChange = [this] {
        auto now = processSlider.getValue();
        transportSource.setPosition(now);
    };

    
    addAndMakeVisible(&playButton);
    playImage = ImageCache::getFromMemory(BinaryData::play_png, BinaryData::play_pngSize);
    pauseImage = ImageCache::getFromMemory(BinaryData::pause_png, BinaryData::pause_pngSize);

    playButton.setImages(false, true, false,
                         playImage, 1.0f, Colours::transparentBlack,
                         playImage, 0.5f, Colours::transparentBlack,
                         playImage, 0.2f, Colours::transparentBlack);

    playButton.setBounds(int((getWidth() - playButtonSize) / 2),
                         int(getHeight() / 2 - 5),
                         playButtonSize, 
                         playButtonSize);
    playButton.onClick = [this] { playOrPause(); };

    addAndMakeVisible(&currentPositionLabel);
    currentPositionLabel.setText("--:-- / --:--", dontSendNotification);
    currentPositionLabel.setColour(currentPositionLabel.textColourId, Colours::grey);
    currentPositionLabel.setBounds(40,
                                   int(getHeight() / 2 + 5),
                                   labelWidth, 
                                   labelHeight);
    

    addAndMakeVisible(&loopingToggle);
    loopingToggle.setButtonText("Loop");
    loopingToggle.onClick = [this] { loopButtonChanged(); };

    formatManager.registerBasicFormats();
    transportSource.addChangeListener(this);

    setAudioChannels(2, 2);
    startTimer(20);

    //addSongtoPlay(File("C:/Users/ronnnhui/Documents/projects/Light-Player/Resource/Sunnyday/output.wav"));
}


AudioPlayer::~AudioPlayer()
{
    shutdownAudio();
}




void AudioPlayer::playOrPause()
{
    if (state == Stopped) {
        state = Playing;
        transportSource.start();
        playButton.setImages(false, true, false,
                             pauseImage, 1.0f, Colours::transparentBlack,
                             pauseImage, 0.5f, Colours::transparentBlack,
                             pauseImage, 0.2f, Colours::transparentBlack);
    }
    else {
        state = Stopped;
        transportSource.stop();
        playButton.setImages(false, true, false,
                             playImage, 1.0f, Colours::transparentBlack,
                             playImage, 0.5f, Colours::transparentBlack,
                             playImage, 0.2f, Colours::transparentBlack);
    }
}

void AudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) 
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void AudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) 
{
    if (readerSource.get() == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    transportSource.getNextAudioBlock(bufferToFill);
}


void AudioPlayer::releaseResources() 
{
    transportSource.releaseResources();
}

void AudioPlayer::resized() 
{
}

void AudioPlayer::changeListenerCallback(ChangeBroadcaster* source) 
{
    if (source == &transportSource)
    {
        if (transportSource.isPlaying())
            state = Playing;
        else
            state = Stopped;
    }
}

void AudioPlayer::timerCallback() 
{
    if (transportSource.isPlaying())
    {
        auto timeLength = currentPositionLabel.getText().substring(5);
        RelativeTime position(transportSource.getCurrentPosition());

        auto minutes = ((int)position.inMinutes()) % 60;
        auto seconds = ((int)position.inSeconds()) % 60;

        auto positionString = String::formatted("%02d:%02d", minutes, seconds) + timeLength;
        currentPositionLabel.setText(positionString, dontSendNotification);
        processSlider.setValue(position.inSeconds());
    }
}

void AudioPlayer::updateLoopState(bool shouldLoop)
{
    if (readerSource.get() != nullptr)
        readerSource->setLooping(shouldLoop);
}

void AudioPlayer::addSongtoPlay(File file)
{
    if (!file.exists()) {
        jassertfalse;    
    }

    auto* reader = formatManager.createReaderFor(file);

    if (reader != nullptr)
    {
        auto newSource = std::make_unique<AudioFormatReaderSource>(reader, true);
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        playButton.setEnabled(true);
        readerSource.reset(newSource.release());
    }

    double songLength = transportSource.getLengthInSeconds();
    auto minutes = (int(songLength) / 60) % 60;
    auto seconds = int(songLength) % 60;
    auto positionString = String::formatted("00:00 / %02d:%02d", minutes, seconds);
    currentPositionLabel.setText(positionString, dontSendNotification);
    processSlider.setRange(0., songLength);
}


String AudioPlayer::getSongLength(File file)
{
    if (!file.exists()) {
        return "";
    }

    auto* reader = formatManager.createReaderFor(file);

    if (reader != nullptr)
    {
        auto newSource = std::make_unique<AudioFormatReaderSource>(reader, true);
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        playButton.setEnabled(true);
        readerSource.reset(newSource.release());
    }

    double songLength = transportSource.getLengthInSeconds();
    auto minutes = (int(songLength) / 60) % 60;
    auto seconds = int(songLength) % 60;
    return String::formatted("%02d:%02d", minutes, seconds);
}

void AudioPlayer::loopButtonChanged()
{
    updateLoopState(loopingToggle.getToggleState());
}