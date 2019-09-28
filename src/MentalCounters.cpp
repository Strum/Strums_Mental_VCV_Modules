///////////////////////////////////////////////////
//
//   Mental Plugin
//   Dual Counter
//
//   Strum 2017 - 2019
//   strum@softhome.net
//   Thanks to ML for the display code
//
///////////////////////////////////////////////////

#include "mental.hpp"

#include <sstream>
#include <iomanip>

struct MentalCounters : Module
{
	enum ParamIds
  {
    RST_BUTTON,
    COUNT_NUM_PARAM,
    RST_BUTTON_2,
    COUNT_NUM_PARAM_2,
		NUM_PARAMS
	};  
	enum InputIds
  {
    CLK_IN,
   	RESET_IN,
    CLK_IN_2,
   	RESET_IN_2,	  
		NUM_INPUTS
	};
	enum OutputIds
  {
		OUTPUT,
    OUTPUT_2,    
		NUM_OUTPUTS
	};

  dsp::SchmittTrigger clock_trigger;
  dsp::SchmittTrigger reset_trigger;
  int count_limit = 0;
  int count = 0;
  dsp::SchmittTrigger clock_trigger_2;
  dsp::SchmittTrigger reset_trigger_2;
  int count_limit_2 = 0;
  int count_2 = 0;
  
	MentalCounters();
 
	void process(const ProcessArgs& args) override;
};

MentalCounters::MentalCounters()
{
  //params.resize(NUM_PARAMS);
	//inputs.resize(NUM_INPUTS);
	//outputs.resize(NUM_OUTPUTS);

  config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS);

  configParam(MentalCounters::COUNT_NUM_PARAM, 0.0, 32.0, 0.0, "");
  configParam(MentalCounters::RST_BUTTON, 0.0, 1.0, 0.0, "");
  configParam(MentalCounters::COUNT_NUM_PARAM_2, 0.0, 32.0, 0.0, "");
  configParam(MentalCounters::RST_BUTTON_2, 0.0, 1.0, 0.0, "");	
}

void MentalCounters::process(const ProcessArgs& args)
{
  count_limit = round(params[COUNT_NUM_PARAM].getValue());
  count_limit_2 = round(params[COUNT_NUM_PARAM_2].getValue());
  
  bool reset = false;
  bool reset_2 = false;
  
  if (reset_trigger.process(params[RST_BUTTON].getValue())  || (reset_trigger.process(inputs[RESET_IN].getVoltage())))
	{
    reset = true;
    count = 0;
    outputs[OUTPUT].setVoltage(0); 
  }  
  if (reset == false)
	{
		if (clock_trigger.process(inputs[CLK_IN].getVoltage()) && count <= count_limit)
					count++;	
  }
  if (count == count_limit) outputs[OUTPUT].setVoltage(10.0);
  if (count > count_limit)
  {
    count = 0;
    outputs[OUTPUT].setVoltage(0); 
  }
  ///////////// counter 2
  if (reset_trigger_2.process(params[RST_BUTTON_2].getValue())  || (reset_trigger_2.process(inputs[RESET_IN_2].getVoltage())))
	{
    reset_2 = true;
    count_2 = 0;
    outputs[OUTPUT_2].setVoltage(0); 
  }  
  if (reset_2 == false)
	{
		if (clock_trigger_2.process(inputs[CLK_IN_2].getVoltage()) && count_2 <= count_limit_2)
					count_2++;	
  }
  if (count_2 == count_limit_2) outputs[OUTPUT_2].setVoltage(10.0);
  if (count_2 > count_limit_2)
  {
    count_2 = 0;
    outputs[OUTPUT_2].setVoltage(0); 
  }  
}
///////////////////////////////////////////////////////////////////
// AS display widget
///////////////////////////////////
/*struct NumberDisplayWidget : TransparentWidget {

  int *value = NULL;
  std::shared_ptr<Font> font;

  NumberDisplayWidget() {
    font = APP->window->loadFont(asset::plugin(pluginInstance, "res/Segment7Standard.ttf"));
  };

  void draw(const DrawArgs &args) override{
    if (!value) {
      return;
    }  
    // text 
    nvgFontSize(args.vg, 18);
    nvgFontFaceId(args.vg, font->handle);
    nvgTextLetterSpacing(args.vg, 2.5);

    std::stringstream to_display;   
    to_display << std::right  << std::setw(2) << *value;

    Vec textPos = Vec(4.0f, 17.0f); 

    NVGcolor textColor = nvgRGB(0xf0, 0x00, 0x00);
    nvgFillColor(args.vg, textColor);
    nvgText(args.vg, textPos.x, textPos.y, to_display.str().c_str(), NULL);
  }
}; */
////////////////////////////////////


////////////////////////////////////
struct NumberDisplayWidget3 : TransparentWidget
{
  
  int *value;
  //int *value = 0;
  MentalCounters *module;
  
  std::shared_ptr<Font> font;

  NumberDisplayWidget3()
  {
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


    //std::stringstream to_display;
    //std::string to_display = std::to_string(*value);
    std::stringstream to_display;

    //if(value) {to_display = std::to_string(*value);}
    
    //to_display << std::setw(3) << *value;

    //while(to_display.length()<3) to_display = ' ' + to_display;
    
    if(module)
    {
      to_display << std::setw(3) << *value;
    }
    else
    {
      to_display << std::setw(3) << "00";
    }

    Vec textPos = Vec(6.0f, 17.0f);

    NVGcolor textColor = nvgRGB(0x00, 0x47, 0x7e);
    nvgFillColor(args.vg, textColor);

    nvgText(args.vg, textPos.x, textPos.y, to_display.str().c_str(), NULL);
    //nvgText(args.vg, textPos.x, textPos.y, to_display.c_str(), NULL);
    //nvgText(vg, textPos.x, textPos.y, to_display.c_str(), NULL);
  }
};

//////////////////////////////////

struct MentalCountersWidget : ModuleWidget
{
  MentalCountersWidget(MentalCounters *module)
  {
    setModule(module);

    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/MentalCounters.svg")));
    
    int group_offset = 190;
    
    addParam(createParam<MedKnob>(Vec(2, 20), module, MentalCounters::COUNT_NUM_PARAM)); 
    addInput(createInput<GateInPort>(Vec(3, 90), module, MentalCounters::CLK_IN));
  	addInput(createInput<GateInPort>(Vec(3, 120), module, MentalCounters::RESET_IN));
    
    addParam(createParam<LEDButton>(Vec(5, 145), module, MentalCounters::RST_BUTTON));
    
    addOutput(createOutput<GateOutPort>(Vec(33, 90), module, MentalCounters::OUTPUT));
    
    NumberDisplayWidget3 *display = new NumberDisplayWidget3();
  	display->box.pos = Vec(5,50);
  	display->box.size = Vec(50, 20);
  	display->module = module;
    display->value = &module->count_limit;
    //if (module) { display->value = &module->count_limit; }
    
  	addChild(display);
    
    /////////// counter 2
    addParam(createParam<MedKnob>(Vec(2, 20 + group_offset), module, MentalCounters::COUNT_NUM_PARAM_2)); 
    addInput(createInput<GateInPort>(Vec(3, 90 + group_offset), module, MentalCounters::CLK_IN_2));
  	addInput(createInput<GateInPort>(Vec(3, 120 + group_offset), module, MentalCounters::RESET_IN_2));
    
    addParam(createParam<LEDButton>(Vec(5, 145 + group_offset), module, MentalCounters::RST_BUTTON_2));
    
    addOutput(createOutput<GateOutPort>(Vec(33, 90 + group_offset), module, MentalCounters::OUTPUT_2));
    
    NumberDisplayWidget3 *display_2 = new NumberDisplayWidget3();
  	display_2->box.pos = Vec(5,50 + group_offset);
  	display_2->box.size = Vec(50, 20);
  	display_2->module = module;
    display_2->value = &module->count_limit_2;
    //if (module) { display_2->value = &module->count_limit_2; }

  	addChild(display_2);   	  
  }
};

Model *modelMentalCounters = createModel<MentalCounters, MentalCountersWidget>("MentalCounters");