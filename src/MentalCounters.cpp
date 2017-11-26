///////////////////////////////////////////////////
//
//   Dual Counter - VCV Module
//
//   Strum 2017
//   Thanks to ML for the diaplay code
//
///////////////////////////////////////////////////

#include "mental.hpp"

#include "dsp/digital.hpp"

#include <sstream>
#include <iomanip>

struct MentalCounters : Module {
	enum ParamIds {
    RST_BUTTON,
    COUNT_NUM_PARAM,
    RST_BUTTON_2,
    COUNT_NUM_PARAM_2,
		NUM_PARAMS
	};  
	enum InputIds {
    CLK_IN,
   	RESET_IN,
    CLK_IN_2,
   	RESET_IN_2,	  
		NUM_INPUTS
	};
	enum OutputIds {
		OUTPUT,
    OUTPUT_2,    
		NUM_OUTPUTS
	};

  SchmittTrigger clock_trigger;
  SchmittTrigger reset_trigger;
  int count_limit = 0;
  int count = 0;
  SchmittTrigger clock_trigger_2;
  SchmittTrigger reset_trigger_2;
  int count_limit_2 = 0;
  int count_2 = 0;
  
	MentalCounters();
  // : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
	void step() override;
};

MentalCounters::MentalCounters()
{
  params.resize(NUM_PARAMS);
	inputs.resize(NUM_INPUTS);
	outputs.resize(NUM_OUTPUTS);
	clock_trigger.setThresholds(0.0, 1.0);
	reset_trigger.setThresholds(0.0, 1.0);
  clock_trigger_2.setThresholds(0.0, 1.0);
	reset_trigger_2.setThresholds(0.0, 1.0);
}

void MentalCounters::step()
{
  count_limit = round(params[COUNT_NUM_PARAM].value);
  count_limit_2 = round(params[COUNT_NUM_PARAM_2].value);
  
  bool reset = false;
  bool reset_2 = false;
  
  if (reset_trigger.process(params[RST_BUTTON].value)  || (reset_trigger.process(inputs[RESET_IN].value)))
	{
    reset = true;
    count = 0;
    outputs[OUTPUT].value = 0; 
  }  
  if (reset == false)
	{
		if (clock_trigger.process(inputs[CLK_IN].value) && count <= count_limit)
					count++;	
  }
  if (count == count_limit) outputs[OUTPUT].value = 10.0;
  if (count > count_limit)
  {
    count = 0;
    outputs[OUTPUT].value = 0; 
  }
  ///////////// counter 2
  if (reset_trigger_2.process(params[RST_BUTTON_2].value)  || (reset_trigger_2.process(inputs[RESET_IN_2].value)))
	{
    reset_2 = true;
    count_2 = 0;
    outputs[OUTPUT_2].value = 0; 
  }  
  if (reset_2 == false)
	{
		if (clock_trigger_2.process(inputs[CLK_IN_2].value) && count_2 <= count_limit_2)
					count_2++;	
  }
  if (count_2 == count_limit_2) outputs[OUTPUT_2].value = 10.0;
  if (count_2 > count_limit_2)
  {
    count_2 = 0;
    outputs[OUTPUT_2].value = 0; 
  }  
}


////////////////////////////////////
struct NumberDisplayWidget : TransparentWidget {

  int *value;
  std::shared_ptr<Font> font;

  NumberDisplayWidget() {
    font = Font::load(assetPlugin(plugin, "res/Segment7Standard.ttf"));
  };

  void draw(NVGcontext *vg) override
  {
    // Background
    NVGcolor backgroundColor = nvgRGB(0x00, 0x00, 0x00);
    nvgBeginPath(vg);
    nvgRoundedRect(vg, 0.0, 0.0, box.size.x, box.size.y, 4.0);
    nvgFillColor(vg, backgroundColor);
    nvgFill(vg);    
    
    // text 
    nvgFontSize(vg, 18);
    nvgFontFaceId(vg, font->handle);
    nvgTextLetterSpacing(vg, 2.5);

    std::stringstream to_display;   
    to_display << std::setw(3) << *value;

    Vec textPos = Vec(6.0f, 17.0f);   
    NVGcolor textColor = nvgRGB(0x33, 0x33, 0xff);
    nvgFillColor(vg, textColor);
    nvgText(vg, textPos.x, textPos.y, to_display.str().c_str(), NULL);
  }
};

//////////////////////////////////
MentalCountersWidget::MentalCountersWidget() {
	MentalCounters *module = new MentalCounters();
	setModule(module);
	box.size = Vec(15*4, 380);
  
	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin,"res/MentalCounters.svg")));
		addChild(panel);
	}
  
  int group_offset = 190;
  
  addParam(createParam<RoundSmallBlackKnob>(Vec(2, 20), module, MentalCounters::COUNT_NUM_PARAM, 0.0, 32.0, 0.0)); 
  addInput(createInput<CVPort>(Vec(3, 90), module, MentalCounters::CLK_IN));
	addInput(createInput<CVPort>(Vec(3, 120), module, MentalCounters::RESET_IN));
  
  addParam(createParam<LEDButton>(Vec(5, 145), module, MentalCounters::RST_BUTTON, 0.0, 1.0, 0.0));
  
  addOutput(createOutput<OutPort>(Vec(33, 90), module, MentalCounters::OUTPUT));
  
  NumberDisplayWidget *display = new NumberDisplayWidget();
	display->box.pos = Vec(5,50);
	display->box.size = Vec(50, 20);
	display->value = &module->count_limit;
	addChild(display);
  
  /////////// counter 2
  addParam(createParam<RoundSmallBlackKnob>(Vec(2, 20 + group_offset), module, MentalCounters::COUNT_NUM_PARAM_2, 0.0, 32.0, 0.0)); 
  addInput(createInput<CVPort>(Vec(3, 90 + group_offset), module, MentalCounters::CLK_IN_2));
	addInput(createInput<CVPort>(Vec(3, 120 + group_offset), module, MentalCounters::RESET_IN_2));
  
  addParam(createParam<LEDButton>(Vec(5, 145 + group_offset), module, MentalCounters::RST_BUTTON_2, 0.0, 1.0, 0.0));
  
  addOutput(createOutput<OutPort>(Vec(33, 90 + group_offset), module, MentalCounters::OUTPUT_2));
  
  NumberDisplayWidget *display_2 = new NumberDisplayWidget();
	display_2->box.pos = Vec(5,50 + group_offset);
	display_2->box.size = Vec(50, 20);
	display_2->value = &module->count_limit_2;
	addChild(display_2);  
  	  
}
