///////////////////////////////////////////////////
//
//   Mental Plugin
//   CLock Divider
//
//   Strum 2017-21
//   strum@softhome.net
//
///////////////////////////////////////////////////

#include "mental.hpp"

////////////////////////////////////////////////
struct ClockDivider : Module
{
	enum ParamIds
	{
		RESET_PARAM,
		NUM_PARAMS
	};
	enum InputIds
	{
		CLOCK_INPUT,
		RESET_INPUT,
		NUM_INPUTS
	};
	enum OutputIds
	{
		OUTPUTS,
		NUM_OUTPUTS = OUTPUTS + 9
	};
  	enum LightIds
  	{
		LIGHTS,
		NUM_LIGHTS = LIGHTS + 9
	};

	int clockCounts[9] = {0};

	int dividers[9] = {2, 4, 8, 16, 32, 3, 5, 7, 12};

	dsp::SchmittTrigger reset_trigger;
	dsp::SchmittTrigger clock_trigger;

	dsp::PulseGenerator clockPulses[9];
  	bool pulses[9] = {false};

	ClockDivider();
	void process(const ProcessArgs& args) override;
};

////////////////////////////////////////////////////////////////

ClockDivider::ClockDivider()
{
  	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	configParam(ClockDivider::RESET_PARAM, 0.0, 1.0, 0.0, "");
}

//////////////////////////////////////////////////////////////////////////////////
void ClockDivider::process(const ProcessArgs& args)
{
	bool reset = false;
  	for (int i = 0; i < 9; ++i)
  	{
  		pulses[i] = false;
  	}

  	if (reset_trigger.process(params[RESET_PARAM].getValue()) || (reset_trigger.process(inputs[RESET_INPUT].getVoltage())))
	{
    	reset = true;
    	for (int i = 0; i < 9; ++i)
    	{
    		clockCounts[i] = 0;
    		outputs[OUTPUTS + i].setVoltage(0);
    		lights[LIGHTS + i].setBrightness(0.f);
    	}
  	}

  	if (reset == false)
	{
		if (clock_trigger.process(inputs[CLOCK_INPUT].getVoltage())) // && count <= count_limit
		{
			for (int i = 0; i < 9; ++i)
			{
				if (clockCounts[i] <= dividers[i])
				{
					clockCounts[i]++;
				}
			}
		}

  	}

  	for (int i = 0; i < 9; ++i)
  	{
	  	if (clockCounts[i] == dividers[i])
	  	{
	        clockPulses[i].trigger(1e-2);
	        clockCounts[i] = 0;
	  	}
	  	pulses[i] = clockPulses[i].process(1.0 / args.sampleRate);
  		outputs[OUTPUTS + i].setVoltage(pulses[i] ? 10.0f : 0.0f);
  		lights[LIGHTS + i].setBrightness((clockCounts[i] == 0) ? 1.f : 0.f);
  	}
}

/////////////////////////////////////////////////////////////////////////////////////
struct ClockDividerWidget : ModuleWidget
{
	ClockDividerWidget(ClockDivider *module)
	{
		setModule(module);

		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/ClockDivider.svg")));

		addInput(createInput<GateInPort>(Vec(3, 20), module, ClockDivider::CLOCK_INPUT));
		addInput(createInput<GateInPort>(Vec(3, 55), module, ClockDivider::RESET_INPUT));
		addParam(createParam<LEDButton>(Vec(5, 80), module, ClockDivider::RESET_PARAM));

	  	for (int i = 0; i < 9; ++i)
	  	{
	  		addOutput(createOutput<GateOutPort>(Vec(2, 120 + i * 25), module, ClockDivider::OUTPUTS + i));
	  		addChild(createLight<MedLight<BlueLED>>(Vec(33, 120 + i * 25), module, ClockDivider::LIGHTS + i));
	  	}
	}
};

Model *modelClockDivider = createModel<ClockDivider, ClockDividerWidget>("ClockDivider");
