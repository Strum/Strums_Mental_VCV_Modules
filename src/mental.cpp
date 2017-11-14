#include "mental.hpp"
#include <math.h>
//#include "dsp.hpp"

Plugin *plugin;

void init(rack::Plugin *p) {
	plugin = p;
	plugin->slug = "mental";
#ifdef VERSION
	p->version = TOSTRING(VERSION);
#endif
  p->website = "https://github.com/Strum/Strums_Mental_VCV_Modules";
  
    p->addModel(createModel<MentalSubMixerWidget>("mental", "MentalSubMixer", "Mental Sub Mixer", MIXER_TAG, PANNING_TAG));
    p->addModel(createModel<MentalMultsWidget>("mental", "MentalMults", "Mental Mults", DUAL_TAG, MULTIPLE_TAG));
    p->addModel(createModel<MentalMixerWidget>("mental", "MentalMixer", "Mental Mixer", MIXER_TAG, PANNING_TAG));
    p->addModel(createModel<MentalFoldWidget>("mental", "MentalFold", "Mental Wave Folder", WAVESHAPER_TAG));
    p->addModel(createModel<MentalClipWidget>("mental", "MentalClip", "Mental Wave Clipper", DISTORTION_TAG));
    p->addModel(createModel<MentalGatesWidget>("mental", "MentalGates", "Mental Gates", UTILITY_TAG));
    p->addModel(createModel<MentalABSwitchesWidget>("mental", "MentalABSwitches", "Mental A/B Switches", SWITCH_TAG, UTILITY_TAG));
    //p->addModel(createModel<MentalNoiseGateWidget>("mental", "mental", "MentalNoiseGate", "Mental Noise Gate"));
    p->addModel(createModel<MentalQuantiserWidget>("mental", "MentalQuantiser", "Mental Quantiser", QUANTIZER_TAG));
    p->addModel(createModel<MentalChordWidget>("mental", "MentalChord", "Mental Chord", CONTROLLER_TAG));
    p->addModel(createModel<MentalMuxesWidget>("mental", "MentalMuxes", "Mental Muxes", UTILITY_TAG));
    p->addModel(createModel<MentalLogicWidget>("mental", "MentalLogic", "Mental Logic", LOGIC_TAG));
    p->addModel(createModel<MentalButtonsWidget>("mental", "MentalButtons", "Mental Buttons", UTILITY_TAG));
    p->addModel(createModel<MentalSumsWidget>("mental", "MentalSums", "Mental Sums", UTILITY_TAG));
    p->addModel(createModel<MentalPitchShiftWidget>("mental", "MentalPitchShift", "Mental Pitch Shifter", UTILITY_TAG));
    p->addModel(createModel<MentalClockDividerWidget>("mental", "MentalClockDivider", "Mental Clock Divider", UTILITY_TAG));
    p->addModel(createModel<MentalCartesianWidget>("mental", "MentalCartesian", "Mental Cartesian Sequencer", SEQUENCER_TAG));
    p->addModel(createModel<MentalPatchMatrixWidget>("mental", "MentalPatchMatrix", "Mental Patch Matrix", UTILITY_TAG)); 
    p->addModel(createModel<MentalBinaryDecoderWidget>("mental", "MentalBinaryDecoder", "Mental Binary Decoder", UTILITY_TAG));
    p->addModel(createModel<MentalSwitch8Widget>("mental", "MentalSwitch8", "Mental 8 Way Switch", UTILITY_TAG));
    p->addModel(createModel<MentalMux8Widget>("mental", "MentalMux8", "Mental 8 to 1 Mux", UTILITY_TAG));
    p->addModel(createModel<MentalCountersWidget>("mental", "MentalCounters", "Mental Counters", UTILITY_TAG));
    p->addModel(createModel<MentalKnobsWidget>("mental", "MentalKnobs", "Mental Knobs", UTILITY_TAG));
    p->addModel(createModel<MentalGateMakerWidget>("mental", "MentalGateMaker", "Mental Gate Maker", UTILITY_TAG));    
    p->addModel(createModel<MentalMasterClockWidget>("mental", "MentalMasterClock", "MentalMasterClock", CLOCK_TAG));
    p->addModel(createModel<MentalPatchNotesWidget>("mental", "MentalPatchNotes", "MentalPatchNotes", UTILITY_TAG));      
}
