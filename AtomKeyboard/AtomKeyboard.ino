#include "M5AtomS3.h"
#include "EspUsbHost.h"
#include "M5UnitSynth.h"

M5Canvas canvas(&M5.Lcd);

//colors
unsigned short myblue=0x020A;
unsigned short arduinoCol=0x0B8D;
unsigned short gray=0xBDF7;
unsigned short darkGra=0x3186;

int n=0;  //keycode for draw
int ins=0;  //instrument
int duration[6]={100,200,300,400,600,800};
int dur=2; // chosen duration
M5UnitSynth synth;
unsigned long timePlayed=0; bool playing=false; bool found=0;
// a=4
int codes[14]={57,4,22,7,9,10,11,13,14,15,51,52,49,40};
int notes[14] = {
  NOTE_B3,
  NOTE_C4,  // C4
  NOTE_D4,  // D4
  NOTE_E4,  // E4
  NOTE_F4,  // F4
  NOTE_G4,  // G4
  NOTE_A4,  // A4
  NOTE_B4,  // B4
  NOTE_C5,  // C5
  NOTE_D5,  // D5
  NOTE_E5,  // E5
  NOTE_F5,  // F5
  NOTE_G5,  // G5
  NOTE_A5
};

int sharpCodes[8]={26,8,23,28,24,18,19,48};
int sharpNotes[8]={
  NOTE_CS4,  // C4
  NOTE_DS4,  // D4
  NOTE_FS4,  // F4
  NOTE_GS4,  // G4
  NOTE_AS4,  // A4
  NOTE_CS5,  // C5
  NOTE_DS5,  // D5
  NOTE_FS5,  // F5
};

String instrumentNames[] = {
    "GrandPiano_1",
    "BrightPiano_2",
    "ElGrdPiano_3",
    "HonkyTonkPiano",
    "ElPiano1",
    "ElPiano2",
    "Harpsichord",
    "Clavi",
    "Celesta",
    "Glockenspiel",
    "MusicBox",
    "Vibraphone",
    "Marimba",
    "Xylophone",
    "TubularBells",
    "Santur",
    "DrawbarOrgan",
    "PercussiveOrgan",
    "RockOrgan",
    "ChurchOrgan",
    "ReedOrgan",
    "AccordionFrench",
    "Harmonica",
    "TangoAccordion",
    "AcGuitarNylon",
    "AcGuitarSteel",
    "AcGuitarJazz",
    "AcGuitarClean",
    "AcGuitarMuted",
    "OverdrivenGuitar",
    "DistortionGuitar",
    "GuitarHarmonics",
    "AcousticBass",
    "FingerBass",
    "PickedBass",
    "FretlessBass",
    "SlapBass1",
    "SlapBass2",
    "SynthBass1",
    "SynthBass2",
    "Violin",
    "Viola",
    "Cello",
    "Contrabass",
    "TremoloStrings",
    "PizzicatoStrings",
    "OrchestralHarp",
    "Timpani",
    "StringEnsemble1",
    "StringEnsemble2",
    "SynthStrings1",
    "SynthStrings2",
    "ChoirAahs",
    "VoiceOohs",
    "SynthVoice",
    "OrchestraHit",
    "Trumpet",
    "Trombone",
    "Tuba",
    "MutedTrumpet",
    "FrenchHorn",
    "BrassSection",
    "SynthBrass1",
    "SynthBrass2",
    "SopranoSax",
    "AltoSax",
    "TenorSax",
    "BaritoneSax",
    "Oboe",
    "EnglishHorn",
    "Bassoon",
    "Clarinet",
    "Piccolo",
    "Flute",
    "Recorder",
    "PanFlute",
    "BlownBottle",
    "Shakuhachi",
    "Whistle",
    "Ocarina",
    "Lead1Square",
    "Lead2Sawtooth",
    "Lead3Calliope",
    "Lead4Chiff",
    "Lead5Charang",
    "Lead6Voice",
    "Lead7Fifths",
    "Lead8BassLead",
    "Pad1Fantasia",
    "Pad2Warm",
    "Pad3PolySynth",
    "Pad4Choir",
    "Pad5Bowed",
    "Pad6Metallic",
    "Pad7Halo",
    "Pad8Sweep",
    "FX1Rain",
    "FX2Soundtrack",
    "FX3Crystal",
    "FX4Atmosphere",
    "FX5Brightness",
    "FX6Goblins",
    "FX7Echoes",
    "FX8SciFi",
    "Sitar",
    "Banjo",
    "Shamisen",
    "Koto",
    "Kalimba",
    "BagPipe",
    "Fiddle",
    "Shanai",
    "TinkleBell",
    "Agogo",
    "SteelDrums",
    "Woodblock",
    "TaikoDrum",
    "MelodicTom",
    "SynthDrum",
    "ReverseCymbal",
    "GtFretNoise",
    "BreathNoise",
    "Seashore",
    "BirdTweet",
    "TelephRing",
    "Helicopter",
    "Applause",
    "Gunshot"
};


void draw()
{
   canvas.fillScreen(BLACK);
   canvas.setTextColor(MAGENTA,BLACK);
   canvas.drawString("2024",100,6);
   canvas.setTextColor(gray,BLACK);
   canvas.drawString("Volos Synth",10,2,2);
   canvas.setTextColor(ORANGE,BLACK);
   canvas.drawString("INST: "+String(ins),10,30,4);
   canvas.setTextColor(gray,BLACK);
   canvas.drawString(instrumentNames[ins],10,54,2);

   canvas.fillRect(8,20,116,4,darkGra);
   canvas.fillRect(6,77,68,50,myblue);
   canvas.fillRect(78,77,48,50,arduinoCol);
   canvas.setTextColor(WHITE,myblue);
   canvas.drawString("DURATION",10,80,2);
   canvas.drawString(String(duration[dur]),10,100,4);
   
   canvas.setTextColor(WHITE,arduinoCol);
   canvas.drawString("KEY",84,80,2);
   canvas.drawString(String(n),84,100,4);
   canvas.pushSprite(0, 0);
    
}

class MyEspUsbHost : public EspUsbHost {
  void onKeyboardKey(uint8_t ascii, uint8_t keycode, uint8_t modifier) {

 
   for(int i=0;i<14;i++){
      if(codes[i]==keycode){
      synth.setNoteOn(0, notes[i], 127);
      playing=true;
      timePlayed=millis();
      i=12;
      }
   }

     for(int i=0;i<8;i++){
      if(sharpCodes[i]==keycode){
      synth.setNoteOn(0, sharpNotes[i], 127);
      playing=true;
      timePlayed=millis();
      i=8;
      }
   }

   if(keycode==79)
   {
    dur++;
    if(dur==6)
    dur=0;
   }

   if(keycode==80)
   {
    dur--;
    if(dur<0)
    dur=5;
   }

    if(keycode==82)
   {
    ins++;
    if(ins>127)
    ins=0;
    synth.setInstrument(0, 0, ins); 
   }

   if(keycode==81)
   {
    ins--;
    if(ins<0)
    ins=127;
    synth.setInstrument(0, 0, ins); 
   }



   n=keycode;
   draw();

  };
};

MyEspUsbHost usbHost;
void setup() {
    auto cfg = M5.config();
    AtomS3.begin(cfg);
    AtomS3.Display.setBrightness(60);
    synth.begin(&Serial2, UNIT_SYNTH_BAUD, 1, 2);
    synth.setInstrument(0, 0, ins); 
    usbHost.begin();
    usbHost.setHIDLocal(HID_LOCAL_Japan_Katakana);
    canvas.createSprite(128, 128);
    draw();


}

void loop() {
 usbHost.task(); 

 if(playing)  
 if(millis()>timePlayed+duration[dur]){
  synth.setAllNotesOff(0);
  playing=false;
 }

 AtomS3.update();
 if (AtomS3.BtnA.wasPressed())
 {
  ins=0; dur=2; 
  synth.setInstrument(0, 0, ins); 
  draw();
 }

}