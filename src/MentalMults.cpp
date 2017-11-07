#include "mental.hpp"

struct MentalMults : Module {
	enum ParamIds {
		NUM_PARAMS
	};
  
	enum InputIds {		
		INPUT_1,
    INPUT_2,
		NUM_INPUTS
	};
	enum OutputIds {
		OUTPUT_1,
    OUTPUT_2,
		OUTPUT_3,
		OUTPUT_4,
		OUTPUT_5,
    OUTPUT_2_1,
    OUTPUT_2_2,
		OUTPUT_2_3,
		OUTPUT_2_4,
		OUTPUT_2_5,
		NUM_OUTPUTS
	};

	MentalMults() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
	void step() override;
};

/*
MentalMults::MentalMults() {
  params.resize(NUM_PARAMS);
	inputs.resize(NUM_INPUTS);
	outputs.resize(NUM_OUTPUTS);
}*/


void MentalMults::step() {
	
  
  float signal_in_1 = inputs[INPUT_1].value;
  
  outputs[OUTPUT_1].value = signal_in_1;
	outputs[OUTPUT_2].value = signal_in_1;
	outputs[OUTPUT_3].value = signal_in_1;
	outputs[OUTPUT_4].value = signal_in_1;
  outputs[OUTPUT_5].value = signal_in_1;
  
  float signal_in_2 = inputs[INPUT_2].value;
  
  outputs[OUTPUT_2_1].value = signal_in_2;
	outputs[OUTPUT_2_2].value = signal_in_2;
	outputs[OUTPUT_2_3].value = signal_in_2;
	outputs[OUTPUT_2_4].value = signal_in_2;
  outputs[OUTPUT_2_5].value = signal_in_2;
}


MentalMultsWidget::MentalMultsWidget() {
	MentalMults *module = new MentalMults();
	setModule(module);
	box.size = Vec(15*2, 380);
  
	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		//panel->setBackground(SVG::load("plugins/mental/res/MentalMults.svg"));
    panel->setBackground(SVG::load(assetPlugin(plugin,"res/MentalMults.svg")));
		addChild(panel);
	}
	
	addInput(createInput<PJ301MPort>(Vec(3, 22), module, MentalMults::INPUT_1));
  addInput(createInput<PJ301MPort>(Vec(3, 190), module, MentalMults::INPUT_2));
  
  addOutput(createOutput<PJ301MPort>(Vec(3, 58), module, MentalMults::OUTPUT_1));
	addOutput(createOutput<PJ301MPort>(Vec(3, 83), module, MentalMults::OUTPUT_2));
	addOutput(createOutput<PJ301MPort>(Vec(3, 108), module, MentalMults::OUTPUT_3));
	addOutput(createOutput<PJ301MPort>(Vec(3, 133), module, MentalMults::OUTPUT_4));
	addOutput(createOutput<PJ301MPort>(Vec(3, 158), module, MentalMults::OUTPUT_5));
  
  addOutput(createOutput<PJ301MPort>(Vec(3, 230), module, MentalMults::OUTPUT_2_1));
	addOutput(createOutput<PJ301MPort>(Vec(3, 255), module, MentalMults::OUTPUT_2_2));
	addOutput(createOutput<PJ301MPort>(Vec(3, 280), module, MentalMults::OUTPUT_2_3));
	addOutput(createOutput<PJ301MPort>(Vec(3, 305), module, MentalMults::OUTPUT_2_4));
	addOutput(createOutput<PJ301MPort>(Vec(3, 330), module, MentalMults::OUTPUT_2_5));
}
