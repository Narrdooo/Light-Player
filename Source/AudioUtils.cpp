#include "AudioUtils.h"
#include "PluginUtils.h"


AudioUtils::AudioUtils()   
{
    formatManager.addDefaultFormats();
    loadDefaultPlugin();

    //createPluginInstance(1);
    //getOrCreatePluginWindow();
    //run("C:/Users/ronnnhui/Documents/projects/Light-Player/Resource/Sunnyday");
}


AudioUtils::~AudioUtils()
{
    pluginWindow.reset(nullptr);
    pluginInstance.reset(nullptr);
}


void AudioUtils::loadDefaultPlugin()
{
    std::vector<String> defaultPath;
    //defaultPath.push_back("C:/Users/ronnnhui/Documents/projects/Light-Player/VstPlugin/OT BRASS/OT BRASS x64.vst3");
    //defaultPath.push_back("C:/Users/ronnnhui/Documents/projects/Light-Player/VstPlugin/Smaolab TaraTube Free-64.vst3");
    //defaultPath.push_back("C:/Users/ronnnhui/Documents/projects/Light-Player/VstPlugin/Lounge Lizard EP-4.vst3");
    //defaultPath.push_back("C:/Users/ronnnhui/Documents/projects/Light-Player/VstPlugin/Organ.vst3");
    //defaultPath.push_back("C:/Users/ronnnhui/Documents/projects/Light-Player/VstPlugin/");
    //defaultPath.push_back("C:/Users/ronnnhui/Documents/projects/Light-Player/VstPlugin/");
    //defaultPath.push_back("C:/Users/ronnnhui/Documents/projects/Light-Player/VstPlugin/");
    //defaultPath.push_back("C:/Users/ronnnhui/Documents/projects/Light-Player/VstPlugin/");
    //defaultPath.push_back("C:/Users/ronnnhui/Documents/projects/Light-Player/VstPlugin/");
    //defaultPath.push_back("C:/Users/ronnnhui/Documents/projects/Light-Player/VstPlugin/");

    for (auto path : defaultPath) {
        loadPluginFromPath(path);
    }
}

void AudioUtils::createPluginInstance(const int idx)
{
    if (auto* desc = typesFound.getUnchecked(idx)) {
        createPluginInstance(idx, *desc);
    }
}


void AudioUtils::createPluginInstance(const int idx, PluginDescription pluginDescription)
{
    String err;
    pluginInstance = formatManager.createPluginInstance(pluginDescription,
                                                        sampleRate,
                                                        bufferSize,
                                                        err);
    if (!pluginInstance) {
        jassertfalse;
    }
}

void AudioUtils::loadPluginFromPath(const String pluginPath)
{
    if (pluginPath.endsWith(".vst3")) {
        // only able to support VST3 instrument currently
        int oldSize = typesFound.size();
        for (int i = 0; i < formatManager.getNumFormats(); ++i)
            knownPluginList.scanAndAddFile(pluginPath,
                                           true,
                                           typesFound,
                                           *formatManager.getFormat(i));
        jassert(typesFound.size() > oldSize);
    }
    else {
        jassertfalse;
    }
}


void AudioUtils::readInputFolder(const File& inputPath)
{
    File audioPath = inputPath.getChildFile("./accompaniment.wav");
    jassert(audioPath.exists());
    readAudioFile(audioPath);

    File midiPath = inputPath.getChildFile("./vocal.mid");
    jassert(midiPath.exists());
    readMidiFile(midiPath);
}


void AudioUtils::readAudioFile(const File& audioPath)
{
    // Create a Audio reader for a given file
    double sampleRate;
    AudioFormatManager fm;
    fm.registerBasicFormats();
    
    auto inputReader = fm.createReaderFor(audioPath);
    if (!inputReader) {
        jassertfalse;
    }
    audioFile.reset(inputReader);
    sampleSize = std::max(sampleSize, (size_t)inputReader->lengthInSamples);
}


void AudioUtils::readMidiFile(const File& midiPath)
{
    // Crearte a MidiFile object for a given file
    midiFile.reset(new MidiFile());
    auto inputStream = midiPath.createInputStream();
    if (!midiFile->readFrom(*inputStream, true)) {
        jassertfalse;
    }

    midiFile->convertTimestampTicksToSeconds();

    for (int i = 0; i < midiFile->getNumTracks(); i++) {
        auto midiTrack = midiFile->getTrack(i);
        for (auto& meh : *midiTrack) {
            auto timestampSamples = secondsToSamples(meh->message.getTimeStamp(), sampleRate);
            sampleSize = std::max(sampleSize, timestampSamples);
        }
    }
}


AudioPluginInstance::BusesLayout AudioUtils::createBusLayout(unsigned int& NumInputChannels,
                                                             unsigned int& NumOutputChannels)
{
    NumInputChannels = 0;
    NumOutputChannels = 0;
    OwnedArray<juce::AudioFormatReader> audioInputFileReaders;
    juce::AudioPluginInstance::BusesLayout layout;
    if (audioInputFileReaders.isEmpty()) {
        // if no input files are provided, use the plugin's default input bus layout
        // to maximize compatibility with synths that expect an input
        layout.inputBuses = pluginInstance->getBusesLayout().inputBuses;
        for (auto& cs : layout.inputBuses) {
            NumInputChannels += (unsigned int)cs.size();
        }

    }
    else {
        for (auto* inputFileReader : audioInputFileReaders) {
            layout.inputBuses.add(
                juce::AudioChannelSet::canonicalChannelSet((int)inputFileReader->numChannels));
            NumInputChannels += inputFileReader->numChannels;
        }
    }

    // create an output bus with the desired amount of channels,
    // defaulting to the same amount of channels as the main input file if one exists,
    // or the plugin's default bus layout otherwise.
    NumOutputChannels = (outputChannelCountOpt.value_or(
        audioInputFileReaders.isEmpty()
        ? (unsigned int)pluginInstance->getBusesLayout().getMainOutputChannels()
        : audioInputFileReaders[0]->numChannels));
    layout.outputBuses.add(
        juce::AudioChannelSet::canonicalChannelSet((int)NumOutputChannels));

    return layout;
}



void AudioUtils::getOrCreatePluginWindow()
{
    auto description = pluginInstance->getPluginDescription();
    if (pluginInstance->hasEditor()) {
        auto localDpiDisabler = makeDPIAwarenessDisablerForPlugin(description);
        auto type = (PluginWindow::Type) 0;
        pluginWindow.reset(new PluginWindow(pluginInstance.get(), 
                                            pluginWindow.get(), 
                                            type));
        pluginWindow->toFront(true);
    }
}


void AudioUtils::run(const String& inputPath)
{
    readInputFolder(File(inputPath));
    auto outputFile = File(inputPath).getChildFile("output.wav");
    saveAudio(outputFile);
}


void AudioUtils::saveAudio(const File& outputFile)
{
    unsigned int NumInputChannels, NumOutputChannels;
    auto layout = createBusLayout(NumInputChannels,
                                                         NumOutputChannels);
    NumOutputChannels = 2;
    if (!pluginInstance->setBusesLayout(layout)) {
        jassertfalse;
    }
    pluginInstance->prepareToPlay(sampleRate, (int)bufferSize);

    if (outputFile.exists()) {
        jassertfalse;
    }
    outputFile.deleteFile();
    auto outputStream = outputFile.createOutputStream(bufferSize);
    if (!outputStream) {
        jassertfalse;
    }

    WavAudioFormat outputFormat;
    outputWriter.reset(outputFormat.createWriterFor(outputStream.release(),
                                                    sampleRate,
                                                    NumOutputChannels,
                                                    16,
                                                    StringPairArray(),
                                                    0));

    AudioBuffer<float> audioBuffer (audioFile->numChannels, bufferSize);
    AudioBuffer<float> sampleBuffer(NumOutputChannels, bufferSize);
    MidiBuffer midiBuffer;
    size_t sampleIndex = 0;
    while (sampleIndex < sampleSize) {
        audioBuffer.clear();
        if (!audioFile->read(audioBuffer.getArrayOfWritePointers(),
                             (int)audioFile->numChannels,
                             sampleIndex,
                             (int)bufferSize)) {
            jassertfalse;
        }

        sampleBuffer.clear();
        midiBuffer.clear();
        for (int i = 0; i < midiFile->getNumTracks(); i++) {
            auto midiTrack = midiFile->getTrack(i);

            for (auto& meh : *midiTrack) {
                auto timestampSamples = secondsToSamples(meh->message.getTimeStamp(), sampleRate);
                if (timestampSamples >= sampleIndex && timestampSamples < sampleIndex + bufferSize) {
                    midiBuffer.addEvent(meh->message, (int)(timestampSamples - sampleIndex));
                }
            }
        }
        pluginInstance->processBlock(sampleBuffer, midiBuffer);

        for (int idx = 0; idx < bufferSize; idx++) {
            float audioValue = audioBuffer.getSample(0, idx);
            sampleBuffer.addSample(0, idx, audioValue);
            sampleBuffer.addSample(1, idx, audioValue);
        }
        
        outputWriter->writeFromAudioSampleBuffer(sampleBuffer, 0, (int)bufferSize);
        sampleIndex += bufferSize;
    }
    outputWriter.reset(nullptr);
}


size_t AudioUtils::secondsToSamples(double sec, double sampleRate) 
{ 
    return (size_t)(sec * sampleRate); 
}