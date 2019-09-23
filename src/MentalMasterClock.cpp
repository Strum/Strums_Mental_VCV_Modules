///////////////////////////////////////////////////
//
//   Master Clock Module
//   VCV Module
//
//   Strum 2017
//
///////////////////////////////////////////////////

#include "mental.hpp"

#include <sstream>
#include <iomanip>

struct LFOGenerator {
	float phase = 0.0;
	float pw = 0.5;
	float freq = 1.0;	
	void setFreq(float freq_to_set)
  {
    freq = freq_to_set;
  }		
	void step(float dt) {
		float deltaPhase = fminf(freq * dt, 0.5);
		phase += deltaPhase;
		if (phase >= 1.0)
			phase -= 1.0;
	}	
	float sqr() {
		float sqr = phase < pw ? 1.0 : -1.0;
		return sqr;
	}
};

struct MentalMasterClock : Module {
	enum ParamIds {    
    TEMPO_PARAM,    
    TIMESIGTOP_PARAM,
    TIMESIGBOTTOM_PARAM,
    RESET_BUTTON,
    RUN_SWITCH,    
		NUM_PARAMS
	};  
	enum InputIds {     
		NUM_INPUTS
	};
	enum OutputIds {
		BEAT_OUT,
    EIGHTHS_OUT,
    SIXTEENTHS_OUT,
    BAR_OUT,       
		NUM_OUTPUTS
	};
  enum LightIds {
		RESET_LED,
    RUN_LED,
		NUM_LIGHTS
	}; 
  
  LFOGenerator clock;
  
  dsp::SchmittTrigger eighths_trig;
	dsp::SchmittTrigger quarters_trig;
  dsp::SchmittTrigger bars_trig;
  
  dsp::SchmittTrigger run_button_trig;
  bool running = true;
  
  int eighths_count = 0;
	int quarters_count = 0;
  int bars_count = 0;
  
  int tempo, time_sig_top, time_sig_bottom = 0;
  float frequency = 2.0;
  int quarters_count_limit = 4;
  int eighths_count_limit = 2;
  int bars_count_limit = 16;
   
  
  MentalMasterClock(); 
	void process(const ProcessArgs& args) override;
  
  json_t *dataToJson() override
  {
		json_t *rootJ = json_object();
    json_t *button_statesJ = json_array();
		json_t *button_stateJ = json_integer((int)running);
		json_array_append_new(button_statesJ, button_stateJ);		
		json_object_set_new(rootJ, "run", button_statesJ);    
    return rootJ;
  }
  
  void dataFromJson(json_t *rootJ) override
  {
    json_t *button_statesJ = json_object_get(rootJ, "run");
		if (button_statesJ)
    {			
				json_t *button_stateJ = json_array_get(button_statesJ,0);
				if (button_stateJ)
					running = !!json_integer_value(button_stateJ);			
		}  
  }  
};

/////////////////////////////////////////////////////////////////////////
MentalMasterClock::MentalMasterClock()
{
  params.resize(NUM_PARAMS);
	inputs.resize(NUM_INPUTS);
	outputs.resize(NUM_OUTPUTS);
  lights.resize(NUM_LIGHTS); 

  configParam(MentalMasterClock::TEMPO_PARAM, 40.0, 250.0, 120.0, "");
  configParam(MentalMasterClock::TIMESIGTOP_PARAM,2.0, 15.0, 4.0, "");
  configParam(MentalMasterClock::TIMESIGBOTTOM_PARAM,0.0, 3.0, 1.0, "");
  configParam(MentalMasterClock::RESET_BUTTON, 0.0, 1.0, 0.0, "");
  configParam(MentalMasterClock::RUN_SWITCH, 0.0, 1.0, 0.0, ""); 
}

void MentalMasterClock::process(const ProcessArgs& args)
{
  if (run_button_trig.process(params[RUN_SWITCH].getValue()))
    {
		  running = !running;
	  }
    lights[RUN_LED].value = running ? 1.0 : 0.0;
    
  tempo = std::round(params[TEMPO_PARAM].getValue());
  time_sig_top = std::round(params[TIMESIGTOP_PARAM].getValue());
  time_sig_bottom = std::round(params[TIMESIGBOTTOM_PARAM].getValue());
  time_sig_bottom = std::pow(2,time_sig_bottom+1);
 
  frequency = tempo/60.0;
  if (params[RESET_BUTTON].getValue() > 0.0) 
  {
    eighths_count = 0;
    quarters_count = 0;
    bars_count = 0; 
    lights[RESET_LED].value = 1.0;
  } else lights[RESET_LED].value = 0.0;
  
  if (!running) 
  {
    eighths_count = 0;
    quarters_count = 0;
    bars_count = 0; 
    outputs[BAR_OUT].setVoltage(0.0);
    outputs[BEAT_OUT].setVoltage(0.0);
    outputs[EIGHTHS_OUT].setVoltage(0.0);
    outputs[SIXTEENTHS_OUT].setVoltage(0.0); 
  } else
  {
  if (time_sig_top == time_sig_bottom)
  {
    clock.setFreq(frequency*4);
    quarters_count_limit = 4;
    eighths_count_limit = 2;
    bars_count_limit = 16;    
  } else
  {
    if (time_sig_bottom == 4)
    {
      quarters_count_limit = 4;
      eighths_count_limit = 2;
      bars_count_limit = time_sig_top * 4;  
      clock.setFreq(frequency*4); 
    }
    if (time_sig_bottom == 8)
    {
      quarters_count_limit = 4;
      eighths_count_limit = 2;
      bars_count_limit = time_sig_top * 2;
      clock.setFreq(frequency*4);
      if ((time_sig_top % 3) == 0)
      {
        quarters_count_limit = 6;
        eighths_count_limit = 2;
        bars_count_limit = (time_sig_top/3) * 6;
        clock.setFreq(frequency*6);
      }      
    }
  }
  
  clock.step(1.0 / args.sampleRate);
  outputs[SIXTEENTHS_OUT].setVoltage(5.0 * clock.sqr());
 
  if (eighths_trig.process(clock.sqr()) && eighths_count <= eighths_count_limit)
    eighths_count++;
  if (eighths_count >= eighths_count_limit)
  {
    eighths_count = 0;    
  }
  if (eighths_count == 0) outputs[EIGHTHS_OUT].setVoltage(5.0);
  else outputs[EIGHTHS_OUT].setVoltage(0.0);
  
  if (quarters_trig.process(clock.sqr()) && quarters_count <= quarters_count_limit)
    quarters_count++;
  if (quarters_count >= quarters_count_limit)
  {
    quarters_count = 0;    
  }
  if (quarters_count == 0) outputs[BEAT_OUT].setVoltage(5.0);
  else outputs[BEAT_OUT].setVoltage(0.0);
  
  if (bars_trig.process(clock.sqr()) && bars_count <= bars_count_limit)
    bars_count++;
  if (bars_count >= bars_count_limit)
  {
    bars_count = 0;    
  }
  if (bars_count == 0) outputs[BAR_OUT].setVoltage(5.0);
  else outputs[BAR_OUT].setVoltage(0.0); 
  
  }
}

////////////////////////////////////
struct NumberDisplayWidget2 : TransparentWidget {

  int *value;

  MentalMasterClock *module;
  std::shared_ptr<Font> font;

  NumberDisplayWidget2() {
    font = APP->window->loadFont(asset::plugin(pluginInstance, "res/Segment7Standard.ttf"));
  };

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
    nvgFontSize(args.vg, 18);
    nvgFontFaceId(args.vg, font->handle);
    nvgTextLetterSpacing(args.vg, 2.5);

    std::stringstream to_display;   
    //to_display << std::setw(3) << *value;
    if(module) {
        to_display << std::setw(3) << *value;
      }
      else {
        to_display << std::setw(3) << "00";
      }

    Vec textPos = Vec(6.0f, 17.0f);   
    NVGcolor textColor = nvgRGB(0x00, 0x47, 0x7e);
    nvgFillColor(args.vg, textColor);
    nvgText(args.vg, textPos.x, textPos.y, to_display.str().c_str(), NULL);
  }
};

//////////////////////////////////
struct MentalMasterClockWidget : ModuleWidget {
  MentalMasterClockWidget(MentalMasterClock *module)
{

  setModule(module);
  
  setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/MentalMasterClock.svg")));
   
    addParam(createParam<MedKnob>(Vec(2, 20), module, MentalMasterClock::TEMPO_PARAM));
    addParam(createParam<MedKnob>(Vec(2, 50), module, MentalMasterClock::TIMESIGTOP_PARAM));
    addParam(createParam<MedKnob>(Vec(2, 80), module, MentalMasterClock::TIMESIGBOTTOM_PARAM));
     
    addOutput(createOutput<GateOutPort>(Vec(90, 110), module, MentalMasterClock::BEAT_OUT)); 
    addOutput(createOutput<GateOutPort>(Vec(90, 140), module, MentalMasterClock::BAR_OUT)); 
    addOutput(createOutput<GateOutPort>(Vec(90, 170), module, MentalMasterClock::EIGHTHS_OUT)); 
    addOutput(createOutput<GateOutPort>(Vec(90, 200), module, MentalMasterClock::SIXTEENTHS_OUT)); 
          
    addParam(createParam<LEDButton>(Vec(5, 140), module, MentalMasterClock::RESET_BUTTON));
    addChild(createLight<MedLight<BlueLED>>(Vec(10, 145), module, MentalMasterClock::RESET_LED));
    
    addParam(createParam<LEDButton>(Vec(5, 110), module, MentalMasterClock::RUN_SWITCH));
    addChild(createLight<MedLight<BlueLED>>(Vec(10, 115), module, MentalMasterClock::RUN_LED));
    
  NumberDisplayWidget2 *display = new NumberDisplayWidget2();
	display->box.pos = Vec(35,20);
	display->box.size = Vec(50, 20);
  display->module = module;
	display->value = &module->tempo;
	addChild(display); 
    
  NumberDisplayWidget2 *display2 = new NumberDisplayWidget2();
	display2->box.pos = Vec(35,50);
	display2->box.size = Vec(50, 20);
  display2->module = module;
	display2->value = &module->time_sig_top;
	addChild(display2); 
  
  NumberDisplayWidget2 *display3 = new NumberDisplayWidget2();
	display3->box.pos = Vec(35,80);
	display3->box.size = Vec(50, 20);
  display3->module = module;
	display3->value = &module->time_sig_bottom;
	addChild(display3); 
 
}
};

Model *modelMentalMasterClock = createModel<MentalMasterClock, MentalMasterClockWidget>("MentalMasterClock");