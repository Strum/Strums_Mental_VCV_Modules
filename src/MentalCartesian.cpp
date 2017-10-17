///////////////////////////////////////////////////////////////////
//
//  Cartesian Sequencer Module for VCV similar to Make Noise Rene
//
//  Strum 2017
//  strum@softhome.net
//
///////////////////////////////////////////////////////////////////

#include "mental.hpp"
#include "dsp/digital.hpp"

struct MentalCartesian : Module {
	enum ParamIds {
    KNOB_PARAM,
    OUT_QNT_SWITCH = KNOB_PARAM + 16,
		NUM_PARAMS
	};
  
	enum InputIds {		
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
	enum OutputIds {
		MAIN_OUT,
    ROW_OUT,
    COLUMN_OUT,    
		NUM_OUTPUTS
	};

  SchmittTrigger leftTrigger;
  SchmittTrigger rightTrigger;
  SchmittTrigger upTrigger;
  SchmittTrigger downTrigger;
  SchmittTrigger resetTrigger;
  SchmittTrigger x_resetTrigger;
  SchmittTrigger y_resetTrigger;
  
  SchmittTrigger quant_buttonTrigger;
  bool quantised = false;
  float quantised_led = 0.0;
  float grid_lights[4][4];
  
  /*for {int i = 0; i < 4 ; i++}
  {
    for {int j = 0; j < 4 ; j++}
    {
      grid_lights[i][j] = 0.0;
    }
  }*/
  
  int x_position = 0;
  int y_position = 0;
    
	MentalCartesian() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
	void step();
   
};

void MentalCartesian::step() {

  	bool step_right = false;
    bool step_left = false;
    bool step_up = false;
    bool step_down = false;
    
    // handle clock inputs
    if (inputs[RIGHT].active)
    {
			if (rightTrigger.process(inputs[RIGHT].value))
      {
				step_right = true;
			}
		}
    if (inputs[LEFT].active)
    {
			if (leftTrigger.process(inputs[LEFT].value))
      {
				step_left = true;
			}
		}
    if (inputs[DOWN].active)
    {
			if (downTrigger.process(inputs[DOWN].value))
      {
				step_down = true;
			}
		}
    if (inputs[UP].active)
    {
			if (upTrigger.process(inputs[UP].value))
      {
				step_up = true;
			}
		}
    // resets
    if (resetTrigger.process(inputs[RESET].value))
    {
      grid_lights[x_position][y_position] = 0.0;
		  x_position = 0;
      y_position = 0;
      grid_lights[x_position][y_position] = 1.0;
      step_right = false;
      step_left = false;
      step_up = false;	
      step_down = false;	
	  }
    if (x_resetTrigger.process(inputs[X_RESET].value))
    {
      grid_lights[x_position][y_position] = 0.0;
		  x_position = 0;
      grid_lights[x_position][y_position] = 1.0;
      step_right = false;
      step_left = false;
      step_up = false;	
      step_down = false;	
	  }
    if (y_resetTrigger.process(inputs[Y_RESET].value))
    {
      grid_lights[x_position][y_position] = 0.0;
		  y_position = 0;
      grid_lights[x_position][y_position] = 1.0;
      step_right = false;
      step_left = false;
      step_up = false;	
      step_down = false;	
	  }
    // change x and y    
    if (step_right)
    {
      grid_lights[x_position][y_position] = 0.0;
      x_position += 1;
      if (x_position > 3) x_position = 0;
      grid_lights[x_position][y_position] = 1.0;
    }
    if (step_left)
    {
      grid_lights[x_position][y_position] = 0.0;
      x_position -= 1;
      if (x_position < 0) x_position = 3;      
      grid_lights[x_position][y_position] = 1.0;
    }
    if (step_down)
    {
      grid_lights[x_position][y_position] = 0.0;
      y_position += 1;
      if (y_position > 3) y_position = 0;
      grid_lights[x_position][y_position] = 1.0;
    }
    if (step_up)
    {
      grid_lights[x_position][y_position] = 0.0;
      y_position -= 1;
      if (y_position < 0) y_position = 3;      
      grid_lights[x_position][y_position] = 1.0;
    }
    if (quant_buttonTrigger.process(params[OUT_QNT_SWITCH].value))
    {
      quantised = !quantised;
    }
    quantised_led = quantised ? 1.0 : 0.0;
    /// set outputs
    int which_knob = y_position * 4 + x_position;
    float main_out = params[KNOB_PARAM + which_knob].value;
    if (quantised)
    {
      int oct = round(main_out);
      float left = main_out - oct;
      int semi = round(left * 12);
      main_out = oct + semi/12.0;
    }
    outputs[MAIN_OUT].value = main_out;    
}

MentalCartesianWidget::MentalCartesianWidget() {
	MentalCartesian *module = new MentalCartesian();
	setModule(module);
	box.size = Vec(15*12, 380);
  
	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin,"res/MentalCartesian.svg")));
		addChild(panel);
	}
 
  int top = 20;
  int left = 3;
  int column_spacing = 30; 
  int row_spacing = 30;
  
	addInput(createInput<PJ301MPort>(Vec(left, top), module, MentalCartesian::LEFT));
  addInput(createInput<PJ301MPort>(Vec(left+column_spacing, top), module, MentalCartesian::RIGHT));
  addInput(createInput<PJ301MPort>(Vec(left+column_spacing * 2, top), module, MentalCartesian::X_CV));
  addInput(createInput<PJ301MPort>(Vec(left+column_spacing * 3, top), module, MentalCartesian::X_RESET));
  
  addInput(createInput<PJ301MPort>(Vec(left, top + row_spacing), module, MentalCartesian::UP));
  addInput(createInput<PJ301MPort>(Vec(left + column_spacing, top + row_spacing), module, MentalCartesian::DOWN));  
  addInput(createInput<PJ301MPort>(Vec(left + column_spacing * 2, top + row_spacing), module, MentalCartesian::Y_CV));
  addInput(createInput<PJ301MPort>(Vec(left + column_spacing * 3, top + row_spacing), module, MentalCartesian::Y_RESET));
    
  addInput(createInput<PJ301MPort>(Vec(left + column_spacing * 3, top + row_spacing * 2), module, MentalCartesian::RESET));
  
  addOutput(createOutput<PJ301MPort>(Vec(150, 20), module, MentalCartesian::MAIN_OUT));
  addParam(createParam<LEDButton>(Vec(150, 50), module, MentalCartesian::OUT_QNT_SWITCH, 0.0, 1.0, 0.0));
	addChild(createValueLight<SmallLight<GreenValueLight>>(Vec(150+5, 50+5), &module->quantised_led));
	addOutput(createOutput<PJ301MPort>(Vec(150, 100), module, MentalCartesian::ROW_OUT));
	addOutput(createOutput<PJ301MPort>(Vec(150, 130), module, MentalCartesian::COLUMN_OUT));
  
  for ( int i = 0 ; i < 4 ; i++)
  {
    for ( int j = 0 ; j < 4 ; j++)
    {
      addParam(createParam<Davies1900hSmallBlackKnob>(Vec(left+column_spacing * i, top + row_spacing * j + 150 ), module, MentalCartesian::KNOB_PARAM + i + j * 4, 0.0, 1.0, 0.0));
      addChild(createValueLight<SmallLight<GreenValueLight>>(Vec(left+column_spacing * i + 10, top + row_spacing * j + 150 + 10), &module->grid_lights[i][j]));
    }
	}
  
	
}
