///////////////////////////////////////////////////
//
//   Mental Plugin
//   Dual 5 in 1 out summing mixers
//
//   Strum 2017-19
//   strum@softhome.net
//
///////////////////////////////////////////////////

#include "mental.hpp"

struct MentalSums : Module
{
	enum ParamIds
  {    
		NUM_PARAMS
	};  
	enum InputIds
  {
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
	enum OutputIds
  {
		OUTPUT_1,
    OUTPUT_2,       
		NUM_OUTPUTS
	};
  
  float sum_out_1 = 0.0;
  float sum_out_2 = 0.0;  
  
	MentalSums()
  {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS);
  }

	void process(const ProcessArgs& args) override;
};

void MentalSums::process(const ProcessArgs& args)
{
  sum_out_1 = 0.0;
  sum_out_2 = 0.0;
  
  for (int i = 0 ; i < 5 ; i++)
  {
    sum_out_1 += inputs[INPUT_1 + i].getVoltage();
    sum_out_2 += inputs[INPUT_2_1 + i].getVoltage();
  }
  
  outputs[OUTPUT_1].setVoltage(sum_out_1);
	outputs[OUTPUT_2].setVoltage(sum_out_2);  
}

////////////////////////////////////////////////////////////////
struct MentalSumsWidget : ModuleWidget
{
	MentalSumsWidget(MentalSums *module)
  {

    setModule(module);

    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/MentalSums.svg")));

    addOutput(createOutput<OutPort>(Vec(3, 22), module, MentalSums::OUTPUT_1));
    addOutput(createOutput<OutPort>(Vec(3, 190), module, MentalSums::OUTPUT_2));
    
    addInput(createInput<InPort>(Vec(3, 58), module, MentalSums::INPUT_1));
    addInput(createInput<InPort>(Vec(3, 83), module, MentalSums::INPUT_2));
    addInput(createInput<InPort>(Vec(3, 108), module, MentalSums::INPUT_3));
    addInput(createInput<InPort>(Vec(3, 133), module, MentalSums::INPUT_4));
    addInput(createInput<InPort>(Vec(3, 158), module, MentalSums::INPUT_5));
    
    addInput(createInput<InPort>(Vec(3, 230), module, MentalSums::INPUT_2_1));
    addInput(createInput<InPort>(Vec(3, 255), module, MentalSums::INPUT_2_2));
    addInput(createInput<InPort>(Vec(3, 280), module, MentalSums::INPUT_2_3));
    addInput(createInput<InPort>(Vec(3, 305), module, MentalSums::INPUT_2_4));
    addInput(createInput<InPort>(Vec(3, 330), module, MentalSums::INPUT_2_5));	
  }
};

Model *modelMentalSums = createModel<MentalSums, MentalSumsWidget>("MentalSums");