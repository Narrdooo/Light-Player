#pragma once
#include <JuceHeader.h>
#include "IODeviceManager.h"
#include "PluginWindow.h"

constexpr bool autoScaleOptionAvailable =
#if JUCE_WINDOWS && JUCE_WIN_PER_MONITOR_DPI_AWARE
true;
#else
false;
#endif

class AudioUtils
{
public:
	AudioUtils();
	~AudioUtils();


    void loadPluginFromPath(const String pluginPath);
	
	size_t secondsToSamples(double sec, double sampleRate);
	void loadDefaultPlugin();
	void createPluginInstance(const int idx);
	void createPluginInstance(const int idx, PluginDescription pluginDescription);
	void readInputFolder(const File& inputPath);
	void readAudioFile(const File& audioPath);
	void readMidiFile(const File& midiPath);
	void run(const String& InputPath);
	AudioPluginInstance::BusesLayout createBusLayout(unsigned int& totalNumInputChannels,
													 unsigned int& totalNumOutputChannels);

	void getOrCreatePluginWindow();

	void saveAudio(const File& outputPath);		// ²Î¿¼plugalyzer


private:
	double sampleRate = 44100;
	unsigned int bufferSize = 1024;
	size_t sampleIndex = 0;
	size_t sampleSize = 0;
	std::optional<unsigned int> outputChannelCountOpt;

	OwnedArray<PluginDescription> typesFound;
	KnownPluginList knownPluginList;
	AudioPluginFormatManager formatManager;

	std::unique_ptr<MidiFile> midiFile;
	std::unique_ptr<AudioFormatReader> audioFile;
	std::unique_ptr<AudioPluginInstance> pluginInstance;
	std::unique_ptr<AudioFormatWriter> outputWriter;

	std::unique_ptr<PluginWindow> pluginWindow;

	//std::unique_ptr<DeviceManagerWindow> deviceManagerWinodw;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioUtils)
};


