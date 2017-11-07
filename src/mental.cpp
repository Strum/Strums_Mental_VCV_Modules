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

    p->addModel(createModel<MentalSubMixerWidget>("mental", "mental", "MentalSubMixer", "Mental Sub Mixer"));
    p->addModel(createModel<MentalMultsWidget>("mental", "mental", "MentalMults", "Mental Mults"));
    p->addModel(createModel<MentalMixerWidget>("mental", "mental", "MentalMixer", "Mental Mixer"));
    p->addModel(createModel<MentalFoldWidget>("mental", "mental", "MentalFold", "Mental Wave Folder"));
    p->addModel(createModel<MentalClipWidget>("mental", "mental", "MentalClip", "Mental Wave Clipper"));
    p->addModel(createModel<MentalGatesWidget>("mental", "mental", "MentalGates", "Mental Gates"));
    p->addModel(createModel<MentalABSwitchesWidget>("mental", "mental", "MentalABSwitches", "Mental A/B Switches"));
    //p->addModel(createModel<MentalNoiseGateWidget>("mental", "mental", "MentalNoiseGate", "Mental Noise Gate"));
    p->addModel(createModel<MentalQuantiserWidget>("mental", "mental", "MentalQuantiser", "Mental Quantiser"));
    p->addModel(createModel<MentalChordWidget>("mental", "mental", "MentalChord", "Mental Chord"));
    p->addModel(createModel<MentalMuxesWidget>("mental", "mental", "MentalMuxes", "Mental Muxes"));
    p->addModel(createModel<MentalLogicWidget>("mental", "mental", "MentalLogic", "Mental Logic"));
    p->addModel(createModel<MentalButtonsWidget>("mental", "mental", "MentalButtons", "Mental Buttons"));
    p->addModel(createModel<MentalSumsWidget>("mental", "mental", "MentalSums", "Mental Sums"));
    p->addModel(createModel<MentalPitchShiftWidget>("mental", "mental", "MentalPitchShift", "Mental Pitch Shifter"));
    p->addModel(createModel<MentalClockDividerWidget>("mental", "mental", "MentalClockDivider", "Mental Clock Divider"));
    p->addModel(createModel<MentalCartesianWidget>("mental", "mental", "MentalCartesian", "Mental Cartesian Sequencer"));
    p->addModel(createModel<MentalPatchMatrixWidget>("mental", "mental", "MentalPatchMatrix", "Mental Patch Matrix")); 
    p->addModel(createModel<MentalBinaryDecoderWidget>("mental", "mental", "MentalBinaryDecoder", "Mental Binary Decoder"));
    p->addModel(createModel<MentalSwitch8Widget>("mental", "mental", "MentalSwitch8", "Mental 8 Way Switch"));
    p->addModel(createModel<MentalMux8Widget>("mental", "mental", "MentalMux8", "Mental 8 to 1 Mux"));
    p->addModel(createModel<MentalCountersWidget>("mental", "mental", "MentalCounters", "Mental Counters"));
    p->addModel(createModel<MentalKnobsWidget>("mental", "mental", "MentalKnobs", "Mental Knobs"));
    p->addModel(createModel<MentalGateMakerWidget>("mental", "mental", "MentalGateMaker", "Mental Gate Maker"));    
    p->addModel(createModel<MentalMasterClockWidget>("mental", "mental", "MentalMasterClock", "MentalMasterClock"));
    p->addModel(createModel<MentalPatchNotesWidget>("mental", "mental", "MentalPatchNotes", "MentalPatchNotes"));      
}
