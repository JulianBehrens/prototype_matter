Processor 
Type SynthChain ID v1.1.3 Bypassed Gain 	      �?Balance 	        VoiceLimit 	      P@KillFadeTime 	      4@IconColour 0 packageName  BuildVersion �  EditorStates BodyShown 0 Visible 1 Solo 0 Folded 1  ChildProcessors  Processor Type MidiProcessorChain ID Midi Processor Bypassed EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors  Processor Type ScriptProcessor ID Interface Bypassed Script �)Content.makeFrontInterface(1040, 660);

Console.clear();
Synth.deferCallbacks(true);
Content.setUseHighResolutionForPanels(true);

Engine.loadNextUserPreset(true);




include("BrowserLAF.js");
include("Icons.js");
include("Licence.js");;


//custom fonts

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
                   Content.getComponent("Knob4a"),
                   Content.getComponent("Knob4b"),
                   Content.getComponent("Knob5"),
                   Content.getComponent("Knob6"),
                   Content.getComponent("Knob7a"),
                   Content.getComponent("Knob7b"),
                   Content.getComponent("Knob8a"),
                   Content.getComponent("Knob8b")];


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



//toggle knobs;

const var Buttonsync = Content.getComponent("Buttonsync");
const var Buttonsync1 = Content.getComponent("Buttonsync1");
const var A = Content.getComponent("Knob4a");
const var B = Content.getComponent("Knob4b");
const var A1 = Content.getComponent("Knob7a");
const var A2 = Content.getComponent("Knob7b");
const var B1 = Content.getComponent("Knob8a");
const var B2 = Content.getComponent("Knob8b");
const var GRAINS = Synth.getEffect("grains")


inline function onButtonsyncControl(number, value)
{
        GRAINS.setAttribute(GRAINS.TempoSync, value);
		A.showControl(value);
        B.showControl(1-value);    
}

inline function onButtonsync1Control(number, value)
{
        GRAINS.setAttribute(GRAINS.Delay_Sync, value);
		A1.showControl(value);
		B1.showControl(value);
        
        A2.showControl(1-value);  
        B2.showControl(1-value);   
}


Buttonsync.setControlCallback(onButtonsyncControl);
Buttonsync1.setControlCallback(onButtonsync1Control);


//zooom

namespace ZoomHandler
{
	const var MIN_ZOOM = 0.85;
	const var MAX_ZOOM = 2.0;
	const var ZOOM_STEP = 0.25;
	const var INTERFACE_WIDTH = 897;
	const var INTERFACE_HEIGHT = 505;

	const var ZoomPanel = Content.getComponent("ZoomPanel");
	
	const var draggerData = [ 110,109,143,130,218,67,147,216,145,66,108,147,216,145,66,143,130,218,67,108,0,0,0,0,143,130,218,67,108,143,130,218,67,0,0,0,0,108,143,130,218,67,147,216,145,66,99,109,143,130,218,67,139,140,96,67,108,139,140,96,67,143,130,218,67,108,66,160,23,67,143,
	130,218,67,108,143,130,218,67,66,160,23,67,108,143,130,218,67,139,140,96,67,99,109,143,130,218,67,102,22,188,67,108,102,22,188,67,143,130,218,67,108,66,160,151,67,143,130,218,67,108,143,130,218,67,66,160,151,67,108,143,130,218,67,102,22,188,67,99,101,
	0,0 ];
	
	
	const var draggerPath = Content.createPath();
	
	
	draggerPath.loadFromData(draggerData);
	
	ZoomPanel.setPaintRoutine(function(g)
	{
		g.setColour(Colours.withAlpha(Colours.black, (this.data.hover && this.data.allowDrag) ? 0.9 : 0.6));
		g.fillPath(draggerPath, [8, 4, 14, 14]);
	});
	
	inline function allowZoom(panel, on)
	{
		panel.data.allowDrag = on;
		panel.setMouseCursor(on ?"BottomRightCornerResizeCursor" : "NormalCursor", Colours.white, [0, 0]);
		panel.repaint();
	}
	
	allowZoom(ZoomPanel, true);
	
	ZoomPanel.setMouseCallback(function(event)
	{
		this.data.hover = event.hover;
		
		if(event.clicked)
		{
			this.data.zoomStart = Settings.getZoomLevel();
		}
		if(event.mouseUp)
		{
			return;
		}
	
		if(event.drag)
		{
			if(!this.data.allowDrag)
				return;
	
			var diagonal = Math.sqrt(INTERFACE_WIDTH*INTERFACE_WIDTH + INTERFACE_HEIGHT*INTERFACE_HEIGHT);
			var currentZoom = Settings.getZoomLevel();
			var dragPixel = 0;
			
			if(event.dragX > event.dragY)
				dragPixel = (event.dragX * currentZoom) / INTERFACE_WIDTH;
			else
				dragPixel = (event.dragY * currentZoom) / INTERFACE_HEIGHT;
			
			
			var maxScaleFactor = Content.getScreenBounds(false)[3] / INTERFACE_HEIGHT;
			var diagonalDrag = this.data.zoomStart + dragPixel;
			
			diagonalDrag += (ZOOM_STEP / 2);
			
			diagonalDrag = Math.min(diagonalDrag, maxScaleFactor);
			
			diagonalDrag -= Math.fmod(diagonalDrag, ZOOM_STEP);
			diagonalDrag = Math.range(diagonalDrag, MIN_ZOOM, MAX_ZOOM);
			
			var zoomToUse = diagonalDrag;
	
			if(currentZoom != zoomToUse)
			{
				Settings.setZoomLevel(zoomToUse);
			}
		}
		
		this.repaint();
	});
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
  EditorStates BodyShown 1 Visible 1 Solo 0 contentShown 1 onInitOpen 1  ChildProcessors   Content  Control type ScriptSliderPack id SliderPack1 value 	      @data [64.MyLy8nYlY5yLyLyO...f+zLyL2SyLyrOyLyL+....9SyLybOMyLy9LyLy7C...3Okl2C9PSCE6yLyLyO...f+.  Control type ScriptSlider id trigger speed value 	         Control type ScriptSlider id Knob4b value 	         Control type ScriptSlider id Knob4a value 	      &@ Control type ScriptSlider id Knob3 value 	      Y@ Control type ScriptSlider id Knob1 value 	      $@ Control type ScriptSlider id Knob7a value 	      @ Control type ScriptSlider id Knob8a value 	      @ Control type ScriptSlider id Knob6 value 	   ����? Control type ScriptSlider id Knob5 value 	      L@ Control type ScriptSlider id Knob2 value 	      (@ Control type ScriptButton id Buttonsync value 	      �? Control type ScriptSlider id grainsize1 value 	      X@ Control type ScriptButton id Buttonsync1 value 	      �? Control type ScriptSlider id main_dial value 	   ����? Control type ScriptSlider id Knob9 value 	   ����? Control type ScriptSlider id Knob7b value 	         Control type ScriptSlider id Knob8b value 	         UIData  ContentProperties DeviceType 	Desktop Component type ScriptImage id background x 	        y 	        width 	     @�@height 	     ��@fileName  {PROJECT_FOLDER}background.png  Component type ScriptPanel id visualizer x 	      z@y 	     �x@width 	     �j@height 	     �W@itemColour 0 itemColour2 0 bgColour 
16777215 textColour 
16777215 enabled 0 Component type ScriptFloatingTile id FloatingTile3 x 	     �T�y 	      R�width 	     w@height 	     pt@ContentType AudioAnalyser bgColour 
16777215 itemColour 3829409705 itemColour2 	4473924 textColour 0 Data 0{
  "ProcessorId": "Analyser3",
  "Index": 0
} parentComponent visualizer  Component type ScriptFloatingTile id FloatingTile1 x 	     �T�y 	     �Q�width 	     w@height 	     @t@ContentType AudioAnalyser bgColour 
16777215 itemColour 2922666444 itemColour2 
16711680 textColour 0 Data 0{
  "ProcessorId": "Analyser1",
  "Index": 0
} parentComponent visualizer  Component type ScriptFloatingTile id FloatingTile2 x 	     �T�y 	     �P�width 	     w@height 	     �s@ContentType AudioAnalyser bgColour 
16777215 itemColour 4279563186 itemColour2 	4473924 textColour 0 Data 0{
  "ProcessorId": "Analyser2",
  "Index": 0
} parentComponent visualizer  Component 
type ScriptPanel id Panel0 x 	     v@y 	      @width 	     �t@height 	     �v@borderRadius 	        borderSize 	        itemColour 0 itemColour2 0 Component type ScriptFloatingTile id fltPresetBrowser x 	      �?y 	       �ContentType PresetBrowser bgColour 0 itemColour 4292864242 Data �{
  "ShowSaveButton": true,
  "ShowExpansionsAsColumn": false,
  "ShowFolderButton": true,
  "ShowNotes": false,
  "ShowEditButtons": true,
  "EditButtonOffset": 10,
  "ShowAddButton": true,
  "ShowRenameButton": false,
  "ShowDeleteButton": true,
  "ShowFavoriteIcon": false,
  "ButtonsInsideBorder": false,
  "NumColumns": 1,
  "ColumnWidthRatio": [
    0.4,
    0.4
  ],
  "ListAreaOffset": [
    0,
    0,
    0,
    0
  ],
  "ColumnRowPadding": [
    10,
    0,
    0,
    10
  ],
  "SearchBarBounds": [
    340,
    5,
    322,
    30
  ],
  "MoreButtonBounds": [],
  "SaveButtonBounds": [],
  "FavoriteButtonBounds": [
    200,
    5,
    30,
    35
  ]
} width 	     �t@height 	     �w@itemColour3 4294901760 itemColour2 0 textColour 4292864242 parentComponent Panel0 Font font_regular  Component 	type ScriptPanel id Panel1 x 	     `v@y 	      $@width 	     @t@height 	     `v@borderRadius 	        borderSize 	        visible Component type ScriptFloatingTile id about x 	      (@y 	      P@ContentType AboutPagePanel bgColour 0 itemColour 4290692558 itemColour2 0 Data �{
  "CopyrightNotice": "Copyright @Prototype Audio",
  "ShowLicensedEmail": true,
  "ShowVersion": true,
  "BuildDate": true,
  "WebsiteURL": "www.prototype.audio",
  "ShowProductName": true,
  "UseCustomImage": false
} width 	     �r@height 	     �l@parentComponent Panel1 Font font_medium textColour 1938663110 FontSize 	      0@ Component 	type ScriptLabel id SerialStateLabel x 	      (@y 	      &@width 	     �r@height 	      �?text  bgColour 	��/�    parentComponent Panel1  Component type ScriptLabel id tag_active x 	      @y 	      $@text Licence: Active fontName font_medium fontSize 	      0@textColour 4290692558 editable 0 width 	     �[@height 	      >@parentComponent Panel1 bgColour 790291 alignment left  Component type ScriptSliderPack id SliderPack2 x 	     �W@y 	     X�@sliderAmount 	       @SliderPackIndex 2 processorId grains width 	      @height 	      >@itemColour 4294967295 itemColour2 
16777215 textColour 
16777215 saveInPreset 0 enabled 0  Component type ScriptSliderPack id SliderPack1 x 	     �\@y 	     H�@width 	     8�@height 	     �S@processorId grains bgColour 
16777215 itemColour 
16777215 itemColour2 
16777215 textColour 
16711680 stepSize 	   ����?showValueOverlay 0  Component type ScriptSlider id trigger speed x 	      8@y 	     ��@isMetaParameter 1 mode TempoSync max 	      2@stepSize 	      �?middlePosition 	      "@itemColour2 1006567424 processorId grains parameterId 
ModSpeed numStrips 	      3@width 	     �S@height 	      C@scaleFactor 	      �?filmstripImage {PROJECT_FOLDER}time.png isPluginParameter 1 pluginParameterName tempo  Component type ScriptSlider id Knob4b x 	      $@y 	      j@middlePosition 	      Y@text Speed$sprint width 	     �p@height 	     �O@itemColour 4294901882 bgColour 	6055537 textColour 4290692558 mode Time max 	     @@stepSize 	      �?isPluginParameter 1 pluginParameterName speed suffix  ms min 	      $@processorId grains parameterId SpeedA visible  Component type ScriptSlider id Knob4a x 	      $@y 	      j@middlePosition 	      "@text Speed$sprint width 	     �p@height 	     �O@itemColour 4294901882 bgColour 	6055537 textColour 4290692558 mode TempoSync max 	      2@stepSize 	      �?isPluginParameter 1 pluginParameterName speed processorId grains parameterId SpeedB  Component type ScriptSlider id Knob3 x 	      $@y 	      $@max 	      Y@stepSize 	      �?suffix % middlePosition 	      I@text Granular Engine$dots width 	     �t@height 	     �O@bgColour 	6055537 itemColour 4294901882 textColour 4290692558 processorId grains parameterId Engine1DryWet isPluginParameter 1 pluginParameterName granular engine mix  Component type ScriptSlider id Knob1 x 	      $@y 	     �a@middlePosition 	     �o@stepSize 	      �?width 	     �t@height 	     �O@text Offset$offset bgColour 	6055537 itemColour 4294901882 textColour 4290692558 processorId grains parameterId Offset max 	     @@min 	      $@suffix ms pluginParameterName offset isPluginParameter 1  Component type ScriptSlider id Knob7a x 	     ȅ@y 	      S@text Left$left max 	      2@middlePosition 	      "@stepSize 	      �?width 	     �p@height 	     �O@bgColour 	6055537 textColour 4290692558 itemColour 4294901882 mode TempoSync isPluginParameter 1 pluginParameterName delay left processorId grains parameterId SpeedL  Component type ScriptSlider id Knob8a x 	     ȅ@y 	     �a@processorId grains parameterId SpeedR max 	      2@mode TempoSync stepSize 	      �?text Right$right width 	     �p@height 	     �O@bgColour 	6055537 textColour 4290692558 itemColour 4294901882 middlePosition 	      "@pluginParameterName delay right isPluginParameter 1  Component type ScriptSlider id Knob6 x 	     ȅ@y 	      j@text Gater$stripes middlePosition 	      �?width 	     �t@height 	     �O@bgColour 	6055537 textColour 4290692558 itemColour 4294901882 suffix % pluginParameterName gater isPluginParameter 1 mode NormalizedPercentage processorId grains parameterId Gate  Component type ScriptSlider id Knob5 x 	     ȅ@y 	      $@text Delay Mix$delay width 	     �t@height 	     �O@bgColour 	6055537 textColour 4290692558 itemColour 4294901882 max 	      Y@stepSize 	      �?suffix % processorId grains parameterId Delay_Mix isPluginParameter 1 pluginParameterName delay mix  Component type ScriptSlider id Knob2 x 	      $@y 	      S@processorId grains parameterId Pitch max 	      (@min 	      (�stepSize 	      �?middlePosition 	        suffix st text Pitch$wave$isBipolar width 	     �t@height 	     �O@bgColour 	6055537 itemColour 4294901882 textColour 4290692558 isPluginParameter 1 pluginParameterName pitch  Component type ScriptButton id Buttonsync x 	     �q@y 	      j@width 	     �O@height 	     �O@bgColour 0 itemColour 0 itemColour2 4278190080 textColour 4278190080 text SNC filmstripImage {PROJECT_FOLDER}sync.png scaleFactor 	      �?isPluginParameter 1 pluginParameterName speed sync  Component type ScriptSlider id grainsize1 x 	     �I@y 	     �q@middlePosition 	     �K@max 	      Y@stepSize 	      �?width 	     �n@height 	     @e@bgColour 	6055537 textColour 4290692558 itemColour 4292864242 numStrips 	      I@min 	      $@filmstripImage {PROJECT_FOLDER}grainsize.png processorId grains parameterId GrainSize scaleFactor 	   ����?isPluginParameter 1 pluginParameterName grainsize mouseSensitivity 	      @ Component type ScriptButton id Buttonsync1 x 	     8�@y 	      S@width 	     �O@height 	      `@bgColour 0 itemColour 0 itemColour2 4278190080 textColour 4278190080 text SNC filmstripImage {PROJECT_FOLDER}sync2.png scaleFactor 	      �?isVertical 0 isPluginParameter 1 pluginParameterName delay sync  Component type ScriptButton id 	Button0 x 	     `v@y 	     �w@width 	     @Y@height 	     �[@text PRESET radioGroup 	      �?filmstripImage {PROJECT_FOLDER}files.png scaleFactor 	      �?saveInPreset 0 isVertical 0  Component type ScriptSlider id main_dial x 	     x@y 	      x@filmstripImage {PROJECT_FOLDER}main_dial.png numStrips 	     �I@width 	     `p@height 	     @Z@scaleFactor 	      �?mouseSensitivity 	      @pluginParameterName sequencer mix processorId grains parameterId depth middlePosition 	      �?isPluginParameter 1  Component type ScriptButton id 	Button1 x 	     H�@y 	     �w@width 	     �X@height 	     �[@text ABOUT radioGroup 	      �?filmstripImage {PROJECT_FOLDER}system.png scaleFactor 	      �?saveInPreset 0 isVertical 0  Component type ScriptSlider id Knob9 x 	     ��@y 	     �r@stepSize 	   @�z�?filmstripImage {PROJECT_FOLDER}feedback.png width 	     �t@height 	     @c@numStrips 	      I@scaleFactor 	      �?processorId grains parameterId Delay_Feedback isPluginParameter 1 pluginParameterName 
feedback mouseSensitivity 	      @ Component type ScriptLabel id Label1 x 	     P�@y 	     P|@text 
Feedback fontName font_medium fontSize 	      4@textColour 4290692558 editable 0 width 	     �r@height 	      >@ Component type ScriptLabel id Label2 x 	      :@y 	     P|@text Grainsize fontName font_medium fontSize 	      4@textColour 4290692558 editable 0 width 	     �r@height 	      >@ Component type ScriptButton id bypass_global x 	      9@y 	     8�@width 	     @S@height 	     �B@filmstripImage {PROJECT_FOLDER}bypass.png scaleFactor 	      �?pluginParameterName bypass isPluginParameter 1 saveInPreset 0 processorId v1.1.2 parameterId Bypass  Component type ScriptSlider id Knob7b x 	     ȅ@y 	      S@text Left$left max 	     @�@middlePosition 	     @@width 	     �p@height 	     �O@bgColour 	6055537 textColour 4290692558 itemColour 4294901882 mode Time isPluginParameter 1 pluginParameterName delay left suffix  ms stepSize 	      �?processorId grains parameterId 
Speed_LA visible  Component type ScriptSlider id Knob8b x 	     ȅ@y 	     �a@max 	     @�@mode Time stepSize 	      �?text Right$right width 	     �p@height 	     �O@bgColour 	6055537 textColour 4290692558 itemColour 4294901882 middlePosition 	     @@pluginParameterName delay right isPluginParameter 1 suffix  ms processorId grains parameterId 
Speed_RA visible  Component 
type ScriptPanel id pnlActivationWrapper x     y     width   height �  itemColour 4279177233 itemColour2 4279177233 borderRadius 	        borderSize 	        Component type ScriptPanel id 
mail_bg1 x 	     �u@y 	      Y@width 	     @u@height 	     �{@textColour 281927404 borderSize 	      �?itemColour2 
97378028 itemColour 
97378028 parentComponent pnlActivationWrapper Component type ScriptLabel id 	subtext x 	      5@y 	     �n@parentComponent 
mail_bg1 fontName font_medium width 	     �r@height 	      4@text 8activation key can be found in your order confirmation editable 0 textColour 1120788204  Component type ScriptLabel id activation_headline x    y T   parentComponent 
mail_bg1 fontName font_bold width 	     �r@height 	      D@text Activation fontSize 	      4@editable 0 textColour 3922583276  Component type ScriptPanel id lic_bg x 	      4@y 	      i@parentComponent 
mail_bg1 width 	     �r@height 	      E@borderSize 	      �?textColour 281927404 itemColour 536870912 itemColour2 536870912  Component type ScriptPanel id 	mail_bg x    y �   parentComponent 
mail_bg1 width 	     �r@height 	      E@borderSize 	      �?textColour 281927404 itemColour2 536870912 itemColour 536870912  Component type ScriptPanel id 	pnlWait x �  y �   visible     width 	(       height 	(       parentComponent pnlActivationWrapper  Component type ScriptButton id btnAuthorize x �  y �  width 	�       height 	#       saveInPreset     parentComponent pnlActivationWrapper isMomentary    filmstripImage {PROJECT_FOLDER}activate.png scaleFactor 	      �? Component type ScriptLabel id lblNotice x _  y o  text  width 	T      height 	       parentComponent pnlActivationWrapper fontName font_medium fontSize 	      .@textColour 2899173100 editable 0 visible      Component type ScriptLabel id lblInputEmail x |  y   text e-mail address width 	      height 	       parentComponent pnlActivationWrapper textColour 1892540140 fontName font_medium fontSize 	      1@alignment left  Component type ScriptLabel id lblInputLicense x }  y 8  text product licence width 	      height 	       parentComponent pnlActivationWrapper textColour 1892540140 fontName font_medium fontSize 	      1@alignment left  Component type ScriptPanel id pnlActivationSuccess x ^  y d   width 	T      height 	�      itemColour 	�    itemColour2 	�    borderSize 	      �?parentComponent pnlActivationWrapper visible     Component type ScriptPanel id bg_div x 	      4@y 	     �a@parentComponent pnlActivationSuccess width 	     �r@height 	      d@borderSize 	      �?textColour 281927404 itemColour2 	        itemColour 	         Component type ScriptLabel id activation_headline1 x 	      4@y 	     �T@parentComponent pnlActivationSuccess fontName font_bold width 	     �r@height 	      D@text Activation fontSize 	      4@editable 0 textColour 3922583276  Component type ScriptLabel id lblActivationSuccess x Y   y �   text Activation successful! width 	�       height 	       parentComponent pnlActivationSuccess fontName font_medium fontSize 	      4@textColour 	��C�    editable 0  Component type ScriptButton id btnCloseAfterActivation x i   y �   text OK width 	�       height 	       saveInPreset     parentComponent pnlActivationSuccess isMomentary    filmstripImage {PROJECT_FOLDER}go.png scaleFactor 	      �? Component 	type ScriptPanel id ZoomPanel x 	     ��@y 	      �@width 	      >@height 	      4@allowCallbacks All Callbacks textColour 	   #    bgColour 	         Processor Type MidiMuter ID MidiMuter1 Bypassed EditorStates BodyShown 1 Visible 1 Solo 0  ChildProcessors   Content  Control type ScriptButton id ignoreButton value 	      �? Control type ScriptButton id fixStuckNotes value 	         Processor Type ModulatorChain ID GainModulation Bypassed Intensity 	      �?EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors   Processor Type ModulatorChain ID PitchModulation Bypassed Intensity 	        EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors   Processor Type EffectChain ID FX Bypassed EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors  Processor Type 
Analyser ID Analyser1 Bypassed BufferSize 	      �@PreviewType 	       @EditorStates BodyShown 1 Visible 1 Solo 0  ChildProcessors   RoutingMatrix NumSourceChannels    Channel0     Send0 ����Channel1    Send1 ���� Processor Type 
Analyser ID Analyser3 Bypassed BufferSize 	      �@PreviewType 	       @EditorStates BodyShown 1 Visible 1 Solo 0  ChildProcessors   RoutingMatrix NumSourceChannels    Channel0     Send0 ����Channel1    Send1 ���� Processor Type Hardcoded Master FX ID grains Bypassed Network mod_fx ModSpeed 	        TempoSync 	      �?GrainSize 	      X@Pitch 	      (@Offset 	      $@Engine1DryWet 	      Y@Delay_Mix 	      L@Delay_Sync 	      �?Delay_Feedback 	   ����?SpeedL 	      @SpeedR 	      @SpeedA 	        SpeedB 	      &@Speed_LA 	        Speed_RA 	        Gate 	   ����?EditorStates BodyShown 1 Visible 1 Solo 0  ChildProcessors   RoutingMatrix NumSourceChannels    Channel0     Send0 ����Channel1    Send1 ���� SliderPacks  SliderPack EmbeddedData [64.MyLy8nYlY5yLyLyO...f+zLyL2SyLyrOyLyL+....9SyLybOMyLy9LyLy7C...3Okl2C9PSCE6yLyLyO...f+.  SliderPack EmbeddedData [64....f+....9C...3O...f+....9C...3O...f+....9C...3O...f+....9C...3O...f+....9C...3O...f+.  SliderPack EmbeddedData 8......zLyL2C  Processor Type 
Analyser ID Analyser2 Bypassed BufferSize 	      �@PreviewType 	       @EditorStates BodyShown 1 Visible 1 Solo 0  ChildProcessors   RoutingMatrix NumSourceChannels    Channel0     Send0 ����Channel1    Send1 ���� RoutingMatrix NumSourceChannels    Channel0     Send0 ����Channel1    Send1 ���� macro_controls  macro name 	Macro 1 value 	        midi_cc ���� macro name 	Macro 2 value 	        midi_cc ���� macro name 	Macro 3 value 	        midi_cc ���� macro name 	Macro 4 value 	        midi_cc ���� macro name 	Macro 5 value 	        midi_cc ���� macro name 	Macro 6 value 	        midi_cc ���� macro name 	Macro 7 value 	        midi_cc ���� macro name 	Macro 8 value 	        midi_cc ���� MidiAutomation   MPEData Enabled  