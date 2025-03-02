Content.makeFrontInterface(380, 540);
Console.clear();


const var ScriptFX1= Synth.getEffect("OTT");
const var Knob1 = Content.getComponent("Knob1");
const var Label1 = Content.getComponent("Label1");



inline function onKnob1Control(component, value)
{
	
	ScriptFX1.setAttribute(ScriptFX1.DryWet, component.getValueNormalized());
	Label1.set("text", Math.round(value) + "");
};
Knob1.setControlCallback(onKnob1Control);



inline function onLabel1Control(component, value)
{
	Knob1.setValue(parseInt(value));
	Knob1.changed();
};
Label1.setControlCallback(onLabel1Control);;




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
 