///////////////////////////////////////////////////
//
//   Pitch Quantiser VCV Module
//
//   Strum 2017
//
///////////////////////////////////////////////////

#include "mental.hpp"

#include "dsp/digital.hpp"

/////////////////////////////////////////////////
struct MentalQuantiser : Module {
	enum ParamIds {
      PITCH_PARAM,
      BUTTON_PARAM,      
      NUM_PARAMS  = BUTTON_PARAM + 12
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

  SchmittTrigger button_triggers[12];
  bool button_states[12] = {1};
  float button_lights[12] = {1.0};
  float output_lights[12] = {0.0};
  float quantised = 0.0;
  bool found = 0;
    
  MentalQuantiser() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
	void step();
};


/////////////////////////////////////////////////////
void MentalQuantiser::step() {

  ////// handle button presses
  for  (int i = 0 ; i < 12 ; i++)
  {
    if (button_triggers[i].process(params[BUTTON_PARAM+i].value))
    {
		  button_states[i] = !button_states[i];
	  }
	  button_lights[i] = button_states[i] ? 1.0 : 0.0;
    output_lights[i]= 0;
  }

  //////// quantise pitch to chromatic scale
  
  float in = inputs[INPUT].value;  
  int octave = round(in);
  float octaves_removed = in - 1.0*octave;
  int semitone = round(octaves_removed*12);
    
  // pitch offset
  
  float pitch_in = round(inputs[PITCH_INPUT].value)/12;
  
  float root_pitch = (pitch_in * (1/12.0)) + (round(params[PITCH_PARAM].value) * (1/12.0)); 
    
  // set reference outputs
  for  (int i = 0 ; i < 12 ; i++)
  {
    outputs[REF_OUT + i].value = root_pitch + i * (1/12.0);
  }
  
  // quantise to scale selected by buttons
  if ( button_states[semitone] )
  {    
    found = 1;
  }
  else
  {
    for (int i = semitone - 1 ; i < 12 ; i++)
    {
      semitone++;
      if (semitone == 12) semitone = 0;
      if (button_states[semitone])
      {
        found = 1;
        break;
      }                       
    }     
  }
  if (found)
  {
    quantised = root_pitch + 1.0 * octave + semitone/12.0;
    outputs[OUTPUT].value = quantised;
    output_lights[semitone] = 1.0;
  }
     
}

//////////////////////////////////////////////////////////////////
MentalQuantiserWidget::MentalQuantiserWidget() {
	MentalQuantiser *module = new MentalQuantiser();
	setModule(module);
	box.size = Vec(15*6, 380);

  {
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		//panel->setBackground(SVG::load("plugins/mental/res/MentalQuantiser.svg"));
    panel->setBackground(SVG::load(assetPlugin(plugin,"res/MentalQuantiser.svg")));
		addChild(panel);
	}
  int top_row = 75;
  int row_spacing = 25; 
	
  addParam(createParam<Davies1900hSmallBlackKnob>(Vec(62, 20), module, MentalQuantiser::PITCH_PARAM, -6.5, 6.5, 0.0));
  addInput(createInput<PJ301MPort>(Vec(63, 50), module, MentalQuantiser::PITCH_INPUT));
  
  addInput(createInput<PJ301MPort>(Vec(3, top_row), module, MentalQuantiser::INPUT));
  addOutput(createOutput<PJ301MPort>(Vec(32, top_row), module, MentalQuantiser::OUTPUT));
  
  for (int i = 0; i < 12 ; i++)
  {  
    addParam(createParam<LEDButton>(Vec(3, top_row + 30 + row_spacing * i), module, MentalQuantiser::BUTTON_PARAM + i, 0.0, 1.0, 0.0));
	  addChild(createValueLight<SmallLight<GreenValueLight>>(Vec(3+5, top_row + 30 + row_spacing * i + 5), &module->button_lights[i]));
    addChild(createValueLight<SmallLight<GreenValueLight>>(Vec(30+5, top_row + 30 + row_spacing * i + 5), &module->output_lights[i]));
    addOutput(createOutput<PJ301MPort>(Vec(63, top_row + 30 + row_spacing * i), module, MentalQuantiser::REF_OUT + i));    
  }
}
