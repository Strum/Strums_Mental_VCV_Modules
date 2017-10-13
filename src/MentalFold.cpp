///////////////////////////////////////////////////
//
//   Wave Folder VCV Module
//
//   Strum 2017
//
///////////////////////////////////////////////////

#include "mental.hpp"

//////////////////////////////////////////////////////
struct MentalFold : Module {
	enum ParamIds {
    THRESH_PARAM,
    NUM_PARAMS
	};
	enum InputIds {
		INPUT_1,
    CV_INPUT,
    NUM_INPUTS
	};
	enum OutputIds {
		OUTPUT_1,
    NUM_OUTPUTS
	};

	MentalFold() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
	void step();
};

/*
MentalFold::MentalFold() {
  params.resize(NUM_PARAMS);
	inputs.resize(NUM_INPUTS);
	outputs.resize(NUM_OUTPUTS);
}*/

/////////////////////////////////////////////////////
void MentalFold::step() {

  float signal_in_1 = inputs[INPUT_1].value;
  float threshold_fold = params[THRESH_PARAM].value * 6 + inputs[CV_INPUT].value;


  float modified2 = signal_in_1;
  if (abs(signal_in_1) > threshold_fold )
  {
    if (signal_in_1 > 0)
    {
     modified2 = threshold_fold - (signal_in_1 - threshold_fold );
    } else
    {
     modified2 = - threshold_fold - (signal_in_1 + threshold_fold );
    }
  }

  outputs[OUTPUT_1].value = modified2;

}

//////////////////////////////////////////////////////////////////
MentalFoldWidget::MentalFoldWidget() {
	MentalFold *module = new MentalFold();
	setModule(module);
	box.size = Vec(15*4, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		//panel->setBackground(SVG::load("plugins/mental/res/MentalFold.svg"));
    panel->setBackground(SVG::load(assetPlugin(plugin,"res/MentalFold.svg")));
		addChild(panel);
	}

  addParam(createParam<SynthTechAlco>(Vec(8, 30), module, MentalFold::THRESH_PARAM, 0.0, 1.0, 1.0));

  addInput(createInput<PJ301MPort>(Vec(18, 87), module, MentalFold::CV_INPUT));
	addInput(createInput<PJ301MPort>(Vec(3, 120), module, MentalFold::INPUT_1));

  addOutput(createOutput<PJ301MPort>(Vec(33, 120), module, MentalFold::OUTPUT_1));

}
