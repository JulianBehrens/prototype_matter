Content.makeFrontInterface(600, 600);

//! =================== [ Samplemap List ] ================== !
const var SamplemapList = Content.addViewport("SamplemapList", 0, 0);

// Allow the viewport to list the items
SamplemapList.set("useList", true);

const var sampleMapList = Sampler.getSampleMapList();


// Populate the formatted list in a new array
const var formattedSamplemaps = [];

for (i = 0; i < sampleMapList.length; i++){
    
    var newName = sampleMapList[i];
    newName = newName.replace("HT_", "");
    
    formattedSamplemaps.push(newName);
}

// Update the viewport
SamplemapList.set("items", formattedSamplemaps.join("\n"));

const var ScriptnodeSyntesiser1 = Synth.getAudioSampleProcessor("Scriptnode Syntesiser1");

const var af = Engine.createAndRegisterAudioFile(0);
const slot = ScriptnodeSyntesiser1.getAudioFile(0);

var sampleMaps = Sampler.getSampleMapList();



const var ComboBox1 = Content.getComponent("ComboBox1");

ComboBox1.set("items", "");
ComboBox1.set("items", sampleMaps.join("\n"));
//ComboBox1.set("items", sampleMaps.replace("HT"));
inline function onComboBox1Control(component, value)
{
slot.loadFile("{XYZ::SampleMap}" + component.getItemText());
};

Content.getComponent("ComboBox1").setControlCallback(onComboBox1Control);
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
 