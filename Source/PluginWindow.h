#include <JuceHeader.h>

inline String getFormatSuffix(const AudioProcessor* plugin)
{
	const auto format = [plugin]()
	{
		if (auto* instance = dynamic_cast<const AudioPluginInstance*> (plugin))
			return instance->getPluginDescription().pluginFormatName;

		return String();
	}();

	return format.isNotEmpty() ? (" (" + format + ")") : format;
}

class PluginWindow : public DocumentWindow
{
public:
	enum class Type
	{
		normal = 0,
		generic,
		programs,
		audioIO,
		debug,
		numTypes
	};

	PluginWindow(AudioPluginInstance* pluginInstance,
				 PluginWindow* pluginWindow,
				 Type t)
		: DocumentWindow(pluginInstance->getName() + getFormatSuffix(pluginInstance), 
						 LookAndFeel::getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId),
						 DocumentWindow::allButtons),
		  type(t)
	{
		setSize(400, 300);
		pluginWindowManager.reset(pluginWindow);

		ui.reset(createProcessorEditor(pluginInstance, type));
		setContentOwned(ui.get(), true);
		setResizable(ui->isResizable(), false);

		setVisible(true);
	}

	~PluginWindow() override
	{
		ui.reset(nullptr);
		clearContentComponent();
	}

	void closeButtonPressed() override
	{
		setVisible(false);
	}

	const Type type;

private:

	static AudioProcessorEditor* createProcessorEditor(AudioPluginInstance* pluginInstance,
													   PluginWindow::Type type)
	{
		if (type == PluginWindow::Type::normal)
		{
			if (pluginInstance->hasEditor())
				if (auto* ui = pluginInstance->createEditorIfNeeded())
					return ui;

			type = PluginWindow::Type::generic;
		}

		if (type == PluginWindow::Type::generic)  return new GenericAudioProcessorEditor(pluginInstance);
		/*if (type == PluginWindow::Type::programs) return new ProgramAudioProcessorEditor(pluginInstance);
		if (type == PluginWindow::Type::audioIO)  return new IOConfigurationWindow(pluginInstance);
		if (type == PluginWindow::Type::debug)    return new PluginDebugWindow(pluginInstance);*/

		jassertfalse;
		return {};
	}
	
	std::unique_ptr<PluginWindow> pluginWindowManager;
	std::unique_ptr<AudioProcessorEditor> ui;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginWindow)
};