///////////////////////////////////////////////////
//
//   Mental Sums summing - VCV Module
//
//   Strum 2017
//
///////////////////////////////////////////////////

#include "mental.hpp"

struct MentalSums : Module {
	enum ParamIds {
    
		NUM_PARAMS
	};  
	enum InputIds {
    INPUT_1,
    INPUT_2,
    INPUT_3,
    INPUT_4,
    INPUT_5,
    INPUT_2_1,
    INPUT_2_2,
    INPUT_2_3,
    INPUT_2_4,
    INPUT_2_5,			  
		NUM_INPUTS
	};
	enum OutputIds {
		OUTPUT_1,
    OUTPUT_2,       
		NUM_OUTPUTS
	};
  
  float sum_out_1 = 0.0;
  float sum_out_2 = 0.0;
  
  
	MentalSums() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
	void step();
};

void MentalSums::step()
{
  sum_out_1 = 0.0;
  sum_out_2 = 0.0;
  
  for (int i = 0 ; i < 5 ; i++)
  {
    sum_out_1 += inputs[INPUT_1 + i].value;
    sum_out_2 += inputs[INPUT_2_1 + i].value;
  }
  
  outputs[OUTPUT_1].value = sum_out_1;
	outputs[OUTPUT_2].value = sum_out_2;
  
}

MentalSumsWidget::MentalSumsWidget() {
	MentalSums *module = new MentalSums();
	setModule(module);
	box.size = Vec(15*2, 380);
  
	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		//panel->setBackground(SVG::load("plugins/mental/res/MentalSums.svg"));
    panel->setBackground(SVG::load(assetPlugin(plugin,"res/MentalSums.svg")));
		addChild(panel);
	}
  addOutput(createOutput<PJ301MPort>(Vec(3, 22), module, MentalSums::OUTPUT_1));
  addOutput(createOutput<PJ301MPort>(Vec(3, 190), module, MentalSums::OUTPUT_2));
  
  addInput(createInput<PJ301MPort>(Vec(3, 58), module, MentalSums::INPUT_1));
	addInput(createInput<PJ301MPort>(Vec(3, 83), module, MentalSums::INPUT_2));
	addInput(createInput<PJ301MPort>(Vec(3, 108), module, MentalSums::INPUT_3));
	addInput(createInput<PJ301MPort>(Vec(3, 133), module, MentalSums::INPUT_4));
	addInput(createInput<PJ301MPort>(Vec(3, 158), module, MentalSums::INPUT_5));
  
  addInput(createInput<PJ301MPort>(Vec(3, 230), module, MentalSums::INPUT_2_1));
	addInput(createInput<PJ301MPort>(Vec(3, 255), module, MentalSums::INPUT_2_2));
	addInput(createInput<PJ301MPort>(Vec(3, 280), module, MentalSums::INPUT_2_3));
	addInput(createInput<PJ301MPort>(Vec(3, 305), module, MentalSums::INPUT_2_4));
	addInput(createInput<PJ301MPort>(Vec(3, 330), module, MentalSums::INPUT_2_5));
	
}
