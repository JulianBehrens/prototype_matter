Content.makeFrontInterface(770, 600);

Console.clear();
Synth.deferCallbacks(true);
Content.setUseHighResolutionForPanels(true);


//include("Activation/_Include.js");
include("BrowserLAF.js");




// global modulator

const var gm = Engine.getGlobalRoutingManager();
const var mod_output = gm.getCable("mod_output");
const var signal_in = gm.getCable("signal_in");
const var Knob1 = Content.getComponent("Knob1");
const var Knob2 = Content.getComponent("Knob2");

mod_output.registerCallback(function(value)
{
	Knob1.setValue(value); // or whatever...
}, AsyncNotification);


signal_in.registerCallback(function(value)
{
	Knob2.setValue(value); // or whatever...
}, AsyncNotification);





//"drawSliderPackBackground",
//"drawSliderPackFlashOverlay",
//"drawSliderPackRightClickLine",
//"drawSliderPackTextPopup"

const var laf = Engine.createGlobalScriptLookAndFeel();

const var SliderPack1 = Content.getComponent("SliderPack1");
const var SliderPack2 = Content.getComponent("SliderPack2");

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
		a[1] = a[1] + a[3] * (1.0 - v);
		a[3] = a[3] - a[1];
	}		


g.fillRoundedRectangle([a[0], a[1], a[2]-1, a[3]], 1);	

});

SliderPack1.setLocalLookAndFeel(SPLAF);
SliderPack2.setLocalLookAndFeel(SPLAF);


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
 