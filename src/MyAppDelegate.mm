//
//
//

#import "MyAppDelegate.h"
#import "ofApp.h"

static NSString *const AUDIOBUS_API_KEY= @"H4sIAAAAAAAAA1WMUQ6CMBBE77LfSBF+DIfwAtaQUhZthJZ0uwZCuLsrITF+znszswLOk4sL1EUGLftuwMabEaGGa3CEI5OzL8iA49CQfeKu/E+dyrzIDXcutEy1VlpJeQoxEdS3FdIyfQeG40P48Rw4SeiQbHRTcsH/X4ojbo+ldM8CRuO5NzZxxCi0H3gW+sZI+7zc7hm4ToxWByStXrhoVRWXCrYPMcQ5E+gAAAA=:HUnk29UKzDS/PD9UHOVxl7jTU+vlW0DBav9t15JARyghFQdYcrAO0a2qUJkKM6304PIi0uIY2ijkcbaJiGKRLNuaNXNSkOBqYzPRdKLH9oYkgC/Z7ye5IyOmCt4j8bUY";

@implementation MyAppDelegate 

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    
    [super applicationDidFinishLaunching:application];
    ofApp *app = new ofApp();
    self.glViewController = [[ofxiOSViewController alloc] initWithFrame:[[UIScreen mainScreen] bounds] app:app ];
    [self.window setRootViewController:self.glViewController];
    
    ofOrientation requested = ofGetOrientation();
    UIInterfaceOrientation interfaceOrientation = UIInterfaceOrientationPortrait;
    interfaceOrientation = UIInterfaceOrientationLandscapeRight;
    switch (requested) {
        case OF_ORIENTATION_DEFAULT:
            interfaceOrientation = UIInterfaceOrientationLandscapeLeft;
            break;
        case OF_ORIENTATION_180:
            interfaceOrientation = UIInterfaceOrientationLandscapeRight;
            break;
        case OF_ORIENTATION_90_RIGHT:
            interfaceOrientation = UIInterfaceOrientationLandscapeLeft;
            break;
        case OF_ORIENTATION_90_LEFT:
            interfaceOrientation = UIInterfaceOrientationLandscapeRight;
            break;
        case OF_ORIENTATION_UNKNOWN:
            interfaceOrientation = UIInterfaceOrientationLandscapeLeft;
            break;
    }
    
    [self.glViewController rotateToInterfaceOrientation:UIInterfaceOrientationPortrait animated:false];
    
    app->setupAudioStream();
    SoundOutputStream *stream = app->getSoundStream()->getSoundOutStream();
    /* You need to set the AudioSession settings again, setupSoundStream() I think sets it to AVAudioSessionCategoryPlayAndRecord? In any case, without calling this after setupSoundStream i could not start from within Audiobus without sound issues. */

    [[AVAudioSession sharedInstance] setActive:YES error:NULL];
    AudioOutputUnitStart(stream.audioUnit);
    [[AVAudioSession sharedInstance] setCategory: AVAudioSessionCategoryPlayback
                                     withOptions: AVAudioSessionCategoryOptionMixWithOthers
                                     error:  NULL];
   
    self.audiobusController = [[ABAudiobusController alloc] initWithApiKey:AUDIOBUS_API_KEY];

    self.audiobusSender = [[ABAudioSenderPort alloc]
                                   initWithName:@"out"
                                   title:NSLocalizedString(@"out", @"")
                                   audioComponentDescription:(AudioComponentDescription) {
                                        .componentType = kAudioUnitType_RemoteGenerator,
                                        .componentSubType = 'out1', // Note single quotes
                                        //this needs to match the audioComponents entry
                                        .componentManufacturer = 'flux' }
                                   audioUnit:stream.audioUnit];
    [_audiobusController addAudioSenderPort:_audiobusSender];
    self.audiobusController.connectionPanelPosition = ABConnectionPanelPositionTop;
    return YES;
    
}

//- (void)applicationDidEnterBackground:(UIApplication *)application {
-(void)applicationDidEnterBackground:(NSNotification *)notification {
    [ofxiOSGetGLView() stopAnimation];
    glFinish();
    ofLog(OF_LOG_VERBOSE, "holdIsOff %i",(dynamic_cast<ofApp*>(ofGetAppPtr())->holdIsOff()));
    //only continue to generate sound when not connected to anything and hold is off
    if ((!_audiobusController.connected && !_audiobusController.memberOfActiveAudiobusSession) &&
        (dynamic_cast<ofApp*>(ofGetAppPtr())->holdIsOff())) {
        ofLog(OF_LOG_VERBOSE, "TURN IT OFF");
        AudioOutputUnitStop(dynamic_cast<ofApp*>(ofGetAppPtr())->getSoundStream()->getSoundOutStream().audioUnit);
        [[AVAudioSession sharedInstance] setActive:NO error:NULL];
    }
}

- (void)applicationWillEnterForeground:(UIApplication *)application {

    [[AVAudioSession sharedInstance] setActive:YES error:NULL];
    AudioOutputUnitStart(_audiobusSender.audioUnit);
    [[AVAudioSession sharedInstance] setCategory: AVAudioSessionCategoryPlayback withOptions: AVAudioSessionCategoryOptionMixWithOthers error:  NULL];
    
}

//check for iia connection, i had a problem with fbos not working when started from inside garageband...
-(void) checkIAACon:(int *)iaaCon{
    UInt32 connected;
    UInt32 dataSize = sizeof(UInt32);
    AudioUnitGetProperty(_audiobusSender.audioUnit,
                         kAudioUnitProperty_IsInterAppConnected,
                         kAudioUnitScope_Global, 0, &connected, &dataSize);
    *iaaCon = connected;
}

//can be called from controlThread.h to test for connection
-(void) checkCon:(bool *)iaaCon{
    UInt32 connected;
    UInt32 dataSize = sizeof(UInt32);
    AudioUnitGetProperty(_audiobusSender.audioUnit,
                         kAudioUnitProperty_IsInterAppConnected,
                         kAudioUnitScope_Global, 0, &connected, &dataSize);
    if(_audiobusController.connected || connected == 1){
        *iaaCon = true;
    } else {
        *iaaCon = false;
    }
}
-(void)applicationWillTerminate:(UIApplication *)application {
    [super applicationWillTerminate:application];
}


@end

