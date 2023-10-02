#include "pch.h"
#include<windows.h>
#include "TheLibrary.h"
using namespace std;

BAKKESMOD_PLUGIN(TheLibrary, "The Library", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;
bool libEnabled = false;
bool bonkersEnabled = false;
bool ballcamtoggleEnabled = false;
bool isReplay = false;

void TheLibrary::onLoad()
{
	_globalCvarManager = cvarManager;
	LOG("The Library loaded!");

	gameWrapper->HookEvent("Function TAGame.GameEvent_TrainingEditor_TA.OnInit", bind(&TheLibrary::OnCustomTrainingLoad, this, std::placeholders::_1));

	cvarManager->registerNotifier("disappear", [this](std::vector<std::string> args) {
		disappear();
		}, "", PERMISSION_ALL);

	cvarManager->registerNotifier("appear", [this](std::vector<std::string> args) {
		appear();
		}, "", PERMISSION_ALL);
	
	cvarManager->registerCvar("lib_enabled", "0", "Enable The Library", true, true, 0, true, 1)
		.addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
		libEnabled = cvar.getBoolValue();
			});

	cvarManager->registerCvar("lib_time", "1", "Reading Level");

	cvarManager->registerCvar("lib_mode", "time", "Reading Mode");

	cvarManager->registerCvar("bonkers_enabled", "0", "Enable Bonkers", true, true, 0, true, 1)
		.addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
		bonkersEnabled = cvar.getBoolValue();
			});

	cvarManager->registerCvar("ballcamtoggle_enabled", "0", "Enable Ball Cam Toggle", true, true, 0, true, 1)
		.addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
		ballcamtoggleEnabled = cvar.getBoolValue();
			});
}

/// Load on custom training load
void TheLibrary::OnCustomTrainingLoad(std::string eventName) {

	if (!libEnabled) { return; }
	
	if (ballcamtoggleEnabled) {
		toggleBallCamOn();
	}
		
	/*gameWrapper->HookEvent("Function GameEvent_Soccar_TA.ReplayPlayback.BeginState",
		[this](std::string eventName) {
			isReplay = true;
		});

	gameWrapper->HookEvent("Function GameEvent_Soccar_TA.ReplayPlayback.EndState",
		[this](std::string eventName) {
			isReplay = false;
		});
	*/
	/// Run on car move event
	gameWrapper->HookEvent("Function TAGame.TrainingEditorMetrics_TA.TrainingShotAttempt",
		[this](std::string eventName) {
			CVarWrapper modeCVar = cvarManager->getCvar("lib_mode");
			if (!modeCVar) { return; }
			std::string libMode = modeCVar.getStringValue();
			if (libMode == "time") {
				CVarWrapper timeCVar = cvarManager->getCvar("lib_time");
				if (!timeCVar) { return; }
				float timeDelay = timeCVar.getFloatValue();
				gameWrapper->SetTimeout([this](GameWrapper* gw) {
					disappear();
					}, timeDelay);
			}

			if (libMode == "jump") {
				gameWrapper->HookEvent("Function TAGame.PlayerController_TA.ToggleJump",
					[this](std::string eventName) {
						disappear();
					});

			}
			///run on car touch ball event
			gameWrapper->HookEvent("Function TAGame.Ball_TA.OnCarTouch",
				[this](std::string eventName) {
					appear();
				});

		});

	/// Run on ball explode
	gameWrapper->HookEvent("Function TAGame.Ball_TA.Explode",
		[this](std::string eventName) {
			appear();			
		});
}

void TheLibrary::disappear()
{
	unsigned long isHidden = 1;
	string camState;
	// null checks
	ServerWrapper server = gameWrapper->GetCurrentGameState();
	if (!server) { return; }
	BallWrapper ball = server.GetBall();
	if (!ball) { return; }
	ball.SetHidden2(isHidden);
	if (bonkersEnabled) {
		CarWrapper car = gameWrapper->GetLocalCar();
		if (!car) { return; }
		car.SetHidden2(isHidden);
	}
	if (ballcamtoggleEnabled) {
		toggleBallCamOff();
	}
}

void TheLibrary::appear()
{
	unsigned long hiddenStatus;
	ServerWrapper server = gameWrapper->GetCurrentGameState();
	if (!server) { return; }
	BallWrapper ball = server.GetBall();
	if (!ball) { return; }
	hiddenStatus = ball.GetbHidden();
	if (hiddenStatus == 1) {
		unsigned long unHidden = 0;
		string camState;
		// null checks
		ServerWrapper server = gameWrapper->GetCurrentGameState();
		if (!server) { return; }
		BallWrapper ball = server.GetBall();
		if (!ball) { return; }
		ball.SetHidden2(unHidden);
		if (bonkersEnabled) {
			CarWrapper car = gameWrapper->GetLocalCar();
			if (!car) { return; }
			car.SetHidden2(unHidden);
		}
		if (ballcamtoggleEnabled) {
			toggleBallCamOn();
		}
		}
	}

void TheLibrary::toggleBallCamOn()
{
	string camState;
	CameraWrapper cam = gameWrapper->GetCamera();
	if (!cam) { return; }
	camState = cam.GetCameraState();
	if (camState != "CameraState_BallCam_TA") {
		PlayerControllerWrapper input = gameWrapper->GetPlayerController();
		if (!input) { return; }
		input.PressSecondaryCamera();
		input.ReleaseSecondaryCamera();
	}
}

void TheLibrary::toggleBallCamOff()
{
	string camState;
	CameraWrapper cam = gameWrapper->GetCamera();
	if (!cam) { return; }
	camState = cam.GetCameraState();
	if (camState == "CameraState_BallCam_TA") {
		PlayerControllerWrapper input = gameWrapper->GetPlayerController();
		if (!input) { return; }
		input.PressSecondaryCamera();
		input.ReleaseSecondaryCamera();
	}
}