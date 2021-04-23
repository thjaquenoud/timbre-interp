/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <vector>
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
struct SliderLookAndFeel : public juce::LookAndFeel_V4
{
    void drawRoundThumb (juce::Graphics& g, float x, float y, float diameter, juce::Colour colour, float outlineThickness)
    {
        auto halfThickness = outlineThickness * 0.5f;

        juce::Path p;
        p.addEllipse (x + halfThickness,
                      y + halfThickness,
                      diameter - outlineThickness,
                      diameter - outlineThickness);

        juce::DropShadow (juce::Colours::black, 1, {}).drawForPath (g, p);

        g.setColour (colour);
        g.fillPath (p);

        g.setColour (colour.brighter());
        g.strokePath (p, juce::PathStrokeType (outlineThickness));
    }
    
    void drawLinearSliderThumb (juce::Graphics& g, int x, int y, int width, int height,
                                float sliderPos, float minSliderPos, float maxSliderPos,
                                const juce::Slider::SliderStyle style, juce::Slider& slider) override
    {
        auto sliderRadius = (float) (getSliderThumbRadius (slider) - 2);

        auto isDownOrDragging = slider.isEnabled() && (slider.isMouseOverOrDragging() || slider.isMouseButtonDown());

        auto knobColour = style == juce::Slider::LinearVertical ? slider.findColour (juce::Slider::thumbColourId)
                                .withMultipliedSaturation ((slider.hasKeyboardFocus (false) || isDownOrDragging) ? 1.3f : 0.9f)
                                .withMultipliedAlpha (slider.isEnabled() ? 1.0f : 0.7f) : juce::Colour((juce::uint8)255, (juce::uint8)0, (juce::uint8)0);

        if (style == juce::Slider::LinearHorizontal || style == juce::Slider::LinearVertical)
        {
            float kx, ky;

            if (style == juce::Slider::LinearVertical)
            {
                kx = (float) x + (float) width * 0.5f;
                ky = sliderPos;
            }
            else
            {
                kx = sliderPos;
                ky = (float) y + (float) height * 0.5f;
            }

            auto outlineThickness = slider.isEnabled() ? 0.8f : 0.3f;

            drawRoundThumb (g,
                            kx - sliderRadius,
                            ky - sliderRadius,
                            sliderRadius * 2.0f,
                            knobColour, outlineThickness);
        }
        else
        {
            // Just call the base class for the demo
            juce::LookAndFeel_V2::drawLinearSliderThumb (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
        }
    }

    void drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           const juce::Slider::SliderStyle style, juce::Slider& slider) override
    {
        g.fillAll (juce::Colour((juce::uint8)255, (juce::uint8)255, (juce::uint8)255, 0.3f));

        if (style == juce::Slider::LinearBar || style == juce::Slider::LinearBarVertical)
        {
            juce::Path p;

            if (style == juce::Slider::LinearBarVertical)
                p.addRectangle ((float) x, sliderPos, (float) width, 1.0f + (float) height - sliderPos);
            else
                p.addRectangle ((float) x, (float) y, sliderPos - (float) x, (float) height);

            auto baseColour = slider.findColour (juce::Slider::rotarySliderFillColourId)
                                    .withMultipliedSaturation (slider.isEnabled() ? 1.0f : 0.5f)
                                    .withMultipliedAlpha (0.8f);

            g.setColour (baseColour);
            g.fillPath (p);

            auto lineThickness = juce::jmin (15.0f, (float) juce::jmin (width, height) * 0.45f) * 0.1f;
            g.drawRect (slider.getLocalBounds().toFloat(), lineThickness);
        }
        else
        {
            drawLinearSliderBackground (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
            drawLinearSliderThumb      (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
        }
    }
    
    void drawLinearSliderBackground (juce::Graphics& g, int x, int y, int width, int height,
                                     float sliderPos,
                                     float minSliderPos,
                                     float maxSliderPos,
                                     const juce::Slider::SliderStyle style, juce::Slider& slider) override
    {
        auto sliderRadius = (float) getSliderThumbRadius (slider) - 5.0f;
        juce::Path on, off;

        if (slider.isHorizontal())
        {
            auto iy = (float) y + (float) height * 0.5f - sliderRadius * 0.5f;
            juce::Rectangle<float> r ((float) x - sliderRadius * 0.5f, iy, (float) width + sliderRadius, sliderRadius);
            auto onW = r.getWidth() * ((float) slider.valueToProportionOfLength (slider.getValue()));

            on.addRectangle (r.removeFromLeft (onW));
            off.addRectangle (r);
        }
        else
        {
            auto ix = (float) x + (float) width * 0.5f - sliderRadius * 0.5f;
            juce::Rectangle<float> r (ix, (float) y - sliderRadius * 0.5f, sliderRadius, (float) height + sliderRadius);
            auto onH = r.getHeight() * ((float) slider.valueToProportionOfLength (slider.getValue()));

            on.addRectangle (r.removeFromBottom (onH));
            off.addRectangle (r);
        }

        g.setColour (slider.findColour (juce::Slider::trackColourId));
        g.fillPath (on);

        g.setColour (slider.findColour (juce::Slider::trackColourId));
        g.fillPath (off);
    }
};

struct ButtonLookAndFeel : public juce::LookAndFeel_V4
{
    void drawButtonText (juce::Graphics& g, juce::TextButton& button, bool, bool isButtonDown) override
    {
        auto font = juce::Font(0.75*button.getHeight());
        g.setFont (font);
        g.setColour (button.findColour (button.getToggleState() ? juce::TextButton::textColourOnId
                                                                : juce::TextButton::textColourOffId)
                           .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f));

        auto yIndent = juce::jmin (4, button.proportionOfHeight (0.3f));
        auto cornerSize = juce::jmin (button.getHeight(), button.getWidth()) / 2;

        auto fontHeight = juce::roundToInt (font.getHeight() * 0.6f);
        auto leftIndent  = juce::jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnLeft()  ? 4 : 2));
        auto rightIndent = juce::jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
        auto textWidth = button.getWidth() - leftIndent - rightIndent;

        auto edge = 4;
        auto offset = isButtonDown ? edge / 2 : 0;

        if (textWidth > 0)
            g.drawFittedText (button.getButtonText(),
                              leftIndent + offset, yIndent + offset, textWidth, button.getHeight() - yIndent * 2 - edge,
                              juce::Justification::centred, 2);
    }

};

class MusicAEAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                     private juce::Slider::Listener
{
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MusicAEAudioProcessor& audioProcessor;
        
    SliderLookAndFeel sliderLookAndFeel;
    ButtonLookAndFeel buttonLookAndFeel;
    
    juce::Slider alpha;
    juce::OwnedArray<juce::Slider> latentSliders;
    
    enum RadioButtonIds
    {
        StateButtons = 1001
    };
    
    void startStop(bool start);
    void reset();
    void sliderValueChanged(juce::Slider* slider) override;
    void onStateChange(enum MusicAE_state new_state);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MusicAEAudioProcessorEditor)
    
public:
    MusicAEAudioProcessorEditor (MusicAEAudioProcessor&);
    ~MusicAEAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    juce::TextButton synthButton {"Synth"}, effectsButton {"Effects"}, mixerButton {"Mixer"},
        loadButton {"Load"}, startButton {"Start"}, resetButton {"Reset"}, stopButton {"Stop"};
    juce::TextEditor modelTextBox;
};
