///////////////////////////////////////////////////
//
//   Mental Plugin for VCV Version 1
//
//   Strum 2018-19
//   strum@softhome.net
//
///////////////////////////////////////////////////
// still to do
// fix master clock, sequencer
// redo guis

#include "rack.hpp"

using namespace rack;

extern Plugin *pluginInstance;

////////////////////
// module widgets
////////////////////


extern Model *modelMentalMults;
extern Model *modelMentalSubMixer;
extern Model *modelMentalMixer;
extern Model *modelMentalFold;
extern Model *modelMentalClip;
extern Model *modelMentalGates;
extern Model *modelMentalABSwitches;
extern Model *modelMentalQuantiser;
extern Model *modelMentalChord;
extern Model *modelMentalMuxes;
extern Model *modelMentalLogic;
extern Model *modelMentalButtons;
extern Model *modelMentalSums;
extern Model *modelMentalPitchShift;
extern Model *modelMentalClockDivider;
extern Model *modelMentalCartesian;
extern Model *modelMentalPatchMatrix;
extern Model *modelMentalBinaryDecoder;
extern Model *modelMentalSwitch8;
extern Model *modelMentalMux8;
extern Model *modelMentalCounters;
extern Model *modelMentalKnobs; 
extern Model *modelMentalGateMaker;
extern Model *modelMentalMasterClock;
extern Model *modelMentalQuadLFO; 
extern Model *modelMentalRadioButtons;


/////////////////////////////////////////////
// Components

// Ports

// InPort

struct InPort : SvgPort
{
	InPort()
	{
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/InPort.svg")));
	}
};

// OutPort

struct OutPort : SvgPort
{
	OutPort()
	{
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/OutPort.svg")));
	}
};

// GateInPort

struct GateInPort : SvgPort
{
	GateInPort()
	{
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/GateInPort.svg")));
	}
};

// GateOutPort

struct GateOutPort : SvgPort
{
	GateOutPort()
	{
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/GateOutPort.svg")));
	}
};

// GateInPort

struct CVInPort : SvgPort
{
	CVInPort()
	{
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/CVInPort.svg")));
	}
};

// GateOutPort

struct CVOutPort : SvgPort
{
	CVOutPort()
	{
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/CVOutPort.svg")));
	}
};

// ThreeWaySwitch

struct ThreeWaySwitch : SvgSwitch
{
	ThreeWaySwitch()
	{
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/Three_0.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/Three_1.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/Three_2.svg")));
	}
};

struct VThreeWaySwitch : SvgSwitch
{
	VThreeWaySwitch()
	{
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/VThree_0.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/VThree_1.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/VThree_2.svg")));
	}
};


// Knobs

struct LrgKnob : RoundKnob
{
	LrgKnob()
	{
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/LrgKnob.svg")));
		box.size = Vec(45,45);
	}
};

struct MedKnob : RoundKnob
{
	MedKnob()
	{
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/MedKnob.svg")));
		box.size = Vec(32,32);
	}
};

struct SmlKnob : RoundKnob
{
	SmlKnob()
	{
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/SmlKnob.svg")));
		box.size = Vec(25,25);
	}
};

/// lights

struct MentalLight : GrayModuleLightWidget
{
	MentalLight() { bgColor = nvgRGB(0x40, 0x40, 0x40); }
};
struct RedLED : MentalLight
{
 	RedLED() { addBaseColor(nvgRGB(0xff, 0x00, 0x00)); }
};

struct BlueLED : MentalLight
{
 	BlueLED() { addBaseColor(nvgRGB(0x00, 0x47, 0x7e)); }
};

struct OrangeLED : MentalLight
{
 	OrangeLED() { addBaseColor(nvgRGB(0xff, 0xA5, 0x00)); }
};

template <typename BASE>
 struct TinyLight : BASE
 {
 	TinyLight() { this->box.size = Vec(4, 4); }
 };

template <typename BASE>
 struct SmlLight : BASE
 {
 	SmlLight() { this->box.size = Vec(8, 8); }
 };

template <typename BASE>
 struct MedLight : BASE
 {
 	MedLight(){ this->box.size = Vec(10, 10);}
 };