///////////////////////////////////////////////////
//
//   Sub Mixer VCV Module
//   Built from fundamental VCMixer 
//
//   Strum 2017
//
///////////////////////////////////////////////////
#include "mental.hpp"

struct MentalSubMixer : Module {
	enum ParamIds {
		MIX_PARAM,
		CH1_PARAM,
    CH1_PAN_PARAM,
		CH2_PARAM,
    CH2_PAN_PARAM,
		CH3_PARAM,
    CH3_PAN_PARAM,
    CH4_PARAM,
    CH4_PAN_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		MIX_CV_INPUT,
		CH1_INPUT,
		CH1_CV_INPUT,
    CH1_PAN_INPUT,
		CH2_INPUT,
		CH2_CV_INPUT,
    CH2_PAN_INPUT,
		CH3_INPUT,
		CH3_CV_INPUT,
    CH3_PAN_INPUT,
    CH4_INPUT,
		CH4_CV_INPUT,
    CH4_PAN_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		MIX_OUTPUT_L,
    MIX_OUTPUT_R,
		CH1_OUTPUT,
		CH2_OUTPUT,
		CH3_OUTPUT,
    CH4_OUTPUT,
		NUM_OUTPUTS
	};

	MentalSubMixer() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {};
	void step();
};


void MentalSubMixer::step() {
  
  float ch1 = inputs[CH1_INPUT].value * params[CH1_PARAM].value * clampf(inputs[CH1_CV_INPUT].normalize(10.0) / 10.0, 0.0, 1.0);

  float pan_cv_in_1 = inputs[CH1_PAN_INPUT].value/5;
  float pan_position_1 = pan_cv_in_1 + params[CH1_PAN_PARAM].value;
   
  if (pan_position_1 < 0) pan_position_1 = 0;
  if (pan_position_1 > 1) pan_position_1 = 1;  
    
  float ch1_l = ch1 * (1-pan_position_1)* 2;
  float ch1_r = ch1 * pan_position_1 * 2;

	float ch2 = inputs[CH2_INPUT].value * params[CH2_PARAM].value * clampf(inputs[CH2_CV_INPUT].normalize(10.0) / 10.0, -1.0, 1.0);
  
  float pan_cv_in_2 = inputs[CH2_PAN_INPUT].value/5;
  float pan_position_2 = pan_cv_in_2 + params[CH2_PAN_PARAM].value;
   
  if (pan_position_2 < 0) pan_position_2 = 0;
  if (pan_position_2 > 1) pan_position_2 = 1;  
    
  float ch2_l = ch2 * (1-pan_position_2)* 2;
  float ch2_r = ch2 * pan_position_2 * 2;
  
	float ch3 = inputs[CH3_INPUT].value * params[CH3_PARAM].value * clampf(inputs[CH3_CV_INPUT].normalize(10.0) / 10.0, 0.0, 1.0);

  float pan_cv_in_3 = inputs[CH3_PAN_INPUT].value/5;
  float pan_position_3 = pan_cv_in_3 + params[CH3_PAN_PARAM].value;
   
  if (pan_position_3 < 0) pan_position_3 = 0;
  if (pan_position_3 > 1) pan_position_3 = 1;  
    
  float ch3_l = ch3 * (1-pan_position_3)* 2;
  float ch3_r = ch3 * pan_position_3 * 2;
    
  float ch4 = inputs[CH4_INPUT].value * params[CH4_PARAM].value * clampf(inputs[CH4_CV_INPUT].normalize(10.0) / 10.0, 0.0, 1.0);

  float pan_cv_in_4 = inputs[CH4_PAN_INPUT].value/5;
  float pan_position_4 = pan_cv_in_4 + params[CH4_PAN_PARAM].value;
   
  if (pan_position_4 < 0) pan_position_4 = 0;
  if (pan_position_4 > 1) pan_position_4 = 1;  
    
  float ch4_l = ch4 * (1-pan_position_4)* 2;
  float ch4_r = ch4 * pan_position_4 * 2;

	float mix_l = (ch1_l + ch2_l + ch3_l + ch4_l) * params[MIX_PARAM].value * inputs[MIX_CV_INPUT].normalize(10.0) / 10.0;
  float mix_r = (ch1_r + ch2_r + ch3_r + ch4_r) * params[MIX_PARAM].value * inputs[MIX_CV_INPUT].normalize(10.0) / 10.0;

	outputs[CH1_OUTPUT].value = ch1;
	outputs[CH2_OUTPUT].value = ch2;
	outputs[CH3_OUTPUT].value = ch3;
  outputs[CH4_OUTPUT].value = ch4;
	outputs[MIX_OUTPUT_L].value = mix_l;
  outputs[MIX_OUTPUT_R].value = mix_r;
}


MentalSubMixerWidget::MentalSubMixerWidget() {
	MentalSubMixer *module = new MentalSubMixer();
	setModule(module);
	box.size = Vec(15*8, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		//panel->setBackground(SVG::load("plugins/mental/res/MentalSubMixer.svg"));
    panel->setBackground(SVG::load(assetPlugin(plugin,"res/MentalSubMixer.svg")));
		addChild(panel);
	}
  int y_offset = 40;
	addParam(createParam<SynthTechAlco>(Vec(23, 15), module, MentalSubMixer::MIX_PARAM, 0.0, 1.0, 0.5));
  addInput(createInput<PJ301MPort>(Vec(33, 65), module, MentalSubMixer::MIX_CV_INPUT));
  
  addOutput(createOutput<PJ301MPort>(Vec(93, 20), module, MentalSubMixer::MIX_OUTPUT_L));
	addOutput(createOutput<PJ301MPort>(Vec(93, 45), module, MentalSubMixer::MIX_OUTPUT_R));
  
	
  addParam(createParam<Davies1900hSmallBlackKnob>(Vec(33,y_offset + 90), module, MentalSubMixer::CH1_PARAM, 0.0, 1.0, 0.0));
  addParam(createParam<Davies1900hSmallBlackKnob>(Vec(63,y_offset + 90), module, MentalSubMixer::CH1_PAN_PARAM, 0.0, 1.0, 0.5));
  
	addParam(createParam<Davies1900hSmallBlackKnob>(Vec(33,y_offset + 150), module, MentalSubMixer::CH2_PARAM, 0.0, 1.0, 0.0));
  addParam(createParam<Davies1900hSmallBlackKnob>(Vec(63,y_offset + 150), module, MentalSubMixer::CH2_PAN_PARAM, 0.0, 1.0, 0.5));
  
	addParam(createParam<Davies1900hSmallBlackKnob>(Vec(33,y_offset + 210), module, MentalSubMixer::CH3_PARAM, 0.0, 1.0, 0.0));
  addParam(createParam<Davies1900hSmallBlackKnob>(Vec(63,y_offset + 210), module, MentalSubMixer::CH3_PAN_PARAM, 0.0, 1.0, 0.5));
  
  addParam(createParam<Davies1900hSmallBlackKnob>(Vec(33,y_offset + 270), module, MentalSubMixer::CH4_PARAM, 0.0, 1.0, 0.0));
  addParam(createParam<Davies1900hSmallBlackKnob>(Vec(63,y_offset + 270), module, MentalSubMixer::CH4_PAN_PARAM, 0.0, 1.0, 0.5));		
  

	addInput(createInput<PJ301MPort>(Vec(3,y_offset + 120), module, MentalSubMixer::CH1_INPUT));
	addInput(createInput<PJ301MPort>(Vec(33,y_offset + 120), module, MentalSubMixer::CH1_CV_INPUT));
  addInput(createInput<PJ301MPort>(Vec(63,y_offset + 120), module, MentalSubMixer::CH1_PAN_INPUT));
  
	addInput(createInput<PJ301MPort>(Vec(3,y_offset + 180), module, MentalSubMixer::CH2_INPUT));
	addInput(createInput<PJ301MPort>(Vec(33,y_offset + 180), module, MentalSubMixer::CH2_CV_INPUT));
  addInput(createInput<PJ301MPort>(Vec(63,y_offset + 180), module, MentalSubMixer::CH2_PAN_INPUT));
  
	addInput(createInput<PJ301MPort>(Vec(3,y_offset + 240), module, MentalSubMixer::CH3_INPUT));
	addInput(createInput<PJ301MPort>(Vec(33,y_offset + 240), module, MentalSubMixer::CH3_CV_INPUT));
  addInput(createInput<PJ301MPort>(Vec(63,y_offset + 240), module, MentalSubMixer::CH3_PAN_INPUT));
  
  addInput(createInput<PJ301MPort>(Vec(3,y_offset + 300), module, MentalSubMixer::CH4_INPUT));
	addInput(createInput<PJ301MPort>(Vec(33,y_offset + 300), module, MentalSubMixer::CH4_CV_INPUT));
  addInput(createInput<PJ301MPort>(Vec(63,y_offset + 300), module, MentalSubMixer::CH4_PAN_INPUT));
    
  
	addOutput(createOutput<PJ301MPort>(Vec(93,y_offset + 120), module, MentalSubMixer::CH1_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(93,y_offset + 180), module, MentalSubMixer::CH2_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(93,y_offset + 240), module, MentalSubMixer::CH3_OUTPUT));
  addOutput(createOutput<PJ301MPort>(Vec(93,y_offset + 300), module, MentalSubMixer::CH4_OUTPUT));
  
}
