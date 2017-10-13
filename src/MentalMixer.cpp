#include "mental.hpp"


struct MentalMixer : Module {
	enum ParamIds {
		MIX_PARAM,
    AUX_SEND_1_PARAM,
    AUX_SEND_2_PARAM,
    AUX_RETURN_1_PARAM,
    AUX_RETURN_2_PARAM,
		CH1_PARAM,
    CH1_PAN_PARAM,
    CH1_AUX_1_PARAM,
    CH1_AUX_2_PARAM,
    CH1_MUTE_PARAM,
		CH2_PARAM,
    CH2_PAN_PARAM,
    CH2_AUX_1_PARAM,
    CH2_AUX_2_PARAM,
    CH2_MUTE_PARAM,
		CH3_PARAM,
    CH3_PAN_PARAM,
    CH3_AUX_1_PARAM,
    CH3_AUX_2_PARAM,
    CH3_MUTE_PARAM,
    CH4_PARAM,
    CH4_PAN_PARAM,
    CH4_AUX_1_PARAM,
    CH4_AUX_2_PARAM,
    CH4_MUTE_PARAM,
    CH5_PARAM,
    CH5_PAN_PARAM,
    CH5_AUX_1_PARAM,
    CH5_AUX_2_PARAM,
    CH5_MUTE_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		CH1_INPUT,
    MIX_CV_INPUT,
		CH1_CV_INPUT,
    CH1_PAN_INPUT,
    CH1_MUTE_INPUT,
		CH2_INPUT,
		CH2_CV_INPUT,
    CH2_PAN_INPUT,
    CH2_MUTE_INPUT,
		CH3_INPUT,
		CH3_CV_INPUT,
    CH3_PAN_INPUT,
    CH3_MUTE_INPUT,
    CH4_INPUT,
		CH4_CV_INPUT,
    CH4_PAN_INPUT,
    CH4_MUTE_INPUT,
		CH5_INPUT,
		CH5_CV_INPUT,
    CH5_PAN_INPUT,
    CH5_MUTE_INPUT,
    RETURN_1_L_INPUT,
    RETURN_1_R_INPUT,
    RETURN_2_L_INPUT,
    RETURN_2_R_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		MIX_OUTPUT_L,
    MIX_OUTPUT_R,
    SEND_1_OUTPUT,
    SEND_2_OUTPUT,
		CH1_OUTPUT,
		CH2_OUTPUT,
    CH3_OUTPUT,
		CH4_OUTPUT,
		CH5_OUTPUT,
		NUM_OUTPUTS
	};

  float Ch1_Mute_Light = 1.0;
  float Ch2_Mute_Light = 1.0;
  float Ch3_Mute_Light = 1.0;
  float Ch4_Mute_Light = 1.0;
  float Ch5_Mute_Light = 1.0;

	MentalMixer() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
	void step();
};

/*
MentalMixer::MentalMixer() {
	params.resize(NUM_PARAMS);
	inputs.resize(NUM_INPUTS);
	outputs.resize(NUM_OUTPUTS);
}*/


void MentalMixer::step() {

  //                                                              clampf(inputs[CH1_CV_INPUT].normalize(10.0) / 10.0, 0.0, 1.0);
  float ch1 = inputs[CH1_INPUT].value * params[CH1_PARAM].value * clampf(inputs[CH1_CV_INPUT].normalize(10.0) / 10.0, 0.0, 1.0);
  float ch1_send_1 = inputs[CH1_INPUT].value * params[CH1_AUX_1_PARAM].value * clampf(inputs[CH1_CV_INPUT].normalize(10.0) / 10.0, 0.0, 1.0);
  float ch1_send_2 = inputs[CH1_INPUT].value * params[CH1_AUX_2_PARAM].value * clampf(inputs[CH1_CV_INPUT].normalize(10.0) / 10.0, 0.0, 1.0);

  float ch1_l = ch1 * (1-params[CH1_PAN_PARAM].value)* 2;
  float ch1_r = ch1 * params[CH1_PAN_PARAM].value * 2;

	float ch2 = inputs[CH2_INPUT].value * params[CH2_PARAM].value * clampf(inputs[CH2_CV_INPUT].normalize(10.0) / 10.0, -1.0, 1.0);
  float ch2_send_1 = inputs[CH2_INPUT].value * params[CH2_AUX_1_PARAM].value * clampf(inputs[CH2_CV_INPUT].normalize(10.0) / 10.0, 0.0, 1.0);
  float ch2_send_2 = inputs[CH2_INPUT].value * params[CH2_AUX_2_PARAM].value * clampf(inputs[CH2_CV_INPUT].normalize(10.0) / 10.0, 0.0, 1.0);

  float ch2_l = ch2 * (1-params[CH2_PAN_PARAM].value)* 2 ;
  float ch2_r = ch2 * params[CH2_PAN_PARAM].value * 2;

	float ch3 = inputs[CH3_INPUT].value * params[CH3_PARAM].value * clampf(inputs[CH3_CV_INPUT].normalize(10.0) / 10.0, 0.0, 1.0);
  float ch3_send_1 = inputs[CH3_INPUT].value * params[CH3_AUX_1_PARAM].value * clampf(inputs[CH3_CV_INPUT].normalize(10.0) / 10.0, 0.0, 1.0);
  float ch3_send_2 = inputs[CH3_INPUT].value * params[CH3_AUX_2_PARAM].value * clampf(inputs[CH3_CV_INPUT].normalize(10.0) / 10.0, 0.0, 1.0);

  float ch3_l = ch3 * (1-params[CH3_PAN_PARAM].value)* 2;
  float ch3_r = ch3 * params[CH3_PAN_PARAM].value * 2;

  float ch4 = inputs[CH4_INPUT].value * params[CH4_PARAM].value * clampf(inputs[CH4_CV_INPUT].normalize(10.0) / 10.0, -1.0, 1.0);
  float ch4_send_1 = inputs[CH4_INPUT].value * params[CH4_AUX_1_PARAM].value * clampf(inputs[CH4_CV_INPUT].normalize(10.0) / 10.0, 0.0, 1.0);
  float ch4_send_2 = inputs[CH4_INPUT].value * params[CH4_AUX_2_PARAM].value * clampf(inputs[CH4_CV_INPUT].normalize(10.0) / 10.0, 0.0, 1.0);

  float ch4_l = ch4 * (1-params[CH4_PAN_PARAM].value)* 2 ;
  float ch4_r = ch4 * params[CH4_PAN_PARAM].value * 2;

	float ch5 = inputs[CH5_INPUT].value * params[CH5_PARAM].value * clampf(inputs[CH5_CV_INPUT].normalize(10.0) / 10.0, 0.0, 1.0);
  float ch5_send_1 = inputs[CH5_INPUT].value * params[CH5_AUX_1_PARAM].value * clampf(inputs[CH3_CV_INPUT].normalize(10.0) / 10.0, 0.0, 1.0);
  float ch5_send_2 = inputs[CH5_INPUT].value * params[CH5_AUX_2_PARAM].value * clampf(inputs[CH3_CV_INPUT].normalize(10.0) / 10.0, 0.0, 1.0);

  float ch5_l = ch5 * (1-params[CH5_PAN_PARAM].value)* 2;
  float ch5_r = ch5 * params[CH5_PAN_PARAM].value * 2;

  // get returns
  float return_1_l = inputs[RETURN_1_L_INPUT].value * params[AUX_RETURN_1_PARAM].value;
  float return_1_r = inputs[RETURN_1_R_INPUT].value * params[AUX_RETURN_1_PARAM].value;
  float return_2_l = inputs[RETURN_2_L_INPUT].value * params[AUX_RETURN_2_PARAM].value;
  float return_2_r = inputs[RETURN_2_R_INPUT].value * params[AUX_RETURN_2_PARAM].value;


	float mix_l = (ch1_l + ch2_l + ch3_l + ch4_l + ch5_l + return_1_l + return_2_l) * params[MIX_PARAM].value * inputs[MIX_CV_INPUT].normalize(10.0) / 10.0;
  float mix_r = (ch1_r + ch2_r + ch3_r + ch4_r + ch5_r + return_1_r + return_2_r) * params[MIX_PARAM].value * inputs[MIX_CV_INPUT].normalize(10.0) / 10.0;

  float send_1_mix = (ch1_send_1 + ch2_send_1 + ch3_send_1 + ch4_send_1 + ch5_send_1) * params[AUX_SEND_1_PARAM].value;
  float send_2_mix = (ch1_send_2 + ch2_send_2 + ch3_send_2 + ch4_send_2 + ch5_send_2) * params[AUX_SEND_2_PARAM].value;

	outputs[CH1_OUTPUT].value = ch1;
	outputs[CH2_OUTPUT].value = ch2;
	outputs[CH3_OUTPUT].value = ch3;
  outputs[CH4_OUTPUT].value = ch4;
	outputs[CH5_OUTPUT].value = ch5;
	outputs[MIX_OUTPUT_L].value = mix_l;
  outputs[MIX_OUTPUT_R].value = mix_r;

  outputs[SEND_1_OUTPUT].value = send_1_mix;
  outputs[SEND_2_OUTPUT].value = send_2_mix;


}


MentalMixerWidget::MentalMixerWidget() {
	MentalMixer *module = new MentalMixer();
	setModule(module);
	box.size = Vec(15*30, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load("plugins/mental/res/Mixer.svg"));
		addChild(panel);
	}
  int column_1 = 33;
  int right_column = 408;

  int input_row = 70;
  int vol_knob_row = input_row + 30;
  int vol_cv_row = input_row + 60;
  int pan_knob_row = input_row + 90;
  int pan_cv_row = input_row + 120;
  int aux_1_row = input_row + 150;
  int aux_2_row = input_row + 180;
  int mute_row = input_row + 210;
  int mute_gate_row = input_row + 240;


	addParam(createParam<SynthTechAlco>(Vec(345, 30), module, MentalMixer::MIX_PARAM, 0.0, 1.0, 0.5)); // master volume

  // send volumes
  addParam(createParam<Trimpot>(Vec(right_column - 25, vol_knob_row), module, MentalMixer::AUX_SEND_1_PARAM, 0.0, 1.0, 0.0));
  addParam(createParam<Trimpot>(Vec(right_column - 25, pan_knob_row), module, MentalMixer::AUX_SEND_2_PARAM, 0.0, 1.0, 0.0));
  // return volumes
  addParam(createParam<Trimpot>(Vec(right_column - 25, aux_2_row), module, MentalMixer::AUX_RETURN_1_PARAM, 0.0, 1.0, 0.0));
  addParam(createParam<Trimpot>(Vec(right_column - 25, mute_gate_row), module, MentalMixer::AUX_RETURN_2_PARAM, 0.0, 1.0, 0.0));

  // channel strips
	addParam(createParam<Trimpot>(Vec(column_1+3, vol_knob_row), module, MentalMixer::CH1_PARAM, 0.0, 1.0, 0.0));
  addParam(createParam<Trimpot>(Vec(column_1+3, pan_knob_row), module, MentalMixer::CH1_PAN_PARAM, 0.0, 1.0, 0.5));
  addParam(createParam<Trimpot>(Vec(column_1+3, aux_1_row), module, MentalMixer::CH1_AUX_1_PARAM, 0.0, 1.0, 0.5));
  addParam(createParam<Trimpot>(Vec(column_1+3, aux_2_row), module, MentalMixer::CH1_AUX_2_PARAM, 0.0, 1.0, 0.5));
  addParam(createParam<LEDButton>(Vec(column_1+3, mute_row), module, MentalMixer::CH1_MUTE_PARAM, 0.0, 1.0, 0.0));
	addChild(createValueLight<SmallLight<GreenValueLight>>(Vec(column_1+8, mute_row+5), &module->Ch1_Mute_Light));

	addParam(createParam<Trimpot>(Vec((column_1 + 33), vol_knob_row), module, MentalMixer::CH2_PARAM, 0.0, 1.0, 0.0));
  addParam(createParam<Trimpot>(Vec((column_1 + 33), pan_knob_row), module, MentalMixer::CH2_PAN_PARAM, 0.0, 1.0, 0.5));
  addParam(createParam<Trimpot>(Vec(column_1+33, aux_1_row), module, MentalMixer::CH2_AUX_1_PARAM, 0.0, 1.0, 0.5));
  addParam(createParam<Trimpot>(Vec(column_1+33, aux_2_row), module, MentalMixer::CH2_AUX_2_PARAM, 0.0, 1.0, 0.5));
  addParam(createParam<LEDButton>(Vec(column_1+33, mute_row), module, MentalMixer::CH2_MUTE_PARAM, 0.0, 1.0, 0.0));
	addChild(createValueLight<SmallLight<GreenValueLight>>(Vec(column_1+38, mute_row+5), &module->Ch2_Mute_Light));

	addParam(createParam<Trimpot>(Vec((column_1 + 63), vol_knob_row), module, MentalMixer::CH3_PARAM, 0.0, 1.0, 0.0));
  addParam(createParam<Trimpot>(Vec((column_1 + 63), pan_knob_row), module, MentalMixer::CH3_PAN_PARAM, 0.0, 1.0, 0.5));
  addParam(createParam<Trimpot>(Vec(column_1+63, aux_1_row), module, MentalMixer::CH3_AUX_1_PARAM, 0.0, 1.0, 0.5));
  addParam(createParam<Trimpot>(Vec(column_1+63, aux_2_row), module, MentalMixer::CH3_AUX_2_PARAM, 0.0, 1.0, 0.5));
  addParam(createParam<LEDButton>(Vec(column_1+63, mute_row), module, MentalMixer::CH3_MUTE_PARAM, 0.0, 1.0, 0.0));
	addChild(createValueLight<SmallLight<GreenValueLight>>(Vec(column_1+68, mute_row+5), &module->Ch3_Mute_Light));

  addParam(createParam<Trimpot>(Vec((column_1 + 93), vol_knob_row), module, MentalMixer::CH4_PARAM, 0.0, 1.0, 0.0));
  addParam(createParam<Trimpot>(Vec((column_1 + 93), pan_knob_row), module, MentalMixer::CH4_PAN_PARAM, 0.0, 1.0, 0.5));
  addParam(createParam<Trimpot>(Vec(column_1+93, aux_1_row), module, MentalMixer::CH4_AUX_1_PARAM, 0.0, 1.0, 0.5));
  addParam(createParam<Trimpot>(Vec(column_1+93, aux_2_row), module, MentalMixer::CH4_AUX_2_PARAM, 0.0, 1.0, 0.5));
  addParam(createParam<LEDButton>(Vec(column_1+93, mute_row), module, MentalMixer::CH4_MUTE_PARAM, 0.0, 1.0, 0.0));
	addChild(createValueLight<SmallLight<GreenValueLight>>(Vec(column_1+98, mute_row+5), &module->Ch4_Mute_Light));

	addParam(createParam<Trimpot>(Vec((column_1 + 123), vol_knob_row), module, MentalMixer::CH5_PARAM, 0.0, 1.0, 0.0));
  addParam(createParam<Trimpot>(Vec((column_1 + 123), pan_knob_row), module, MentalMixer::CH5_PAN_PARAM, 0.0, 1.0, 0.5));
  addParam(createParam<Trimpot>(Vec(column_1+123, aux_1_row), module, MentalMixer::CH5_AUX_1_PARAM, 0.0, 1.0, 0.5));
  addParam(createParam<Trimpot>(Vec(column_1+123, aux_2_row), module, MentalMixer::CH5_AUX_2_PARAM, 0.0, 1.0, 0.5));
  addParam(createParam<LEDButton>(Vec(column_1+123, mute_row), module, MentalMixer::CH5_MUTE_PARAM, 0.0, 1.0, 0.0));
	addChild(createValueLight<SmallLight<GreenValueLight>>(Vec(column_1+128, mute_row+5), &module->Ch5_Mute_Light));

	//addInput(createInput<PJ301MPort>(Vec(160, 69), module, MentalMixer::MIX_CV_INPUT));

	addInput(createInput<PJ301MPort>(Vec(column_1, input_row), module, MentalMixer::CH1_INPUT));
	addInput(createInput<PJ301MPort>(Vec(column_1, vol_cv_row), module, MentalMixer::CH1_CV_INPUT));
  addInput(createInput<PJ301MPort>(Vec(column_1, pan_cv_row), module, MentalMixer::CH1_PAN_INPUT));
  addInput(createInput<PJ301MPort>(Vec(column_1, mute_gate_row), module, MentalMixer::CH1_MUTE_INPUT));

	addInput(createInput<PJ301MPort>(Vec((column_1 + 30), input_row), module, MentalMixer::CH2_INPUT));
	addInput(createInput<PJ301MPort>(Vec((column_1 + 30), vol_cv_row), module, MentalMixer::CH2_CV_INPUT));
  addInput(createInput<PJ301MPort>(Vec((column_1 + 30), pan_cv_row), module, MentalMixer::CH2_PAN_INPUT));
  addInput(createInput<PJ301MPort>(Vec(column_1 + 30, mute_gate_row), module, MentalMixer::CH2_MUTE_INPUT));

	addInput(createInput<PJ301MPort>(Vec((column_1 + 60), input_row), module, MentalMixer::CH3_INPUT));
	addInput(createInput<PJ301MPort>(Vec((column_1 + 60), vol_cv_row), module, MentalMixer::CH3_CV_INPUT));
  addInput(createInput<PJ301MPort>(Vec((column_1 + 60), pan_cv_row), module, MentalMixer::CH3_PAN_INPUT));
  addInput(createInput<PJ301MPort>(Vec(column_1 + 60, mute_gate_row), module, MentalMixer::CH3_MUTE_INPUT));

  addInput(createInput<PJ301MPort>(Vec((column_1 + 90), input_row), module, MentalMixer::CH4_INPUT));
	addInput(createInput<PJ301MPort>(Vec((column_1 + 90), vol_cv_row), module, MentalMixer::CH4_CV_INPUT));
  addInput(createInput<PJ301MPort>(Vec((column_1 + 90), pan_cv_row), module, MentalMixer::CH4_PAN_INPUT));
  addInput(createInput<PJ301MPort>(Vec(column_1 + 90, mute_gate_row), module, MentalMixer::CH4_MUTE_INPUT));

	addInput(createInput<PJ301MPort>(Vec((column_1 + 120), input_row), module, MentalMixer::CH5_INPUT));
	addInput(createInput<PJ301MPort>(Vec((column_1 + 120), vol_cv_row), module, MentalMixer::CH5_CV_INPUT));
  addInput(createInput<PJ301MPort>(Vec((column_1 + 120), pan_cv_row), module, MentalMixer::CH5_PAN_INPUT));
  addInput(createInput<PJ301MPort>(Vec(column_1 + 120, mute_gate_row), module, MentalMixer::CH5_MUTE_INPUT));


  //
  addOutput(createOutput<PJ301MPort>(Vec(right_column, 30), module, MentalMixer::MIX_OUTPUT_L));
	addOutput(createOutput<PJ301MPort>(Vec(right_column, 70), module, MentalMixer::MIX_OUTPUT_R));

  addOutput(createOutput<PJ301MPort>(Vec(right_column, 120), module, MentalMixer::SEND_1_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(right_column, 180), module, MentalMixer::SEND_2_OUTPUT));

  addInput(createInput<PJ301MPort>(Vec(right_column, 225), module, MentalMixer::RETURN_1_L_INPUT));
  addInput(createInput<PJ301MPort>(Vec(right_column, 250), module, MentalMixer::RETURN_1_R_INPUT));

  addInput(createInput<PJ301MPort>(Vec(right_column, 295), module, MentalMixer::RETURN_2_L_INPUT));
  addInput(createInput<PJ301MPort>(Vec(right_column, 320), module, MentalMixer::RETURN_2_R_INPUT));


  //addOutput(createOutput<PJ301MPort>(Vec(165, 145), module, MentalMixer::CH1_OUTPUT));
	//addOutput(createOutput<PJ301MPort>(Vec(165, 225), module, MentalMixer::CH2_OUTPUT));
	//addOutput(createOutput<PJ301MPort>(Vec(165, 306), module, MentalMixer::CH3_OUTPUT));
}
