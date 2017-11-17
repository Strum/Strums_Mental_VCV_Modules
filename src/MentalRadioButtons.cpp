///////////////////////////////////////////////////
//
//   Radio Buttons VCV Module
//
//   Strum 2017
//
///////////////////////////////////////////////////

#include "mental.hpp"

#include "dsp/digital.hpp"


struct MentalRadioButtons : Module {
	enum ParamIds {
    BUTTON2,
    BUTTON_PARAM = BUTTON2 + 7,
		NUM_PARAMS = BUTTON_PARAM + 7
	};  
	enum InputIds {
    INS1,
    INS2 = INS1 +	7,	  
		NUM_INPUTS = INS2 + 7
	};
	enum OutputIds {
    BUTTON2_OUT,
		OUTPUT = BUTTON2_OUT +7,    
		NUM_OUTPUTS = OUTPUT + 7
	};
  enum LightIds {
		BUTTON_LEDS,
    BUTTON2_LEDS = BUTTON_LEDS + 7,
		NUM_LIGHTS = BUTTON2_LEDS + 7
	};

  SchmittTrigger button_triggers[7];
  SchmittTrigger button2_triggers[7];
  //SchmittTrigger momentary_trigger;
  //float button_leds[7] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};  
  bool button_states[7] = {1,0,0,0,0,0,0};
  bool button2_states[7] = {1,0,0,0,0,0,0};
  //float moment_leds[7] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};
  
	MentalRadioButtons() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
	void step() override;
  
  json_t *toJson() override
  {
		json_t *rootJ = json_object();
    
    // button states
		json_t *button_statesJ = json_array();
		for (int i = 0; i < 7; i++)
    {
			json_t *button_stateJ = json_integer((int) button_states[i]);
			json_array_append_new(button_statesJ, button_stateJ);
		}
		json_object_set_new(rootJ, "buttons", button_statesJ);
    
    json_t *button2_statesJ = json_array();
		for (int i = 0; i < 7; i++)
    {
			json_t *button2_stateJ = json_integer((int) button2_states[i]);
			json_array_append_new(button2_statesJ, button2_stateJ);
		}
		json_object_set_new(rootJ, "buttons2", button2_statesJ);     
    return rootJ;
  }
  
  void fromJson(json_t *rootJ) override
  {
    // button states
		json_t *button_statesJ = json_object_get(rootJ, "buttons");
		if (button_statesJ)
    {
			for (int i = 0; i < 7; i++)
      {
				json_t *button_stateJ = json_array_get(button_statesJ, i);
				if (button_stateJ)
					button_states[i] = !!json_integer_value(button_stateJ);
			}
		}
    json_t *button2_statesJ = json_object_get(rootJ, "buttons2");
		if (button2_statesJ)
    {
			for (int i = 0; i < 7; i++)
      {
				json_t *button2_stateJ = json_array_get(button2_statesJ, i);
				if (button2_stateJ)
					button2_states[i] = !!json_integer_value(button2_stateJ);
			}
		}    
  }
};

void MentalRadioButtons::step()
{
  for  (int i = 0 ; i < 7 ; i++)
  {
    if (button_triggers[i].process(params[BUTTON_PARAM + i].value) || button_triggers[i].process(inputs[INS1 + i].value))
    {
      for (int j = 0 ; j < 7 ; j ++)
      {
        button_states[j] = false ;
      }
		  button_states[i] = !button_states[i];      
	  }
    //button_leds[i] = button_states[i] ? 1.0 : 0.0;
    lights[BUTTON_LEDS + i ].value  = (button_states[i]) ? 1.0 : 0.0;
    outputs[OUTPUT + i].value = button_states[i] * 10.0;
    
    if (button2_triggers[i].process(params[BUTTON2 + i].value) || button_triggers[i].process(inputs[INS2 + i].value))
    {
      for (int j = 0 ; j < 7 ; j ++)
      {        
        button2_states[j] = false ;
      }
		  button2_states[i] = !button2_states[i];      
	  }
    //button_leds[i] = button_states[i] ? 1.0 : 0.0;
    lights[BUTTON2_LEDS + i ].value  = (button2_states[i]) ? 1.0 : 0.0;
    outputs[BUTTON2_OUT + i].value = button2_states[i] * 10.0;    
  }
  
}

MentalRadioButtonsWidget::MentalRadioButtonsWidget() {
	MentalRadioButtons *module = new MentalRadioButtons();
	setModule(module);
	box.size = Vec(15*6, 380);
  
	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		//panel->setBackground(SVG::load("plugins/mental/res/MentalButtons.svg"));
    panel->setBackground(SVG::load(assetPlugin(plugin,"res/MentalRadioButtons.svg")));
		addChild(panel);
	}
	
  int spacing = 25; 
  int group_offset = 190;
  int top_space = 15;
  for (int i = 0; i < 7 ; i++)
  {  
    addInput(createInput<PJ301MPort>(Vec(3, top_space + spacing * i), module, MentalRadioButtons::INS1 + i));
    addOutput(createOutput<PJ301MPort>(Vec(63, top_space + spacing * i), module, MentalRadioButtons::OUTPUT + i));
    addParam(createParam<LEDButton>(Vec(35, top_space + 3 + spacing * i), module, MentalRadioButtons::BUTTON_PARAM +i, 0.0, 1.0, 0.0));
    addChild(createLight<MediumLight<GreenLight>>(Vec(40, top_space + 8 + spacing * i), module, MentalRadioButtons::BUTTON_LEDS + i));
  
	  /// group 2
   addInput(createInput<PJ301MPort>(Vec(3, 10 + group_offset + spacing * i), module, MentalRadioButtons::INS2 + i)); 
   addOutput(createOutput<PJ301MPort>(Vec(63, 10 + group_offset +  spacing * i), module, MentalRadioButtons::BUTTON2_OUT + i));
   addParam(createParam<LEDButton>(Vec(35, 10 + 3 + group_offset +  spacing * i), module, MentalRadioButtons::BUTTON2 + i, 0.0, 1.0, 0.0));
   addChild(createLight<MediumLight<GreenLight>>(Vec(40,10 + 8 + group_offset +  spacing * i), module, MentalRadioButtons::BUTTON2_LEDS + i));
  }
  
}
