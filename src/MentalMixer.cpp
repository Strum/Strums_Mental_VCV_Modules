///////////////////////////////////////////////////
//
//   Mixer VCV Module
//
//   Strum 2017
//
///////////////////////////////////////////////////

#include "mental.hpp"
#include "dsp/digital.hpp"

///////////////////////////////////////////////////
struct MentalMixer : Module {
	enum ParamIds {
		MIX_PARAM,
    AUX_SEND_1_PARAM,
    AUX_SEND_2_PARAM,
    AUX_RETURN_1_PARAM,
    AUX_RETURN_2_PARAM,
		VOL_PARAM,
    PAN_PARAM = VOL_PARAM + 12,
    AUX_1_PARAM = PAN_PARAM + 12 ,
    AUX_2_PARAM = AUX_1_PARAM + 12,
    MUTE_PARAM = AUX_2_PARAM + 12,		
		NUM_PARAMS = MUTE_PARAM + 12
	};
	enum InputIds {
		CH_INPUT,
		CH_VOL_INPUT = CH_INPUT + 12,
    CH_PAN_INPUT = CH_VOL_INPUT + 12,
    CH_MUTE_INPUT = CH_PAN_INPUT + 12,		
    RETURN_1_L_INPUT = CH_MUTE_INPUT + 12,
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
		NUM_OUTPUTS
	};
  SchmittTrigger mute_triggers[12];
  float mute_lights[12] = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
  bool mute_states[12]= {1,1,1,1,1,1,1,1,1,1,1,1};
  float channel_ins[12];
  float pan_cv_ins[12];
  float pan_positions[12]; 
  float channel_sends_1[12];
  float channel_sends_2[12];
  float channel_outs_l[12];
  float channel_outs_r[12];
  float send_1_sum = 0.0;
  float send_2_sum = 0.0;
  float left_sum = 0.0;
  float right_sum = 0.0;
  
	MentalMixer() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
	void step();
};

///////////////////////////////////////////////////////////////////
void MentalMixer::step() {

  //clampf(inputs[CH1_CV_INPUT].normalize(10.0) / 10.0, 0.0, 1.0);
  send_1_sum = 0.0;
  send_2_sum = 0.0;
  left_sum = 0.0;
  right_sum = 0.0;
  
  for  (int i = 0 ; i < 12; i++)
  {
    if (mute_triggers[i].process(params[MUTE_PARAM + i].value))
    {
		  mute_states[i] = !mute_states[i];
	  }
    mute_lights[i] = mute_states[i] ? 1.0 : 0.0;
  }
  for (int i = 0 ; i < 12 ; i++)
  {  
    channel_ins[i] = inputs[CH_INPUT + i].value * params[VOL_PARAM + i].value * clampf(inputs[CH_VOL_INPUT + i].normalize(10.0) / 10.0, 0.0, 1.0);
    
    if (!mute_states[i] || inputs[CH_MUTE_INPUT + i].value > 0.0 )
    {
      channel_ins[i] = 0.0;
      mute_lights[i] = 0;      
    }
    
    channel_sends_1[i] = channel_ins[i] * params[AUX_1_PARAM + i].value * clampf(inputs[CH_VOL_INPUT + i].normalize(10.0) / 10.0, 0.0, 1.0);
    channel_sends_2[i] = channel_ins[i] * params[AUX_2_PARAM + i].value * clampf(inputs[CH_VOL_INPUT + i].normalize(10.0) / 10.0, 0.0, 1.0);

    pan_cv_ins[i] = inputs[CH_PAN_INPUT + i].value/5;
    pan_positions[i] = pan_cv_ins[i] + params[PAN_PARAM+i].value;   
    if (pan_positions[i] < 0) pan_positions[i] = 0;
    if (pan_positions[i] > 1) pan_positions[i] = 1;    
    channel_outs_l[i]= channel_ins[i] * (1-pan_positions[i])* 2;
    channel_outs_r[i]= channel_ins[i] * pan_positions[i] * 2;      
    
    send_1_sum += channel_sends_1[i];
    send_2_sum += channel_sends_2[i];
    left_sum += channel_outs_l[i];
    right_sum += channel_outs_r[i];    
  }
	

  // get returns
  float return_1_l = inputs[RETURN_1_L_INPUT].value * params[AUX_RETURN_1_PARAM].value;
  float return_1_r = inputs[RETURN_1_R_INPUT].value * params[AUX_RETURN_1_PARAM].value;
  float return_2_l = inputs[RETURN_2_L_INPUT].value * params[AUX_RETURN_2_PARAM].value;
  float return_2_r = inputs[RETURN_2_R_INPUT].value * params[AUX_RETURN_2_PARAM].value;

	float mix_l = (left_sum + return_1_l + return_2_l) * params[MIX_PARAM].value;
  float mix_r = (right_sum + return_1_r + return_2_r) * params[MIX_PARAM].value;

  float send_1_mix = (send_1_sum) * params[AUX_SEND_1_PARAM].value;
  float send_2_mix = (send_2_sum) * params[AUX_SEND_2_PARAM].value;

		
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
		//panel->setBackground(SVG::load("plugins/mental/res/Mixer.svg"));
    panel->setBackground(SVG::load(assetPlugin(plugin,"res/Mixer.svg")));
		addChild(panel);
	}
  int column_1 = 18;
  int right_column = 408;

  int top_row = 90;
  int row_spacing = 28;
  int column_spacing = 30;


	addParam(createParam<SynthTechAlco>(Vec(345, 30), module, MentalMixer::MIX_PARAM, 0.0, 1.0, 0.5)); // master volume

  // send volumes
  addParam(createParam<Davies1900hSmallBlackKnob>(Vec(right_column - 25, 120), module, MentalMixer::AUX_SEND_1_PARAM, 0.0, 1.0, 0.0));
  addParam(createParam<Davies1900hSmallBlackKnob>(Vec(right_column - 25, 180), module, MentalMixer::AUX_SEND_2_PARAM, 0.0, 1.0, 0.0));
  // return volumes
  addParam(createParam<Davies1900hSmallBlackKnob>(Vec(right_column - 25, 250), module, MentalMixer::AUX_RETURN_1_PARAM, 0.0, 1.0, 0.0));
  addParam(createParam<Davies1900hSmallBlackKnob>(Vec(right_column - 25, 300), module, MentalMixer::AUX_RETURN_2_PARAM, 0.0, 1.0, 0.0));

  // channel strips
  for (int i = 0 ; i < 12 ; i++)
  {
    addInput(createInput<PJ301MPort>(Vec(column_1+column_spacing*i, top_row), module, MentalMixer::CH_INPUT + i));
	  addParam(createParam<Trimpot>(Vec(column_1+column_spacing*i,top_row + row_spacing), module, MentalMixer::VOL_PARAM + i, 0.0, 1.0, 0.0));
    addInput(createInput<PJ301MPort>(Vec(column_1+column_spacing*i,top_row + row_spacing * 2), module, MentalMixer::CH_VOL_INPUT + i));
    
    addParam(createParam<Trimpot>(Vec(column_1+column_spacing*i, top_row + row_spacing * 3), module, MentalMixer::PAN_PARAM + i, 0.0, 1.0, 0.5));
    addInput(createInput<PJ301MPort>(Vec(column_1+column_spacing*i, top_row + row_spacing * 4), module, MentalMixer::CH_PAN_INPUT + i));
    
    addParam(createParam<Trimpot>(Vec(column_1+column_spacing*i, top_row + row_spacing * 5), module, MentalMixer::AUX_1_PARAM + i, 0.0, 1.0, 0.5));
    addParam(createParam<Trimpot>(Vec(column_1+column_spacing*i, top_row + row_spacing * 6), module, MentalMixer::AUX_2_PARAM + i, 0.0, 1.0, 0.5));

    addParam(createParam<LEDButton>(Vec(column_1+column_spacing*i,top_row + row_spacing * 7), module, MentalMixer::MUTE_PARAM + i, 0.0, 1.0, 0.0));
	  addChild(createValueLight<SmallLight<GreenValueLight>>(Vec(column_1+column_spacing*i + 5, top_row + row_spacing * 7 + 5), &module->mute_lights[i]));
    addInput(createInput<PJ301MPort>(Vec(column_1+column_spacing*i, top_row + row_spacing * 8), module, MentalMixer::CH_MUTE_INPUT + i));

	}

  // outputs
  addOutput(createOutput<PJ301MPort>(Vec(right_column, 20), module, MentalMixer::MIX_OUTPUT_L));
	addOutput(createOutput<PJ301MPort>(Vec(right_column, 50), module, MentalMixer::MIX_OUTPUT_R));

  addOutput(createOutput<PJ301MPort>(Vec(right_column, 120), module, MentalMixer::SEND_1_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(right_column, 180), module, MentalMixer::SEND_2_OUTPUT));

  addInput(createInput<PJ301MPort>(Vec(right_column, 225), module, MentalMixer::RETURN_1_L_INPUT));
  addInput(createInput<PJ301MPort>(Vec(right_column, 250), module, MentalMixer::RETURN_1_R_INPUT));

  addInput(createInput<PJ301MPort>(Vec(right_column, 295), module, MentalMixer::RETURN_2_L_INPUT));
  addInput(createInput<PJ301MPort>(Vec(right_column, 320), module, MentalMixer::RETURN_2_R_INPUT));


}
