///////////////////////////////////////////////////
//
//   Mental Plugin
//   Gate maker, make gates by clock - VCV Module
//
//   Strum 2017-21
//	 strum@softhome.net
//
///////////////////////////////////////////////////

#include "mental.hpp"

#include <sstream>
#include <iomanip>

struct GateMaker : Module {
	enum ParamIds {
    RST_BUTTON,
    TRIG_BUTTON,
    CYCLE_BUTTON,
    COUNT_NUM_PARAM,
    DELAY_PARAM,
    TAIL_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
    CLK_IN,
   	RESET_IN,
    TRIGGER_IN,
    CYCLE_IN,
    COUNT_CV,
    DELAY_CV,
    TAIL_CV,
		NUM_INPUTS
	};
	enum OutputIds {
		OUTPUT,
    FINISH_OUT,
    TRIG_OUT,
		NUM_OUTPUTS
	};
  enum LightIds {
		CYCLE_BUTTON_LED,
		NUM_LIGHTS
	};

  dsp::SchmittTrigger clock_trigger;
  dsp::SchmittTrigger reset_trigger;
  dsp::SchmittTrigger cycle__button_trigger;
  dsp::SchmittTrigger trigger_trigger;


  int count_limit = 0;
  int count_on = 0;
  int count_disp = 0;
  int count = 0;
  int delay = 0;
  int tail = 0;
  bool cycle = true;
  bool cycle_button_state = true;

  bool triggered = false;
  bool mode = false;

  GateMaker();

	void process(const ProcessArgs& args) override;
};

GateMaker::GateMaker()
{
  //params.resize(NUM_PARAMS);
	//inputs.resize(NUM_INPUTS);
	//outputs.resize(NUM_OUTPUTS);
  //lights.resize(NUM_LIGHTS);

  config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

  configParam(GateMaker::COUNT_NUM_PARAM, 0.0, 32.0, 0.0, "");
  configParam(GateMaker::DELAY_PARAM, 0.0, 32.0, 0.0, "");
  configParam(GateMaker::TAIL_PARAM, 0.0, 32.0, 0.0, "");
  configParam(GateMaker::RST_BUTTON, 0.0, 1.0, 0.0, "");
  configParam(GateMaker::TRIG_BUTTON, 0.0, 1.0, 0.0, "");
  configParam(GateMaker::CYCLE_BUTTON, 0.0, 1.0, 0.0, "");
}

void GateMaker::process(const ProcessArgs& args)
{


  if (cycle__button_trigger.process(params[CYCLE_BUTTON].getValue()))
  {
    cycle_button_state = !cycle_button_state;
    count = 0;
    outputs[OUTPUT].setVoltage(0);
    outputs[FINISH_OUT].setVoltage(0.0);
  }

  if (inputs[CYCLE_IN].getVoltage() || cycle_button_state)
  {
    cycle = true;
    lights[CYCLE_BUTTON_LED].value = 1.0;

  } else
  {
    cycle = false;
    lights[CYCLE_BUTTON_LED].value = 0.0;

  }
  delay = round(params[DELAY_PARAM].getValue());
  count_on = round(params[COUNT_NUM_PARAM].getValue());
  count_disp = count_on + 1;
  tail = round(params[TAIL_PARAM].getValue());
  count_limit = count_on + delay + tail;

  bool reset = false;

  if (trigger_trigger.process(params[TRIG_BUTTON].getValue())  || (trigger_trigger.process(inputs[TRIGGER_IN].getVoltage())))
	{
    reset = true;
    triggered = true;
    count = 0;
    outputs[OUTPUT].setVoltage(0);
    outputs[FINISH_OUT].setVoltage(0.0);
  }

  if (reset_trigger.process(params[RST_BUTTON].getValue())  || (reset_trigger.process(inputs[RESET_IN].getVoltage())))
	{
    reset = true;
    count = 0;
    outputs[OUTPUT].setVoltage(0);
    outputs[FINISH_OUT].setVoltage(0.0);
  }
  if (reset == false && (cycle || triggered))
	{
		if (clock_trigger.process(inputs[CLK_IN].getVoltage()) && count <= count_limit)
					count++;
  }

  if (count >= delay && count <= (count_on + delay)) outputs[OUTPUT].setVoltage(10.0);
  else outputs[OUTPUT].setVoltage(0.0);
  if (count > count_limit)
  {
    count = 0;
    if (triggered) outputs[OUTPUT].setVoltage(0);
    triggered = false;
    outputs[FINISH_OUT].setVoltage(10.0);
  }

}


////////////////////////////////////

struct NumberDisplayWidget : TransparentWidget {

  int *value;

  GateMaker *module;

  std::shared_ptr<Font> font;

  NumberDisplayWidget() {
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
struct GateMakerWidget : ModuleWidget {
  GateMakerWidget(GateMaker *module)
{

  setModule(module);

  setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/GateMaker.svg")));

  addParam(createParam<MedKnob>(Vec(2, 20), module, GateMaker::COUNT_NUM_PARAM));
  addInput(createInput<CVInPort>(Vec(33, 20), module, GateMaker::COUNT_CV));
  NumberDisplayWidget *count_display = new NumberDisplayWidget();
	count_display->box.pos = Vec(63,20);
	count_display->box.size = Vec(50, 20);
  count_display->module = module;
	count_display->value = &module->count_disp;
	addChild(count_display);

  addParam(createParam<MedKnob>(Vec(2, 50), module, GateMaker::DELAY_PARAM));
  addInput(createInput<CVInPort>(Vec(33, 50), module, GateMaker::DELAY_CV));
  NumberDisplayWidget *delay_display = new NumberDisplayWidget();
	delay_display->box.pos = Vec(63,50);
	delay_display->box.size = Vec(50, 20);
  delay_display->module = module;
	delay_display->value = &module->delay;
	addChild(delay_display);

  addParam(createParam<MedKnob>(Vec(2, 80), module, GateMaker::TAIL_PARAM));
  addInput(createInput<CVInPort>(Vec(33, 80), module, GateMaker::TAIL_CV));
  NumberDisplayWidget *tail_display = new NumberDisplayWidget();
	tail_display->box.pos = Vec(63,80);
	tail_display->box.size = Vec(50, 20);
  tail_display->module = module;
	tail_display->value = &module->tail;
	addChild(tail_display);

  int offset = 30;

  addInput(createInput<GateInPort>(Vec(3, 80 + offset), module, GateMaker::CLK_IN));
	addInput(createInput<GateInPort>(Vec(3, 110 + offset), module, GateMaker::RESET_IN));
  addParam(createParam<LEDButton>(Vec(35, 110 + offset), module, GateMaker::RST_BUTTON));
  addInput(createInput<GateInPort>(Vec(3, 140 + offset), module, GateMaker::TRIGGER_IN));
  addParam(createParam<LEDButton>(Vec(35, 140 + offset), module, GateMaker::TRIG_BUTTON));

  addParam(createParam<LEDButton>(Vec(35, 170 + offset), module, GateMaker::CYCLE_BUTTON));
  addChild(createLight<MedLight<BlueLED>>(Vec(35+5, 170 + offset + 5), module, GateMaker::CYCLE_BUTTON_LED));
  addInput(createInput<GateInPort>(Vec(3, 170 + offset), module, GateMaker::CYCLE_IN));

  addOutput(createOutput<GateOutPort>(Vec(93, 110), module, GateMaker::OUTPUT));
  addOutput(createOutput<GateOutPort>(Vec(93, 140), module, GateMaker::TRIG_OUT));
  addOutput(createOutput<GateOutPort>(Vec(93, 170), module, GateMaker::FINISH_OUT));

}
};

Model *modelGateMaker = createModel<GateMaker, GateMakerWidget>("GateMaker");
