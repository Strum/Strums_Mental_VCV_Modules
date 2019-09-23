///////////////////////////////////////////////////
//
//   Pitch Quantiser VCV Module
//
//   Strum 2017
//
///////////////////////////////////////////////////

#include "mental.hpp"


/////////////////////////////////////////////////
struct MentalQuantiser : Module {
	enum ParamIds {
      PITCH_PARAM,
      BUTTON_PARAM,      
      NUM_PARAMS = BUTTON_PARAM + 12
	};

	enum InputIds {
      INPUT,
      PITCH_INPUT,
      NUM_INPUTS
	};
	enum OutputIds {
      OUTPUT,
      REF_OUT,
      NUM_OUTPUTS = REF_OUT + 12
	};
  enum LightIds {
		BUTTON_LIGHTS,
    OUTPUT_LIGHTS = BUTTON_LIGHTS + 12,
		NUM_LIGHTS = OUTPUT_LIGHTS + 12
	};

  dsp::SchmittTrigger button_triggers[12];
  
  bool button_states[12] = {true,true,true,true,true,true,true,true,true,true,true,true};
  float quantised = 0.0;
  bool found = false;
  int last_found = 0;
   
  MentalQuantiser() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

    configParam(MentalQuantiser::PITCH_PARAM, -6.5, 6.5, 0.0, "");
    for (int i = 0; i < 12; ++i)
    {
      configParam(MentalQuantiser::BUTTON_PARAM + i, 0.0, 1.0, 0.0, ""); 
    }
    
  }
	void process(const ProcessArgs& args) override;
  
  json_t *dataToJson() override
  {
		json_t *rootJ = json_object();
    
    // button states
		json_t *button_statesJ = json_array();
		for (int i = 0; i < 12; i++)
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
			for (int i = 0; i < 12; i++)
      {
				json_t *button_stateJ = json_array_get(button_statesJ, i);
				if (button_stateJ)
					button_states[i] = !!json_integer_value(button_stateJ);
			}
		}  
  }
};


/////////////////////////////////////////////////////
void MentalQuantiser::process(const ProcessArgs& args) {

  ////// handle button presses
  for  (int i = 0 ; i < 12 ; i++)
  {
    if (button_triggers[i].process(params[BUTTON_PARAM+i].getValue()))
    {
		  button_states[i] = !button_states[i];
	  }
    lights[BUTTON_LIGHTS + i ].value  = (button_states[i]) ? 1.0 : 0.0;
    lights[OUTPUT_LIGHTS + i].value = 0.0;
  }

  // pitch offset
  float pitch_in = round(inputs[PITCH_INPUT].getVoltage())/12;
  float root_pitch = (pitch_in * (1/12.0)) + (round(params[PITCH_PARAM].getValue()) * (1/12.0)); 
    
  // set reference outputs
  for  (int i = 0 ; i < 12 ; i++)
  {
    outputs[REF_OUT + i].setVoltage(root_pitch + i * (1/12.0));
  }
  
  //////// quantise pitch to chromatic scale
  float in = inputs[INPUT].getVoltage();  
  int octave = round(in);
  float octaves_removed = in - 1.0*octave;
  int semitone = round(octaves_removed*12);
  if (semitone < 0)
  { 
    semitone +=12;
    octave -= 1;
  }
  quantised = root_pitch + 1.0 * octave + semitone/12.0;
    
  // quantise to scale selected by buttons
  if (button_states[semitone])
  {    
    found = true;    
    outputs[OUTPUT].setVoltage(quantised);
    lights[OUTPUT_LIGHTS + semitone].value  = 1.0;
  }     
}

//////////////////////////////////////////////////////////////////
struct MentalQuantiserWidget : ModuleWidget {
  MentalQuantiserWidget(MentalQuantiser *module){

    setModule(module);

  setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/MentalQuantiser.svg")));

  int top_row = 40;
  int row_spacing = 25; 
	
  addParam(createParam<MedKnob>(Vec(62, 15), module, MentalQuantiser::PITCH_PARAM));
  addInput(createInput<CVInPort>(Vec(63, 45), module, MentalQuantiser::PITCH_INPUT));
  
  addInput(createInput<CVInPort>(Vec(3, top_row), module, MentalQuantiser::INPUT));
  addOutput(createOutput<CVOutPort>(Vec(32, top_row), module, MentalQuantiser::OUTPUT));
  
  for (int i = 0; i < 12 ; i++)
  {  
    addParam(createParam<LEDButton>(Vec(3, top_row + 30 + row_spacing * i), module, MentalQuantiser::BUTTON_PARAM + i));
	  addChild(createLight<MedLight<BlueLED>>(Vec(3+5, top_row + 30 + row_spacing * i + 5), module, MentalQuantiser::BUTTON_LIGHTS + i));
    addChild(createLight<MedLight<BlueLED>>(Vec(30+5, top_row + 30 + row_spacing * i + 5), module, MentalQuantiser::OUTPUT_LIGHTS + i));
    addOutput(createOutput<CVOutPort>(Vec(63, top_row + 40 + row_spacing * i), module, MentalQuantiser::REF_OUT + i));    
  }
  }
};

Model *modelMentalQuantiser = createModel<MentalQuantiser, MentalQuantiserWidget>("MentalQuantiser");