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

  MentalClip() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
  void step() override;
};

/*
MentalClip::MentalClip() {
  params.resize(NUM_PARAMS);
  inputs.resize(NUM_INPUTS);
  outputs.resize(NUM_OUTPUTS);
}*/

/////////////////////////////////////////////////////
void MentalClip::step() {

  float signal_in1 = inputs[INPUT1].value;
  float threshold1 = params[THRESH1_PARAM].value * 6 + inputs[THRESH1_CV_INPUT].value/2;
  float gain1 = params[GAIN1_PARAM].value * 5 + inputs[GAIN1_CV_INPUT].value / 2;

  float signal_in2 = inputs[INPUT2].value;
  float threshold2 = params[THRESH2_PARAM].value * 6 + inputs[THRESH2_CV_INPUT].value/2;
  float gain2 = params[GAIN2_PARAM].value * 5 + inputs[GAIN2_CV_INPUT].value / 2;


  float clipped1 = signal_in1;
  float clipped2 = signal_in2;

  if (abs(signal_in1) > threshold1)
  {
    if (signal_in1 > 0)
    {
     clipped1 = threshold1;
    } else
    {
     clipped1 = - threshold1;
    }
  }

  if (abs(signal_in2) > threshold2)
  {
    if (signal_in2 > 0)
    {
     clipped2 = threshold2;
    } else
    {
     clipped2 = - threshold2;
    }
  }
  outputs[OUTPUT1].value = clipped1 * gain1;
  outputs[OUTPUT2].value = clipped2 * gain2;
}

//////////////////////////////////////////////////////////////////
MentalClipWidget::MentalClipWidget() {
  MentalClip *module = new MentalClip();
  setModule(module);
  box.size = Vec(15*2, 380);

  {
    SVGPanel *panel = new SVGPanel();
    panel->box.size = box.size;
    //panel->setBackground(SVG::load("plugins/mental/res/MentalClip.svg"));
    panel->setBackground(SVG::load(assetPlugin(plugin,"res/MentalClip.svg")));
    addChild(panel);
  }
  

  // label
  addParam(createParam<Trimpot>(Vec(6, box.size.y / 2 - 169), module, MentalClip::THRESH1_PARAM, 0.0, 1.0, 1.0));
  addInput(createInput<CVInPort>(Vec(3, box.size.y / 2 - 148), module, MentalClip::THRESH1_CV_INPUT));
  // label
  addParam(createParam<Trimpot>(Vec(6, box.size.y / 2 - 112), module, MentalClip::GAIN1_PARAM, 0.0, 1.0, 0.5));
  addInput(createInput<CVInPort>(Vec(3, box.size.y / 2 - 91), module, MentalClip::GAIN1_CV_INPUT));
  // output  
  addInput(createInput<InPort>(Vec(3, box.size.y / 2 - 55), module, MentalClip::INPUT1));
  addOutput(createOutput<OutPort>(Vec(3, box.size.y / 2 - 28), module, MentalClip::OUTPUT1));

  
  // label
  addParam(createParam<Trimpot>(Vec(6, box.size.y - 175), module, MentalClip::THRESH2_PARAM, 0.0, 1.0, 1.0));
  addInput(createInput<CVInPort>(Vec(3, box.size.y - 154), module, MentalClip::THRESH2_CV_INPUT));
  // label
  addParam(createParam<Trimpot>(Vec(6, box.size.y - 118), module, MentalClip::GAIN2_PARAM, 0.0, 1.0, 0.5));
  addInput(createInput<CVInPort>(Vec(3, box.size.y - 97), module, MentalClip::GAIN2_CV_INPUT));
  // output  
  addInput(createInput<InPort>(Vec(3, box.size.y - 61), module, MentalClip::INPUT2));
  addOutput(createOutput<OutPort>(Vec(3, box.size.y - 34), module, MentalClip::OUTPUT2));

}