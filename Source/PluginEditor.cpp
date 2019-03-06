/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#define _USE_MATH_DEFINES
#include <math.h>

//==============================================================================
FilterAudioProcessorEditor::FilterAudioProcessorEditor (FilterAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p),
	  mParameters(p.getState()),
	  mMagView(p.getState())
{
	// Plugin size
	// Initialise GUI elements
	initialiseGUI();
	this->setResizable(true, true);
	float maxWidth = 600.f;
	float maxHeight = 400.f;
	this->setResizeLimits(160, 160, maxWidth, maxHeight);
    setSize (270.f, 271.f);
}

FilterAudioProcessorEditor::~FilterAudioProcessorEditor() {}

//==============================================================================
void FilterAudioProcessorEditor::paint (Graphics& g) 
{
	// Plugin background
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId)); 
}

void FilterAudioProcessorEditor::resized()
{
	// GUI parameters===============================================

	// GUI area
	auto area = getLocalBounds().reduced(10.f, 10.f);

	// MagView
	float magViewRatio = mMagViewRatio;
	float magViewMinHeight = 80.f;
	float magViewMinWidth = magViewMinHeight * magViewRatio;
	float magViewHeight = (area.getWidth() / magViewRatio);
	
	//Knobs
	float knobWidth = 60.f;
	float knobHeight = knobWidth;
	float labelHeight = mLabelFontSize;

	// Buttons
	float buttonWidth = ((2.f * knobWidth) / 3.f) - 6.f;
	float buttonHeight = buttonWidth / 1.6f;

	// parameterBox = knobs + buttons
	float parameterWidth = 2 * knobWidth;
	float spaceBetween = 20.f;
	float parameterHeight = labelHeight + knobHeight + buttonHeight + spaceBetween;
	
	// Portrait or landscape mode (true if portrait)
	bool isPortrait = area.getHeight() * 1.5f > area.getWidth();
	// Is magView shown (portrait & landscape conditions)
	bool magViewPortraitCondition = area.getHeight() - parameterHeight > magViewMinHeight;
	bool magViewLandscapeCondition = area.getWidth() - parameterWidth - spaceBetween > magViewMinWidth;

	float magViewPortraitWidth = area.getWidth();
	float magViewPortraitHeight = magViewPortraitWidth / magViewRatio;

	float magViewLandscapeWidth = area.getWidth() - parameterWidth - spaceBetween;
	float magViewCurrHeightLandspace = magViewLandscapeWidth / magViewRatio;
	float magViewLandscapeHeight = magViewCurrHeightLandspace < area.getHeight() ? magViewCurrHeightLandspace : area.getHeight();

	// Perform layout with FlexBox==================================
	// Frequency knob
	FlexBox fcBox;
	fcBox.justifyContent = FlexBox::JustifyContent::center;
	fcBox.alignContent   = FlexBox::AlignContent::center;
	fcBox.flexDirection  = FlexBox::Direction::column;
	fcBox.items.addArray({ FlexItem(mFcLabel).withWidth(knobWidth).withHeight(labelHeight),
						   FlexItem(mFcSlider).withWidth(knobWidth).withHeight(knobHeight) });
	// Resonance knob
	FlexBox resBox;
	resBox.justifyContent = FlexBox::JustifyContent::center;
	resBox.alignContent   = FlexBox::AlignContent::center;
	resBox.flexDirection  = FlexBox::Direction::column;
	resBox.items.addArray({ FlexItem(mResLabel).withWidth(knobWidth).withHeight(labelHeight),
						    FlexItem(mResSlider).withWidth(knobWidth).withHeight(knobHeight) }); 

	// Both kobs and their labels
	FlexBox sliderBox;
	sliderBox.justifyContent = FlexBox::JustifyContent::center;
	sliderBox.alignContent   = FlexBox::AlignContent::center;
	sliderBox.flexDirection  = FlexBox::Direction::row;
	sliderBox.items.addArray({ FlexItem(fcBox).withHeight(labelHeight + knobHeight).withWidth(knobWidth), 
							   FlexItem(resBox).withHeight(labelHeight + knobHeight).withWidth(knobWidth)});

	// Filter type buttons
	FlexBox buttonBox;
	buttonBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	buttonBox.alignContent   = FlexBox::AlignContent::center;
	buttonBox.flexDirection  = FlexBox::Direction::row;
	buttonBox.items.addArray({ FlexItem(mLPButton).withWidth(buttonWidth).withHeight(buttonHeight),
							   FlexItem(mHPButton).withWidth(buttonWidth).withHeight(buttonHeight),
							   FlexItem(mBPButton).withWidth(buttonWidth).withHeight(buttonHeight) });

	// Parameter control box (knobs + buttons)
	FlexBox parameterBox;
	parameterBox.justifyContent = FlexBox::JustifyContent::center;
	parameterBox.alignContent   = FlexBox::AlignContent::center;
	parameterBox.flexDirection  = FlexBox::Direction::column;
	parameterBox.items.addArray({ FlexItem(sliderBox).withWidth(parameterWidth).withHeight(knobHeight + labelHeight),
							      FlexItem(buttonBox).withWidth(parameterWidth).withHeight(buttonHeight + spaceBetween)});
	
	// Master box that lays out all the components in  the window
	FlexBox masterBox;
	FlexBox magBox;
	magBox.justifyContent = FlexBox::JustifyContent::center;
	magBox.alignContent   = FlexBox::AlignContent::center;
	magBox.flexDirection  = FlexBox::Direction::column;

	// Portrait mode
	if (isPortrait)
	{

		// masterBox settings
		masterBox.alignContent   = FlexBox::AlignContent::center;
		masterBox.justifyContent = FlexBox::JustifyContent::center;
		masterBox.flexDirection  = FlexBox::Direction::column;
		float parameterBoxSpace = 20.f;

		if (magViewPortraitCondition)
		{
			// MagView box
			magBox.items.add(FlexItem(mMagView).withWidth (magViewPortraitWidth).withHeight (magViewPortraitHeight));
			
			mMagView.setVisible(true);
			masterBox.items.addArray({ FlexItem(magBox).withWidth(area.getWidth()).withHeight(magViewPortraitHeight),
									   FlexItem(parameterBox).withWidth(area.getWidth()).withHeight(parameterHeight + parameterBoxSpace) });
		}
		else
		{
			mMagView.setVisible(false);
			masterBox.items.addArray({ FlexItem(parameterBox).withWidth(parameterWidth).withHeight(parameterHeight) });
		}
	}
	// Landscape mode
	else
	{

		// masterBox settings
		masterBox.alignContent   = FlexBox::AlignContent::spaceAround;
		masterBox.justifyContent = magViewLandscapeCondition ? FlexBox::JustifyContent::spaceBetween : FlexBox::JustifyContent::center;
		masterBox.flexDirection  = FlexBox::Direction::row;

		float parameterBoxSpace = 20.f;

		if (magViewLandscapeCondition)
		{
			mMagView.setVisible(true);
			// MagView box
			magBox.items.add(FlexItem(mMagView).withWidth (magViewLandscapeWidth).withHeight (magViewLandscapeHeight));
			
			masterBox.items.addArray({ FlexItem(magBox).withWidth(magViewLandscapeWidth).withHeight(area.getHeight()),
									   FlexItem(parameterBox).withWidth(parameterWidth + parameterBoxSpace).withHeight(area.getHeight()) });
		}
		else
		{
			mMagView.setVisible(false);
			masterBox.items.addArray({ FlexItem(parameterBox).withWidth(parameterWidth).withHeight(area.getHeight()) });
		}
	}
	// Perform layout
	masterBox.performLayout(area.toFloat());
}

void FilterAudioProcessorEditor::initialiseGUI()
{
	// Set up sliders
	mFcSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	mResSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	mFcSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 1.f, 1.f);
	mResSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 1.f, 1.f);
	addAndMakeVisible(mFcSlider);
	addAndMakeVisible(mResSlider);
	mFcSlider.setLookAndFeel(&knobLookAndFeel);
	mResSlider.setLookAndFeel(&knobLookAndFeel);
	// Slider attachments
	mFcAttachment.reset(new SliderAttachment(mParameters, "fc", mFcSlider));
	mResAttachment.reset(new SliderAttachment(mParameters, "res", mResSlider));
	// fc slider into log10 scale
	mFcSlider.setSkewFactorFromMidPoint(1000.0);

	// Set up Labels
	mFcLabel.setText("Freq", dontSendNotification);
	mResLabel.setText("Res", dontSendNotification);
	mSelectLabel.setText("Type",dontSendNotification);
	mFcLabel.setJustificationType(Justification::centred);
	mResLabel.setJustificationType(Justification::centred);
	mSelectLabel.setJustificationType(Justification::centred);
	mFcLabel.setFont(mLabelFontSize);
	mResLabel.setFont(mLabelFontSize);
	mSelectLabel.setFont(mLabelFontSize);
	addAndMakeVisible(mResLabel);
	addAndMakeVisible(mFcLabel);
	addAndMakeVisible(mSelectLabel);
	addAndMakeVisible(mMagView);
	// Set up buttons
	float buttonWidth = 20.f;
	float buttonHeight = 15.f;
	mLPButton.setSize(buttonWidth, buttonHeight);
	mLPButton.setLookAndFeel(&mLPButtonLookAndFeel);
	addAndMakeVisible(mLPButton);
	mHPButton.setSize(buttonWidth, buttonHeight);
	mHPButton.setLookAndFeel(&mHPButtonLookAndFeel);
	addAndMakeVisible(mHPButton);
	mBPButton.setSize(buttonWidth, buttonHeight);
	mBPButton.setLookAndFeel(&mBPButtonLookAndFeel);
	addAndMakeVisible(mBPButton);
	mLPButton.addListener(this);
	mHPButton.addListener(this);
	mBPButton.addListener(this);
	mLPButton.setClickingTogglesState(true);
	mHPButton.setClickingTogglesState(true);
	mBPButton.setClickingTogglesState(true);
	int filterType = *mParameters.getRawParameterValue("selectFilter");
	if (filterType == 0)
		mLPButton.setToggleState(true, true);
	else if (filterType == 1)
		mHPButton.setToggleState(true, true);
	else
		mBPButton.setToggleState(true, true);
}

void FilterAudioProcessorEditor::buttonClicked(Button* b)
{
	Value filterType = mParameters.getParameterAsValue("selectFilter");
	// Lowpass button
	if (b == &mLPButton)
	{
		filterType = 0;
		if (mLPButton.getToggleState())
		{
			if (mHPButton.getToggleState())
			{
				mHPButton.setToggleState(false, false);
				mLPButton.setToggleState(true, false);
			}
			if (mBPButton.getToggleState())
			{
				mBPButton.setToggleState(false, false);
				mLPButton.setToggleState(true, false);
			}
		}
	}
	if (b == &mHPButton)
	{
		filterType = 1;
		if (mHPButton.getToggleState())
		{
			if (mLPButton.getToggleState())
			{
				mLPButton.setToggleState(false, false);
				mHPButton.setToggleState(true, false);
			}
			if (mBPButton.getToggleState())
			{
				mBPButton.setToggleState(false, false);
				mHPButton.setToggleState(true, false);
			}
		}
	}
	if (b == &mBPButton)
	{
		filterType = 2;
		if (mBPButton.getToggleState())
		{
			if (mLPButton.getToggleState())
			{
				mLPButton.setToggleState(false, false);
				mBPButton.setToggleState(true, false);
			}
			if (mHPButton.getToggleState())
			{
				mHPButton.setToggleState(false, false);
				mBPButton.setToggleState(true, false);
			}
		}
	}
}
