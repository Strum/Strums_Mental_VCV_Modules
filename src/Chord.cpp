///////////////////////////////////////////////////
//
//   Mental Plugin
//   Chord
//
//   Strum 2017-21
//   strum@softhome.net
//
///////////////////////////////////////////////////

#include "mental.hpp"

/////////////////////////////////////////////////
struct Chord : Module
{
	enum ParamIds
  {
      OFFSET_PARAM,
      INVERSION_PARAM,
      VOICING_PARAM,
      NUM_PARAMS
	};

	enum InputIds
  {
      INPUT,
      OFFSET_CV_INPUT,
      INVERSION_CV_INPUT,
      VOICING_CV_INPUT,
      FLAT_3RD_INPUT,
      FLAT_5TH_INPUT,
      FLAT_7TH_INPUT,
      SUS_2_INPUT,
      SUS_4_INPUT,
      SIX_FOR_5_INPUT,
      ONE_FOR_7_INPUT,
      FLAT_9_INPUT,
      SHARP_9_INPUT,
      SIX_FOR_7_INPUT,
      SHARP_5_INPUT,
      NUM_INPUTS
	};
	enum OutputIds
  {
      OUTPUT_1,
      OUTPUT_2,
      OUTPUT_3,
      OUTPUT_4,
      OUTPUT_ROOT,
      OUTPUT_THIRD,
      OUTPUT_FIFTH,
      OUTPUT_SEVENTH,
      NUM_OUTPUTS
	};

	Chord()
  {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS);

    configParam(Chord::OFFSET_PARAM, 0.0, 1.0, 0.5, "");
    configParam(Chord::INVERSION_PARAM, 0.0, 1.0, 0.0, "");
    configParam(Chord::VOICING_PARAM, 0.0, 1.0, 0.0, "");
  }
	void process(const ProcessArgs& args) override;
};


/////////////////////////////////////////////////////
void Chord::process(const ProcessArgs& args)
{
  float in = inputs[INPUT].getVoltage();
  int octave = round(in);

  float offset_raw = (params[OFFSET_PARAM].getValue()) * 12 - 6 + (inputs[OFFSET_CV_INPUT].getVoltage()) / 1.5;
  float pitch_offset = round(offset_raw) / 12;

  float root = in - 1.0*octave + pitch_offset;
  float root_or_2nd = root;

  float inversion_raw = (params[INVERSION_PARAM].getValue()) * 4 - 1 + (inputs[INVERSION_CV_INPUT].getVoltage() / 3);
  int inversion = round(inversion_raw);
  if (inversion > 2) inversion = 2;
  if (inversion < -1) inversion = -1;

  float voicing_raw = (params[VOICING_PARAM].getValue()) * 5 - 2 + (inputs[VOICING_CV_INPUT].getVoltage() / 3);
  int voicing = round(voicing_raw);
  if (voicing > 2) voicing = 2;
  if (voicing < -2) voicing = -2;

  float voice_1 = 0.0;
  float voice_2 = 0.0;
  float voice_3 = 0.0;
  float voice_4 = 0.0;

  int third = 4;
  int fifth = 7;
  int seventh = 11;

  if (inputs[FLAT_3RD_INPUT].getVoltage() > 0.0) third = 3;
  if (inputs[FLAT_5TH_INPUT].getVoltage() > 0.0) fifth = 6;
  if (inputs[SHARP_5_INPUT].getVoltage() > 0.0) fifth = 8;
  if (inputs[FLAT_7TH_INPUT].getVoltage() > 0.0) seventh = 10;

  if (inputs[SUS_2_INPUT].getVoltage() > 0.0) root_or_2nd = root + (2 * (1.0/12.0));
  if (inputs[SUS_4_INPUT].getVoltage() > 0.0) third = 5;
  if (inputs[SIX_FOR_5_INPUT].getVoltage() > 0.0) fifth = 9;
  if (inputs[SIX_FOR_7_INPUT].getVoltage() > 0.0) seventh = 9;

  if (inputs[FLAT_9_INPUT].getVoltage() > 0.0) root_or_2nd = root + 1.0/12.0;
  if (inputs[SHARP_9_INPUT].getVoltage() > 0.0) root_or_2nd = root + (3 * (1.0/12.0));
  if (inputs[ONE_FOR_7_INPUT].getVoltage() > 0.0) seventh = 12;

  outputs[OUTPUT_ROOT].setVoltage(root);
  outputs[OUTPUT_THIRD].setVoltage(root + third * (1.0/12.0));
  outputs[OUTPUT_FIFTH].setVoltage(root + fifth * (1.0/12.0));
  outputs[OUTPUT_SEVENTH].setVoltage(root + seventh * (1.0/12.0));

  if (inversion == -1 )
  {
    voice_1 = root_or_2nd;
    voice_2 = root + third * (1.0/12.0);
    voice_3 = root + fifth * (1.0/12.0);
    voice_4 = root + seventh * (1.0/12.0);
  }
  if (inversion == 0 )
  {
    voice_1 = root + third * (1.0/12.0);
    voice_2 = root + fifth * (1.0/12.0);
    voice_3 = root + seventh * (1.0/12.0);
    voice_4 = root_or_2nd + 1.0;
  }
  if (inversion == 1)
  {
    voice_1 = root + fifth * (1.0/12.0);
    voice_2 = root + seventh * (1.0/12.0);
    voice_3 = root_or_2nd + 1.0;
    voice_4 = root + 1.0 + third * (1.0/12.0);
  }
  if (inversion == 2 )
  {
    voice_1 = root + seventh * (1.0/12.0);
    voice_2 = root_or_2nd + 1.0;
    voice_3 = root + 1.0 + third * (1.0/12.0);
    voice_4 = root + 1.0 + fifth * (1.0/12.0);
  }

  if (voicing == -1) voice_2 -= 1.0;
  if (voicing == -0) voice_3 -= 1.0;
  if (voicing == 1)
  {
    voice_2 -= 1.0;
    voice_4 -= 1.0;
  }
  if (voicing == 2)
  {
    voice_2 += 1.0;
    voice_4 += 1.0;
  }

  outputs[OUTPUT_1].setVoltage(voice_1);
  outputs[OUTPUT_2].setVoltage(voice_2);
  outputs[OUTPUT_3].setVoltage(voice_3);
  outputs[OUTPUT_4].setVoltage(voice_4);
}

//////////////////////////////////////////////////////////////////
struct ChordWidget : ModuleWidget
{
  ChordWidget(Chord *module)
  {
    setModule(module);

    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Chord.svg")));

    addParam(createParam<MedKnob>(Vec(3, 20), module, Chord::OFFSET_PARAM));
    addInput(createInput<CVInPort>(Vec(3, 50), module, Chord::OFFSET_CV_INPUT));
    addParam(createParam<MedKnob>(Vec(33, 20), module, Chord::INVERSION_PARAM));
    addParam(createParam<MedKnob>(Vec(63, 20), module, Chord::VOICING_PARAM));

    addInput(createInput<CVInPort>(Vec(3, 100), module, Chord::INPUT));
    addInput(createInput<CVInPort>(Vec(33, 50), module, Chord::INVERSION_CV_INPUT));
    addInput(createInput<CVInPort>(Vec(63, 50), module, Chord::VOICING_CV_INPUT));

    addInput(createInput<GateInPort>(Vec(3, 130), module, Chord::FLAT_3RD_INPUT));
    addInput(createInput<GateInPort>(Vec(3, 155), module, Chord::FLAT_5TH_INPUT));
    addInput(createInput<GateInPort>(Vec(3, 180), module, Chord::FLAT_7TH_INPUT));

    addInput(createInput<GateInPort>(Vec(3, 210), module, Chord::SUS_2_INPUT));
    addInput(createInput<GateInPort>(Vec(3, 235), module, Chord::SUS_4_INPUT));
    addInput(createInput<GateInPort>(Vec(3, 260), module, Chord::SIX_FOR_5_INPUT));
    addInput(createInput<GateInPort>(Vec(3, 285), module, Chord::ONE_FOR_7_INPUT));
    addInput(createInput<GateInPort>(Vec(3, 310), module, Chord::FLAT_9_INPUT));
    addInput(createInput<GateInPort>(Vec(3, 335), module, Chord::SHARP_9_INPUT));
    addInput(createInput<GateInPort>(Vec(33, 316), module, Chord::SIX_FOR_7_INPUT));
    addInput(createInput<GateInPort>(Vec(33, 341), module, Chord::SHARP_5_INPUT));


    addOutput(createOutput<CVOutPort>(Vec(63, 100), module, Chord::OUTPUT_ROOT));
    addOutput(createOutput<CVOutPort>(Vec(63, 125), module, Chord::OUTPUT_THIRD));
    addOutput(createOutput<CVOutPort>(Vec(63, 150), module, Chord::OUTPUT_FIFTH));
    addOutput(createOutput<CVOutPort>(Vec(63, 175), module, Chord::OUTPUT_SEVENTH));

    addOutput(createOutput<CVOutPort>(Vec(63, 250), module, Chord::OUTPUT_1));
    addOutput(createOutput<CVOutPort>(Vec(63, 275), module, Chord::OUTPUT_2));
    addOutput(createOutput<CVOutPort>(Vec(63, 300), module, Chord::OUTPUT_3));
    addOutput(createOutput<CVOutPort>(Vec(63, 325), module, Chord::OUTPUT_4));
  }
};

Model *modelChord = createModel<Chord, ChordWidget>("Chord");
