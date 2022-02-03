///////////////////////////////////////////////////
//
//   Mental Plugin
//   Cartesian Sequencer a la Rene
//
//   Strum 2017-22
//   strum@sodaisland.net
//
///////////////////////////////////////////////////

#include "mental.hpp"

struct MentalCartesian : Module
{
	enum ParamIds
  {
    BUTTON_PARAM,
    KNOB_PARAM = BUTTON_PARAM + 16,
    NUM_PARAMS = KNOB_PARAM + 16		
	};  
	enum InputIds
  {		
		UP,
    DOWN,
    LEFT,
    RIGHT,
    X_CV,
    Y_CV,
    RESET,
    X_RESET,
    Y_RESET,
		NUM_INPUTS
	};
	enum OutputIds
  {
		UNQUANT_OUT,
    QUANT_OUT,
    ROW_OUT,
    COLUMN_OUT = ROW_OUT + 4,    
		NUM_OUTPUTS = COLUMN_OUT + 4
	};
  enum LightIds
  {
		BUTTON_LIGHTS,
    GRID_LIGHTS = BUTTON_LIGHTS + 16,
		NUM_LIGHTS = GRID_LIGHTS + 16
	};
  
  dsp::SchmittTrigger leftTrigger;
  dsp::SchmittTrigger rightTrigger;
  dsp::SchmittTrigger upTrigger;
  dsp::SchmittTrigger downTrigger;
  dsp::SchmittTrigger resetTrigger;
  dsp::SchmittTrigger x_resetTrigger;
  dsp::SchmittTrigger y_resetTrigger;
    
  dsp::SchmittTrigger button_triggers[4][4];
    
  float row_outs[4] = {0.0,0.0,0.0,0.0};
  float column_outs[4] = {0.0,0.0,0.0,0.0};
  
  int x_position = 0;
  int y_position = 0;
    
	MentalCartesian()
  {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
        configParam(MentalCartesian::KNOB_PARAM + i + j * 4, -2.0, 2.0, 0.0, "");
        configParam(MentalCartesian::BUTTON_PARAM + i + j * 4, 0.0, 1.0, 0.0, "");
      }
    }
  }

	void process(const ProcessArgs& args) override;   
};

void MentalCartesian::process(const ProcessArgs& args)
{
    bool step_right = false;
    bool step_left = false;
    bool step_up = false;
    bool step_down = false;
    
    lights[GRID_LIGHTS + x_position + y_position * 4].value = 1.0;
    
    // handle clock inputs
    if (inputs[RIGHT].isConnected())
    {
			if (rightTrigger.process(inputs[RIGHT].getVoltage()))
      {
				step_right = true;
			}
		}
    if (inputs[LEFT].isConnected())
    {
			if (leftTrigger.process(inputs[LEFT].getVoltage()))
      {
				step_left = true;
			}
		}
    if (inputs[DOWN].isConnected())
    {
			if (downTrigger.process(inputs[DOWN].getVoltage()))
      {
				step_down = true;
			}
		}
    if (inputs[UP].isConnected())
    {
			if (upTrigger.process(inputs[UP].getVoltage()))
      {
				step_up = true;
			}
		}
    // resets
    if (resetTrigger.process(inputs[RESET].getVoltage()))
    {
      lights[GRID_LIGHTS + x_position + y_position * 4].value = 0.0;
		  x_position = 0;
      y_position = 0;
      lights[GRID_LIGHTS + x_position + y_position * 4].value = 1.0;
      step_right = false;
      step_left = false;
      step_up = false;	
      step_down = false;	
	  }
    if (x_resetTrigger.process(inputs[X_RESET].getVoltage()))
    {
      lights[GRID_LIGHTS + x_position + y_position * 4].value = 0.0;
		  x_position = 0;
      lights[GRID_LIGHTS + x_position + y_position * 4].value = 1.0;
      step_right = false;
      step_left = false;
      step_up = false;	
      step_down = false;	
	  }
    if (y_resetTrigger.process(inputs[Y_RESET].getVoltage()))
    {
      lights[GRID_LIGHTS + x_position + y_position * 4].value = 0.0;
		  y_position = 0;
      lights[GRID_LIGHTS + x_position + y_position * 4].value = 1.0;
      step_right = false;
      step_left = false;
      step_up = false;	
      step_down = false;	
	  }
    // handle CV inputs
    int x_cv = round(inputs[X_CV].getVoltage());
    if (x_cv < 0 ) x_cv = 0;
    if (x_cv > 3 ) x_cv = 3;
    std::div_t division_x;
    division_x = div(x_position + x_cv,4);
    lights[GRID_LIGHTS + x_position + y_position * 4].value = 0.0;
    x_position = division_x.rem;
    lights[GRID_LIGHTS + x_position + y_position * 4].value = 1.0;
    int y_cv = round(inputs[Y_CV].getVoltage());
    if (y_cv < 0 ) y_cv = 0;
    if (y_cv > 3 ) y_cv = 3;    
    std::div_t division_y;
    division_y = div(y_position + y_cv,4);
    lights[GRID_LIGHTS + x_position + y_position * 4].value = 0.0;
    y_position = division_y.rem;
    lights[GRID_LIGHTS + x_position + y_position * 4].value = 1.0;
    // handle button triggers
    for (int i = 0 ; i < 4 ; i++)
    {
      for (int j = 0 ; j < 4 ; j++)
      {
        if ((params[BUTTON_PARAM + i + j * 4].getValue()))
        {
          lights[BUTTON_LIGHTS + i + j * 4].value = 1.0;
          lights[GRID_LIGHTS + x_position + y_position * 4].value = 0.0;
          x_position = i;
          y_position = j;
          lights[GRID_LIGHTS + x_position + y_position * 4].value = 1.0;
        } else lights[BUTTON_LIGHTS + i + j * 4].value = 0.0;
      }		  
	  }
    
    // change x and y    
    if (step_right)
    {
      lights[GRID_LIGHTS + x_position + y_position * 4].value = 0.0;
      x_position += 1;
      if (x_position > 3) x_position = 0;
      lights[GRID_LIGHTS + x_position + y_position * 4].value = 1.0;
    }
    if (step_left)
    {
      lights[GRID_LIGHTS + x_position + y_position * 4].value = 0.0;
      x_position -= 1;
      if (x_position < 0) x_position = 3;      
      lights[GRID_LIGHTS + x_position + y_position * 4].value = 1.0;
    }
    if (step_down)
    {
      lights[GRID_LIGHTS + x_position + y_position * 4].value = 0.0;
      y_position += 1;
      if (y_position > 3) y_position = 0;
      lights[GRID_LIGHTS + x_position + y_position * 4].value = 1.0;
    }
    if (step_up)
    {
      lights[GRID_LIGHTS + x_position + y_position * 4].value = 0.0;
      y_position -= 1;
      if (y_position < 0) y_position = 3;      
      lights[GRID_LIGHTS + x_position + y_position * 4].value = 1.0;
    }
    
    /// set outputs
    int which_knob = y_position * 4 + x_position;
    float main_out = params[KNOB_PARAM + which_knob].getValue();
    
    int oct = round(main_out);
    float left = main_out - oct;
    int semi = round(left * 12);
    float quant_out = oct + semi/12.0;
    for (int i = 0 ; i < 4 ; i++)
    {
      row_outs[i] = params[KNOB_PARAM + y_position * 4 + i ].getValue();
      column_outs[i] = params[KNOB_PARAM + x_position + i * 4].getValue();
      outputs[ROW_OUT + i ].setVoltage(row_outs[i]);
      outputs[COLUMN_OUT + i ].setVoltage(column_outs[i]);            
    }
    
    outputs[UNQUANT_OUT].setVoltage(main_out);
    outputs[QUANT_OUT].setVoltage(quant_out);     
}

////////////////////////////////
struct MentalCartesianWidget : ModuleWidget
{
  MentalCartesianWidget(MentalCartesian *module)
  {

    setModule(module);
    
    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/MentalCartesian.svg")));
   
    int top = 25;
    int left = 3;
    int column_spacing = 25; 
    int row_spacing = 25;
    int button_offset = 140;
  	addInput(createInput<GateInPort>(Vec(left, top), module, MentalCartesian::LEFT));
    addInput(createInput<GateInPort>(Vec(left+column_spacing, top), module, MentalCartesian::RIGHT));
    addInput(createInput<CVInPort>(Vec(left+column_spacing * 2, top), module, MentalCartesian::X_CV));
    addInput(createInput<GateInPort>(Vec(left+column_spacing * 3, top), module, MentalCartesian::X_RESET));
    
    addInput(createInput<GateInPort>(Vec(left, top + 30), module, MentalCartesian::UP));
    addInput(createInput<GateInPort>(Vec(left + column_spacing, top + 30), module, MentalCartesian::DOWN));  
    addInput(createInput<CVInPort>(Vec(left + column_spacing * 2, top + 30), module, MentalCartesian::Y_CV));
    addInput(createInput<GateInPort>(Vec(left + column_spacing * 3, top + 30), module, MentalCartesian::Y_RESET));
      
    addInput(createInput<GateInPort>(Vec(left + column_spacing * 3, top + row_spacing * 2 + 10), module, MentalCartesian::RESET));
    
    addOutput(createOutput<CVOutPort>(Vec(213, 20), module, MentalCartesian::UNQUANT_OUT));  
    addOutput(createOutput<CVOutPort>(Vec(213, 50), module, MentalCartesian::QUANT_OUT));	
    
    for ( int i = 0 ; i < 4 ; i++)
    {
      for ( int j = 0 ; j < 4 ; j++)
      {
        addParam(createParam<SmlKnob>(Vec(left+column_spacing * i, top + row_spacing * j + 150 ), module, MentalCartesian::KNOB_PARAM + i + j * 4));
        addChild(createLight<MedLight<BlueLED>>(Vec(left+column_spacing * i + 4, top + row_spacing * j + 150 + 4), module, MentalCartesian::GRID_LIGHTS + i + j * 4));
        addParam(createParam<LEDButton>(Vec(button_offset+left+column_spacing * i, top + row_spacing * j + 150 ), module, MentalCartesian::BUTTON_PARAM + i + j * 4));
        addChild(createLight<MedLight<BlueLED>>(Vec(button_offset+left+column_spacing * i + 5, top + row_spacing * j + 150 + 5), module, MentalCartesian::BUTTON_LIGHTS + i + j * 4));
      }
      addOutput(createOutput<CVOutPort>(Vec(left+column_spacing * i, top + row_spacing * 4 + 150 ), module, MentalCartesian::ROW_OUT + i));
      addOutput(createOutput<CVOutPort>(Vec(left+column_spacing * 4, top + row_spacing * i + 150 ), module, MentalCartesian::COLUMN_OUT + i));
  	}  
	}
};

Model *modelMentalCartesian = createModel<MentalCartesian, MentalCartesianWidget>("MentalCartesian");