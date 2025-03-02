//browser---------------------------------------------------------------------------------------------------------------------------


const browserLaf = Engine.createGlobalScriptLookAndFeel();
//const browserLaf = Content.createLocalLookAndFeel();


// Search and Fav Icons

browserLaf.registerFunction("createPresetBrowserIcons", function(id)
{
    if (id == "favorite_on")		 	   
		return ICON_NO_4_path;
   
   
    if (id == "favorite_off")
        return ICON_NO_5_path;
        
    if (id == "searchIcon")
        return ICON_NO_6_path;
});

//Background

browserLaf.registerFunction("drawPresetBrowserColumnBackground", function(g, obj)
    {
	    var a = obj.area;

	    if (obj.text == "Add a Bank")
	        obj.text = "Select a Library";
	        
	    if (obj.text == "Select a Column")
	        obj.text = "Select a Category";
	        
	    if (a[2] > 300 && obj.text != "")
	        obj.text = "No Results";		
		g.setColour(0x3D000000);
	    g.fillRoundedRectangle([a[0], a[1], a[2], a[3]], 6);
	    
	    g.setColour(obj.textColour);
	    g.setFontWithSpacing("font_medium", 14, 0.05);
	    g.drawAlignedText(obj.text, a, "centred");
    });

browserLaf.registerFunction("drawPresetBrowserListItem", function(g, obj)
{

			g.setFontWithSpacing("font_medium", 20, 0.05);
	var a = [obj.area[0], obj.area[1], obj.area[2], obj.area[3]];
	
	if (obj.columnIndex == -1)
			 {
			g.setFontWithSpacing("font_medium", 20, 0.05);
				var a = [obj.area[0], obj.area[1], obj.area[2], obj.area[3]];	
			 }
	
		    


//expansion image
	loadExpansionImagesIntoLaf(browserLaf);
	if (obj.columnIndex == -1)
		 {
			g.setColour(Colours.withAlpha(Colours.white, obj.hover ? 1.0 : 0.5));
			g.setColour(Colours.withAlpha(Colours.white, obj.selected ? 1.0 : 0.5));
			g.drawImage(obj.text, a, 0, 0);
		 }



//selected state


	if(obj.selected)
	{

	g.setColour(obj.itemColour);
	g.fillRoundedRectangle(obj.area, 2.0);		    
				

	g.setColour(0xFB111111);
	g.drawAlignedText(obj.text, [a[0] + 10, a[1], a[2] , a[3]], "left");
	



//expansion styling
	
	
	if (obj.columnIndex == -1)
	
	{
		g.setColour(Colours.withAlpha(Colours.white, obj.selected ? 1.0 : 0.5));
		g.drawImage(obj.text, a, 0, 0);
		
		
		g.setGradientFill([(0xFF762EFF), 10, 0.1* obj.area[3], (0xFF2EFFE9), 1.0*obj.area[2] / 1000 - 4, 1.4*obj.area[3]]);		    
		

		
		g.fillRoundedRectangle([a[0], a[1], a[2] / 1000 + 10, a[3]], {CornerSize: 2, Rounded:[1, 0, 1, 0]});
	
	
		g.setColour(0xFFB3B3B4);
		g.drawAlignedText(obj.text, [a[0] + 30, a[1], a[2] , a[3]], "left");



	}






	}

//other styling
else 

{
//unselected preset
	    
		g.setColour(0x60A4B4D6);
		g.drawAlignedText(obj.text, [a[0] + 10, a[1], a[2] , a[3]], "left");

}


});





//Scrollbar

const SCROLLBAR_PADDING = 1;
browserLaf.registerFunction('drawScrollbar', function(g, obj) {
	var a = obj.handle;
	var pa = [
		a[0] + SCROLLBAR_PADDING + 7,
		a[1] + SCROLLBAR_PADDING + 2,
		a[2] - SCROLLBAR_PADDING * 13,
		a[3] - SCROLLBAR_PADDING * 6,
	];




	g.setColour(0xFF0C0C0C);
	g.fillRoundedRectangle(pa, 3);

if (obj.over)
{
	g.setColour(0xFFDFE8F2);
	g.fillRoundedRectangle(pa, 3);
}



});


//dialog----------------------------------------------------------------------------------------------

inline function removeFromTop(area, amountToRemove)
{
    local a = [area[0], area[1], area[2], amountToRemove];
    
    area[1] += amountToRemove;
    area[2] -= amountToRemove;
    return a;
}


browserLaf.registerFunction("drawPresetBrowserDialog", function(g, obj)
{
	
	if (obj.title == "Add new User Preset")
		{
				g.setFontWithSpacing("font_medium", 16, 0);
				//g.setColour(0xDA000000);
				g.fillAll(0xDA000000);
				
				//g.fillRoundedRectangle(obj.area, 3.0);
				g.setColour(0x0CDFE8F2);
   				g.fillRoundedRectangle(obj.labelArea, 2.0);
   				
   				//headline
   				g.setColour(obj.textColour);
   				g.drawAlignedText(obj.text, removeFromTop(obj.area, 50), "centred");
		}
		
		else 
		{
				g.setFontWithSpacing("font_medium", 16, 0);
				g.fillAll(0xDA000000);
				
				g.setColour(0x0CDFE8F2);
   				g.fillRoundedRectangle(obj.labelArea, 2.0);
   				
   				//headline
   				g.setColour(obj.textColour);
   				g.drawAlignedText(obj.text, removeFromTop(obj.area, 130), "centred");
		}
		

    
});

// add & delete buttons

browserLaf.registerFunction("drawDialogButton", function(g, obj)
{
	g.setFontWithSpacing("font_medium", 15, 0.05);

	if (obj.text == "Add")
	{
		g.setFontWithSpacing("font_medium", 15, 0.05);
		g.setColour(obj.textColour);
		g.fillPath(ICON_NO_1_path, [5, 5, 14, 14]);

		//hover
	    
	    if(obj.over)
	    {
		
		g.setColour(0xFFDFE8F2);
		g.fillRoundedRectangle(obj.area, 2.0);
		
	    g.setColour(0xFB111111);	    
	    g.fillPath(ICON_NO_1_path, [5, 5, 14, 14]);

	    }    
		
		//pressed	    
	   
	    if(obj.down)
	    {    
	       
	       
		g.setColour(0xFFDFE8F2);
		g.fillRoundedRectangle(obj.area, 2.0);
		
	    g.setColour(0xFB111111);	    
	    g.fillPath(ICON_NO_1_path, [5, 5, 14, 14]);    
	   	
	   	}

	    g.drawAlignedText(obj.text + " Preset", [30, 5, 70, 15], "left");
	
	}
	
//---------------------------------------------------------------------------------------------------------	


else if (obj.text == "Delete")
	{
		g.setFont("Default", 20.0);
		g.setColour(obj.textColour);
		g.fillPath(ICON_NO_3_path, [6, 5, 20, 15]);	

		//hover
	    
	    if(obj.over)
	    {
		
		g.setColour(0xFFDFE8F2);
		g.fillRoundedRectangle(obj.area, 2.0);
		
	    g.setColour(0xFB111111);	    
	    g.fillPath(ICON_NO_3_path, [6, 5, 20, 15]);	

	    }    
		
		//pressed	    
	   
	    if(obj.down)
	    {    
	       


		g.setColour(0xFFDFE8F2);
		g.fillRoundedRectangle(obj.area, 2.0);
		
	    g.setColour(0xFB111111);	    
	    g.fillPath(ICON_NO_3_path, [6, 5, 20, 15]);	    
	   	
	   	}

	    g.drawAlignedText(obj.text + " File", [40, 5, 70, 15], "left");
	
	}	
	
	
//---------------------------------------------------------------------------------------------------------	
	
	
	else if (obj.text == "Save Preset")
	{
	g.setColour(obj.textColour);
	g.drawAlignedText(obj.text, obj.area, "centred");
	



	
	
	if(obj.over)
	    {

		g.setColour(0xFFDFE8F2);
		g.fillRoundedRectangle(obj.area, 2.0);
		g.setColour(0xFB111111);
		g.drawAlignedText(obj.text, obj.area, "centred");
			

	    }    
		
			        
	if(obj.down)
	g.fillAll(0x22000000);			        
	g.setColour(Colours.white);

	}    
	
//---------------------------------------------------------------------------------------------------------		
	
	else if (obj.text == "More")
	{
	g.setColour(obj.textColour);
	g.drawAlignedText(obj.text, obj.area, "centred");
	



	
	
	if(obj.over)
	    {

		g.setColour(0xFFDFE8F2);
		g.fillRoundedRectangle(obj.area, 2.0);
		g.setColour(0xFB111111);
		g.drawAlignedText(obj.text, obj.area, "centred");
			

	    }    
		
			        
	if(obj.down)
	g.fillAll(0x22000000);			        
	g.setColour(Colours.white);

	}    

	
	else if (obj.text == "OK")
	{
		g.setColour(0xFFDFE8F2);
		g.fillRoundedRectangle(obj.area, 2.0);
		g.setColour(0xFB111111);
		g.drawAlignedText(obj.text, obj.area, "centred");
	



	
	
	if(obj.over)
	    {

		g.setColour(0xFFDFE8F2);
		g.fillRoundedRectangle(obj.area, 2.0);
		g.setColour(0xFB111111);
		g.drawAlignedText(obj.text, obj.area, "centred");
			

	    }    
		
			        
	if(obj.down)
	g.fillAll(0x22000000);			        
	g.setColour(Colours.white);

	}    
	
	
	else if (obj.text == "Cancel")
	{
	g.setColour(obj.textColour);
	g.drawAlignedText(obj.text, obj.area, "centred");
	g.setColour(0x11DFE8F2);
	g.fillRoundedRectangle(obj.area, 2.0);
	
	



	
	
	if(obj.over)
	    {

		g.setColour(0xFFDFE8F2);
		g.fillRoundedRectangle(obj.area, 2.0);
		g.setColour(0xFB111111);
		g.drawAlignedText(obj.text, obj.area, "centred");
			

	    }    
		
			        
	if(obj.down)
	g.fillAll(0x22000000);			        
	g.setColour(Colours.white);

	}    
	   
	
	else if (obj.text == "Replace All")
	{
	
	} 
	
	else if (obj.text == "Replace Next")
	{
	
	} 
	
	else if (obj.text == "Next")
	{
	
	}  
	
	
	else if (obj.text == "Undo")
	{
	
	}  
	
	
	else if (obj.text == "Save")
	{
	
	}  
	
	 
	else if (obj.text == "Change sample folder location")
	{
	
	}  
	
	
	else 
	{
		g.fillAll(0x22000000);
    
	    if(obj.over)
	        g.fillAll(0x22000000);
	        
	    if(obj.down)
	        g.fillAll(0x22000000);
	        
	    g.setColour(obj.textColour);
	    g.drawAlignedText(obj.text, obj.area, "centred");
	}  
    
});



//Search Bar
browserLaf.registerFunction("drawPresetBrowserSearchBar", function(g, obj)
{
    g.setColour(0xFF0C0C0C); 
    g.fillRoundedRectangle([0, 0, 322, 30], 2.0);
    g.setGradientFill([(0xFF762EFF), 2, 0.1* obj.area[3], (0xFF2EFFE9), 1.0*obj.area[2] / 1000 - 2, 1.4*obj.area[3]]);
    g.fillPath(obj.icon, [8, 8, 15, 15]);
    
});






// fltPresetBrowser

inline function loadExpansionImagesIntoLaf(obj)
{
	local expHandler = Engine.createExpansionHandler();
	
	for (e in expHandler.getExpansionList())
	{
		local img = e.getWildcardReference("Icon.png");
		
		if (isDefined(img))
		{
			obj.loadImage(img, e.getProperties().Name);
		}
	}
}

const var fltPresetBrowser = Content.getComponent("fltPresetBrowser");
fltPresetBrowser.setLocalLookAndFeel(browserLaf);


//--------------------------------------------------------------------------------------------------------------------

const LAF = Engine.createGlobalScriptLookAndFeel();

LAF.registerFunction("getIdealPopupMenuItemSize", function(obj)
{
	
	return [211, 30]; 

	
});

LAF.registerFunction("drawPopupMenuItem", function(g, obj)
{
    var a = obj.area;
	
	g.setFontWithSpacing("font_medium", 15, 0.05);
	g.setColour(0x70DFE8F2);
	g.drawAlignedText(obj.text, [a[0] + 10, a[1], a[2], a[3]], "left");
	
   
    if(obj.isHighlighted)
    {
        g.setColour(0xFFDFE8F2);
        g.fillRoundedRectangle([3, 1, 209, 28], 1.0);
        g.setColour(0xFB111111);
        g.drawAlignedText(obj.text, [a[0] + 10, a[1], a[2], a[3]], "left");
        
    }
    
    if(obj.isSeparator)
    {
    	g.fillAll(0xFF000000);
    	g.setColour(0xFFDFE8F2);
    	g.fillRoundedRectangle([3, 15, 10, 1], 1.0);	
    }

});

LAF.registerFunction("drawPopupMenuBackground", function(g, obj)
{	
	g.fillAll(0xFF000000);
	//01
	g.setColour(0x10DFE8F2);
	g.fillRoundedRectangle([3, 3, 209, 29], 1.0);
	//02
	g.setColour(0x10DFE8F2);
	g.fillRoundedRectangle([3, 63, 209, 58], 1.0);
	//03
	g.setColour(0x10DFE8F2);
	g.fillRoundedRectangle([3, 153, 209, 58], 1.0);

});





//other dialogue----------------------------------------------------------------

LAF.registerFunction("drawAlertWindow", function(g, obj)
{
    //g.setColour(0xFFDFE8F2);
	//g.fillRoundedRectangle(obj.area, 10.0);
    g.fillAll(0xDA000000);
    
    var a = [0, 0, obj.area[2], 40];
    g.setColour(0x0CDFE8F2);
    g.fillRect(a);
    g.setColour(0x10DFE8F2);
    g.drawRect(obj.area, 1.0);
	g.setFontWithSpacing("font_bold", 16, 0);
    g.setColour(0xFFDFE8F2);
    g.drawAlignedText(obj.title, a, "centred");
    
    
});

LAF.registerFunction("drawAlertWindowIcon", function(g, obj)
{
	var p = Content.createPath();	

    if(obj.type == "Question")
        p.loadFromData([110,109,140,129,15,68,16,19,221,67,98,227,104,14,68,16,19,221,67,139,124,13,68,14,83,220,67,132,188,12,68,10,211,218,67,98,124,252,11,68,8,83,217,67,121,156,11,68,224,114,215,67,121,156,11,68,220,50,213,67,98,121,156,11,68,114,1,211,67,124,252,11,68,
206,40,209,67,132,188,12,68,204,168,207,67,98,139,124,13,68,164,40,206,67,227,104,14,68,162,104,205,67,140,129,15,68,162,104,205,67,108,52,121,22,68,162,104,205,67,108,137,76,13,68,106,23,143,67,98,216,115,11,68,4,107,141,67,65,6,10,68,160,35,139,67,
193,3,9,68,245,64,136,67,98,64,1,8,68,110,94,133,67,0,128,7,68,195,128,129,67,0,128,7,68,234,79,121,67,98,0,128,7,68,240,177,110,67,30,123,8,68,27,111,101,67,89,113,10,68,106,135,93,67,98,147,103,12,68,186,159,85,67,32,186,14,68,226,171,81,67,2,105,17,
68,226,171,81,67,98,146,235,19,68,226,171,81,67,8,3,22,68,138,105,84,67,104,175,23,68,212,228,89,67,98,206,91,25,68,29,96,95,67,32,120,26,68,44,143,102,67,113,4,27,68,250,113,111,67,108,102,167,39,68,250,113,111,67,108,102,167,39,68,60,172,90,67,98,102,
167,39,68,52,231,87,67,136,226,39,68,243,144,85,67,168,88,40,68,129,169,83,67,98,218,206,40,68,8,194,81,67,60,102,41,68,75,206,80,67,239,30,42,68,75,206,80,67,98,96,193,42,68,75,206,80,67,82,70,43,68,206,208,81,67,196,173,43,68,206,213,83,67,98,36,21,
44,68,205,218,85,67,213,72,44,68,71,34,88,67,213,72,44,68,60,172,90,67,108,169,198,50,68,63,235,65,67,98,201,60,51,68,148,18,64,67,60,212,51,68,196,227,62,67,222,140,52,68,206,94,62,67,98,128,69,53,68,225,217,61,67,209,1,54,68,168,72,62,67,229,193,54,
68,44,171,63,67,108,75,55,68,68,89,162,89,67,98,60,188,68,68,202,201,90,67,142,24,69,68,154,88,92,67,80,76,69,68,218,78,94,67,98,0,128,69,68,19,69,96,67,222,116,69,68,160,103,98,67,15,43,69,68,122,182,100,67,98,206,217,68,68,83,5,103,67,149,112,68,68,
120,96,104,67,84,239,67,68,224,199,104,67,98,18,110,67,68,71,47,105,67,248,234,66,68,63,207,104,67,6,102,66,68,214,167,103,67,108,192,6,53,68,162,176,77,67,108,118,161,44,68,154,202,111,67,108,118,161,44,68,87,185,128,67,108,192,6,53,68,180,141,146,67,
108,6,102,66,68,206,101,133,67,98,88,242,66,68,47,210,132,67,74,119,67,68,44,173,132,67,219,244,67,68,14,247,132,67,98,109,114,68,68,239,64,133,67,206,217,68,68,145,249,133,67,15,43,69,68,243,32,135,67,98,222,116,69,68,121,72,136,67,40,126,69,68,123,
104,137,67,182,70,69,68,30,129,138,67,98,86,15,69,68,192,153,139,67,125,173,68,68,5,112,140,67,44,33,68,68,164,3,141,67,108,134,26,55,68,199,55,153,67,98,178,75,54,68,172,6,154,67,63,132,53,68,93,106,154,67,62,196,52,68,253,98,154,67,98,60,4,52,68,158,
91,154,67,90,90,51,68,42,196,153,67,169,198,50,68,200,156,152,67,108,213,72,44,68,67,60,140,67,98,213,72,44,68,168,232,141,67,36,21,44,68,42,38,143,67,196,173,43,68,15,245,143,67,98,82,70,43,68,208,195,144,67,96,193,42,68,48,43,145,67,239,30,42,68,48,
43,145,67,98,60,102,41,68,48,43,145,67,218,206,40,68,96,177,144,67,168,88,40,68,156,189,143,67,98,136,226,39,68,217,201,142,67,102,167,39,68,199,158,141,67,102,167,39,68,67,60,140,67,108,102,167,39,68,20,173,129,67,108,33,216,26,68,20,173,129,67,98,33,
216,26,68,91,30,131,67,72,166,26,68,173,143,132,67,152,66,26,68,240,0,134,67,98,231,222,25,68,52,114,135,67,141,106,25,68,26,161,136,67,155,229,24,68,90,141,137,67,108,111,108,42,68,162,104,205,67,108,45,41,52,68,162,104,205,67,98,208,65,53,68,162,104,
205,67,250,47,54,68,164,40,206,67,189,243,54,68,204,168,207,67,98,110,183,55,68,206,40,209,67,71,25,56,68,114,1,211,67,71,25,56,68,220,50,213,67,98,71,25,56,68,224,114,215,67,110,183,55,68,8,83,217,67,189,243,54,68,10,211,218,67,98,250,47,54,68,14,83,
220,67,208,65,53,68,16,19,221,67,45,41,52,68,16,19,221,67,108,140,129,15,68,16,19,221,67,99,109,2,105,17,68,208,133,134,67,98,134,203,18,68,208,133,134,67,165,246,19,68,224,149,133,67,96,234,20,68,195,181,131,67,98,26,222,21,68,177,213,129,67,249,87,
22,68,128,247,126,67,249,87,22,68,234,79,121,67,98,249,87,22,68,76,168,115,67,26,222,21,68,107,244,110,67,96,234,20,68,70,52,107,67,98,165,246,19,68,34,116,103,67,134,203,18,68,16,148,101,67,2,105,17,68,16,148,101,67,98,27,255,15,68,16,148,101,67,34,
210,14,68,34,116,103,67,25,226,13,68,70,52,107,67,98,16,242,12,68,107,244,110,67,12,122,12,68,76,168,115,67,12,122,12,68,234,79,121,67,98,12,122,12,68,128,247,126,67,16,242,12,68,177,213,129,67,25,226,13,68,195,181,131,67,98,34,210,14,68,224,149,133,
67,27,255,15,68,208,133,134,67,2,105,17,68,208,133,134,67,99,109,69,3,28,68,162,104,205,67,108,128,237,36,68,162,104,205,67,108,212,156,20,68,104,247,141,67,98,32,105,20,68,74,65,142,67,226,23,20,68,123,135,142,67,25,169,19,68,217,201,142,67,98,80,58,
19,68,90,12,143,67,94,229,18,68,14,53,143,67,73,170,18,68,205,67,143,67,108,69,3,28,68,162,104,205,67,99,101,0,0]);

    g.setColour(0xFFDFE8F2);
    g.fillPath(p, [6, 6, obj.area[2]-30, obj.area[3]-35]);
});
