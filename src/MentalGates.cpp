///////////////////////////////////////////////////
//
//   Gates VCV Module
//
//   Strum 2017
//
///////////////////////////////////////////////////

#include "mental.hpp"
/////////////////////////////////////////////////

struct MentalGates : Module {
	enum ParamIds {
      BUTTON_PARAM,
      NUM_PARAMS = BUTTON_PARAM + 4
	};

	enum InputIds {
      INPUT,
      GATE_INPUT = INPUT + 4,
      NUM_INPUTS = GATE_INPUT + 4
	};
	enum OutputIds {
      OUTPUT,
      NUM_OUTPUTS = OUTPUT + 4
	};

  SchmittTrigger button_triggers[4];
  bool button_on[4] = {0,0,0,0};
  float button_lights[4] = {0.0,0.0,0.0};
  float on_leds[4] = {0.0,0.0,0.0};
  float signal[4] = {0.0,0.0,0.0};
  float on[4] = {0.0,0.0,0.0};
  
	MentalGates() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
	void step();
};

/////////////////////////////////////////////////////

void MentalGates::step() {

for (int i = 0 ; i < 4 ; i++)
  {
    signal[i] = inputs[INPUT + i].value;
    on[i] = inputs[GATE_INPUT + i].value;
  
    if (button_triggers[i].process(params[BUTTON_PARAM + i].value))
    {
	    button_on[i] = !button_on[i];
    }
    button_lights[i] = button_on[i] ? 1.0 : 0.0;

    if (button_on[i] || ( on[i] > 0.0))
    {
      outputs[OUTPUT + i].value = 0.0;
      on_leds[i] = 1.0;
    }
    else
    {
      outputs[OUTPUT + i].value = signal[i];
      on_leds[i] = 0.0;
    }
  }
}

//////////////////////////////////////////////////////////////////
MentalGatesWidget::MentalGatesWidget() {
	MentalGates *module = new MentalGates();
	setModule(module);
	box.size = Vec(15*4, 380);

  {
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load("plugins/mental/res/MentalGates.svg"));
		addChild(panel);
	}
  int group_spacing = 85;
  for (int i = 0 ; i < 4 ; i++)
  {
	  addInput(createInput<PJ301MPort>(Vec(3, group_spacing * i +  60), module, MentalGates::INPUT + i));
    addInput(createInput<PJ301MPort>(Vec(3, group_spacing * i +  28), module, MentalGates::GATE_INPUT + i));
    addOutput(createOutput<PJ301MPort>(Vec(32, group_spacing * i +  60), module, MentalGates::OUTPUT + i));


    addChild(createValueLight<SmallLight<GreenValueLight>>(Vec(26, group_spacing * i + 17), &module->on_leds[i]));
    addParam(createParam<LEDButton>(Vec(35, group_spacing * i +  31), module, MentalGates::BUTTON_PARAM + i, 0.0, 1.0, 0.0));
	  addChild(createValueLight<SmallLight<GreenValueLight>>(Vec(35+5, group_spacing * i +  31+5), &module->button_lights[i]));
  
  }
}
