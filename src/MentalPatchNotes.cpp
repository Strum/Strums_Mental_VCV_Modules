///////////////////////////////////////////////////
//
//   Patch notes  VCV Module
//   Large text field for putting notes in a patch
//
//   Strum 2017
//
///////////////////////////////////////////////////

#include "mental.hpp"


//////////////////////////////////
MentalPatchNotesWidget::MentalPatchNotesWidget() {
	
	
  
	box.size = Vec(15*14, 380);  
	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		
    panel->setBackground(SVG::load(assetPlugin(plugin,"res/MentalPatchNotes.svg")));
		addChild(panel);
	}	
  patch_notes = new TextField();
  patch_notes->box.pos = Vec(5,20);
  patch_notes->box.size = Vec(200,340);
  //patch_notes->multiline = true;
  addChild(patch_notes); 
}

json_t *MentalPatchNotesWidget::toJson() {
	json_t *rootJ = ModuleWidget::toJson();

	// text
	json_object_set_new(rootJ, "text", json_string(patch_notes->text.c_str()));

	return rootJ;
}

void MentalPatchNotesWidget::fromJson(json_t *rootJ) {
	ModuleWidget::fromJson(rootJ);

	// text
	json_t *textJ = json_object_get(rootJ, "text");
	if (textJ)
		patch_notes->text = json_string_value(textJ);
}
