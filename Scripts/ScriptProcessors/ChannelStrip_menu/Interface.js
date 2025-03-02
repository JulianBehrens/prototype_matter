Content.makeFrontInterface(600, 540);
Console.clear();


//Pages

const var NUM_BUTTONS = 3;
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



// Number Counter - Dry Wet
//const var ScriptFX1= Synth.getEffect("OTT");
const var Knob1 = Content.getComponent("Knob1");
const var Label1 = Content.getComponent("Label1");



inline function onKnob1Control(component, value)
{
	
	//ScriptFX1.setAttribute(ScriptFX1.DryWet, component.getValueNormalized());
	Label1.set("text", Math.round(value) + "");
};
Knob1.setControlCallback(onKnob1Control);



inline function onLabel1Control(component, value)
{
	Knob1.setValue(parseInt(value));
	Knob1.changed();
};
Label1.setControlCallback(onLabel1Control);;;
;





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
 