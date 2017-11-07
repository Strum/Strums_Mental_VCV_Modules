///////////////////////////////////////////////////
//
//   8 way switch with Binary Decoder selector VCV Module
//
//   Strum 2017
//
///////////////////////////////////////////////////

#include "mental.hpp"

struct MentalSwitch8 : Module {
	enum ParamIds {
    NUM_PARAMS
	};  
	enum InputIds {
    SIG_INPUT,
    INPUT_1,
    INPUT_2,
    INPUT_4,		  
		NUM_INPUTS
	};
	enum OutputIds {
    OUTPUT,    
		NUM_OUTPUTS = OUTPUT + 8
	};
  enum LightIds {
    OUTPUT_LEDS,
    NUM_LIGHTS = OUTPUT_LEDS + 8
  };
  
  //float input_leds[3] = {0.0,0.0,0.0};  
  //float output_leds[8] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
  float in_1 = 0.0;
  float in_2 = 0.0;
  float in_4 = 0.0;
  
  int one, two, four, decoded;
  
	MentalSwitch8() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
	void step() override;  
};

void MentalSwitch8::step()
{
  for ( int i = 0 ; i < 8 ; i ++)
  {
    //output_leds[i] = 0.0;
    lights[OUTPUT_LEDS + i].value = 0.0;
    outputs[OUTPUT + i].value = 0.0;
  }
  
  in_1 = inputs[INPUT_1].value;
  in_2 = inputs[INPUT_2].value;
  in_4 = inputs[INPUT_4].value;
  
  if (in_1 > 0.0 ) 
  {
    one = 1;
    //input_leds[0] = 1.0;
  } else
  {
    one = 0;
    //input_leds[0] = 0.0;
  }
  if (in_2 > 0.0) 
  {
    two = 2;
    //input_leds[1] = 1.0;
  } else
  {
    two = 0;
    //input_leds[1] = 0.0;
  }
  if (in_4 > 0.0) 
  {
    four = 4;
    //input_leds[2] = 1.0;
  } else
  {
    four = 0;
    //input_leds[2] = 0.0;    
  }
  
  decoded = one + two + four;  
  outputs[OUTPUT + decoded].value = inputs[SIG_INPUT].value;
  //output_leds[decoded] = 1.0;  
  lights[OUTPUT_LEDS + decoded].value = 1.0;
}

MentalSwitch8Widget::MentalSwitch8Widget() {
	MentalSwitch8 *module = new MentalSwitch8();
	setModule(module);
	box.size = Vec(15*5, 380);
  
	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin,"res/MentalSwitch8.svg")));
		addChild(panel);
	}
	
  int spacing = 25; 
  int top_space = 15;
  
  addInput(createInput<PJ301MPort>(Vec(3, top_space), module, MentalSwitch8::INPUT_1));
  addInput(createInput<PJ301MPort>(Vec(3, top_space + spacing), module, MentalSwitch8::INPUT_2));
  addInput(createInput<PJ301MPort>(Vec(3, top_space + spacing * 2), module, MentalSwitch8::INPUT_4));
  
  addInput(createInput<PJ301MPort>(Vec(3, top_space + spacing * 3 + 15), module, MentalSwitch8::SIG_INPUT));
  
  for (int i = 0; i < 8 ; i++)
  {  
   addOutput(createOutput<PJ301MPort>(Vec(30, top_space + spacing * i), module, MentalSwitch8::OUTPUT + i));   	 
   addChild(createLight<MediumLight<GreenLight>>(Vec(60, top_space +  spacing * i + 8), module,MentalSwitch8::OUTPUT_LEDS + i));
  }
  
}
