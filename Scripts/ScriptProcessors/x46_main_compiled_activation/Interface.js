Content.makeFrontInterface(1040, 660);

Console.clear();
Synth.deferCallbacks(true);
Content.setUseHighResolutionForPanels(true);

Engine.loadNextUserPreset(true);




include("BrowserLAF.js");
include("Icons.js");
include("Serials.js");;


Engine.loadFontAs("{PROJECT_FOLDER}fonts/SpaceGrotesk-Medium.ttf", "font_medium");
Engine.loadFontAs("{PROJECT_FOLDER}fonts/Panchang-Bold.ttf", "font_bold");

Engine.setGlobalFont("font_medium");




//tabbed view

const var NUM_BUTTONS = 2;
const var buttons = [];
const var panels = [];

for (i = 0; i < NUM_BUTTONS; i++)
{
    buttons[i] = Content.getComponent("Button" + (i));
    panels[i] = Content.getComponent("Panel" + (i));
    buttons[i].setControlCallback(onButtonControl);
}

inline function onButtonControl(component, value)
{
	local idx = buttons.indexOf(component);
		
	for (i = 0; i < panels.length; i++)
    {
        panels[i].showControl(idx == i);
        buttons[i].setValue(i == idx && value);
    }
	
}

onButtonControl(buttons[0], true);



//Sync Unsync Button
const var SYNC = Content.getComponent("Buttonsync");
const var Knob4 = Content.getComponent("Knob4");

Content.setPropertiesFromJSON("Knob4", {
  "mode": "TempoSync",
  "stepSize": 1,
});


const var GRAINS = Synth.getEffect("grains");

inline function onKnob4Control(component, value)
{
    GRAINS.setAttribute(GRAINS.Speed, value);
}
Content.getComponent("Knob4").setControlCallback(onKnob4Control);




inline function onSYNCControl(component, value)
{
  GRAINS.setAttribute(GRAINS.TempoSync, value);
  
	if(value)
	{
		// Switch the knob to tempo sync mode
		Knob4.set("mode", "TempoSync");
		Knob4.set("min", 0);
		Knob4.set("max", 18);
		Knob4.set("stepSize", 1);
		//Knob4.set("middlePosition", 250);

	}
	else
	{
		// Switch the knob to frequency mode
		Knob4.set("mode", "Time");
		Knob4.set("min", 10);
		Knob4.set("max", 500);
		Knob4.set("middlePosition", 100);
		Knob4.set("stepSize", 1);

	}
	
Knob4.setValue(GRAINS.getAttribute(GRAINS.Speed));
	
}

Content.getComponent("Buttonsync").setControlCallback(onSYNCControl);


//2

//Sync Unsync Button
const var SYNC2 = Content.getComponent("Buttonsync1");
const var Knob7 = Content.getComponent("Knob7");
const var Knob8 = Content.getComponent("Knob8");

Content.setPropertiesFromJSON("Knob7", {
  "mode": "TempoSync",
  "stepSize": 1,
});

Content.setPropertiesFromJSON("Knob8", {
  "mode": "TempoSync",
  "stepSize": 1,
});


inline function onKnob7Control(component, value)
{
    GRAINS.setAttribute(GRAINS.SpeedL, value);
}
Content.getComponent("Knob7").setControlCallback(onKnob7Control);

inline function onKnob8Control(component, value)
{
    GRAINS.setAttribute(GRAINS.SpeedR, value);
}
Content.getComponent("Knob8").setControlCallback(onKnob8Control);



inline function onSYNC2Control(component, value)
{
  GRAINS.setAttribute(GRAINS.TempoSync, value);
  
	if(value)
	{
		// Switch the knob to tempo sync mode
		Knob7.set("mode", "TempoSync");
		Knob7.set("min", 0);
		Knob7.set("max", 18);
		Knob7.set("stepSize", 1);
		//Knob4.set("middlePosition", 250);
		
		// Switch the knob to tempo sync mode
		Knob8.set("mode", "TempoSync");
		Knob8.set("min", 0);
		Knob8.set("max", 18);
		Knob8.set("stepSize", 1);
		//Knob4.set("middlePosition", 250);

	}
	else
	{
		// Switch the knob to frequency mode
		Knob7.set("mode", "Time");
		Knob7.set("min", 0);
		Knob7.set("max", 1000);
		Knob7.set("middlePosition", 500);
		Knob7.set("stepSize", 1);
		
		// Switch the knob to frequency mode
		Knob8.set("mode", "Time");
		Knob8.set("min", 0);
		Knob8.set("max", 1000);
		Knob8.set("middlePosition", 500);
		Knob8.set("stepSize", 1);

	}
	
Knob7.setValue(GRAINS.getAttribute(GRAINS.SpeedL));
Knob8.setValue(GRAINS.getAttribute(GRAINS.SpeedR));
	
}

Content.getComponent("Buttonsync1").setControlCallback(onSYNC2Control);


// HOW TO USE
// - Name your parameter
// - set your parameter type
// - make sure to set the correct middlePosition
// - adjust the mouse sensitivity
// add to Knobs array or use Knob.setLocalLookAndFeel(LAF_Knob);

// How to add custom Icons
// At the end of the file you can find the icons. I recommend to put this into a namespace called Icons. Using the hise svg importer add the path as a HISE number path. Do not use SVG only number path as this is stylable

// Use this format to assign a icon to a slider:
// ParameterName$iconName
// its important to add the $ 
// everything after the $ is interepreted as icon name
// make sure you icons all have the same spacings.

// You can find a config to tweak the look and feel and adjust it to your spacing. 

const var LAF_Knob = Content.createLocalLookAndFeel();

// add the knobs here that you want to style
const var Knobs = [Content.getComponent("Knob1"),
                   Content.getComponent("Knob2"),
                   Content.getComponent("Knob3"),
                   Content.getComponent("Knob4"),
                   Content.getComponent("Knob5"),
                   Content.getComponent("Knob6"),
                   Content.getComponent("Knob7"),
                   Content.getComponent("Knob8")];


for (knb in Knobs) {
	
	// assign knob to local look and feel
	knb.setLocalLookAndFeel(LAF_Knob);
	
}

// LAF FUNCTION
LAF_Knob.registerFunction('drawRotarySlider', knobLAF);



inline function knobLAF(g, obj) {
	
	local a = obj.area;
	
	// TWEAK THIS SETTINGS TO FIT YOUR SCALING 
	local ICON_SIZE = 20;
	local BORDER_RADIUS = 1;
	
	// local SLIDER_BORDER_RADIUS = 1; ROUNDED SLIDER :)
	local SLIDER_BORDER_RADIUS = 0;
	local SLIDER_THICKNESS = 3;
	
	// SPACING
	local LEFT_PADDING = 15;
	local RIGHT_PADDING = 15;
	
	local ICON_SLIDER_GAP = 10;
	local LABEL_SLIDER_GAP = 6;
	local LABEL_SLIDER_TOP_PADDING = 3;
	
	// FONTS
	local LABEL_FONT = "font_medium";
	local LABEL_FONT_SIZE = 20;
	local LABEL_FONT_SPACING = 0.04;
	
	local VALUE_FONT = "font_bold";
	local VALUE_FONT_SIZE = 20;
	local VALUE_FONT_SPACING = 0.04;
	
	// COLORS
	local SLIDER_BACKGROUND_COLOR = Colours.withAlpha(obj.bgColour,0.2);
	
	// USE OBJ COLORS;
	// I recommend to use custom colors here
	local bgColor = obj.bgColour;
	local invertedbgColor = obj.itemColour1;
	local textColor = obj.textColour;
	local invertedTextCol = obj.itemColour2;
	
	local value = obj.valueAsText;
	local normalizedValue = obj.valueNormalized;
	local text = obj.text.split('$');
	local label = text[0];
	local icon = text[1];
	local isBipolar = obj.text.contains('$isBipolar');
	
	local iconArea = [
		a[0] + LEFT_PADDING,
		a[1] + a[3] / 2 - ICON_SIZE / 2,
		ICON_SIZE,
		ICON_SIZE
	];
	
	local rightArea = [
		a[0] + ICON_SIZE * 2 + ICON_SLIDER_GAP,
		a[1] + LABEL_SLIDER_TOP_PADDING,
		a[2] - ICON_SIZE * 2 - ICON_SLIDER_GAP - RIGHT_PADDING,
		a[3]
	];
	
	local labelArea = [
		rightArea[0],
		rightArea[1] - LABEL_SLIDER_GAP,
		rightArea[2],
		rightArea[3]
	];
	
	local sliderArea = [
		rightArea[0],
		rightArea[1] + rightArea[3] / 2 + LABEL_SLIDER_GAP,
		rightArea[2],
		SLIDER_THICKNESS
	];
	
	local sliderWithValue = [
		rightArea[0],
		rightArea[1] + rightArea[3] / 2 + LABEL_SLIDER_GAP,
		rightArea[2] * normalizedValue,
		SLIDER_THICKNESS
	];
	
	if (isBipolar) {
		local newValue = -1 * (0.5 - normalizedValue);
	    sliderWithValue = [
			rightArea[0] + rightArea[2] / 2,
			rightArea[1] + rightArea[3] / 2 + LABEL_SLIDER_GAP,
			rightArea[2] * newValue,
			SLIDER_THICKNESS
	    ];
	}
	
	if (obj.hover || obj.clicked) {
		bgColor = invertedbgColor;
		textColor = invertedTextCol;
	}

	g.setColour(bgColor);
	g.fillRoundedRectangle(a, BORDER_RADIUS);
	
	g.setColour(SLIDER_BACKGROUND_COLOR);
	g.fillRoundedRectangle(sliderArea, SLIDER_BORDER_RADIUS);
	
	g.setColour(textColor);
	g.fillPath(icons[icon], iconArea);
	
	g.setFontWithSpacing(LABEL_FONT, LABEL_FONT_SIZE, LABEL_FONT_SPACING);
	g.drawAlignedText(label, labelArea, 'left');
	
	g.setFontWithSpacing(VALUE_FONT, VALUE_FONT_SIZE, VALUE_FONT_SPACING);
	g.drawAlignedText(value, labelArea, 'right');
	g.fillRoundedRectangle(sliderWithValue, SLIDER_BORDER_RADIUS);
}


//"drawSliderPackBackground",
//"drawSliderPackFlashOverlay",
//"drawSliderPackRightClickLine",
//"drawSliderPackTextPopup"


const var SliderPack1 = Content.getComponent("SliderPack1");

const var SPLAF = Content.createLocalLookAndFeel();

SPLAF.registerFunction("drawLinearSlider", function(g, obj)
{
	// ID is the number of the slider
	var a = obj.area;
	var v = obj.valueNormalized;
	var l = Colours.pink;
	var r = Colours.blue;
		

	var c = Colours.mix(l, r, parseInt(obj.id) / 16);


	
	g.setColour(0xFFDDE8F3);

	{
		a[1] = a[1]+8 + a[3]/ 1.1 * (1.0 - v);
		a[3] = a[3] - a[1];
	}		
	

//stripes
//mid
g.setGradientFill([(0xFFDDE8F3), 2, 0.1* obj.area[3], (0x108AD5FF), 1.0*obj.area[2] / 1000 - 2, 1.4*obj.area[3]]);
g.fillRoundedRectangle([a[0]+26, a[1]+1, a[2]-a[2]+2, a[3]-1], 1.0);

//left
g.setColour(0x20DDE8F3);
g.fillRoundedRectangle([a[0]+30, a[1]+1, a[2]-a[2]+2, a[3]-1], 1.0);

//right
g.setColour(0x40DDE8F3);
g.fillRoundedRectangle([a[0]+22, a[1]+1, a[2]-a[2]+2, a[3]-1], 1.0);




//mid
g.setColour(0xFFDDE8F3);
g.fillRoundedRectangle([a[0]-a[0]+13, a[1]-6, a[2]-a[2]+28, a[3]-a[3]+5], 1.0);

//left
g.setColour(0x60DDE8F3);
g.fillRoundedRectangle([a[0]+6, a[1]-6, a[2]-a[2]+5, a[3]-a[3]+5], 1.0);

//right
g.setColour(0x60DDE8F3);
g.fillRoundedRectangle([a[0]+43, a[1]-6, a[2]-a[2]+5, a[3]-a[3]+5], 1.0);



});

SliderPack1.setLocalLookAndFeel(SPLAF);


SPLAF.registerFunction("drawSliderPackFlashOverlay", function(g, obj)
{
var a = obj.area;	


g.setColour(0x10DFE8F2);
g.fillRoundedRectangle(obj.area, 3.0);
{
	a[1] = a[1] + a[3]-2 * (1.0);
	a[3] = a[3] - a[1];

}	




g.setColour(0x10DFE8F2);
g.fillRoundedRectangle(obj.area, 3.0);

g.setColour(0xFFDFE8F2);
g.fillRoundedRectangle([a[0]-26, a[1]+1, a[2]+2, a[3]+10], 1.0);

});


//Activation


namespace Authorisation
{
    const var SerialInput = Content.getComponent("SerialInput");
    const var Description = Content.getComponent("Description");
    const var SerialStateLabel = Content.getComponent("SerialStateLabel");
    const var AuthorisationDialogue = Content.getComponent("AuthorisationDialogue");
    const var GlobalMute = Synth.getMidiProcessor("GlobalMute");
    
    /** Checks if the serial input is valid and stores the result if successful. */
    inline function onSubmitButtonControl(component, value)
    {
        if(!value) // Just execute once
            return;
    
        local v = SerialInput.getValue();
        Console.print(v);
	
        // Checks if it's in the input
        if(serials.Data.contains(v))
        {
            Console.print("Serial number found");
        
            local data = 
            {
                "Serial": v
            };
        
            // Stores the file to the hard drive. In HISE it will be the project folder
            // but in the compiled plugin it will use the parent directory to the 
            // user preset directory (which is usually the app data folder).
            Engine.dumpAsJSON(data, "../RegistrationInfo.js");
            
            setValidLicense(true);
        }
        else
        {
            Console.print("Invalid serial number");
            Description.set("text", "Invalid serial number. The number you supplied does not match");
            
            setValidLicense(false);
        }
    };

    Content.getComponent("SubmitButton").setControlCallback(onSubmitButtonControl);


    inline function setValidLicense(isValid)
    {
        // Do whatever you want to do here. I suggest a MIDI muter...
        GlobalMute.setAttribute(0, 1 - isValid);
    
        if(isValid)
        {
            // Change this to any other visual indication...
            SerialStateLabel.set("bgColour", Colours.greenyellow);
            AuthorisationDialogue.set("visible", false);
        }
        else
        {
            SerialStateLabel.set("bgColour", Colours.red);
            AuthorisationDialogue.set("visible", true);
        }
    }

    inline function checkOnLoad()
    {
        // Clear the input
        SerialInput.set("text", "");
        
        // Load the serial from the stored file
        local pData = Engine.loadFromJSON("../RegistrationInfo.js");
        Console.print("Checking serial");
    
        if(pData)    
        {
            local v = pData.Serial;
            Console.print("Restored serial: " + v);
        
            if(serials.Data.contains(v))
            {
                setValidLicense(true);
                return;
            }
        }
    
        setValidLicense(false);
    }

    // Call this on startup
    checkOnLoad();

}

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
 