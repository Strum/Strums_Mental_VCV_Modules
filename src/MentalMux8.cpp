///////////////////////////////////////////////////
//
//   8 to 1 Mux with Binary Decoder selector VCV Module
//
//   Strum 2017
//
///////////////////////////////////////////////////

#include "mental.hpp"

struct MentalMux8 : Module {
	enum ParamIds {
    NUM_PARAMS
	};  
	enum InputIds {
    INPUT_1,
    INPUT_2,
    INPUT_4,
    SIG_INPUT,		  
		NUM_INPUTS = SIG_INPUT + 8
	};
	enum OutputIds {
    OUTPUT,    
		NUM_OUTPUTS
	};
  enum LightIds {
    INPUT_LEDS,
    NUM_LIGHTS = INPUT_LEDS + 8
  };
  
  //float input_leds[3] = {0.0,0.0,0.0};  
  //float input_leds[8] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
  float in_1 = 0.0;
  float in_2 = 0.0;
  float in_4 = 0.0;
  
  int one, two, four, decoded;
  
	MentalMux8() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
	void step() override;  
};

void MentalMux8::step()
{
  for ( int i = 0 ; i < 8 ; i ++)
  {
    //input_leds[i] = 0.0; 
    lights[INPUT_LEDS + i].value = 0.0;   
  }
  outputs[OUTPUT].value = 0.0;
  
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
  outputs[OUTPUT].value = inputs[SIG_INPUT + decoded].value;
  //input_leds[decoded] = 1.0; 
  lights[INPUT_LEDS + decoded].value = 1.0; 
}

MentalMux8Widget::MentalMux8Widget() {
	MentalMux8 *module = new MentalMux8();
	setModule(module);
	box.size = Vec(15*5, 380);
  
	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin,"res/MentalMux8.svg")));
		addChild(panel);
	}
	
  int spacing = 25; 
  int top_space = 15;
  
  addInput(createInput<PJ301MPort>(Vec(3, top_space), module, MentalMux8::INPUT_1));
  addInput(createInput<PJ301MPort>(Vec(3, top_space + spacing), module, MentalMux8::INPUT_2));
  addInput(createInput<PJ301MPort>(Vec(3, top_space + spacing * 2), module, MentalMux8::INPUT_4));
  
  for (int i = 0; i < 8 ; i++)
  {  
   addInput(createInput<PJ301MPort>(Vec(3, top_space + spacing * i + 100), module, MentalMux8::SIG_INPUT + i));   	 
   addChild(createLight<MediumLight<GreenLight>>(Vec(33, top_space +  spacing * i + 8 + 100), module, MentalMux8::INPUT_LEDS + i));
  }
  
  addOutput(createOutput<PJ301MPort>(Vec(30, top_space + spacing), module, MentalMux8::OUTPUT));
  
  
  
}
