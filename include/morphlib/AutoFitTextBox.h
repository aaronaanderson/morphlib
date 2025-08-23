#pragma once


#include <juce_gui_basics/juce_gui_basics.h>

/**
    AutoFitLabel
    ------------
    A JUCE Label subclass that automatically adjusts its font size so that
    its text fits within the current bounds.

    - Works on resize, look-and-feel change, and text change.
    - Uses a binary search to find the largest font size that still fits.
    - Optional min/max point size range.

    ChatGPT wrote this, I don't like to spend time on such things - AA
*/

namespace morph
{
class AutoFitTextBox : public juce::Label,
                       private juce::AsyncUpdater
{
public:
    AutoFitTextBox() = default;

    AutoFitTextBox (const juce::String& componentName,
                    const juce::String& labelText);

    // Set minimum and maximum font size in points
    void setFitRange (float minPt, float maxPt, float searchTolerance = 0.25f);

    void resized()            override;           
    void lookAndFeelChanged() override;
    void parentSizeChanged()  override; 
    void visibilityChanged()  override; 

protected:
    // Called whenever text is changed via setText()
    void textWasChanged()     override;

    // AsyncUpdater callback
    void handleAsyncUpdate()  override;

private:
    void refreshFit();
    bool fits (const juce::String& text, float pt, juce::Rectangle<int> area) const;

    // State
    float minPoint   = 9.0f;
    float maxPoint   = 24.0f;
    float tolerance  = 0.25f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AutoFitTextBox)
};
}