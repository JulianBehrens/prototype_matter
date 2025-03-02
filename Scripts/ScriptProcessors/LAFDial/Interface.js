Content.makeFrontInterface(600, 600);

// ------------- HISE Constants

// Define HISE Constants.
const WIDGET_STARTX = 0;
const WIDGET_STARTY = 1;
const WIDGET_WIDTH = 2;
const WIDGET_HEIGHT = 3;





// ------------- Toolkit Default Fonts

// Load the fonts from the "Images" folder in your project folder.
//Engine.loadFontAs("{PROJECT_FOLDER}oxygen.regular.ttf", 'OxygenRegular');
//Engine.loadFontAs("{PROJECT_FOLDER}oxygen.bold.ttf", 'OxygenBold');


// Define the font defaults.
const DEFAULT_MEDIUMFONT = "OxygenRegular";
const DEFAULT_BOLDFONT = "OxygenBold";
const DEFAULT_LABEL_FONTSIZE = 12;
const DEFAULT_SMALL_FONTSIZE = 10;


//////////////////////////// STANDARD KNOB //////////////////////////////////
//
// This is a drop-in replacement for HISE's standard knob. You can simply 
// assign this LAF to your existing slider, and it will maintain it's 
// existing style you specified in the Inspector. The customisable LAF 
// variables I've added additional control over look and feel for:
//
// • Mouse-over events (all button state configurations).
// • Indicator values (off and on).
// • Separate colours for the text legend and value.
// • Disabled state colours (all graphic elements).
//
// ------------- Inspector Colour Mapping
//
// This LAF maps the same Colours (in the Inspector) as HISE's slider.
//
// • bgColour for the border.
// • itemColour is the top gradient of the knob surface.
// • itemColour2 bottom gradient of the knob surface.
// • textColour is the text legend.
//
/////////////////////////////////////////////////////////////////////////////

// ------------- Basic Operations

// Create an LAF for the widget.
const LAF_StandardKnob = Content.createLocalLookAndFeel();

// Register and define the drawing function for the knob.
LAF_StandardKnob.registerFunction ("drawRotarySlider", function (graphics,widget) {
	
	// ------------- Initialisation
	
	// Create descriptive variables name for the widget's properties.
	var widgetArea = widget.area;
	var widgetHeight = widget.area [WIDGET_HEIGHT];
	var widgetWidth = widget.area [WIDGET_WIDTH];
	var widgetWIDGET_STARTX = widget.area [WIDGET_STARTX];
	var widgetStartY = widget.area [WIDGET_STARTY];
		
	// Set customisable LAF variables.
	var widgetIndicatorColour = 0xFF00FF00;
	var enabledKnobwidgetValueColour = 0XFF838387;
	var disabledKnobwidgetValueColour= 0xFF424246;
	var widgetKnobActiveColour = 0xFFC1C1C2;
	var widgetKnobMargin = widgetHeight/4;
	var widgetHorizontalTextOffset = widgetHeight;
	var widgetLegendTextVerticalOffset = -8;
	var widgetValueTextVerticalOffset = 6;
	var widgetOffsetAdjustment = 0.08;
	var numberOfDecimalPlaces = 0;
	var opacity;

	// ------------- Knob Surface
	
	// Specify and store the knob's desired surface in an array.
	var KnobSurfaceArea = [widgetKnobMargin, widgetKnobMargin, widgetHeight - (2 * widgetKnobMargin), widgetHeight - ( 2* widgetKnobMargin)];

	// Set the knob surface's colour information.
	if (widget.enabled) {
		
		// If the the widget is enabled, and the mouse is over it or 
		// its been clicked, then set opacity to 100%; otherwise set 
		// it to 80%.
		if ( widget.hover || widget.clicked) { opacity = 1.0; } else { opacity = 0.8; }	
	
		// Create a colour gradient from itemColour1 to itemColour2 with 
		// whatever opacity was set.
		graphics.setGradientFill ([Colours.withAlpha(widget.itemColour1, opacity), widgetKnobMargin / 2, widgetKnobMargin / 2, Colours.withAlpha(widget.itemColour2, opacity), widgetHeight-widgetKnobMargin, widgetHeight-widgetKnobMargin]);
		
	} else {
	
		// If the widget isn't enabled, make the widget a dark, unform 
		// colour indicating that it is disabled. Don't show 
		// mouse-over changes.
		graphics.setColour (Colours.withAlpha (widget.itemColour2, 0.5));
	}
	
	// Draw the knob.
	graphics.fillEllipse (KnobSurfaceArea);
	
	// ------------- Knob Text Legend
	
	// Set the text colour.
	if (widget.enabled) {
	
		// If the widget is enabled, use the textColour field from the 
		// Inspector.
		graphics.setColour (widget.textColour);

	} else {

		// Otherwise, use our default colour for disabled legend text.
		graphics.setColour (WIDGETOFF_INDICATOR_COLOUR);
	}
	
	// Set the font and size.
	graphics.setFont (DEFAULT_BOLDFONT, DEFAULT_LABEL_FONTSIZE); 

	// Draw the text.
	graphics.drawAlignedText (widget.text, [widgetWIDGET_STARTX + widgetHorizontalTextOffset, widgetLegendTextVerticalOffset, widgetHeight + widgetHorizontalTextOffset, widgetHeight], "left");    
	
	// ------------- Knob text Value
	
	// Set the fobt face and size.
	graphics.setFont (DEFAULT_MEDIUMFONT, DEFAULT_SMALL_FONTSIZE); 
	
	if (widget.enabled) {
		
		// If the widget is enabled, use the enabled value colour.
		graphics.setColour (enabledKnobwidgetValueColour);

	} else {
		
		// Otherwise, use the default disabled value colour.
		graphics.setColour (disabledKnobwidgetValueColour);
	}
	
	// Format the value string to a consistent number of decimal places.
	var ConcatenatedValue = Engine.doubleToString (widget.value,numberOfDecimalPlaces) + widget.suffix;
	
	// Draw the text.
	graphics.drawAlignedText (ConcatenatedValue, [widgetWIDGET_STARTX + widgetHorizontalTextOffset, widgetValueTextVerticalOffset, widgetHeight+widgetHorizontalTextOffset, widgetHeight], "left");    
	
	// ------------- Value Arc
	//
	// The math here is basically voodoo. Part of the reason it's complex 
	// is so every graphic element scales correctly when you resize the 
	// widget. The value arc is the widget's value indicator.
	
	// Create an empty path for drawing the arc.
	var pathSemiCircle = Content.createPath();
	
	// Use the widget's minimum and maximum values (set in the Inspector) 
	// to set the range for the path.		
	var ValueRange = widget.max - widget.min;
	
	// Define the (graphical) offsets for where the path begins and ends.
	var startOffset = 2.4;
	var endOffset = -startOffset + 2.0 * startOffset * widget.valueNormalized;
	if ((widget.min < 0) && (widget.max > 0)) startOffset = - 0.08;
	
	// Specify a thickness (stroke) for the arc.
	var arcThickness = 0.02;
	
	// Based on the thickness, compute the arc's width.
	var arcWidth = 1.0 - 2.0 * arcThickness;
	
	// I…don't remember what this is for.
	var ArcOffset = 0.1;
		   
	// Add the arc to the path.
	pathSemiCircle.addArc ([arcThickness / 2 + ArcOffset, arcThickness / 2 + ArcOffset, arcWidth + arcThickness - ArcOffset*2, arcWidth + arcThickness - ArcOffset*2], -startOffset - widgetOffsetAdjustment, endOffset + widgetOffsetAdjustment);   
	
	// The path area is…let's just keep going.   
    var pathArea = pathSemiCircle.getBounds (widgetHeight);  
	
	// Set the colour information.
	if (widget.enabled) {	

		// If the the widget is enabled, and the mouse is over it or its 
		// been clicked, then set opacity to 100%; otherwise set it to 80%. 
		if (widget.hover || widget.clicked) { opacity = 1.0; } else { opacity = 0.8; }

		// Set the colour with whatever we set the opacity to.
		graphics.setColour (Colours.withAlpha(widgetIndicatorColour, opacity));
		
	} else {

		// If the widget isn't enabled, make the indicator a darker version of
		// indicator-off colour.
		graphics.setColour (Colours.withAlpha(WIDGETOFF_INDICATOR_COLOUR, 0.5));
	}
	
    // Draw the arc. Finaly.
    graphics.drawPath (pathSemiCircle, pathArea, widgetWidth * arcThickness);

	// ------------- Value Pointer
	//
	// This is the dot on the knob that shows its relative value to the 
	// value arc.

    // Rotate the indicator so it follows the arc.
    graphics.rotate (endOffset, [widgetHeight / 2, widgetHeight / 2]);

	// Set its colour.
    if (widget.enabled) {

	    // If the widget is enabled, use the widget's text colour.
    	graphics.setColour (widget.textColour);

    } else {

	    // Otherwise use a darkened version. 
    	graphics.setColour (Colours.withAlpha(widget.textColour, 0.5));
    }
    
    // Draw the indicator.
    graphics.fillRoundedRectangle ([widgetHeight / 2 - widgetHeight * -0.01, widgetHeight / 2 - widgetHeight * 0.295, widgetHeight  * 0.03, widgetHeight * 0.03], 3);
});


const var knbStandard1 = Content.getComponent("knob1");

knbStandard1.setLocalLookAndFeel(LAF_StandardKnob);

function onNoteOn()
{
	
}
 function onNoteOff()
{
	
}
 function onController()
{
	
}
 function onTimer()
{
	
}
 function onControl(number, value)
{
	
}
 