///////////////////////////////////////////////////
//
//   Mental Plugin for VCV Version 1
//
//   Strum 2017-21
//   strum@softhome.net
//
///////////////////////////////////////////////////

#include "mental.hpp"
#include <math.h>

Plugin *pluginInstance;

void init(rack::Plugin *p)
{
    pluginInstance = p;

    p->addModel(modelMults);
    p->addModel(modelSubMixer);
    p->addModel(modelMixer);
    p->addModel(modelFold);
    p->addModel(modelClip);
    p->addModel(modelGates);
    p->addModel(modelABSwitches);
    p->addModel(modelQuantiser);
    p->addModel(modelChord);
    p->addModel(modelMuxes);
    p->addModel(modelLogic);
    p->addModel(modelButtons);
    p->addModel(modelSums);
    p->addModel(modelPitchShift);
    p->addModel(modelClockDivider);
    p->addModel(modelCartesian);
    p->addModel(modelPatchMatrix);
    p->addModel(modelBinaryDecoder);
    p->addModel(modelSwitch8);
    p->addModel(modelMux8);
    p->addModel(modelCounters);
    p->addModel(modelKnobs);
    p->addModel(modelGateMaker);
    p->addModel(modelMasterClock);
    p->addModel(modelQuadLFO);
    p->addModel(modelRadioButtons);
}
