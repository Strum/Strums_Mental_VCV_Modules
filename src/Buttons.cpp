///////////////////////////////////////////////////
//
//   Mental Plugin
//   Buttons
//
//   Strum 2017-21
//   strum@softhome.net
//
///////////////////////////////////////////////////

#include "mental.hpp"

struct Buttons : Module
{
	enum ParamIds
  {
    MOMENT,
    BUTTON_PARAM = MOMENT + 7,
		NUM_PARAMS = BUTTON_PARAM + 7
	};
	enum InputIds
  {
		NUM_INPUTS
	};
	enum OutputIds
  {
    MOMENT_OUT,
		OUTPUT = MOMENT_OUT +7,
		NUM_OUTPUTS = OUTPUT + 7
	};
  enum LightIds
  {
		BUTTON_LEDS,
    MOMENT_LEDS = BUTTON_LEDS + 7,
		NUM_LIGHTS = MOMENT_LEDS + 7
	};

  dsp::SchmittTrigger button_triggers[7];
  bool button_states[7] = {0,0,0,0,0,0,0};

	Buttons()
  {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    for (int i = 0; i < 7; ++i)
    {
      configParam(Buttons::BUTTON_PARAM +i, 0.0, 1.0, 0.0, "");
      configParam(Buttons::MOMENT + i, 0.0, 1.0, 0.0, "");
    }

  }

	void process(const ProcessArgs& args) override;

  json_t *dataToJson() override
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
    return rootJ;
  }

  void dataFromJson(json_t *rootJ) override
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
  }
};

void Buttons::process(const ProcessArgs& args)
{
  for  (int i = 0 ; i < 7 ; i++)
  {
    if (button_triggers[i].process(params[BUTTON_PARAM + i].getValue()))
    {
		  button_states[i] = !button_states[i];
	  }
    lights[BUTTON_LEDS + i ].value  = (button_states[i]) ? 1.0 : 0.0;
    outputs[OUTPUT + i].setVoltage(button_states[i] * 10.0);
    if (params[MOMENT + i].getValue() > 0.0)
    {
      lights[MOMENT_LEDS + i ].value  = 1.0;
      outputs[MOMENT_OUT + i].setVoltage(10.0);
	  }
    else
    {
      lights[MOMENT_LEDS + i ].value  = 0.0;
      outputs[MOMENT_OUT + i].setVoltage(0.0);
	  }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
struct ButtonsWidget : ModuleWidget
{
  ButtonsWidget(Buttons *module)
  {
    setModule(module);

    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Buttons.svg")));

    int spacing = 25;
    int group_offset = 184;
    int top_space = 15;
    for (int i = 0; i < 7 ; i++)
    {
      addOutput(createOutput<GateOutPort>(Vec(33, top_space + spacing * i), module, Buttons::OUTPUT + i));
      addParam(createParam<LEDButton>(Vec(5, top_space + 3 + spacing * i), module, Buttons::BUTTON_PARAM +i));
      addChild(createLight<MedLight<BlueLED>>(Vec(10, top_space + 8 + spacing * i), module, Buttons::BUTTON_LEDS + i));

  	  /// momentarys
     addOutput(createOutput<GateOutPort>(Vec(33, 10 + group_offset +  spacing * i), module, Buttons::MOMENT_OUT + i));
     addParam(createParam<LEDButton>(Vec(5, 10 + 3 + group_offset +  spacing * i), module, Buttons::MOMENT + i));
     addChild(createLight<MedLight<BlueLED>>(Vec(10,10 + 8 + group_offset +  spacing * i), module, Buttons::MOMENT_LEDS + i));
    }
  }
};

Model *modelButtons = createModel<Buttons, ButtonsWidget>("Buttons");
