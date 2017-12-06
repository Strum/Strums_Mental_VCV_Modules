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
    THRESH_PARAM2,
    GAIN_PARAM2,
    NUM_PARAMS
	};
	enum InputIds {
		INPUT_1,
    THRESH_CV_INPUT,
    GAIN_CV_INPUT,
    INPUT_2,
    THRESH_CV_INPUT2,
    GAIN_CV_INPUT2,
    NUM_INPUTS
	};
	enum OutputIds {
		OUTPUT_1,
    OUTPUT_2,
    NUM_OUTPUTS
	};

	MentalFold() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
	void step() override;
};

/////////////////////////////////////////////////////
void MentalFold::step() {

  float signal_in_1 = inputs[INPUT_1].value;
  float signal_in_2 = inputs[INPUT_2].value;
  
  float threshold_fold = params[THRESH_PARAM].value * 6 + inputs[THRESH_CV_INPUT].value;
  float threshold_fold2 = params[THRESH_PARAM2].value * 6 + inputs[THRESH_CV_INPUT2].value;
  
  float gain = params[GAIN_PARAM].value * 5 + inputs[GAIN_CV_INPUT].value / 2;
  float gain2 = params[GAIN_PARAM2].value * 5 + inputs[GAIN_CV_INPUT2].value / 2;

  float modified = signal_in_1;
  float modified2 = signal_in_2;
  
  if (std::abs(signal_in_1) > threshold_fold )
  {
    if (signal_in_1 > 0)
    {
     modified = threshold_fold - (signal_in_1 - threshold_fold );
    } else
    {
     modified = - threshold_fold - (signal_in_1 + threshold_fold );
    }
  }
  
  if (std::abs(signal_in_2) > threshold_fold2 )
  {
    if (signal_in_2 > 0)
    {
     modified2 = threshold_fold2 - (signal_in_2 - threshold_fold2 );
    } else
    {
     modified2 = - threshold_fold2 - (signal_in_2 + threshold_fold2 );
    }
  }

  outputs[OUTPUT_1].value = modified * gain;
  outputs[OUTPUT_2].value = modified2 * gain2;

}

//////////////////////////////////////////////////////////////////
MentalFoldWidget::MentalFoldWidget() {
	MentalFold *module = new MentalFold();
	setModule(module);
	box.size = Vec(15*2, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		//panel->setBackground(SVG::load("plugins/mental/res/MentalFold.svg"));
    panel->setBackground(SVG::load(assetPlugin(plugin,"res/MentalFold.svg")));
		addChild(panel);
	}
  
  // label
  addParam(createParam<Trimpot>(Vec(6, box.size.y / 2 - 169), module, MentalFold::THRESH_PARAM, 0.0, 1.0, 1.0));
  addInput(createInput<CVInPort>(Vec(3, box.size.y / 2 - 148), module, MentalFold::THRESH_CV_INPUT));
  // label
  addParam(createParam<Trimpot>(Vec(6, box.size.y / 2 - 112), module, MentalFold::GAIN_PARAM, 0.0, 1.0, 0.5));
  addInput(createInput<CVInPort>(Vec(3, box.size.y / 2 - 91), module, MentalFold::GAIN_CV_INPUT));
  // output  
  addInput(createInput<InPort>(Vec(3, box.size.y / 2 - 55), module, MentalFold::INPUT_1));
  addOutput(createOutput<OutPort>(Vec(3, box.size.y / 2 - 28), module, MentalFold::OUTPUT_1));

  
  // label
  addParam(createParam<Trimpot>(Vec(6, box.size.y - 177), module, MentalFold::THRESH_PARAM2, 0.0, 1.0, 1.0));
  addInput(createInput<CVInPort>(Vec(3, box.size.y - 156), module, MentalFold::THRESH_CV_INPUT2));
  // label
  addParam(createParam<Trimpot>(Vec(6, box.size.y - 120), module, MentalFold::GAIN_PARAM2, 0.0, 1.0, 0.5));
  addInput(createInput<CVInPort>(Vec(3, box.size.y - 99), module, MentalFold::GAIN_CV_INPUT2));
  // output  
  addInput(createInput<InPort>(Vec(3, box.size.y - 63), module, MentalFold::INPUT_2));
  addOutput(createOutput<OutPort>(Vec(3, box.size.y - 36), module, MentalFold::OUTPUT_2));


}
