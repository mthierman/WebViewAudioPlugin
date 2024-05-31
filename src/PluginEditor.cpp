#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Resource.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    choc::ui::WebView::Options options;
    options.enableDebugMode = true;
    options.acceptsFirstMouseClick = true;

    options.fetchResource =
        [=](const std::string& path) -> std::optional<choc::ui::WebView::Options::Resource>
    {
        std::vector<Resource> resources;

        resources.emplace_back(Resource("/", "index_html"));
        resources.emplace_back(Resource("/assets/index.css", "index_css"));
        resources.emplace_back(Resource("/assets/index.js", "index_js"));
        resources.emplace_back(Resource("/logo_dark.png", "logo_dark_png"));
        resources.emplace_back(Resource("/logo_light.png", "logo_light_png"));
        resources.emplace_back(Resource("/favicon.ico", "favicon_ico"));

        for (const auto& res : resources)
        {
            if (path == res.m_path) { return res.m_resource; }
        }

        return choc::ui::WebView::Options::Resource{"", "text/plain"};
    };

    webView = std::make_unique<choc::ui::WebView>(options);

#if JUCE_MAC
    webViewContainer = std::make_unique<juce::NSViewComponent>();
    webViewContainer->setView(webView->getViewHandle());
#elif JUCE_WINDOWS
    webViewContainer = std::make_unique<juce::HWNDComponent>();
    webViewContainer->setHWND(webView->getViewHandle());
#endif

    // addAndMakeVisible(*webViewContainer);
    addChildComponent(*webViewContainer);

    auto receiveGainValue{[this](float newGainValue)
    {
        webView->evaluateJavascript("receiveGainValue(" + std::to_string(newGainValue) + ");",
                                    nullptr);
    }};

    gainAttachment = std::make_unique<juce::ParameterAttachment>(
        *processorRef.getParamList().getParameter("GAIN"), receiveGainValue);

    auto onGainSliderChanged{[this](const choc::value::ValueView& args) -> choc::value::Value
    {
        auto json{juce::JSON::parse(choc::json::toString(args))};

        if (json.isArray())
        {
            auto gainVal{json[0].toString().getFloatValue()};
            gainAttachment->setValueAsCompleteGesture(gainVal);
        }

        else { return choc::value::Value(-1); }

        return choc::value::Value(0);
    }};

    webView->bind("gainValueCallback", onGainSliderChanged);

#if defined(HOT_RELOAD)
    webView->navigate("http://localhost:5173/");
#endif

    setResizable(true, true);
    setResizeLimits(400, 300, 800, 600);
    setSize(400, 300);

    webViewContainer->setVisible(true);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void AudioPluginAudioProcessorEditor::resized() { webViewContainer->setBounds(getLocalBounds()); }
