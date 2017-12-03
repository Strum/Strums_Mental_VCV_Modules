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
		CH_VOL_PARAM,
		CH_PAN_PARAM = CH_VOL_PARAM + 4,
		NUM_PARAMS = CH_PAN_PARAM + 4
	};

	enum InputIds {
		MIX_CV_INPUT,
		CH_INPUT,
		CH_VOL_INPUT = CH_INPUT + 4,
		CH_PAN_INPUT = CH_VOL_INPUT + 4,
		NUM_INPUTS = CH_PAN_INPUT + 4
	};

	enum OutputIds {
		MIX_OUTPUT_L, MIX_OUTPUT_R,
		CH_OUTPUT,
		NUM_OUTPUTS = CH_OUTPUT + 4
	};

	float channel_ins[4];
	float pan_cv_ins[4];
	float pan_positions[4]; 
	float channel_outs_l[4];
	float channel_outs_r[4];
	float left_sum = 0.0;
	float right_sum = 0.0;
	
	MentalSubMixer() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {};
	void step() override;
};


void MentalSubMixer::step() {
	left_sum = 0.0;
	right_sum = 0.0;

	for (int i = 0 ; i < 4 ; i++)	{

		channel_ins[i] = inputs[CH_INPUT + i].value * params[CH_VOL_PARAM + i].value * clampf(inputs[CH_VOL_INPUT + i].normalize(10.0) / 10.0, 0.0, 1.0);
		
    	pan_cv_ins[i] = inputs[CH_PAN_INPUT + i].value/5;
    	pan_positions[i] = pan_cv_ins[i] + params[CH_PAN_PARAM + i].value;   
    	if (pan_positions[i] < 0) pan_positions[i] = 0;
    	if (pan_positions[i] > 1) pan_positions[i] = 1;
    	channel_outs_l[i]= channel_ins[i] * (1-pan_positions[i])* 2;
    	channel_outs_r[i]= channel_ins[i] * pan_positions[i] * 2;      
        
    	left_sum += channel_outs_l[i];
    	right_sum += channel_outs_r[i];
    }
    float mix_l = left_sum * params[MIX_PARAM].value * clampf(inputs[MIX_CV_INPUT].normalize(10.0) / 10.0, 0.0, 1.0);
    float mix_r = right_sum * params[MIX_PARAM].value * clampf(inputs[MIX_CV_INPUT].normalize(10.0) / 10.0, 0.0, 1.0);

    outputs[MIX_OUTPUT_L].value = mix_l;
    outputs[MIX_OUTPUT_R].value = mix_r;

   	outputs[CH_OUTPUT ].value = channel_ins[0];
	outputs[CH_OUTPUT + 1].value = channel_ins[1];
	outputs[CH_OUTPUT + 2].value = channel_ins[2];
	outputs[CH_OUTPUT + 3].value = channel_ins[3];
}


MentalSubMixerWidget::MentalSubMixerWidget() {
	MentalSubMixer *module = new MentalSubMixer();
	setModule(module);
	box.size = Vec(15*8, 380);
	int stripwidth = 28;
	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		//panel->setBackground(SVG::load("plugins/mental/res/MentalSubMixer.svg"));
	    panel->setBackground(SVG::load(assetPlugin(plugin,"res/MentalSubMixer.svg")));
		addChild(panel);
	}
	// master section
  	addOutput(createOutput<OutPort>(Vec( 6 + stripwidth, 20), module, MentalSubMixer::MIX_OUTPUT_L));
	addOutput(createOutput<OutPort>(Vec( 6 + stripwidth * 2 , 20), module, MentalSubMixer::MIX_OUTPUT_R));
	addParam(createParam<SynthTechAlco>(Vec( 9 + stripwidth , 50), module, MentalSubMixer::MIX_PARAM, 0.0, 1.0, 0.5));
	addInput(createInput<CVInPort>(Vec( 6 + stripwidth * 1.5 , 100), module, MentalSubMixer::MIX_CV_INPUT));
	// channel strips
	for (int i = 0 ; i < 4 ; i++)	{
		// input
		addInput(createInput<InPort>(Vec( 6 + stripwidth * i , box.size.y - 182 ), module, MentalSubMixer::CH_INPUT + i));
		// gain
		addParam(createParam<Trimpot>(Vec( 9 + stripwidth * i , box.size.y - 148 ), module, MentalSubMixer::CH_VOL_PARAM + i, 0.0, 1.0, 0.0));
		addInput(createInput<CVInPort>(Vec( 6 + stripwidth * i , box.size.y - 126 ), module, MentalSubMixer::CH_VOL_INPUT + i));
		// pan
		addParam(createParam<Trimpot>(Vec( 9 + stripwidth * i , box.size.y - 92 ), module, MentalSubMixer::CH_PAN_PARAM + i, 0.0, 1.0, 0.5));
		addInput(createInput<CVInPort>(Vec( 6 + stripwidth * i , box.size.y - 70 ), module, MentalSubMixer::CH_PAN_INPUT + i));
		// output
		addOutput(createOutput<OutPort>(Vec( 6 + stripwidth * i , box.size.y - 36 ), module, MentalSubMixer::CH_OUTPUT + i));
	}
}
