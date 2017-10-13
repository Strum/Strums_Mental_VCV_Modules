///////////////////////////////////////////////////
//
//   Wave Folder VCV Module
//
//   Strum 2017
//
///////////////////////////////////////////////////

#include "mental.hpp"

//////////////////////////////////////////////////////
struct MentalClip : Module {
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

	MentalClip() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
	void step();
};

/*
MentalClip::MentalClip() {
  params.resize(NUM_PARAMS);
	inputs.resize(NUM_INPUTS);
	outputs.resize(NUM_OUTPUTS);
}*/

/////////////////////////////////////////////////////
void MentalClip::step() {

  float signal_in = inputs[INPUT_1].value;
  float threshold = params[THRESH_PARAM].value * 6 + inputs[CV_INPUT].value;


  float clipped = signal_in;
  if (abs(signal_in) > threshold)
  {
    if (signal_in > 0)
    {
     clipped = threshold;
    } else
    {
     clipped = - threshold;
    }
  }

  outputs[OUTPUT_1].value = clipped;

}

//////////////////////////////////////////////////////////////////
MentalClipWidget::MentalClipWidget() {
	MentalClip *module = new MentalClip();
	setModule(module);
	box.size = Vec(15*4, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load("plugins/mental/res/MentalClip.svg"));
		addChild(panel);
	}

  addParam(createParam<SynthTechAlco>(Vec(8, 30), module, MentalClip::THRESH_PARAM, 0.0, 1.0, 1.0));

	addInput(createInput<PJ301MPort>(Vec(3, 120), module, MentalClip::INPUT_1));
  addInput(createInput<PJ301MPort>(Vec(18, 87), module, MentalClip::CV_INPUT));

  addOutput(createOutput<PJ301MPort>(Vec(33, 120), module, MentalClip::OUTPUT_1));

}
