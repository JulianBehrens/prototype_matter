Processor 
Type SynthChain ID x46 Bypassed Gain 	      �?Balance 	        VoiceLimit 	      P@KillFadeTime 	      4@IconColour 0 packageName  BuildVersion �  EditorStates BodyShown 0 Visible 1 Solo 0 Folded 1  ChildProcessors  Processor Type MidiProcessorChain ID Midi Processor Bypassed EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors  Processor Type ScriptProcessor ID Interface Bypassed Script �Content.makeFrontInterface(770, 600);

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
  EditorStates BodyShown 1 Visible 1 Solo 0 contentShown 1 onInitOpen 1  ChildProcessors   Content  Control type ScriptSliderPack id SliderPack3 value     Control type ScriptSliderPack id SliderPack2 value 	      ,@ Control type ScriptSliderPack id SliderPack1 value 	      .@ Control type ScriptSlider id Knob1 value 	   ����? Control type ScriptSlider id Knob2 value 	     ��? Control type ScriptSlider id trigger speed value 	      @ Control type ScriptSlider id divisions value 	      @ Control type ScriptSlider id free value 	      H@ Control type ScriptSlider id grain size value 	   `ff�? Control type ScriptSlider id grain size2 value 	     �b@ Control type ScriptSlider id 	repeats value 	     �X@ Control type ScriptSlider id Knob10 value 	      (@ Control type ScriptSlider id 
engine 2 value 	   �=
�? Control type ScriptSlider id pitch value 	      (� Control type ScriptSlider id smear value 	      4@ Control type ScriptButton id 
beatsync value 	         UIData  ContentProperties DeviceType 	Desktop Component type ScriptImage id background x 	        y 	        width 	     �@height 	     ��@fileName  {PROJECT_FOLDER}background.png  Component type ScriptSliderPack id SliderPack3 x 	      $@y 	     �u@sliderAmount 	       @SliderPackIndex 2 processorId Mod FX width 	     p�@height 	     �[@itemColour 352321535 itemColour2 
16777215 textColour 
16777215  Component type ScriptSliderPack id SliderPack2 x 	      <@y 	     ��@width 	     `�@height 	     �B@processorId Mod FX bgColour 
16777215 itemColour 
16777215 itemColour2 
16777215 textColour 
16777215 stepSize 	   ����?showValueOverlay 0 SliderPackIndex 1  Component type ScriptSliderPack id SliderPack1 x 	      <@y 	     �~@width 	     `�@height 	     �B@processorId Mod FX bgColour 
16777215 itemColour 
16777215 itemColour2 
16777215 textColour 
16777215 stepSize 	   ����?showValueOverlay 0  Component type ScriptSlider id Knob1 x 	     �p@y 	     �V@processorId Mod FX parameterId Tempo stepSize 	        numStrips 	     �b@width 	     @o@height 	     @o@filmstripImage {PROJECT_FOLDER}bubble.png scaleFactor 	      �?Component type ScriptSlider id Knob2 x 	        y 	        processorId Mod FX parameterId Tempo stepSize 	        numStrips 	     �b@width 	     @o@height 	     @o@filmstripImage {PROJECT_FOLDER}overlay2.png scaleFactor 	      �?parentComponent Knob1  Component type ScriptSlider id trigger speed x 	     �t@y 	     `x@isMetaParameter 1 processorId Mod FX parameterId Mod Speed mode TempoSync max 	      2@stepSize 	      �?middlePosition 	      "@ Component 
type ScriptSlider id divisions x 	      I@y 	      n@processorId Mod FX parameterId sync Grain Speed mode TempoSync max 	      2@stepSize 	      �?middlePosition 	      "@ Component type ScriptSlider id free x 	      I@y 	     �f@processorId Mod FX parameterId free Grain Speed mode Time max 	     @@stepSize 	      �?suffix  ms middlePosition 	     @o@ Component type ScriptSlider id grain size x 	      I@y 	      N@processorId Mod FX parameterId Grain Size  Component type ScriptSlider id grain size2 x 	     ��@y 	     @e@processorId Mod FX parameterId Engine 2 Grain Size text grain size max 	     @@mode Time suffix ms middlePosition 	     �o@stepSize 	      �?min 	      $@ Component 
type ScriptSlider id 	repeats x 	     ��@y 	     �l@processorId Mod FX parameterId Engine 2 Grain Speed max 	     @@mode Time suffix ms stepSize 	      �? Component type ScriptSlider id Knob10 x 	     ��@y 	     �[@processorId Mod FX parameterId Engine 2 Pitch text pitch max 	      (@min 	      (�stepSize 	      �?middlePosition 	        suffix st  Component type ScriptSlider id 
engine 2 x 	     ��@y 	      I@isMetaParameter 1 processorId Mod FX parameterId Engine 2 DryWet  Component type ScriptSlider id pitch x 	      I@y 	      ^@processorId Mod FX parameterId Pitch max 	      (@min 	      (�stepSize 	      �?middlePosition 	        suffix st  Component 
type ScriptSlider id smear x 	     ��@y 	      r@processorId Mod FX parameterId Smear suffix % middlePosition 	      I@max 	      Y@stepSize 	      �? Component type ScriptButton id 
beatsync x 	      I@y 	     `s@processorId Mod FX parameterId Sync on off  Processor Type ModulatorChain ID GainModulation Bypassed Intensity 	      �?EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors   Processor Type ModulatorChain ID PitchModulation Bypassed Intensity 	        EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors   Processor Type EffectChain ID FX Bypassed EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors  Processor Type Delay ID Delay1 Bypassed DelayTimeLeft 	      @DelayTimeRight 	      @FeedbackLeft 	   @33�?FeedbackRight 	   ����?LowPassFreq 	     ��@HiPassFreq 	      D@Mix 	   �z�?TempoSync 	      �?EditorStates BodyShown 1 Visible 1 Solo 0  ChildProcessors   RoutingMatrix NumSourceChannels    Channel0     Send0 ����Channel1    Send1 ���� Processor Type 
ScriptFX ID grains Bypassed NumSliderPacks    NumDisplayBuffers    Script � function prepareToPlay(sampleRate, blockSize)
{
	
}
 function processBlock(channels)
{
	
}
 function onControl(number, value)
{
	
}
  EditorStates BodyShown 1 Visible 1 Solo 0  ChildProcessors   RoutingMatrix NumSourceChannels    Channel0     Send0 ����Channel1    Send1 ���� Content   Networks  Network ID mod_fx Version 0.0.0 AllowCompilation 1  UIData  ContentProperties DeviceType 	Desktop  Processor Type 
Dynamics ID Dynamics1 Bypassed GateEnabled 	        GateThreshold 	        GateAttack 	      �?GateRelease 	      Y@CompressorEnabled 	      �?CompressorThreshold 	   @3�F�CompressorRatio 	      @CompressorAttack 	      $@CompressorRelease 	      Y@LimiterEnabled 	      �?LimiterThreshold 	   ����LimiterAttack 	      �?LimiterRelease 	      $@CompressorMakeup 	      �?LimiterMakeup 	        EditorStates BodyShown 1 Visible 1 Solo 0  ChildProcessors   RoutingMatrix NumSourceChannels    Channel0     Send0 ����Channel1    Send1 ���� RoutingMatrix NumSourceChannels    Channel0     Send0 ����Channel1    Send1 ���� macro_controls  macro name 	Macro 1 value 	        midi_cc ���� macro name 	Macro 2 value 	        midi_cc ���� macro name 	Macro 3 value 	        midi_cc ���� macro name 	Macro 4 value 	        midi_cc ���� macro name 	Macro 5 value 	        midi_cc ���� macro name 	Macro 6 value 	        midi_cc ���� macro name 	Macro 7 value 	        midi_cc ���� macro name 	Macro 8 value 	        midi_cc ���� MidiAutomation   MPEData Enabled  