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

	MentalPitchShift() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS);
    configParam(MentalPitchShift::OCTAVE_SHIFT_1, -4.5, 4.5, 0.0, "");
    configParam(MentalPitchShift::OCTAVE_SHIFT_2, -4.5, 4.5, 0.0, "");
    configParam(MentalPitchShift::SEMITONE_SHIFT_1, -6.5, 6.5, 0.0, "");
    configParam(MentalPitchShift::SEMITONE_SHIFT_2, -6.5, 6.5, 0.0, "");
  }
  
  float octave_1_out = 0.0;
  float octave_2_out = 0.0;
  float semitone_1_out = 0.0;
  float semitone_2_out = 0.0;  
  
	void process(const ProcessArgs& args) override;
};

/////////////////////////////////////////////////////
void MentalPitchShift::process(const ProcessArgs& args) {

  octave_1_out = inputs[OCTAVE_SHIFT_1_INPUT].getVoltage() + round(params[OCTAVE_SHIFT_1].getValue()) + round(inputs[OCTAVE_SHIFT_1_CVINPUT].getVoltage()/2);
  octave_2_out = inputs[OCTAVE_SHIFT_2_INPUT].getVoltage() + round(params[OCTAVE_SHIFT_2].getValue()) + round(inputs[OCTAVE_SHIFT_1_CVINPUT].getVoltage()/2);
  semitone_1_out = inputs[SEMITONE_SHIFT_1_INPUT].getVoltage() + round(params[SEMITONE_SHIFT_1].getValue())*(1.0/12.0) + round(inputs[SEMITONE_SHIFT_1_CVINPUT].getVoltage()/2)*(1.0/12.0);
  semitone_2_out = inputs[SEMITONE_SHIFT_2_INPUT].getVoltage() + round(params[SEMITONE_SHIFT_2].getValue())*(1.0/12.0) + round(inputs[SEMITONE_SHIFT_2_CVINPUT].getVoltage()/2)*(1.0/12.0);
    
  outputs[OCTAVE_SHIFT_1_OUTPUT].setVoltage(octave_1_out);
  outputs[OCTAVE_SHIFT_2_OUTPUT].setVoltage(octave_2_out);
  outputs[SEMITONE_SHIFT_1_OUTPUT].setVoltage(semitone_1_out);
  outputs[SEMITONE_SHIFT_2_OUTPUT].setVoltage(semitone_2_out);

}

//////////////////////////////////////////////////////////////////
struct MentalPitchShiftWidget : ModuleWidget {
  MentalPitchShiftWidget(MentalPitchShift *module){

    setModule(module);

  setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/MentalPitchShift.svg")));

  addParam(createParam<MedKnob>(Vec(2, 20), module, MentalPitchShift::OCTAVE_SHIFT_1));
  addParam(createParam<MedKnob>(Vec(2, 80), module, MentalPitchShift::OCTAVE_SHIFT_2));
  addParam(createParam<MedKnob>(Vec(2, 140), module, MentalPitchShift::SEMITONE_SHIFT_1));
  addParam(createParam<MedKnob>(Vec(2, 200), module, MentalPitchShift::SEMITONE_SHIFT_2));

  addInput(createInput<CVInPort>(Vec(3, 50), module, MentalPitchShift::OCTAVE_SHIFT_1_INPUT));
	addInput(createInput<CVInPort>(Vec(3, 110), module, MentalPitchShift::OCTAVE_SHIFT_2_INPUT));
  addInput(createInput<CVInPort>(Vec(3, 170), module, MentalPitchShift::SEMITONE_SHIFT_1_INPUT));
	addInput(createInput<CVInPort>(Vec(3, 230), module, MentalPitchShift::SEMITONE_SHIFT_2_INPUT));
  
  addInput(createInput<CVInPort>(Vec(33, 20), module, MentalPitchShift::OCTAVE_SHIFT_1_CVINPUT));
	addInput(createInput<CVInPort>(Vec(33, 80), module, MentalPitchShift::OCTAVE_SHIFT_2_CVINPUT));
  addInput(createInput<CVInPort>(Vec(33, 140), module, MentalPitchShift::SEMITONE_SHIFT_1_CVINPUT));
	addInput(createInput<CVInPort>(Vec(33, 200), module, MentalPitchShift::SEMITONE_SHIFT_2_CVINPUT));

  addOutput(createOutput<CVOutPort>(Vec(33, 50), module, MentalPitchShift::OCTAVE_SHIFT_1_OUTPUT));
  addOutput(createOutput<CVOutPort>(Vec(33, 110), module, MentalPitchShift::OCTAVE_SHIFT_2_OUTPUT));
  addOutput(createOutput<CVOutPort>(Vec(33, 170), module, MentalPitchShift::SEMITONE_SHIFT_1_OUTPUT));
  addOutput(createOutput<CVOutPort>(Vec(33, 230), module, MentalPitchShift::SEMITONE_SHIFT_2_OUTPUT));

}
};

Model *modelMentalPitchShift = createModel<MentalPitchShift, MentalPitchShiftWidget>("MentalPitchShift");