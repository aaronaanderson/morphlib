#include "morphlib/AutoFitTextBox.h"

using namespace morph;



AutoFitTextBox::AutoFitTextBox (const juce::String& componentName,
                                const juce::String& labelText)
    : juce::Label (componentName, labelText) {}

    // Set minimum and maximum font size in points
void AutoFitTextBox::setFitRange (float minPt, float maxPt, float searchTolerance)
{
    jassert (minPt > 0.0f && maxPt >= minPt);
    minPoint   = minPt;
    maxPoint   = maxPt;
    tolerance  = searchTolerance;
    triggerAsyncUpdate();
}

void AutoFitTextBox::resized()            { triggerAsyncUpdate(); }
void AutoFitTextBox::lookAndFeelChanged() { triggerAsyncUpdate(); }
void AutoFitTextBox::parentSizeChanged()  { triggerAsyncUpdate(); }
void AutoFitTextBox::visibilityChanged()  { triggerAsyncUpdate(); }
void AutoFitTextBox::textWasChanged()     { triggerAsyncUpdate(); }

void AutoFitTextBox::handleAsyncUpdate()  { refreshFit(); }

void AutoFitTextBox::refreshFit()
{
    auto area = getLocalBounds();
    if (area.isEmpty() || getText().isEmpty())
        return;

    float lo = minPoint, hi = maxPoint, best = minPoint;

    // Binary search for the largest size that fits
    while (hi - lo > tolerance)
    {
        float mid = 0.5f * (lo + hi);
        if (fits (getText(), mid, area)) { best = mid; lo = mid; }
        else hi = mid;
    }

    juce::Font f = getFont();
    f.setHeight (best);
    setFont (f);
}

bool AutoFitTextBox::fits (const juce::String& text, float pt, juce::Rectangle<int> area) const
{
    juce::AttributedString as;
    as.setJustification (getJustificationType());
    as.append (text, juce::Font (pt));

    juce::TextLayout tl;
    tl.createLayout (as, (float) area.getWidth());

    return tl.getHeight() <= (float) area.getHeight();
}