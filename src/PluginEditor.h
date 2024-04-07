#pragma once

#include "PluginProcessor.h"
#include <juce_gui_extra/juce_gui_extra.h>
#include <gui/choc_WebView.h>

//==============================================================================
class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor
{
  public:
    explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

  private:
    AudioPluginAudioProcessor& processorRef;

#if JUCE_MAC
    std::unique_ptr<juce::NSViewComponent> webViewContainer;
#elif JUCE_WINDOWS
    std::unique_ptr<juce::HWNDComponent> webViewContainer;
#endif

    std::unique_ptr<choc::ui::WebView> webView;
    std::unique_ptr<juce::ParameterAttachment> gainAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
