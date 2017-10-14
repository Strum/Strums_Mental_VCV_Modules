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
  float threshold = params[THRESH_PARAM].value * 6 + inputs[THRESH_CV_INPUT].value/2;
  
  float gain = params[GAIN_PARAM].value * 5 + inputs[GAIN_CV_INPUT].value / 2;


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

  outputs[OUTPUT_1].value = clipped * gain;

}

//////////////////////////////////////////////////////////////////
MentalClipWidget::MentalClipWidget() {
	MentalClip *module = new MentalClip();
	setModule(module);
	box.size = Vec(15*4, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		//panel->setBackground(SVG::load("plugins/mental/res/MentalClip.svg"));
    panel->setBackground(SVG::load(assetPlugin(plugin,"res/MentalClip.svg")));
		addChild(panel);
	}

  addParam(createParam<Davies1900hSmallBlackKnob>(Vec(2, 20), module, MentalClip::THRESH_PARAM, 0.0, 1.0, 1.0));
  addParam(createParam<Davies1900hSmallBlackKnob>(Vec(2, 80), module, MentalClip::GAIN_PARAM, 0.0, 1.0, 0.5));
	
  addInput(createInput<PJ301MPort>(Vec(3, 50), module, MentalClip::THRESH_CV_INPUT));
  addInput(createInput<PJ301MPort>(Vec(3, 110), module, MentalClip::GAIN_CV_INPUT));

  addInput(createInput<PJ301MPort>(Vec(3, 140), module, MentalClip::INPUT_1));
  addOutput(createOutput<PJ301MPort>(Vec(33, 140), module, MentalClip::OUTPUT_1));

}
