///////////////////////////////////////////////////
//
//   Multiplexers VCV Module
//
//   Strum 2017
//
///////////////////////////////////////////////////

#include "mental.hpp"

struct MentalMuxes : Module {
	enum ParamIds {
		NUM_PARAMS
	};  
	enum InputIds {		
		INPUT_1A,
    INPUT_2A,
    SELECT_A,
    INPUT_1B,
    INPUT_2B,
    SELECT_B, 
    INPUT_1C,
    INPUT_2C,
    INPUT_3C,
    INPUT_4C,
    SELECT_C,     
		NUM_INPUTS
	};
	enum OutputIds {
		OUTPUT_A,
    OUTPUT_B,
    OUTPUT_C,        
		NUM_OUTPUTS
	};

  float level_led_a1 = 0.0;
  float level_led_a2 = 0.0;
  float level_led_b1 = 0.0;
  float level_led_b2 = 0.0;
  float level_led_c1 = 0.0;
  float level_led_c2 = 0.0;
  float level_led_c3 = 0.0;
  float level_led_c4 = 0.0;
  
	MentalMuxes() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
	void step();
};

void MentalMuxes::step()
{
  
  float signal_in_a1 = inputs[INPUT_1A].value;
  float signal_in_a2 = inputs[INPUT_2A].value;
  float select_a = inputs[SELECT_A].value;
  
  if (select_a > 0.0 )
  {
    outputs[OUTPUT_A].value = signal_in_a2;
    level_led_a2 = abs((signal_in_a2 / 3));
    level_led_a1 = 0.0;
  }
  else
  {
    outputs[OUTPUT_A].value = signal_in_a1;
    level_led_a1 = abs((signal_in_a1 / 3));
    level_led_a2 = 0.0;
  }
  float signal_in_b1 = inputs[INPUT_1B].value;
  float signal_in_b2 = inputs[INPUT_2B].value;
  float select_b = inputs[SELECT_B].value;
  
  if (select_b > 0.0 )
  {
    outputs[OUTPUT_B].value = signal_in_b2;
    level_led_b2 = abs((signal_in_b2 / 3));
    level_led_b1 = 0.0;
  }
  else
  {
    outputs[OUTPUT_B].value = signal_in_b1;
    level_led_b1 = abs((signal_in_b1 / 3));
    level_led_b2 = 0.0;
  }
  
  float signal_in_c1 = inputs[INPUT_1C].value;
  float signal_in_c2 = inputs[INPUT_2C].value;
  float signal_in_c3 = inputs[INPUT_3C].value;
  float signal_in_c4 = inputs[INPUT_4C].value;
  float select_c = inputs[SELECT_C].value;
  int selector = round(abs(select_c));
  if (selector > 3) selector = 3;
    
  if (selector == 0 )
  {
    outputs[OUTPUT_C].value = signal_in_c1;
    level_led_c1 = abs((signal_in_c1 / 3));
    level_led_c2 = 0.0;
    level_led_c3 = 0.0;
    level_led_c4 = 0.0;    
  }
  if (selector == 1 )
  {
    outputs[OUTPUT_C].value = signal_in_c2;
    level_led_c2 = abs((signal_in_c2 / 3));
    level_led_c1 = 0.0;
    level_led_c3 = 0.0;
    level_led_c4 = 0.0;
  }
  if (selector == 2 )
  {
    outputs[OUTPUT_C].value = signal_in_c3;
    level_led_c3 = abs((signal_in_c3 / 3));
    level_led_c1 = 0.0;
    level_led_c2 = 0.0;
    level_led_c4 = 0.0;    
  }
  if (selector == 3 )
  {
    outputs[OUTPUT_C].value = signal_in_c4;
    level_led_c4 = abs((signal_in_c4 / 3));
    level_led_c1 = 0.0;
    level_led_c2 = 0.0;
    level_led_c3 = 0.0;
  }
}

MentalMuxesWidget::MentalMuxesWidget() {
	MentalMuxes *module = new MentalMuxes();
	setModule(module);
	box.size = Vec(15*4, 380);
  
	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		//panel->setBackground(SVG::load("plugins/mental/res/MentalMuxes.svg"));
    panel->setBackground(SVG::load(assetPlugin(plugin,"res/MentalMuxes.svg")));
		addChild(panel);
	}
	int group_offset = 90;
  addInput(createInput<PJ301MPort>(Vec(3, 75), module, MentalMuxes::SELECT_A));  
	addInput(createInput<PJ301MPort>(Vec(3, 25), module, MentalMuxes::INPUT_1A));
  addInput(createInput<PJ301MPort>(Vec(3, 50), module, MentalMuxes::INPUT_2A));  
  
  addOutput(createOutput<PJ301MPort>(Vec(33, 75), module, MentalMuxes::OUTPUT_A));
  
  addChild(createValueLight<SmallLight<GreenValueLight>>(Vec(41, 32), &module->level_led_a1));
  addChild(createValueLight<SmallLight<GreenValueLight>>(Vec(41, 58), &module->level_led_a2));
  
  addInput(createInput<PJ301MPort>(Vec(3, group_offset + 75), module, MentalMuxes::SELECT_B));  
	addInput(createInput<PJ301MPort>(Vec(3, group_offset + 25), module, MentalMuxes::INPUT_1B));
  addInput(createInput<PJ301MPort>(Vec(3, group_offset + 50), module, MentalMuxes::INPUT_2B));  
  
  addOutput(createOutput<PJ301MPort>(Vec(33,group_offset + 75), module, MentalMuxes::OUTPUT_B));
  
  addChild(createValueLight<SmallLight<GreenValueLight>>(Vec(41,group_offset + 32), &module->level_led_b1));
  addChild(createValueLight<SmallLight<GreenValueLight>>(Vec(41,group_offset + 58), &module->level_led_b2));
  
  addInput(createInput<PJ301MPort>(Vec(3, group_offset * 2 + 125), module, MentalMuxes::SELECT_C));  
	addInput(createInput<PJ301MPort>(Vec(3, group_offset * 2 + 25), module, MentalMuxes::INPUT_1C));
  addInput(createInput<PJ301MPort>(Vec(3, group_offset * 2 + 50), module, MentalMuxes::INPUT_2C));
  addInput(createInput<PJ301MPort>(Vec(3, group_offset * 2 + 75), module, MentalMuxes::INPUT_3C));
  addInput(createInput<PJ301MPort>(Vec(3, group_offset * 2 + 100), module, MentalMuxes::INPUT_4C));    
  
  addOutput(createOutput<PJ301MPort>(Vec(33,group_offset * 2 + 125), module, MentalMuxes::OUTPUT_C));
  
  addChild(createValueLight<SmallLight<GreenValueLight>>(Vec(41,group_offset * 2 + 32), &module->level_led_c1));
  addChild(createValueLight<SmallLight<GreenValueLight>>(Vec(41,group_offset * 2 + 58), &module->level_led_c2));
  addChild(createValueLight<SmallLight<GreenValueLight>>(Vec(41,group_offset * 2 + 82), &module->level_led_c3));
  addChild(createValueLight<SmallLight<GreenValueLight>>(Vec(41,group_offset * 2 + 108), &module->level_led_c4));
	  
}
