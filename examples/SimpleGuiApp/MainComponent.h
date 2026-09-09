#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

class MainComponent : public juce::Component,
                       public juce::Button::Listener,
                       public juce::Slider::Listener
{
public:
    MainComponent();
    ~MainComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

private:
    juce::TextButton helloButton{"Click Me!"};
    juce::Slider valueSlider;
    juce::Label sliderLabel;
    juce::Label outputLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
