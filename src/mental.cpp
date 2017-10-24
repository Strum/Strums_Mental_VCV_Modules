#include "mental.hpp"
#include <math.h>
//#include "dsp.hpp"

Plugin *plugin;

void init(rack::Plugin *p) {
	plugin = p;
	plugin->slug = "mental";
	plugin->name = "mental";

    createModel<MentalSubMixerWidget>(plugin, "MentalSubMixer", "Mental Sub Mixer");
    createModel<MentalMultsWidget>(plugin, "MentalMults", "Mental Mults");
    createModel<MentalMixerWidget>(plugin, "MentalMixer", "Mental Mixer");
    createModel<MentalFoldWidget>(plugin, "MentalFold", "Mental Wave Folder");
    createModel<MentalClipWidget>(plugin, "MentalClip", "Mental Wave Clipper");
    createModel<MentalGatesWidget>(plugin, "MentalGates", "Mental Gates");
    createModel<MentalABSwitchesWidget>(plugin, "MentalABSwitches", "Mental A/B Switches");
    //createModel<MentalNoiseGateWidget>(plugin, "MentalNoiseGate", "Mental Noise Gate");
    createModel<MentalQuantiserWidget>(plugin, "MentalQuantiser", "Mental Quantiser");
    createModel<MentalChordWidget>(plugin, "MentalChord", "Mental Chord");
    createModel<MentalMuxesWidget>(plugin, "MentalMuxes", "Mental Muxes");
    createModel<MentalLogicWidget>(plugin, "MentalLogic", "Mental Logic");
    createModel<MentalButtonsWidget>(plugin, "MentalButtons", "Mental Buttons");
    createModel<MentalSumsWidget>(plugin, "MentalSums", "Mental Sums");
    createModel<MentalPitchShiftWidget>(plugin, "MentalPitchShift", "Mental Pitch Shifter");
    createModel<MentalClockDividerWidget>(plugin, "MentalClockDivider", "Mental Clock Divider");
    createModel<MentalCartesianWidget>(plugin, "MentalCartesian", "Mental Cartesian Sequencer");
    createModel<MentalPatchMatrixWidget>(plugin, "MentalPatchMatrix", "Mental Patch Matrix"); 
    createModel<MentalBinaryDecoderWidget>(plugin, "MentalBinaryDecoder", "Mental Binary Decoder");
    createModel<MentalSwitch8Widget>(plugin, "MentalSwitch8", "Mental 8 Way Switch");
    createModel<MentalMux8Widget>(plugin, "MentalMux8", "Mental 8 to 1 Mux");
    createModel<MentalCountersWidget>(plugin, "MentalCounters", "Mental Counters");       
}
