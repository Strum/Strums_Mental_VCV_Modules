///////////////////////////////////////////////////
//
//   Mental Plugin
//   Dual Wave Clipper
//
//   Strum 2017-21
//   strum@softhome.net
//
///////////////////////////////////////////////////

#include "mental.hpp"

//////////////////////////////////////////////////////
struct Clip : Module
{
  enum ParamIds
  {
    THRESH1_PARAM, GAIN1_PARAM,
    THRESH2_PARAM, GAIN2_PARAM,
    NUM_PARAMS
  };
  enum InputIds
  {
    INPUT1, THRESH1_CV_INPUT, GAIN1_CV_INPUT,
    INPUT2, THRESH2_CV_INPUT, GAIN2_CV_INPUT,
    NUM_INPUTS
  };
  enum OutputIds
  {
    OUTPUT1,
    OUTPUT2,
    NUM_OUTPUTS
  };

  Clip()
  {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS);
    configParam(Clip::THRESH1_PARAM, 0.0, 1.0, 1.0, "");
    configParam(Clip::GAIN1_PARAM, 0.0, 1.0, 0.5, "");
    configParam(Clip::THRESH2_PARAM, 0.0, 1.0, 1.0, "");
    configParam(Clip::GAIN2_PARAM, 0.0, 1.0, 0.5, "");
  }
  void process(const ProcessArgs& args) override;
};

/////////////////////////////////////////////////////
void Clip::process(const ProcessArgs& args)
{

  float signal_in1 = inputs[INPUT1].getVoltage();
  float threshold1 = params[THRESH1_PARAM].getValue() * 6 + inputs[THRESH1_CV_INPUT].getVoltage()/2;
  float gain1 = params[GAIN1_PARAM].getValue() * 5 + inputs[GAIN1_CV_INPUT].getVoltage() / 2;

  float signal_in2 = inputs[INPUT2].getVoltage();
  float threshold2 = params[THRESH2_PARAM].getValue() * 6 + inputs[THRESH2_CV_INPUT].getVoltage()/2;
  float gain2 = params[GAIN2_PARAM].getValue() * 5 + inputs[GAIN2_CV_INPUT].getVoltage() / 2;


  float clipped1 = signal_in1;
  float clipped2 = signal_in2;

  if (std::abs(signal_in1) > threshold1)
  {
    if (signal_in1 > 0)
    {
     clipped1 = threshold1;
    } else
    {
     clipped1 = - threshold1;
    }
  }

  if (std::abs(signal_in2) > threshold2)
  {
    if (signal_in2 > 0)
    {
     clipped2 = threshold2;
    } else
    {
     clipped2 = - threshold2;
    }
  }
  outputs[OUTPUT1].setVoltage(clipped1 * gain1);
  outputs[OUTPUT2].setVoltage(clipped2 * gain2);
}

//////////////////////////////////////////////////////////////////
struct ClipWidget : ModuleWidget
{
  ClipWidget(Clip *module)
  {
    setModule(module);

    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Clip.svg")));


    addParam(createParam<SmlKnob>(Vec(6, box.size.y / 2 - 169), module, Clip::THRESH1_PARAM));
    addInput(createInput<CVInPort>(Vec(3, box.size.y / 2 - 148), module, Clip::THRESH1_CV_INPUT));

    addParam(createParam<SmlKnob>(Vec(6, box.size.y / 2 - 112), module, Clip::GAIN1_PARAM));
    addInput(createInput<CVInPort>(Vec(3, box.size.y / 2 - 91), module, Clip::GAIN1_CV_INPUT));
    // output
    addInput(createInput<InPort>(Vec(3, box.size.y / 2 - 55), module, Clip::INPUT1));
    addOutput(createOutput<OutPort>(Vec(3, box.size.y / 2 - 28), module, Clip::OUTPUT1));


    addParam(createParam<SmlKnob>(Vec(6, box.size.y - 175), module, Clip::THRESH2_PARAM));
    addInput(createInput<CVInPort>(Vec(3, box.size.y - 154), module, Clip::THRESH2_CV_INPUT));

    addParam(createParam<SmlKnob>(Vec(6, box.size.y - 122), module, Clip::GAIN2_PARAM));
    addInput(createInput<CVInPort>(Vec(3, box.size.y - 101), module, Clip::GAIN2_CV_INPUT));

    addInput(createInput<InPort>(Vec(3, box.size.y - 65), module, Clip::INPUT2));
    addOutput(createOutput<OutPort>(Vec(3, box.size.y - 38), module, Clip::OUTPUT2));
  }
};

Model *modelClip = createModel<Clip, ClipWidget>("Clip");
