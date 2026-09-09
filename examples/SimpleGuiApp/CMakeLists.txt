# ==============================================================================
#
#  Simple JUCE GUI Application
#  A basic example demonstrating JUCE GUI components
#
# ==============================================================================

juce_add_gui_app(SimpleGuiApp
    PRODUCT_NAME "Simple GUI App")

target_sources(SimpleGuiApp PRIVATE
    Main.cpp
    MainComponent.h
    MainComponent.cpp)

target_compile_features(SimpleGuiApp PRIVATE cxx_std_17)

target_link_libraries(SimpleGuiApp PRIVATE
    juce::juce_gui_extra
    juce::juce_gui_basics
    juce::juce_core
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)
