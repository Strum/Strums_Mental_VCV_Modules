///////////////////////////////////////////////////
//
//   Mental Plugin
//   Knobs - outputs constants
//
//   Strum 2017-22
//   strum@sodaisland.net
//
///////////////////////////////////////////////////

#include "mental.hpp"

#include <sstream>
#include <iomanip>

struct MentalKnobs : Module {
	enum ParamIds {
    STEP_SWITCH,
    BI_SWITCH = STEP_SWITCH + 3,
    STEPSIZE_SWITCH = BI_SWITCH + 3,
    KNOB_PARAM = STEPSIZE_SWITCH + 3,    
    SCALE_PARAM = KNOB_PARAM + 3,
		NUM_PARAMS = SCALE_PARAM + 3
	};  
	enum InputIds {     
		NUM_INPUTS
	};
	enum OutputIds {
		OUTPUT,       
		NUM_OUTPUTS = OUTPUT + 3
	};
  enum LightIds {
		BUTTON_LEDS,    
		NUM_LIGHTS = BUTTON_LEDS + 9
	};  
  
  float knob_value[3] = {0.0,0.0,0.0};
  float scale_value[3] = {0.0,0.0,0.0};
  float output_value[3] = {0.0,0.0,0.0};
  //int display_value[3] = {0,0,0};
  float display_value[3] = {0.0,0.0,0.0};
  dsp::SchmittTrigger step_switch_trigger[3],bi_switch_trigger[3],stepsize_switch_trigger[3];
  bool switch_states[3][3] = {{0,0,0},
                             {0,0,0},
                             {0,0,0}};
    
                            
  int octaves[3] = {0,0,0};
  int semitones[3] = {0,0,0};

  MentalKnobs()
  {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    
    for (int i = 0; i < 3; ++i)
    {
      configParam(MentalKnobs::KNOB_PARAM + i, -1.0, 1.0, 0.0, "");
      configParam(MentalKnobs::SCALE_PARAM + i,0.0, 10.0, 1.0, "");
      configParam(MentalKnobs::STEP_SWITCH + i, 0.0, 1.0, 0.0, "");
      configParam(MentalKnobs::BI_SWITCH + i, 0.0, 1.0, 0.0, "");
      configParam(MentalKnobs::STEPSIZE_SWITCH + i, 0.0, 1.0, 0.0, "");  
    }
    
  }

	void process(const ProcessArgs& args) override;
  
  json_t *dataToJson() override
  {
		json_t *rootJ = json_object();
    
    // button states
		json_t *switch_statesJ = json_array();
		for (int i = 0; i < 3; i++)
    {
      for (int j = 0 ; j < 3 ; j++)
      {
			  json_t *switch_stateJ = json_integer((int) switch_states[i][j]);
			  json_array_append_new(switch_statesJ, switch_stateJ);
      }
		}
		json_object_set_new(rootJ, "switches", switch_statesJ);    
    return rootJ;
  }
  
  void dataFromJson(json_t *rootJ) override
  {
    // button states
		json_t *switch_statesJ = json_object_get(rootJ, "switches");
		if (switch_statesJ)
    {
			for (int i = 0; i < 3 ; i++)
      {
				for (int j = 0 ; j < 3 ; j++)
        {
          json_t *switch_stateJ = json_array_get(switch_statesJ, i * 3 + j);
				  if (switch_stateJ)
					  switch_states[i][j] = !!json_integer_value(switch_stateJ);
        }
			}
		}  
  }
};


void MentalKnobs::process(const ProcessArgs& args)
{
  for ( int i = 0 ; i < 3 ; i++)
  {
  // button triggers
  if (step_switch_trigger[i].process(params[STEP_SWITCH + i].getValue()))
  {
	  switch_states[0][i] = !switch_states[0][i];
	}  
  lights[BUTTON_LEDS + i].value = switch_states[0][i] ? 1.0 : 0.0;
  if (stepsize_switch_trigger[i].process(params[STEPSIZE_SWITCH + i].getValue()))
  {
	  switch_states[1][i] = !switch_states[1][i];
	}  
  lights[BUTTON_LEDS + 6 + i].value = switch_states[1][i] ? 1.0 : 0.0;
  if (bi_switch_trigger[i].process(params[BI_SWITCH + i].getValue()))
  {
	  switch_states[2][i] = !switch_states[2][i];
	}  
  lights[BUTTON_LEDS + 3 + i].value = switch_states[2][i] ? 1.0 : 0.0;
  knob_value[i] = params[KNOB_PARAM + i].getValue();
  scale_value[i] = params[SCALE_PARAM + i].getValue();
  if (!switch_states[2][i]) // bi switch
  {
    knob_value[i] = std::abs(knob_value[i]); 
  }
  output_value[i] = knob_value[i] * scale_value[i];
  if (switch_states[0][i]) // step switch
  {    
    if (switch_states[1][i]) // stepsize switch - quantise to semitones
    {
      octaves[i] = std::round(output_value[i]);
      semitones[i] = std::round((output_value[i] - octaves[i])*12);
      if (semitones[i] < 0)
      { 
        semitones[i] +=12;
        octaves[i] -= 1;
      }
      output_value[i] = octaves[i] + semitones[i]/12.0;
    } else output_value[i] = std::round(output_value[i]);
  }  
  
  //display_value[i] = std::round(output_value[i]);
  outputs[OUTPUT + i].setVoltage(output_value[i]);
  }  
}

////////////////////////////////////

struct NumberDisplayWidget4 : TransparentWidget {

  //int *value;
  float *value; 

  MentalKnobs *module;
  
  void draw(const DrawArgs& args) override
  {
    // Background
    NVGcolor backgroundColor = nvgRGB(0x00, 0x00, 0x00);
    NVGcolor StrokeColor = nvgRGB(0x00, 0x47, 0x7e);
    nvgBeginPath(args.vg);
    nvgRoundedRect(args.vg, -1.0, -1.0, box.size.x+2, box.size.y+2, 4.0);
    nvgFillColor(args.vg, StrokeColor);
    nvgFill(args.vg);
    nvgBeginPath(args.vg);
    nvgRoundedRect(args.vg, 0.0, 0.0, box.size.x, box.size.y, 4.0);
    nvgFillColor(args.vg, backgroundColor);
    nvgFill(args.vg);    
    
    // text 
    std::shared_ptr<Font> font = APP->window->loadFont(asset::plugin(pluginInstance, "res/Segment7Standard.ttf"));
    if (font)
    {
      nvgFontSize(args.vg, 14);
      nvgFontFaceId(args.vg, font->handle);
      nvgTextLetterSpacing(args.vg, 1.75);
    }

    std::stringstream to_display;

    //to_display << std::setw(3) << *value;
    if(module) {
        to_display << std::setw(5) << *value;
      }
      else {
        to_display << std::setw(5) << "0.000";
      }

    Vec textPos = Vec(6.0f, 16.0f);   
    NVGcolor textColor = nvgRGB(0x00, 0x47, 0x7e);
    nvgFillColor(args.vg, textColor);
    nvgText(args.vg, textPos.x, textPos.y, to_display.str().c_str(), NULL);
  }
};

//////////////////////////////////
struct MentalKnobsWidget : ModuleWidget {
  MentalKnobsWidget(MentalKnobs *module)
{

  setModule(module);
	
  setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/MentalKnobs.svg")));

  int group_offset = 120;    
  for (int i = 0 ; i < 3 ; i++)
  {
    addParam(createParam<MedKnob>(Vec(2, 20+group_offset*i), module, MentalKnobs::KNOB_PARAM + i));
    addParam(createParam<MedKnob>(Vec(32, 20+group_offset*i), module, MentalKnobs::SCALE_PARAM + i)); 
    
    addParam(createParam<LEDButton>(Vec(5, 50+group_offset*i), module, MentalKnobs::STEP_SWITCH + i));
    addChild(createLight<MedLight<BlueLED>>(Vec(10, 55+group_offset*i), module, MentalKnobs::BUTTON_LEDS + i ));
    addParam(createParam<LEDButton>(Vec(5, 75+group_offset*i), module, MentalKnobs::BI_SWITCH + i));
    addChild(createLight<MedLight<BlueLED>>(Vec(10, 80+group_offset*i), module, MentalKnobs::BUTTON_LEDS + 3 + i));
    addParam(createParam<LEDButton>(Vec(35, 50+group_offset*i), module, MentalKnobs::STEPSIZE_SWITCH + i));
    addChild(createLight<MedLight<BlueLED>>(Vec(40, 55+group_offset*i), module, MentalKnobs::BUTTON_LEDS + 6 + i));
    
    addOutput(createOutput<CVOutPort>(Vec(33, 75+group_offset*i), module, MentalKnobs::OUTPUT + i));     
  }
  
  NumberDisplayWidget4 *display = new NumberDisplayWidget4();
	display->box.pos = Vec(4,105);
	display->box.size = Vec(53, 20);
  display->module = module;
	display->value = &module->output_value[0];
	addChild(display); 
    
  NumberDisplayWidget4 *display2 = new NumberDisplayWidget4();
	display2->box.pos = Vec(4,105+group_offset);
	display2->box.size = Vec(53, 20);
  display2->module = module;
	display2->value = &module->output_value[1];
	addChild(display2); 
  
  NumberDisplayWidget4 *display3 = new NumberDisplayWidget4();
	display3->box.pos = Vec(4,105+group_offset * 2);
	display3->box.size = Vec(53, 20);
  display3->module = module;
	display3->value = &module->output_value[2];
	addChild(display3); 
 
}
};

Model *modelMentalKnobs = createModel<MentalKnobs, MentalKnobsWidget>("MentalKnobs");