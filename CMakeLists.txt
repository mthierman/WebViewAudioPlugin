cmake_minimum_required(VERSION 3.28)

project(
    WebViewAudioPlugin
    VERSION 0.0.1
)

include(FetchContent)

FetchContent_Declare(
    juce
    URL https://github.com/juce-framework/JUCE/archive/refs/heads/master.zip
)

FetchContent_MakeAvailable(juce)

FetchContent_Declare(
    choc
    URL https://github.com/Tracktion/choc/archive/refs/heads/main.zip
)

FetchContent_MakeAvailable(choc)

set(GUI_SOURCES
    ${PROJECT_SOURCE_DIR}/gui/index.html
    ${PROJECT_SOURCE_DIR}/gui/css/index.css
    ${PROJECT_SOURCE_DIR}/gui/index.ts
    ${PROJECT_SOURCE_DIR}/gui/public/favicon.ico
    ${PROJECT_SOURCE_DIR}/gui/public/logo_dark.png
    ${PROJECT_SOURCE_DIR}/gui/public/logo_light.png
)

set(GUI_OUTPUT
    ${PROJECT_BINARY_DIR}/gui/index.html
    ${PROJECT_BINARY_DIR}/gui/assets/index.css
    ${PROJECT_BINARY_DIR}/gui/assets/index.js
    ${PROJECT_BINARY_DIR}/gui/favicon.ico
    ${PROJECT_BINARY_DIR}/gui/logo_dark.png
    ${PROJECT_BINARY_DIR}/gui/logo_light.png
)

add_custom_command(
    OUTPUT ${GUI_OUTPUT}
    COMMAND npm run build
    DEPENDS ${GUI_SOURCES}
)

add_custom_target(
    Vite
    ALL
    DEPENDS ${GUI_OUTPUT}
)

juce_add_binary_data(
    BinaryData
    HEADER_NAME
    "BinaryData.h"
    NAMESPACE
    "BinaryData"
    SOURCES
    ${GUI_OUTPUT}
)

juce_add_plugin(
    ${PROJECT_NAME}
    ICON_BIG
    ${PROJECT_SOURCE_DIR}/gui/public/icon.png
    ICON_SMALL
    ${PROJECT_SOURCE_DIR}/gui/public/icon.png
    COMPANY_NAME
    ${PROJECT_NAME}
    IS_SYNTH
    FALSE
    NEEDS_MIDI_INPUT
    FALSE
    NEEDS_MIDI_OUTPUT
    FALSE
    IS_MIDI_EFFECT
    FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS
    TRUE
    COPY_PLUGIN_AFTER_BUILD
    FALSE
    PLUGIN_MANUFACTURER_CODE
    Wvap
    PLUGIN_CODE
    Wvap
    FORMATS
    AU
    VST3
    Standalone
    PRODUCT_NAME
    ${PROJECT_NAME}
)

target_compile_features(
    ${PROJECT_NAME}
    PRIVATE cxx_std_20
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE ${choc_SOURCE_DIR}
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE src/PluginEditor.cpp
            src/PluginProcessor.cpp
)

target_compile_definitions(
    ${PROJECT_NAME}
    PUBLIC JUCE_WEB_BROWSER=0
           JUCE_USE_CURL=0
           JUCE_VST3_CAN_REPLACE_VST2=0
           $<$<CONFIG:Debug>:HOT_RELOAD=1>
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE BinaryData
            juce::juce_audio_utils
            juce::juce_gui_extra
    PUBLIC juce::juce_recommended_config_flags
           juce::juce_recommended_lto_flags
           juce::juce_recommended_warning_flags
)

add_dependencies(
    BinaryData
    Vite
)
