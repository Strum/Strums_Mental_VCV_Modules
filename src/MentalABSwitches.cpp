///////////////////////////////////////////////////
//
//   Mental Plugin
//   A/B Switches
//
//   Strum 2017-19
//   strum@softhome.net
//
///////////////////////////////////////////////////

#include "mental.hpp"

/////////////////////////////////////////////////
struct MentalABSwitches : Module
{
	enum ParamIds
  {
      BUTTON_PARAM,
      NUM_PARAMS = BUTTON_PARAM + 4
	};
	enum InputIds
  {
      INPUT,
      SEL_INPUT = INPUT + 4,
      NUM_INPUTS = SEL_INPUT + 4
	};
	enum OutputIds
  {
      OUTPUT_A,
      OUTPUT_B = OUTPUT_A + 4,
      NUM_OUTPUTS = OUTPUT_B + 4
	};
  enum LightIds
  {
		BUTTON_LIGHTS,
    A_LEDS = BUTTON_LIGHTS + 4,
    B_LEDS = A_LEDS + 4,
		NUM_LIGHTS = B_LEDS + 4
	};

  dsp::SchmittTrigger button_triggers[4];
  bool button_on[4] = {0,0,0,0};
      
	MentalABSwitches()
  {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    for (int i = 0; i < 4; ++i)
    {    
      configParam(MentalABSwitches::BUTTON_PARAM + i, 0.0, 1.0, 0.0, "");
    }
  }

	void process(const ProcessArgs& args) override;
  
  json_t *dataToJson() override
  {
		json_t *rootJ = json_object();
    
    // button states
		json_t *button_statesJ = json_array();
		for (int i = 0; i < 4; i++)
    {
			json_t *button_stateJ = json_integer((int) button_on[i]);
			json_array_append_new(button_statesJ, button_stateJ);
		}
		json_object_set_new(rootJ, "buttons", button_statesJ);    
    return rootJ;
  }
  
  void dataFromJson(json_t *rootJ) override
  {
    // button states
		json_t *button_statesJ = json_object_get(rootJ, "buttons");
		if (button_statesJ)
    {
			for (int i = 0; i < 4; i++)
      {
				json_t *button_stateJ = json_array_get(button_statesJ, i);
				if (button_stateJ)
					button_on[i] = !!json_integer_value(button_stateJ);
			}
		}  
  }
};

/////////////////////////////////////////////////////
void MentalABSwitches::process(const ProcessArgs& args)
{
  for (int i = 0 ; i < 4 ; i++)
  {
    float signal = inputs[INPUT + i].getVoltage();
    float sel = inputs[SEL_INPUT + i].getVoltage();
  
    if (button_triggers[i].process(params[BUTTON_PARAM + i].getValue()))
    {
	    button_on[i] = !button_on[i];
    }
    if (button_on[i] || ( sel > 0.0))
    {
      outputs[OUTPUT_A + i].setVoltage(0.0);
      outputs[OUTPUT_B + i].setVoltage(signal);
      
      lights[B_LEDS + i].value = 1.0;
      lights[A_LEDS + i].value = 0.0;
    }
    else
    {
      outputs[OUTPUT_A + i].setVoltage(signal);
      outputs[OUTPUT_B + i].setVoltage(0.0);
      
      lights[B_LEDS + i].value = 0.0;
      lights[A_LEDS + i].value = 1.0;
    }
  }
}

//////////////////////////////////////////////////////////////////
struct MentalABSwitchesWidget : ModuleWidget
{
  MentalABSwitchesWidget(MentalABSwitches *module)
  {

    setModule(module);
    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/MentalABSwitches.svg")));

    int group_spacing = 85;
  
    for (int i = 0 ; i < 4 ; i++)
    {
  	  addInput(createInput<InPort>(Vec(3, group_spacing * i + 25), module, MentalABSwitches::INPUT + i));
      addInput(createInput<GateInPort>(Vec(3, group_spacing * i + 75), module, MentalABSwitches::SEL_INPUT + i));
    
      addOutput(createOutput<OutPort>(Vec(33, group_spacing * i + 25), module, MentalABSwitches::OUTPUT_A + i));
      addOutput(createOutput<OutPort>(Vec(33, group_spacing * i + 50), module, MentalABSwitches::OUTPUT_B + i));

      addChild(createLight<MedLight<BlueLED>>(Vec(62, group_spacing * i + 34), module, MentalABSwitches::A_LEDS + i ));
      addChild(createLight<MedLight<BlueLED>>(Vec(62, group_spacing * i + 59), module, MentalABSwitches::B_LEDS + i));
    
      addParam(createParam<LEDButton>(Vec(6, group_spacing * i + 54), module, MentalABSwitches::BUTTON_PARAM + i));
  	  addChild(createLight<MedLight<BlueLED>>(Vec(6+5, group_spacing * i + 54+5), module, MentalABSwitches::BUTTON_LIGHTS + i));
    }
  }
};

Model *modelMentalABSwitches = createModel<MentalABSwitches, MentalABSwitchesWidget>("MentalABSwitches");