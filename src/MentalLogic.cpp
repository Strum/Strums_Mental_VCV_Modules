///////////////////////////////////////////////////
//
//   Logic Gates VCV Module
//
//   Strum 2017
//
///////////////////////////////////////////////////

#include "mental.hpp"

struct MentalLogic : Module {
	enum ParamIds {
		NUM_PARAMS
	};  
	enum InputIds {		
		INPUT_A_1,
    INPUT_B_1,
    INPUT_A_2,
    INPUT_B_2,
    INPUT_INV_1,
    INPUT_INV_2,
    INPUT_A_3,
    INPUT_B_3,
    INPUT_C_3,
    INPUT_D_3,
    INPUT_E_3,    
    NUM_INPUTS
	};
	enum OutputIds {
		OUTPUT_AND_1,
    OUTPUT_OR_1,
    OUTPUT_AND_2,
    OUTPUT_OR_2,
    OUTPUT_INV_1,
    OUTPUT_INV_2,
    OUTPUT_OR_3,        
		NUM_OUTPUTS
	};

  float and_led_1 = 0.0;
  float or_led_1 = 0.0;
  float and_led_2 = 0.0;
  float or_led_2 = 0.0;
  float inv_led_1 = 0.0;
  float inv_led_2 = 0.0;
  float or_led_3 = 0.0;
  
	MentalLogic() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
	void step();
};

void MentalLogic::step()
{
  
  float signal_in_A1 = inputs[INPUT_A_1].value;
  float signal_in_B1 = inputs[INPUT_B_1].value;
  float signal_in_A2 = inputs[INPUT_A_2].value;
  float signal_in_B2 = inputs[INPUT_B_2].value;
  float inv_1_input = inputs[INPUT_INV_1].value;
  float inv_2_input = inputs[INPUT_INV_2].value;
  float or_3_A_input = inputs[INPUT_A_3].value;
  float or_3_B_input = inputs[INPUT_B_3].value;
  float or_3_C_input = inputs[INPUT_C_3].value;
  float or_3_D_input = inputs[INPUT_D_3].value;
  float or_3_E_input = inputs[INPUT_E_3].value;
  
  if (inv_1_input > 0.0)
  { 
    outputs[OUTPUT_INV_1].value = 0.0;
    inv_led_1 = 0.0;
  }
  else
  {
    outputs[OUTPUT_INV_1].value = 1.0;
    inv_led_1 = 1.0;
  }
  if (inv_2_input > 0.0)
  { 
    outputs[OUTPUT_INV_2].value = 0.0;
    inv_led_2 = 0.0;
  }
  else
  {
    outputs[OUTPUT_INV_2].value = 1.0;
    inv_led_2 = 1.0;
  }
    
  //////////////////////////
    
  if (signal_in_A1 > 0.0 && signal_in_B1 > 0.0 )
  {
    outputs[OUTPUT_AND_1].value = 1.0;    
    and_led_1 = 1.0;
  }
  else 
  {
    outputs[OUTPUT_AND_1].value = 0.0;    
    and_led_1 = 0.0;
  }
  if (signal_in_A1 > 0.0 || signal_in_B1 > 0.0 )
  {
    outputs[OUTPUT_OR_1].value = 1.0;
    or_led_1 = 1.0;
  }
  else 
  {
    outputs[OUTPUT_OR_1].value = 0.0;    
    or_led_1 = 0.0;
  }
  //////////////////////////////////////
  if (signal_in_A2 > 0.0 && signal_in_B2 > 0.0 )
  {
    outputs[OUTPUT_AND_2].value = 1.0;    
    and_led_2 = 1.0;
  }
  else 
  {
    outputs[OUTPUT_AND_2].value = 0.0;    
    and_led_2 = 0.0;
  }
  if (signal_in_A2 > 0.0 || signal_in_B2 > 0.0 )
  {
    outputs[OUTPUT_OR_2].value = 1.0;
    or_led_2 = 1.0;
  }
  else 
  {
    outputs[OUTPUT_OR_2].value = 0.0;    
    or_led_2 = 0.0;
  } 
  //////////////// Big or
  if ( or_3_A_input > 0.0 || or_3_B_input > 0.0 || or_3_C_input > 0.0 || or_3_D_input > 0.0 || or_3_E_input > 0.0 )
  {
    outputs[OUTPUT_OR_3].value = 1.0;
    or_led_3 = 1.0;
  }
  else 
  {
    outputs[OUTPUT_OR_3].value = 0.0;    
    or_led_3 = 0.0;
  } 
}

/////////////////////////
MentalLogicWidget::MentalLogicWidget() {
	MentalLogic *module = new MentalLogic();
	setModule(module);
	box.size = Vec(15*5, 380);
  
	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		//panel->setBackground(SVG::load("plugins/mental/res/MentalLogic.svg"));
    panel->setBackground(SVG::load(assetPlugin(plugin,"res/MentalLogic.svg")));
		addChild(panel);
	}
	
  int input_column = 3;
  int output_column = 28;
  int led_column = 58;
  int first_row = 25;
  int row_spacing = 25;
  int vert_offset = 60;
  
  addInput(createInput<PJ301MPort>(Vec(input_column, first_row), module, MentalLogic::INPUT_A_1));
  addInput(createInput<PJ301MPort>(Vec(input_column, first_row+row_spacing), module, MentalLogic::INPUT_B_1));  
  
  addOutput(createOutput<PJ301MPort>(Vec(output_column, first_row), module, MentalLogic::OUTPUT_AND_1));
  addOutput(createOutput<PJ301MPort>(Vec(output_column, first_row+row_spacing), module, MentalLogic::OUTPUT_OR_1));  
  
  addChild(createValueLight<SmallLight<GreenValueLight>>(Vec(led_column, first_row + 8), &module->and_led_1));
  addChild(createValueLight<SmallLight<GreenValueLight>>(Vec(led_column, first_row+row_spacing + 8), &module->or_led_1));
  
  ////////////////////////////
  
  addInput(createInput<PJ301MPort>(Vec(input_column, vert_offset + first_row), module, MentalLogic::INPUT_A_2));
  addInput(createInput<PJ301MPort>(Vec(input_column, vert_offset + first_row + row_spacing), module, MentalLogic::INPUT_B_2));  
  
  addOutput(createOutput<PJ301MPort>(Vec(output_column, vert_offset + first_row), module, MentalLogic::OUTPUT_AND_2));
  addOutput(createOutput<PJ301MPort>(Vec(output_column, vert_offset + first_row + row_spacing), module, MentalLogic::OUTPUT_OR_2));  
  
  addChild(createValueLight<SmallLight<GreenValueLight>>(Vec(led_column, vert_offset +  first_row + 8), &module->and_led_2));
  addChild(createValueLight<SmallLight<GreenValueLight>>(Vec(led_column, vert_offset +first_row + row_spacing + 8), &module->or_led_2));
  
  ///// Inverters
  
  addInput(createInput<PJ301MPort>(Vec(input_column, vert_offset * 2 + first_row), module, MentalLogic::INPUT_INV_1));
  addInput(createInput<PJ301MPort>(Vec(input_column, vert_offset * 2 + first_row + row_spacing), module, MentalLogic::INPUT_INV_2));  
  
  addOutput(createOutput<PJ301MPort>(Vec(output_column, vert_offset * 2 + first_row), module, MentalLogic::OUTPUT_INV_1));
  addOutput(createOutput<PJ301MPort>(Vec(output_column, vert_offset * 2 + first_row + row_spacing), module, MentalLogic::OUTPUT_INV_2));
  
  addChild(createValueLight<SmallLight<GreenValueLight>>(Vec(led_column, vert_offset * 2 + first_row + 8), &module->inv_led_1));
  addChild(createValueLight<SmallLight<GreenValueLight>>(Vec(led_column, vert_offset * 2 + first_row + row_spacing + 8), &module->inv_led_2));
  
  ////// Big or
  addInput(createInput<PJ301MPort>(Vec(input_column, vert_offset + 150), module, MentalLogic::INPUT_A_3));
  addInput(createInput<PJ301MPort>(Vec(input_column, vert_offset + 175), module, MentalLogic::INPUT_B_3));
  addInput(createInput<PJ301MPort>(Vec(input_column, vert_offset + 200), module, MentalLogic::INPUT_C_3));
  addInput(createInput<PJ301MPort>(Vec(input_column, vert_offset + 225), module, MentalLogic::INPUT_D_3));
  addInput(createInput<PJ301MPort>(Vec(input_column, vert_offset + 250), module, MentalLogic::INPUT_E_3));  
  
	addOutput(createOutput<PJ301MPort>(Vec(output_column, vert_offset + 150), module, MentalLogic::OUTPUT_OR_3));
  addChild(createValueLight<SmallLight<GreenValueLight>>(Vec(led_column, vert_offset + 158), &module->or_led_3));   
}
