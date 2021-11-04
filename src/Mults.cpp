///////////////////////////////////////////////////
//
//   Mental Plugin
//   Multiples - dual 1 in 5 out multiples
//
//   Strum 2017-21
//   strum@softhome.net
//
///////////////////////////////////////////////////

#include "mental.hpp"

struct Mults : Module
{
	enum ParamIds
	{
		NUM_PARAMS
	};

	enum InputIds
	{
		INPUT_1,
    	INPUT_2,
		NUM_INPUTS
	};
	enum OutputIds
	{
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

	Mults()
	{
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS);
	}

	void process(const ProcessArgs& args) override;
};

void Mults::process(const ProcessArgs& args)
{
  	float signal_in_1 = inputs[INPUT_1].getVoltage();

  	outputs[OUTPUT_1].setVoltage(signal_in_1);
	outputs[OUTPUT_2].setVoltage(signal_in_1);
	outputs[OUTPUT_3].setVoltage(signal_in_1);
	outputs[OUTPUT_4].setVoltage(signal_in_1);
  	outputs[OUTPUT_5].setVoltage(signal_in_1);

 	float signal_in_2 = inputs[INPUT_2].getVoltage();

  	outputs[OUTPUT_2_1].setVoltage(signal_in_2);
	outputs[OUTPUT_2_2].setVoltage(signal_in_2);
	outputs[OUTPUT_2_3].setVoltage(signal_in_2);
	outputs[OUTPUT_2_4].setVoltage(signal_in_2);
  	outputs[OUTPUT_2_5].setVoltage(signal_in_2);
}

/////////////////////////////////////////////////////////////////////////////////////////
struct MultsWidget : ModuleWidget
{
	MultsWidget(Mults *module)
	{
		setModule(module);

		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Mults.svg")));

		addInput(createInput<InPort>(Vec(3, 22), module, Mults::INPUT_1));
  		addInput(createInput<InPort>(Vec(3, 190), module, Mults::INPUT_2));

  		addOutput(createOutput<OutPort>(Vec(3, 58), module, Mults::OUTPUT_1));
		addOutput(createOutput<OutPort>(Vec(3, 83), module, Mults::OUTPUT_2));
		addOutput(createOutput<OutPort>(Vec(3, 108), module, Mults::OUTPUT_3));
		addOutput(createOutput<OutPort>(Vec(3, 133), module, Mults::OUTPUT_4));
		addOutput(createOutput<OutPort>(Vec(3, 158), module, Mults::OUTPUT_5));

  		addOutput(createOutput<OutPort>(Vec(3, 230), module, Mults::OUTPUT_2_1));
		addOutput(createOutput<OutPort>(Vec(3, 255), module, Mults::OUTPUT_2_2));
		addOutput(createOutput<OutPort>(Vec(3, 280), module, Mults::OUTPUT_2_3));
		addOutput(createOutput<OutPort>(Vec(3, 305), module, Mults::OUTPUT_2_4));
		addOutput(createOutput<OutPort>(Vec(3, 330), module, Mults::OUTPUT_2_5));
	}
};

Model *modelMults = createModel<Mults, MultsWidget>("Mults");
