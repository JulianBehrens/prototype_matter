Processor 
Type SynthChain ID 
x46_main Bypassed Gain 	      �?Balance 	        VoiceLimit 	      P@KillFadeTime 	      4@IconColour 0 packageName  BuildVersion �  EditorStates BodyShown 0 Visible 1 Solo 0 Folded 1  ChildProcessors  Processor Type MidiProcessorChain ID Midi Processor Bypassed EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors  Processor Type ScriptProcessor ID Interface Bypassed Script F+Content.makeFrontInterface(770, 600);

Console.clear();
Synth.deferCallbacks(true);
Content.setUseHighResolutionForPanels(true);


//include("Activation/_Include.js");
//include("BrowserLAF.js");


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
                   Content.getComponent("Knob8"),
                   Content.getComponent("Knob9")];


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
	local LABEL_FONT = 'Arial';
	local LABEL_FONT_SIZE = 14;
	local LABEL_FONT_SPACING = 0;
	
	local VALUE_FONT = 'Arial';
	local VALUE_FONT_SIZE = 20;
	local VALUE_FONT_SPACING = -0.03;
	
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

reg svgData;
const icons = {};

// ICON
svgData = [ 110,109,96,128,38,68,94,11,181,67,98,154,83,35,68,94,11,181,67,201,156,32,68,129,202,178,67,202,91,30,68,8,73,174,67,98,204,26,28,68,142,199,169,67,93,250,26,68,45,90,164,67,93,250,26,68,194,0,158,67,98,93,250,26,68,127,140,151,67,188,26,28,68,14,24,
146,67,120,91,30,68,140,163,141,67,98,53,156,32,68,235,46,137,67,230,82,35,68,187,244,134,67,155,127,38,68,187,244,134,67,98,188,185,41,68,187,244,134,67,246,115,44,68,169,46,137,67,54,174,46,68,199,162,141,67,98,135,232,48,68,230,22,146,67,175,5,50,
68,22,139,151,67,175,5,50,68,55,255,157,67,98,175,5,50,68,195,88,164,67,168,232,48,68,102,198,169,67,152,174,46,68,100,72,174,67,98,138,116,44,68,96,202,178,67,113,186,41,68,94,11,181,67,96,128,38,68,94,11,181,67,99,109,153,122,38,68,200,7,172,67,98,
246,116,40,68,200,7,172,67,196,31,42,68,115,169,170,67,4,123,43,68,202,236,167,67,98,68,214,44,68,32,48,165,67,228,131,45,68,202,228,161,67,228,131,45,68,198,10,158,67,98,228,131,45,68,12,22,154,67,166,214,44,68,112,192,150,67,60,124,43,68,240,9,148,
67,98,210,33,42,68,113,83,145,67,119,119,40,68,49,248,143,67,10,125,38,68,49,248,143,67,98,8,144,36,68,49,248,143,67,250,233,34,68,172,82,145,67,208,138,33,68,128,7,148,67,98,182,43,32,68,84,188,150,67,24,124,31,68,10,17,154,67,24,124,31,68,228,5,158,
67,98,24,124,31,68,232,223,161,67,66,43,32,68,4,44,165,67,151,137,33,68,89,234,167,67,98,236,231,34,68,140,168,170,67,152,141,36,68,200,7,172,67,153,122,38,68,200,7,172,67,99,109,254,127,38,68,191,197,207,67,98,188,58,32,68,191,197,207,67,204,186,26,
68,184,13,207,67,42,0,22,68,236,157,205,67,98,135,69,17,68,0,46,204,67,221,76,13,68,171,146,202,67,43,22,10,68,206,203,200,67,98,228,90,9,68,46,110,200,67,182,189,8,68,83,174,199,67,160,62,8,68,89,140,198,67,98,139,191,7,68,64,106,197,67,0,128,7,68,36,
25,196,67,0,128,7,68,232,152,194,67,98,0,128,7,68,240,118,193,67,176,194,7,68,230,138,192,67,15,72,8,68,171,212,191,67,98,120,205,8,68,146,30,191,67,42,100,9,68,67,242,190,67,35,12,10,68,226,79,191,67,98,158,155,12,68,124,198,192,67,172,14,15,68,129,
243,193,67,77,101,17,68,206,214,194,67,98,237,187,19,68,62,186,195,67,99,122,21,68,113,97,196,67,176,160,22,68,140,204,196,67,98,224,172,19,68,246,37,192,67,84,111,17,68,26,54,186,67,13,232,15,68,26,253,178,67,98,196,96,14,68,26,196,171,67,34,157,13,
68,26,200,164,67,34,157,13,68,216,8,158,67,98,34,157,13,68,109,120,145,67,197,250,13,68,239,120,134,67,11,182,14,68,182,20,122,67,98,84,113,15,68,144,55,103,67,5,68,16,68,213,64,87,67,30,46,17,68,142,48,74,67,98,248,90,17,68,64,85,71,67,202,185,17,68,
173,235,68,67,146,74,18,68,219,243,66,67,98,93,219,18,68,246,251,64,67,182,131,19,68,0,0,64,67,154,67,20,68,0,0,64,67,98,35,221,20,68,0,0,64,67,248,85,21,68,200,10,65,67,28,174,21,68,81,32,67,67,98,64,6,22,68,218,53,69,67,234,26,22,68,156,144,71,67,23,
236,21,68,142,48,74,67,98,208,48,21,68,120,110,84,67,254,155,20,68,176,34,94,67,161,45,20,68,54,77,103,67,98,70,191,19,68,182,119,112,67,6,116,19,68,11,84,119,67,228,75,19,68,59,226,123,67,98,255,205,21,68,62,248,111,67,126,200,24,68,30,22,103,67,106,
59,28,68,227,59,97,67,98,78,174,31,68,162,97,91,67,117,26,35,68,135,116,88,67,254,127,38,68,135,116,88,67,98,64,197,44,68,135,116,88,67,133,67,50,68,20,235,89,67,205,250,54,68,47,216,92,67,98,21,178,59,68,80,197,95,67,204,175,63,68,180,2,99,67,208,243,
66,68,91,144,102,67,98,176,170,67,68,134,94,103,67,12,69,68,68,46,231,104,67,7,195,68,68,92,42,107,67,98,2,65,69,68,124,109,109,67,0,128,69,68,216,14,112,67,0,128,69,68,106,14,115,67,98,0,128,69,68,133,116,117,67,164,60,69,68,223,87,119,67,14,182,68,
68,112,184,120,67,98,104,47,68,68,0,25,122,67,89,153,67,68,168,107,122,67,208,243,66,68,94,176,121,67,98,182,113,64,68,67,195,118,67,102,20,62,68,71,105,116,67,226,219,59,68,118,162,114,67,98,93,163,57,68,166,219,112,67,148,230,55,68,250,167,111,67,136,
165,54,68,110,7,111,67,98,98,173,57,68,94,69,121,67,59,228,59,68,124,160,130,67,19,74,61,68,32,125,137,67,98,235,175,62,68,202,89,144,67,223,98,63,68,9,48,151,67,223,98,63,68,252,255,157,67,98,223,98,63,68,128,138,170,67,218,1,63,68,14,145,181,67,223,
63,62,68,200,19,191,67,98,245,125,61,68,96,150,200,67,159,174,60,68,170,135,208,67,222,209,59,68,162,231,214,67,98,240,153,59,68,86,79,216,67,251,56,59,68,166,130,217,67,254,174,58,68,144,129,218,67,98,241,36,58,68,122,128,219,67,163,126,57,68,0,0,220,
67,4,188,56,68,0,0,220,67,98,95,32,56,68,0,0,220,67,172,163,55,68,46,122,219,67,14,70,55,68,174,110,218,67,98,112,232,54,68,14,99,217,67,0,209,54,68,126,47,216,67,207,255,54,68,152,211,214,67,98,28,187,55,68,100,207,209,67,158,81,56,68,110,17,205,67,
86,195,56,68,184,153,200,67,98,253,52,57,68,1,34,196,67,151,136,57,68,116,178,192,67,36,190,57,68,16,75,190,67,98,232,19,55,68,196,90,196,67,77,14,52,68,118,200,200,67,83,173,48,68,38,148,203,67,98,106,76,45,68,214,95,206,67,248,231,41,68,191,197,207,
67,254,127,38,68,191,197,207,67,99,109,94,130,38,68,40,194,198,67,98,126,37,44,68,40,194,198,67,113,243,48,68,233,199,194,67,21,236,52,68,106,211,186,67,98,186,228,56,68,236,222,178,67,20,225,58,68,158,65,169,67,20,225,58,68,60,251,157,67,98,20,225,58,
68,252,180,146,67,244,227,56,68,22,25,137,67,181,233,52,68,206,39,129,67,98,118,239,48,68,246,108,114,67,206,32,44,68,161,123,106,67,158,125,38,68,161,123,106,67,98,126,218,32,68,161,123,106,67,139,12,28,68,32,112,114,67,230,19,24,68,138,44,129,67,98,
61,27,20,68,12,33,137,67,232,30,18,68,91,190,146,67,232,30,18,68,189,4,158,67,98,232,30,18,68,253,74,169,67,8,28,20,68,227,230,178,67,69,22,24,68,43,216,186,67,98,134,16,28,68,116,201,194,67,45,223,32,68,40,194,198,67,94,130,38,68,40,194,198,67,99,101,
0,0 ];

icons.cyclone = Content.createPath();
icons.cyclone.loadFromData(svgData);






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
  EditorStates BodyShown 1 Visible 1 Solo 0 contentShown 1 onInitOpen 1  ChildProcessors   Content  Control type ScriptSliderPack id SliderPack3 value 	      �?data 8.8nvE+DtdT7C  Control type ScriptSliderPack id SliderPack1 value 	      .@data [64......zLyL8C...3OMyLS+zLyL6yYlY1O...f+....9C...3OZlYF+zLyL6C...vOyLyL+nYlY7SyLyjO......  Control type ScriptSlider id trigger speed value 	      @ Control type ScriptSlider id Knob4 value 	         Control type ScriptSlider id Knob3 value 	     �r@ Control type ScriptSlider id Knob1 value 	         Control type ScriptSlider id Knob7 value 	         Control type ScriptSlider id Knob8 value 	         Control type ScriptSlider id Knob6 value 	         Control type ScriptSlider id Knob5 value 	         Control type ScriptSlider id Knob2 value 	      � Control type ScriptSlider id Knob9 value 	     �C@ Control type ScriptButton id 
beatsync value 	         UIData  ContentProperties DeviceType 	Desktop Component type ScriptImage id background x 	        y 	        width 	     �@height 	     ��@fileName  {PROJECT_FOLDER}background.png  Component type ScriptSliderPack id SliderPack3 x 	      $@y 	     �u@sliderAmount 	       @SliderPackIndex 2 processorId Mod FX width 	     p�@height 	     �[@itemColour 352321535 itemColour2 
16777215 textColour 
16777215  Component type ScriptSliderPack id SliderPack1 x 	     �\@y 	     �}@width u  height N   processorId Mod FX bgColour 
16777215 itemColour 
16777215 itemColour2 
16777215 textColour 
16777215 stepSize 	   ����?showValueOverlay 0  Component type ScriptSlider id trigger speed x 	     �t@y 	     `x@isMetaParameter 1 processorId Mod FX parameterId Mod Speed mode TempoSync max 	      2@stepSize 	      �?middlePosition 	      "@itemColour2 1006567424  Component type ScriptSlider id Knob4 x 	      .@y 	     �o@processorId Mod FX parameterId sync Grain Speed mode TempoSync max 	      2@stepSize 	      �?middlePosition 	      "@text Divisions$cyclone width 	      n@height 	      O@itemColour 4292864242 bgColour 	6055537 textColour 4290692558  Component type ScriptSlider id Knob3 x 	      .@y 	     `g@processorId Mod FX parameterId free Grain Speed mode Time max 	     @@stepSize 	      �?suffix  ms middlePosition 	     @o@text Free$cyclone width 	      n@height 	      O@bgColour 	6055537 itemColour 4292864242 textColour 4290692558  Component type ScriptSlider id Knob1 x 	      .@y 	      L@middlePosition 	      "@stepSize 	      �?max 	      2@width 	      n@height 	      O@text Grainsize$cyclone bgColour 	6055537 itemColour 4292864242 mode TempoSync textColour 4290692558  Component type ScriptSlider id Knob7 x 	     �@y 	     `g@processorId Mod FX parameterId Engine 2 Grain Size text Grainsize$cyclone max 	     @@mode Time suffix ms middlePosition 	     �o@stepSize 	      �?min 	      $@width 	      n@height 	      O@bgColour 	6055537 textColour 4290692558 itemColour 4292864242  Component type ScriptSlider id Knob8 x 	     �@y 	     �o@processorId Mod FX parameterId Engine 2 Grain Speed max 	     @@mode Time suffix ms stepSize 	      �?text Repeats$cyclone width �   height >   bgColour 	6055537 textColour 4290692558 itemColour 4292864242  Component type ScriptSlider id Knob6 x 	     �@y 	     �^@processorId Mod FX parameterId Engine 2 Pitch text Pitch$cyclone max 	      (@min 	      (�stepSize 	      �?middlePosition 	        suffix st width 	      n@height 	      O@bgColour 	6055537 textColour 4290692558 itemColour 4292864242  Component type ScriptSlider id Knob5 x 	     �@y 	      L@processorId Mod FX parameterId Engine 2 DryWet text Engine2$cyclone width 	      n@height 	      O@bgColour 	6055537 textColour 4290692558 itemColour 4292864242  Component type ScriptSlider id Knob2 x 	      .@y 	     �^@processorId Mod FX parameterId Pitch max 	      (@min 	      (�stepSize 	      �?middlePosition 	        suffix st text Pitch$cyclone$isBipolar width 	      n@height 	      O@bgColour 	6055537 itemColour 4292864242 textColour 4290692558  Component type ScriptSlider id Knob9 x 	     X�@y 	     �t@processorId Mod FX parameterId Smear suffix % middlePosition 	      I@max 	      Y@stepSize 	      �?text Smear$cyclone width 	      i@height 	     @Y@bgColour 	6055537 textColour 4290692558 itemColour 4292864242  Component type ScriptButton id 
beatsync x 	      Y@y 	     �v@processorId Mod FX parameterId Sync on off width 	     �V@height 	      D@bgColour 0 itemColour 0 itemColour2 4278190080 textColour 4278190080  Component type ScriptFloatingTile id FloatingTile1 x 	      p@y 	      L@width �   height   ContentType AudioAnalyser bgColour 	���     itemColour 	����    itemColour2 4282664004 textColour 0 Data 0{
  "ProcessorId": "Analyser1",
  "Index": 1
}  Processor Type ModulatorChain ID GainModulation Bypassed Intensity 	      �?EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors   Processor Type ModulatorChain ID PitchModulation Bypassed Intensity 	        EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors   Processor Type EffectChain ID FX Bypassed EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors  Processor Type Delay ID Delay1 Bypassed DelayTimeLeft 	      @DelayTimeRight 	      @FeedbackLeft 	   @33�?FeedbackRight 	   ����?LowPassFreq 	     ��@HiPassFreq 	      D@Mix 	   �z�?TempoSync 	      �?EditorStates BodyShown 1 Visible 1 Solo 0  ChildProcessors   RoutingMatrix NumSourceChannels    Channel0     Send0 ����Channel1    Send1 ���� Processor Type 
ScriptFX ID grains Bypassed NumSliderPacks    NumDisplayBuffers    Script � function prepareToPlay(sampleRate, blockSize)
{
	
}
 function processBlock(channels)
{
	
}
 function onControl(number, value)
{
	
}
  EditorStates BodyShown 1 Visible 1 Solo 0  ChildProcessors   RoutingMatrix NumSourceChannels    Channel0     Send0 ����Channel1    Send1 ���� Content   Networks  Network ID mod_fx Version 0.0.0 AllowCompilation 1  UIData  ContentProperties DeviceType 	Desktop  Processor Type 
Analyser ID Analyser1 Bypassed BufferSize 	      �@PreviewType 	      @EditorStates BodyShown 1 Visible 1 Solo 0  ChildProcessors   RoutingMatrix NumSourceChannels    Channel0     Send0 ����Channel1    Send1 ���� Processor Type 
Dynamics ID Dynamics1 Bypassed GateEnabled 	        GateThreshold 	        GateAttack 	      �?GateRelease 	      Y@CompressorEnabled 	      �?CompressorThreshold 	   @3�F�CompressorRatio 	      @CompressorAttack 	      $@CompressorRelease 	      Y@LimiterEnabled 	      �?LimiterThreshold 	   ����LimiterAttack 	      �?LimiterRelease 	      $@CompressorMakeup 	      �?LimiterMakeup 	        EditorStates BodyShown 1 Visible 1 Solo 0  ChildProcessors   RoutingMatrix NumSourceChannels    Channel0     Send0 ����Channel1    Send1 ���� RoutingMatrix NumSourceChannels    Channel0     Send0 ����Channel1    Send1 ���� macro_controls  macro name 	Macro 1 value 	        midi_cc ���� macro name 	Macro 2 value 	        midi_cc ���� macro name 	Macro 3 value 	        midi_cc ���� macro name 	Macro 4 value 	        midi_cc ���� macro name 	Macro 5 value 	        midi_cc ���� macro name 	Macro 6 value 	        midi_cc ���� macro name 	Macro 7 value 	        midi_cc ���� macro name 	Macro 8 value 	        midi_cc ���� MidiAutomation   MPEData Enabled  