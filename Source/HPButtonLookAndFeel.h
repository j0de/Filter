/*
  ==============================================================================

    HPButtonLookAndFeel.h
    Created: 2 Mar 2019 4:06:12pm
    Author:  Joonas

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

// LookAndFeel for the high pass filter. Use with TextButton
//
class HPButtonLookAndFeel : public LookAndFeel_V4
{
public:
	void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown) override
	{
		// Button width and height
		float width = button.getWidth();
		float height = button.getHeight();

		// Path of button outline
		Path buttonPath;
		buttonPath.startNewSubPath(0.f, height * .5f);
		buttonPath.lineTo(0.f, height * .8f);
		buttonPath.lineTo(width * .2f, height);
		buttonPath.lineTo(width * .8f, height);
		buttonPath.lineTo(width, height * .8f);
		buttonPath.lineTo(width, height * .2f);
		buttonPath.lineTo(width * .8f, 0.f);
		buttonPath.lineTo(width * .2f, 0.f);
		buttonPath.lineTo(0.f, height * .2f);
		buttonPath.lineTo(0.f, height * .5f);
		// Round corners in path
		Path roundedButtonPath = buttonPath.createPathWithRoundedCorners(10.0f);
		// Set background button colour
		g.setColour(backgroundColour);
		// Fill button path
		g.fillPath(roundedButtonPath);

		// Set line colour for drawings
		// Toggle ON
		if (button.getToggleState())
			g.setColour(Colours::white);
		// Toggle OFF
		else
			g.setColour(Colours::white.darker(.8f));

		// Outline of button
		// g.strokePath(roundedButtonPath, PathStrokeType(1.f));

		// Path to draw image of the filter
		Path p;
		p.startNewSubPath(width * .25f, height);
		p.lineTo(width / 2.f, height  / 2.f);
		p.lineTo(width, height / 2.f);
		Path roundedPath = p.createPathWithRoundedCorners(20.0f);
		g.strokePath(roundedPath, PathStrokeType(1.f));
	}
};