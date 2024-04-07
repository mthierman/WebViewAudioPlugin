#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    choc::ui::WebView::Options options;
    options.enableDebugMode = true;
    options.acceptsFirstMouseClick = true;

    options.fetchResource =
        [](const std::string& path) -> std::optional<choc::ui::WebView::Options::Resource>
    {
        if (path == "/")
        {
            return choc::ui::WebView::Options::Resource{
                juce::String::createStringFromData(BinaryData::index_html,
                                                   BinaryData::index_htmlSize)
                    .toStdString(),
                choc::web::getMIMETypeFromFilename("index.html")};
        }
        else if (path == "/assets/index.css")
        {
            return choc::ui::WebView::Options::Resource{
                juce::String::createStringFromData(BinaryData::index_css, BinaryData::index_cssSize)
                    .toStdString(),
                choc::web::getMIMETypeFromFilename("index.css")};
        }
        else if (path == "/assets/index.js")
        {
            return choc::ui::WebView::Options::Resource{
                juce::String::createStringFromData(BinaryData::index_js, BinaryData::index_jsSize)
                    .toStdString(),
                choc::web::getMIMETypeFromFilename("index.js")};
        }
        else if (path == "/logo_dark.png")
        {
            juce::MemoryBlock mb;
            mb.replaceAll(BinaryData::logo_dark_png, BinaryData::logo_dark_pngSize);
            return choc::ui::WebView::Options::Resource{
                std::string(mb.begin(), mb.end()),
                choc::web::getMIMETypeFromFilename("favicon.png")};
        }
        else if (path == "/logo_light.png")
        {
            juce::MemoryBlock mb;
            mb.replaceAll(BinaryData::logo_light_png, BinaryData::logo_light_pngSize);
            return choc::ui::WebView::Options::Resource{
                std::string(mb.begin(), mb.end()),
                choc::web::getMIMETypeFromFilename("favicon.png")};
        }
        else if (path == "/favicon.ico")
        {
            juce::MemoryBlock mb;
            mb.replaceAll(BinaryData::favicon_ico, BinaryData::favicon_icoSize);
            return choc::ui::WebView::Options::Resource{
                std::string(mb.begin(), mb.end()),
                choc::web::getMIMETypeFromFilename("favicon.ico")};
        }
        else
        {
            return choc::ui::WebView::Options::Resource{
                "", choc::web::getMIMETypeFromFilename("index.html")};
        }
    };

    webView = std::make_unique<choc::ui::WebView>(options);

#if JUCE_MAC
    webViewContainer = std::make_unique<juce::NSViewComponent>();
    webViewContainer->setView(webView->getViewHandle());
#elif JUCE_WINDOWS
    webViewContainer = std::make_unique<juce::HWNDComponent>();
    webViewContainer->setHWND(webView->getViewHandle());
#endif

    addAndMakeVisible(*webViewContainer);

    auto receiveGainValue = [this](float newGainValue)
    {
        std::string javaScript = "receiveGainValue(" + std::to_string(newGainValue) + ");";
        webView->evaluateJavascript(javaScript, nullptr);
    };

    gainAttachment = std::make_unique<juce::ParameterAttachment>(
        *processorRef.getParamList().getParameter("GAIN"), receiveGainValue);

    auto onGainSliderChanged = [this](const choc::value::ValueView& args) -> choc::value::Value
    {
        auto json = juce::JSON::parse(choc::json::toString(args));

        if (json.isArray())
        {
            float gainVal = json[0].toString().getFloatValue();
            gainAttachment->setValueAsCompleteGesture(gainVal);
        }
        else { return choc::value::Value(-1); }

        return choc::value::Value(0);
    };

    webView->bind("gainValueCallback", onGainSliderChanged);

#if defined(HOT_RELOAD)
    webView->navigate("http://localhost:5173/");
#endif

    setResizable(true, true);
    setResizeLimits(400, 300, 800, 600);
    setSize(400, 300);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void AudioPluginAudioProcessorEditor::resized() { webViewContainer->setBounds(getLocalBounds()); }
