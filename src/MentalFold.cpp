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
    THRESH1_PARAM, GAIN1_PARAM,
    THRESH2_PARAM, GAIN2_PARAM,
    NUM_PARAMS
  };
  enum InputIds {
    INPUT1, THRESH1_CV_INPUT, GAIN1_CV_INPUT,
    INPUT2, THRESH2_CV_INPUT, GAIN2_CV_INPUT,
    NUM_INPUTS
  };
  enum OutputIds {
    OUTPUT1,
    OUTPUT2,
    NUM_OUTPUTS
  };

  MentalFold() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
  void step() override;
};


/////////////////////////////////////////////////////

static void folderStep(Input &in, Param &thresh, Input &threshcv, Param &gain, Input &gaincv, Output &out) {
  float signal_in = in.value;
  float threshold = thresh.value * 6 + threshcv.value / 2;
  float level = gain.value * 5 + gaincv.value / 2;

  float folded = signal_in;

  if (std::abs(signal_in) > threshold) {
    if (signal_in > 0) {
      folded = threshold - (signal_in + threshold);
    }
    else {
      folded = - threshold - (signal_in + threshold);
    }
  }
  out.value = folded * level;
}


void MentalFold::step() {
  folderStep(inputs[INPUT1], params[THRESH1_PARAM], inputs[THRESH1_CV_INPUT], params[GAIN1_PARAM], inputs[GAIN1_CV_INPUT], outputs[OUTPUT1]);
  folderStep(inputs[INPUT2], params[THRESH2_PARAM], inputs[THRESH2_CV_INPUT], params[GAIN2_PARAM], inputs[GAIN2_CV_INPUT], outputs[OUTPUT2]);
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
  addParam(createParam<Trimpot>(Vec(6, box.size.y / 2 - 169), module, MentalFold::THRESH1_PARAM, 0.0, 1.0, 1.0));
  addInput(createInput<CVInPort>(Vec(3, box.size.y / 2 - 148), module, MentalFold::THRESH1_CV_INPUT));
  // label
  addParam(createParam<Trimpot>(Vec(6, box.size.y / 2 - 112), module, MentalFold::GAIN1_PARAM, 0.0, 1.0, 0.5));
  addInput(createInput<CVInPort>(Vec(3, box.size.y / 2 - 91), module, MentalFold::GAIN1_CV_INPUT));
  // output  
  addInput(createInput<InPort>(Vec(3, box.size.y / 2 - 55), module, MentalFold::INPUT1));
  addOutput(createOutput<OutPort>(Vec(3, box.size.y / 2 - 28), module, MentalFold::OUTPUT1));

  
  // label
  addParam(createParam<Trimpot>(Vec(6, box.size.y - 177), module, MentalFold::THRESH2_PARAM, 0.0, 1.0, 1.0));
  addInput(createInput<CVInPort>(Vec(3, box.size.y - 156), module, MentalFold::THRESH2_CV_INPUT));
  // label
  addParam(createParam<Trimpot>(Vec(6, box.size.y - 120), module, MentalFold::GAIN2_PARAM, 0.0, 1.0, 0.5));
  addInput(createInput<CVInPort>(Vec(3, box.size.y - 99), module, MentalFold::GAIN2_CV_INPUT));
  // output  
  addInput(createInput<InPort>(Vec(3, box.size.y - 63), module, MentalFold::INPUT2));
  addOutput(createOutput<OutPort>(Vec(3, box.size.y - 36), module, MentalFold::OUTPUT2));

}
