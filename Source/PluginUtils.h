#pragma once
#include <JuceHeader.h>

enum class AutoScale
{
    scaled,
    unscaled,
    useDefault
};


//AutoScale getAutoScaleValueForPlugin(const String& identifier)
//{
//    if (identifier.isNotEmpty())
//    {
//        auto plugins = StringArray::fromLines(getAppProperties().getUserSettings()->getValue("autoScalePlugins"));
//        plugins.removeEmptyStrings();
//
//        for (auto& plugin : plugins)
//        {
//            auto fromIdentifier = plugin.fromFirstOccurrenceOf(identifier, false, false);
//
//            if (fromIdentifier.isNotEmpty())
//                return autoScaleFromString(fromIdentifier.fromFirstOccurrenceOf(":", false, false));
//        }
//    }
//
//    return AutoScale::useDefault;
//}

static bool isAutoScaleAvailableForPlugin(const PluginDescription& description)
{
    return autoScaleOptionAvailable
        && description.pluginFormatName.containsIgnoreCase("VST");
}

bool shouldAutoScalePlugin(const PluginDescription& description)
{
    if (!isAutoScaleAvailableForPlugin(description))
        return false;

    /*const auto scaleValue = getAutoScaleValueForPlugin(description.fileOrIdentifier);

    return (scaleValue == AutoScale::scaled
              || (scaleValue == AutoScale::useDefault
                  && getAppProperties().getUserSettings()->getBoolValue("autoScalePluginWindows")));*/

    return true;
}

static std::unique_ptr<ScopedDPIAwarenessDisabler> makeDPIAwarenessDisablerForPlugin(const PluginDescription& desc)
{
    return shouldAutoScalePlugin(desc) ? std::make_unique<ScopedDPIAwarenessDisabler>()
        : nullptr;
}
