#include "MainComponent.h"

MainComponent::MainComponent()
{
    helloButton.addListener(this);
    addAndMakeVisible(helloButton);

    sliderLabel.setText("Value:", juce::dontSendNotification);
    addAndMakeVisible(sliderLabel);

    valueSlider.setRange(0.0, 100.0);
    valueSlider.setValue(50.0);
    valueSlider.addListener(this);
    addAndMakeVisible(valueSlider);

    outputLabel.setText("Output: 50", juce::dontSendNotification);
    outputLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(outputLabel);

    setSize(500, 300);
}

MainComponent::~MainComponent()
{
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText("Simple JUCE GUI Application", getLocalBounds().removeFromTop(60),
               juce::Justification::centredTop, true);
}

void MainComponent::resized()
{
    auto area = getLocalBounds().reduced(20);

    auto titleArea = area.removeFromTop(80);
    titleArea.removeFromTop(60);

    helloButton.setBounds(area.removeFromTop(40).withSizeKeepingCentre(150, 40));

    area.removeFromTop(20);

    auto sliderArea = area.removeFromTop(50);
    sliderLabel.setBounds(sliderArea.removeFromLeft(80));
    valueSlider.setBounds(sliderArea);

    area.removeFromTop(20);
    outputLabel.setBounds(area.removeFromTop(40));
}

void MainComponent::buttonClicked(juce::Button* button)
{
    if (button == &helloButton)
    {
        juce::AlertWindow::showMessageBox(
            juce::AlertWindow::InfoIcon,
            "Hello!",
            "You clicked the button!\n\nCurrent slider value: " + juce::String(valueSlider.getValue(), 1));
    }
}

void MainComponent::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &valueSlider)
    {
        outputLabel.setText("Output: " + juce::String((int)valueSlider.getValue()),
                           juce::dontSendNotification);
    }
}
