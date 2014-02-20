#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "AudioUnit.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class AUSpeechSynthApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );
    void keyDown( KeyEvent event );
	void update();
	void draw();
    
    // GenericUnit is the root object of all Audio Units in this cinder block.
	// It can be configured to host any available Audio Unit.
	// This one will be configured to host the AUMatrixReverb unit later in setup().
	au::GenericUnit reverb;
    
	// There are several subclasses of GenericUnit which provide helpful
	// convenience functions and utilities. The following are all subclasses
	// of GenericUnit
	au::SpeechSynth speechSynth; // <- speaks text out loud
	au::Mixer mixer; // <- multichannel mixer w/ stereo pan
	au::Output output; // <- output unit which sends audio to speakers
    
	// The Tap is an object that you can stick inside Audio Unit chains to
	// extract samples (for audio analysis, visualization, etc).
	au::Tap tap;
    
	// Samples are extracted from the Tap via a TapSampleBuffer
	au::TapSampleBuffer sampleBuffer;
    
    
};

void AUSpeechSynthApp::setup()
{
    // Audio Units are accessed by their description, which consist of three
	// OSTypes (which are represented as 4-char sequences between ' characters)
    
	// The description for the reverb unit is 'aufx' 'mrev' 'appl'
	// This breaks down as follows:
	// 'aufx' = the type of audio unit (in this case, an effect)
	// 'mrev' = the specific unit (in this case, the matrix reverb)
	// 'appl' = it's made by Apple
    
	// Each of the Apple-defined OSTypes also has a corresponding constant.
	// For example, the description 'aufx' 'mrev' 'appl' could also be expressed as
	// kAudioUnitType_Effect, kAudioUnitSubType_MatrixReverb, kAudioUnitManufacturer_Apple
    
	// Here, we're configuring the reverb GenericUnit to represent the matrix reverb.
	// Note that kAudioUnitManufacturer_Apple is the default for the third argument,
	// so it isn't necessary here.
	reverb = au::GenericUnit(kAudioUnitType_Effect, kAudioUnitSubType_MatrixReverb);
    
	// Audio Units are typically hooked up to each other to do things. Here we're
	// connecting up a chain which goes speech synth -> reverb -> tap -> mixer -> output
	speechSynth.connectTo(reverb).connectTo(tap).connectTo(mixer).connectTo(output);
    
	// Audio Units work on a "pull" model, which means that the output unit drives
	// the chain be requesting audio from the mixer, which requests audio from the
	// reverb unit and so on. Here, we tell the output unit to start pulling audio
	output.start();
    
	// Now, the output unit is pulling audio from the speech synth, through the
	// effect units and then sending the resulting audio to the speakers. Since the
	// speech synth doesn't have anything to say yet, it's just producing silence.
	// In mouseDown() we'll give the speech synth something to say.
	// (Aside: The speech synth is pretty handy as a debug mechanism to check your setup)
}

void AUSpeechSynthApp::mouseDown( MouseEvent event )
{
    std::string hello("Höre! Still jetzt, dies ist eine Demonstration, eine Demonstration der Neukölln Klang Recherche. ");
	hello.append("Aus ! Aus 24 Stunden Klangmaterial, gefunden am alltäglichen Ort der Finderinnen und Finder, ");
	hello.append("aufgezeichnet, geschnitten, abgelegt als Reisebericht, als Marsch durch das Unverständnis ");
	hello.append("auseinandergenommen und gewürfelt, Mimikry in schwarzen Dosen ");
    hello.append("Pathos und Packeeeeeeeeetdienst,Packeeeeeeeeeeeeeetdienst, Spielhöllöllöllöllöllöllöllöleeeeee und Alleeeeeeeeeeeeeeeen ");
	hello.append("musikalisch bearbeitet, synthetisiert, und präsentiert all denen ");
	hello.append("die hören, die verstehen wollen, wollen ist ganz wichtig, gaaaaaaaaaaanz, gaaaaaaaaaaaanz, die nicht glauben wollen, was unhörbar ist, unhörbar ist.");
    hello.append("Höre, Neukölln! .... Stille jetzt.");
	speechSynth.say(hello);
}

void AUSpeechSynthApp::keyDown( KeyEvent event )
{
	if(event.getChar() == 'r') {
		reverb.showUI();
	}
}

void AUSpeechSynthApp::update()
{
    // The mixer's pan setting should be between -1 and 1 (full left to full right)
	float pan = (getMousePos().x - getWindowPosX()) / (float)getWindowWidth();
	pan = (pan * 2) - 1;
	if(pan >  1) pan = 1;
	if(pan < -1) pan = -1;
	mixer.setPan(pan);
}

void AUSpeechSynthApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
    
	// samples extracted from a tap will be in the range of -1 to 1
	tap.getSamples(sampleBuffer);
	PolyLine2f waveform;
	for(int i = 0; i < sampleBuffer.size(); i++) {
		float x = i / (float)sampleBuffer.size() * (float)getWindowWidth();
		float y = ((sampleBuffer[i] + 1) / 2.) * (float)getWindowHeight();
		waveform.push_back(Vec2f(x,y));
	}
    
	gl::draw(waveform);
}

CINDER_APP_NATIVE( AUSpeechSynthApp, RendererGl )
