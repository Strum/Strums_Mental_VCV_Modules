///////////////////////////////////////////////////
//
//   Mental Plugin
//   A/B Switches
//
//   Strum 2017-21
//   strum@softhome.net
//
///////////////////////////////////////////////////

#include "mental.hpp"

/////////////////////////////////////////////////
struct ABSwitches : Module
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

	ABSwitches()
  {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    for (int i = 0; i < 4; ++i)
    {
      configParam(ABSwitches::BUTTON_PARAM + i, 0.0, 1.0, 0.0, "");
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
void ABSwitches::process(const ProcessArgs& args)
{
  for (int i = 0 ; i < 4 ; i++) // loop through switches
  {
    if (button_triggers[i].process(params[BUTTON_PARAM + i].getValue()))
    {
      button_on[i] = !button_on[i];
    }
    if (inputs[INPUT + i].isConnected())
    {
      float signal = inputs[INPUT + i].getVoltage();

      if (inputs[SEL_INPUT + i].isConnected())
      {
        //float sel = inputs[SEL_INPUT + i].getVoltage();
        if (inputs[SEL_INPUT + i].getVoltage() > 0.0)
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
      else
      {
        if (button_on[i])
        {
          outputs[OUTPUT_A + i].setVoltage(0.0);
          outputs[OUTPUT_B + i].setVoltage(signal);

          lights[B_LEDS + i].value = 1.0;
          lights[A_LEDS + i].value = 0.0;
          lights[BUTTON_LIGHTS + i].value = 1.0;
        }
        else
        {
          outputs[OUTPUT_A + i].setVoltage(signal);
          outputs[OUTPUT_B + i].setVoltage(0.0);

          lights[B_LEDS + i].value = 0.0;
          lights[A_LEDS + i].value = 1.0;
          lights[BUTTON_LIGHTS + i].value = 0.0;
        }
      }
    }
  }
}

//////////////////////////////////////////////////////////////////
struct ABSwitchesWidget : ModuleWidget
{
  ABSwitchesWidget(ABSwitches *module)
  {

    setModule(module);
    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/ABSwitches.svg")));

    int group_spacing = 85;

    for (int i = 0 ; i < 4 ; i++)
    {
  	  addInput(createInput<InPort>(Vec(3, group_spacing * i + 25), module, ABSwitches::INPUT + i));
      addInput(createInput<GateInPort>(Vec(3, group_spacing * i + 75), module, ABSwitches::SEL_INPUT + i));

      addOutput(createOutput<OutPort>(Vec(33, group_spacing * i + 25), module, ABSwitches::OUTPUT_A + i));
      addOutput(createOutput<OutPort>(Vec(33, group_spacing * i + 50), module, ABSwitches::OUTPUT_B + i));

      addChild(createLight<MedLight<BlueLED>>(Vec(62, group_spacing * i + 34), module, ABSwitches::A_LEDS + i ));
      addChild(createLight<MedLight<BlueLED>>(Vec(62, group_spacing * i + 59), module, ABSwitches::B_LEDS + i));

      addParam(createParam<LEDButton>(Vec(6, group_spacing * i + 54), module, ABSwitches::BUTTON_PARAM + i));
  	  addChild(createLight<MedLight<BlueLED>>(Vec(6+5, group_spacing * i + 54+5), module, ABSwitches::BUTTON_LIGHTS + i));
    }
  }
};

Model *modelABSwitches = createModel<ABSwitches, ABSwitchesWidget>("ABSwitches");
