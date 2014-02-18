#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class AUSpeechSynthApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void AUSpeechSynthApp::setup()
{
}

void AUSpeechSynthApp::mouseDown( MouseEvent event )
{
}

void AUSpeechSynthApp::update()
{
}

void AUSpeechSynthApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( AUSpeechSynthApp, RendererGl )
