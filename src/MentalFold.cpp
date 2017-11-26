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
    GAIN_PARAM,
    NUM_PARAMS
	};
	enum InputIds {
		INPUT_1,
    THRESH_CV_INPUT,
    GAIN_CV_INPUT,
    NUM_INPUTS
	};
	enum OutputIds {
		OUTPUT_1,
    NUM_OUTPUTS
	};

	MentalFold() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
	void step() override;
};

/////////////////////////////////////////////////////
void MentalFold::step() {

  float signal_in_1 = inputs[INPUT_1].value;
  float threshold_fold = params[THRESH_PARAM].value * 6 + inputs[THRESH_CV_INPUT].value;
  
  float gain = params[GAIN_PARAM].value * 5 + inputs[GAIN_CV_INPUT].value / 2;

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

  outputs[OUTPUT_1].value = modified2 * gain;

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
  
  addParam(createParam<RoundSmallBlackKnob>(Vec(2, 20), module, MentalFold::THRESH_PARAM, 0.0, 1.0, 1.0));
  addParam(createParam<RoundSmallBlackKnob>(Vec(2, 80), module, MentalFold::GAIN_PARAM, 0.0, 1.0, 0.5));
	
  addInput(createInput<CVPort>(Vec(3, 50), module, MentalFold::THRESH_CV_INPUT));
  addInput(createInput<CVPort>(Vec(3, 110), module, MentalFold::GAIN_CV_INPUT));

  addInput(createInput<InPort>(Vec(3, 140), module, MentalFold::INPUT_1));
  addOutput(createOutput<OutPort>(Vec(33, 140), module, MentalFold::OUTPUT_1));

}
