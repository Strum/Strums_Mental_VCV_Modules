///////////////////////////////////////////////////
//
//   Mental Plugin for VCV Version 1
//
//   Strum 2018-22
//   strum@sodaisland.net
//
///////////////////////////////////////////////////

#include "mental.hpp"
#include <math.h>

Plugin *pluginInstance;

void init(rack::Plugin *p)
{
    pluginInstance = p;
    
    p->addModel(modelMentalMults);
    p->addModel(modelMentalSubMixer);
    p->addModel(modelMentalMixer);
    p->addModel(modelMentalFold);
    p->addModel(modelMentalClip);
    p->addModel(modelMentalGates);
    p->addModel(modelMentalABSwitches);
    p->addModel(modelMentalQuantiser);
    p->addModel(modelMentalChord);
    p->addModel(modelMentalMuxes);
    p->addModel(modelMentalLogic);
    p->addModel(modelMentalButtons);
    p->addModel(modelMentalSums);
    p->addModel(modelMentalPitchShift);
    p->addModel(modelMentalClockDivider);
    p->addModel(modelMentalCartesian);
    p->addModel(modelMentalPatchMatrix);
    p->addModel(modelMentalBinaryDecoder);
    p->addModel(modelMentalSwitch8);
    p->addModel(modelMentalMux8);
    p->addModel(modelMentalCounters);
    p->addModel(modelMentalKnobs);
    p->addModel(modelMentalGateMaker);
    p->addModel(modelMentalMasterClock);
    p->addModel(modelMentalQuadLFO);
    p->addModel(modelMentalRadioButtons);         
}
