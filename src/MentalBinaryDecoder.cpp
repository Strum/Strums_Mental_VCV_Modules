///////////////////////////////////////////////////
//
//   Mental Plugin
//   Binary Decoder
//
//   Strum 2017-22
//   strum@sodaisland.net
//
///////////////////////////////////////////////////

#include "mental.hpp"

struct MentalBinaryDecoder : Module
{
	enum ParamIds
  {
    NUM_PARAMS
	};  
	enum InputIds
  {
    INPUT_1,
    INPUT_2,
    INPUT_4,		  
		NUM_INPUTS
	};
	enum OutputIds
  {
    OUTPUT,    
		NUM_OUTPUTS = OUTPUT + 8
	};
  enum LightIds
  {
		OUTPUT_LEDS,
		NUM_LIGHTS = OUTPUT_LEDS + 8
	};
    
  float in_1 = 0.0;
  float in_2 = 0.0;
  float in_4 = 0.0;
  
  int one, two, four, last_led, decoded;
  
	MentalBinaryDecoder()
  {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
  }

	void process(const ProcessArgs& args) override;  
};

void MentalBinaryDecoder::process(const ProcessArgs& args)
{
  for ( int i = 0 ; i < 8 ; i ++)
  {    
    lights[OUTPUT_LEDS + i].value = 0.0;
    outputs[OUTPUT + i].setVoltage(0.0);
  }
  
  in_1 = inputs[INPUT_1].getVoltage();
  in_2 = inputs[INPUT_2].getVoltage();
  in_4 = inputs[INPUT_4].getVoltage();
  
  if (in_1 > 0.0 ) 
  {
    one = 1;
  } else
  {
    one = 0;
  }
  if (in_2 > 0.0) 
  {
    two = 2;
  } else
  {
    two = 0;
  }
  if (in_4 > 0.0) 
  {
    four = 4;
  } else
  {
    four = 0;
  }
  
  decoded = one + two + four;  
  outputs[OUTPUT + decoded].setVoltage(10.0);
  lights[OUTPUT_LEDS + decoded].value = 1.0;  
}

//////////////////////////////////////////////////////////////////////////////
struct MentalBinaryDecoderWidget : ModuleWidget
{
  MentalBinaryDecoderWidget(MentalBinaryDecoder *module)
  {
  	setModule(module);
    
    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/MentalBinaryDecoder.svg")));

    int spacing = 25; 
    int top_space = 15;
    
    addInput(createInput<GateInPort>(Vec(3, top_space), module, MentalBinaryDecoder::INPUT_1));
    addInput(createInput<GateInPort>(Vec(3, top_space + spacing), module, MentalBinaryDecoder::INPUT_2));
    addInput(createInput<GateInPort>(Vec(3, top_space + spacing * 2), module, MentalBinaryDecoder::INPUT_4));
    
    for (int i = 0; i < 8 ; i++)
    {  
     addOutput(createOutput<GateOutPort>(Vec(30, top_space + spacing * i), module, MentalBinaryDecoder::OUTPUT + i));   	 
     addChild(createLight<MedLight<BlueLED>>(Vec(60, top_space +  spacing * i + 8), module, MentalBinaryDecoder::OUTPUT_LEDS + i));
    }  
  }
};

Model *modelMentalBinaryDecoder = createModel<MentalBinaryDecoder, MentalBinaryDecoderWidget>("MentalBinaryDecoder");