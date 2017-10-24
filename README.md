Strum's Mental VCV Module Suite

strum@softhome.net

Simple but useful modules for VCVRack

Version : 0.4d

Works with Rack 0.4.0 only, api changes from 0.3 versions broke it and i didn't want to apply conditional compilation when
in a few weeks noboby will be using version 0.3 anymore anyway. 

Based largely on the fundamental modules but I have also used some code from other module
developers and learned various ways of doing things from the other suites of modules as well.

The modules are mostly utilities, which you can never have enough of, and I have
focused on keeping them small to pack a lot of functionality into as little possible screen area.

Currently Compiles against the lastest code as of 13 Oct 2017.

Module List

Chord - 
  Inspired by Q-Bit chord Eurorack module, takes input pitch and builds a chord
  from it based on various parameters and logic inputs.

Gates - 
  Manual or remotely controllable gates

Logic - 
  Various logic gates

Mults - 
  Simple 2*5 Multiples

Muxes - 
  2 * 2 into 1 muxes and 1 4 into 1 mux

Quantiser - 
  Takes inputs pitchs and quantises to scale selected by buttons for each pitch
  Also functions as a pitch reference

SubMixer - 
  4 Channel stereo mixer with pan based on fundametal VCMixer

A/B Switches - 
  A/B switches

Buttons - 
  Buttons for controlling other modules manually
  7 On/off switches in the top section and 7 momentary buttons in the lower.
  
Mixer - 
  12 Channel Mixer with 2 auxiliary sends and returns and channel mutes.

Clipper - 
  Simple Clipping distortion

Wave Folder - 
  Simple Waver folder
  
Pitch Shifter - 
  Shift pitches by octaves or semitones

Sums - 
  2 * 5 into 1 summing mixers, useful for adding cvs for pitch and modulation   

Clock Divider - 
  Based on autodafe's clock divider but I've simplified the code, made the gui
  smaller and added extra divisions 3,5,7 and 12
  
Cartesian Sequencer - 
  Cartesian sequencer similar to make noise rene.
  
Patch Matrix - 
  10 by 10 Patch matrix
  
Binary decoder - 
  3 bit binary decoder true at one of 8 outputs dependent on logic state of the 3 inputs
  
8 way switch - based on binary decoder, switches a signal to one of 8 outputs based on 3 bit selector inputs

8 to 1 Mux - The reverse of the 8 way switch, 8 inputs 1 out selected with 3 logic inputs

Dual Counters - for euclidean rhythms

