///////////////////////////////////////////////////
//
//   Pitch Shifter VCV Module
//
//   Strum 2017
//
///////////////////////////////////////////////////

#include "mental.hpp"


//////////////////////////////////////////////////////
struct MentalPitchShift : Module {
	enum ParamIds {
    OCTAVE_SHIFT_1,
    OCTAVE_SHIFT_2,
    SEMITONE_SHIFT_1,
    SEMITONE_SHIFT_2,    
    NUM_PARAMS
	};
	enum InputIds {
		OCTAVE_SHIFT_1_INPUT,
    OCTAVE_SHIFT_2_INPUT,
    SEMITONE_SHIFT_1_INPUT,
    SEMITONE_SHIFT_2_INPUT, 
    OCTAVE_SHIFT_1_CVINPUT,
    OCTAVE_SHIFT_2_CVINPUT,
    SEMITONE_SHIFT_1_CVINPUT,
    SEMITONE_SHIFT_2_CVINPUT, 
    NUM_INPUTS
	};
	enum OutputIds {
		OCTAVE_SHIFT_1_OUTPUT,
    OCTAVE_SHIFT_2_OUTPUT,
    SEMITONE_SHIFT_1_OUTPUT,
    SEMITONE_SHIFT_2_OUTPUT, 
    NUM_OUTPUTS
	};

	MentalPitchShift() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
  
  float octave_1_out = 0.0;
  float octave_2_out = 0.0;
  float semitone_1_out = 0.0;
  float semitone_2_out = 0.0;
  
  
	void step() override;
};

/*
MentalPitchShift::MentalPitchShift() {
  params.resize(NUM_PARAMS);
	inputs.resize(NUM_INPUTS);
	outputs.resize(NUM_OUTPUTS);
}*/

/////////////////////////////////////////////////////
void MentalPitchShift::step() {

  octave_1_out = inputs[OCTAVE_SHIFT_1_INPUT].value + round(params[OCTAVE_SHIFT_1].value) + round(inputs[OCTAVE_SHIFT_1_CVINPUT].value/2);
  octave_2_out = inputs[OCTAVE_SHIFT_2_INPUT].value + round(params[OCTAVE_SHIFT_2].value) + round(inputs[OCTAVE_SHIFT_1_CVINPUT].value/2);
  semitone_1_out = inputs[SEMITONE_SHIFT_1_INPUT].value + round(params[SEMITONE_SHIFT_1].value)*(1.0/12.0) + round(inputs[SEMITONE_SHIFT_1_CVINPUT].value/2)*(1.0/12.0);
  semitone_2_out = inputs[SEMITONE_SHIFT_2_INPUT].value + round(params[SEMITONE_SHIFT_2].value)*(1.0/12.0) + round(inputs[SEMITONE_SHIFT_2_CVINPUT].value/2)*(1.0/12.0);
    
  outputs[OCTAVE_SHIFT_1_OUTPUT].value = octave_1_out;
  outputs[OCTAVE_SHIFT_2_OUTPUT].value = octave_2_out;
  outputs[SEMITONE_SHIFT_1_OUTPUT].value = semitone_1_out;
  outputs[SEMITONE_SHIFT_2_OUTPUT].value = semitone_2_out;

}

//////////////////////////////////////////////////////////////////
MentalPitchShiftWidget::MentalPitchShiftWidget() {
	MentalPitchShift *module = new MentalPitchShift();
	setModule(module);
	box.size = Vec(15*4, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		//panel->setBackground(SVG::load("plugins/mental/res/MentalPitchShift.svg"));
    panel->setBackground(SVG::load(assetPlugin(plugin,"res/MentalPitchShift.svg")));
		addChild(panel);
	}

  addParam(createParam<RoundSmallBlackKnob>(Vec(2, 20), module, MentalPitchShift::OCTAVE_SHIFT_1, -4.5, 4.5, 0.0));
  addParam(createParam<RoundSmallBlackKnob>(Vec(2, 80), module, MentalPitchShift::OCTAVE_SHIFT_2, -4.5, 4.5, 0.0));
  addParam(createParam<RoundSmallBlackKnob>(Vec(2, 140), module, MentalPitchShift::SEMITONE_SHIFT_1, -6.5, 6.5, 0.0));
  addParam(createParam<RoundSmallBlackKnob>(Vec(2, 200), module, MentalPitchShift::SEMITONE_SHIFT_2, -6.5, 6.5, 0.0));

  addInput(createInput<InPort>(Vec(3, 50), module, MentalPitchShift::OCTAVE_SHIFT_1_INPUT));
	addInput(createInput<InPort>(Vec(3, 110), module, MentalPitchShift::OCTAVE_SHIFT_2_INPUT));
  addInput(createInput<InPort>(Vec(3, 170), module, MentalPitchShift::SEMITONE_SHIFT_1_INPUT));
	addInput(createInput<InPort>(Vec(3, 230), module, MentalPitchShift::SEMITONE_SHIFT_2_INPUT));
  
  addInput(createInput<InPort>(Vec(33, 20), module, MentalPitchShift::OCTAVE_SHIFT_1_CVINPUT));
	addInput(createInput<InPort>(Vec(33, 80), module, MentalPitchShift::OCTAVE_SHIFT_2_CVINPUT));
  addInput(createInput<InPort>(Vec(33, 140), module, MentalPitchShift::SEMITONE_SHIFT_1_CVINPUT));
	addInput(createInput<InPort>(Vec(33, 200), module, MentalPitchShift::SEMITONE_SHIFT_2_CVINPUT));

  addOutput(createOutput<OutPort>(Vec(33, 50), module, MentalPitchShift::OCTAVE_SHIFT_1_OUTPUT));
  addOutput(createOutput<OutPort>(Vec(33, 110), module, MentalPitchShift::OCTAVE_SHIFT_2_OUTPUT));
  addOutput(createOutput<OutPort>(Vec(33, 170), module, MentalPitchShift::SEMITONE_SHIFT_1_OUTPUT));
  addOutput(createOutput<OutPort>(Vec(33, 230), module, MentalPitchShift::SEMITONE_SHIFT_2_OUTPUT));

}
