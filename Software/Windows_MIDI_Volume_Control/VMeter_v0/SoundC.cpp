#include "stdafx.h"
#include "SoundC.h"

float panLeft = 1.0;
float panRight = 1.0;
HMIXER hMixer;
IAudioEndpointVolume *endpointVolume = NULL;

BOOL IsVistaOrLater() {
	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	return (osvi.dwMajorVersion >= 6);
}

float GetMasterVolumeXP() {
	MMRESULT result;
	if (!hMixer)
		result = mixerOpen(&hMixer, MIXER_OBJECTF_MIXER, 0, 0, 0);

	//Next, we need to get the default output line of the mixer device.
	MIXERLINE ml = {0};
	ml.cbStruct = sizeof(MIXERLINE);
	ml.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
	result = mixerGetLineInfo((HMIXEROBJ) hMixer, &ml, MIXER_GETLINEINFOF_COMPONENTTYPE);

	//Next, we need to get the volume control of the speaker line.
	MIXERLINECONTROLS mlc = {0};
	MIXERCONTROL mc = {0};
	mlc.cbStruct = sizeof(MIXERLINECONTROLS);
	mlc.dwLineID = ml.dwLineID;
	mlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
	mlc.cControls = 1;
	mlc.pamxctrl = &mc;
	mlc.cbmxctrl = sizeof(MIXERCONTROL);
	result = mixerGetLineControls((HMIXEROBJ) hMixer, &mlc, MIXER_GETLINECONTROLSF_ONEBYTYPE);

	//We now have everything identified. All that's left is to actually set the volume level.
	
	if (hMixer == NULL) return FALSE;
	
	MIXERCONTROLDETAILS_UNSIGNED mxcdVolume[2];
	MIXERCONTROLDETAILS mxcd;
	mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID = mc.dwControlID;
	mxcd.cChannels = 2;
	mxcd.cMultipleItems = 0;
	mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	mxcd.paDetails = mxcdVolume;
	if (::mixerGetControlDetails((HMIXEROBJ) hMixer,
								 &mxcd,
								 MIXER_OBJECTF_HMIXER |
								 MIXER_GETCONTROLDETAILSF_VALUE)
		!= MMSYSERR_NOERROR)
	{
		return FALSE;
	}

	float left = (float)mxcdVolume[0].dwValue / 65535;
	float right = (float)mxcdVolume[1].dwValue / 65535;
	if ((left > 0) && (right>0)) {
		if (left > right) {
			panLeft = 1.0;
			panRight = (right / left);
		} else {
			if (left < right) {
				panLeft = (left / right);
				panRight = 1.0;
			}
		}
	}
	//mixerClose(hMixer);
	return (max(left,right));
}

float GetMasterVolumeVista() {
	/*HRESULT hr;
	float result = 0;

	CoInitialize(NULL);
	IMMDeviceEnumerator *deviceEnumerator = NULL;
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
	IMMDevice *defaultDevice = NULL;

	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);

	deviceEnumerator->Release();
	deviceEnumerator = NULL;

	IAudioEndpointVolume *endpointVolume = NULL;
	hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
	defaultDevice->Release();
	defaultDevice = NULL; */
	TCHAR str[255]; 
	// -------------------------
	float left;
	float right;
	float master;
	int static counter = 0;

	if (!endpointVolume) {
		getVolumeEndPointVolumeForVista();
	}

	endpointVolume->GetChannelVolumeLevelScalar(0,&left);
	endpointVolume->GetChannelVolumeLevelScalar(1,&right);
	endpointVolume->GetMasterVolumeLevelScalar(&master);

	//if (counter++ == 20) {		
	//	_stprintf(str, _T("left: %f, right: %f, master: %f\n"),left,right,master);
	//	OutputDebugString(str);
	//	counter = 0;
	//}
	//	

	if ((left > 0) && (right>0)) {
		if (left > right) {
			panLeft = 1.0;
			panRight = (right / left);
		} else {
			if (left < right) {
				panLeft = (left / right);
				panRight = 1.0;
			}
		}
	}

	//endpointVolume->Release();
	//CoUninitialize();

	//return (max(left,right));
	return master;
}






void SetMasterVolumeVista(float v) {
	//HRESULT hr;
	double newVolume = v;

	//CoInitialize(NULL);
	//IMMDeviceEnumerator *deviceEnumerator = NULL;
	//hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
	//IMMDevice *defaultDevice = NULL;

	//hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
	//deviceEnumerator->Release();
	//deviceEnumerator = NULL;

	//IAudioEndpointVolume *endpointVolume = NULL;
	//hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
	//defaultDevice->Release();
	//defaultDevice = NULL; 
	
	// -------------------------
	if (!endpointVolume) {
		getVolumeEndPointVolumeForVista();
	}

	//endpointVolume->SetChannelVolumeLevelScalar(0,v*panLeft,NULL);
	//endpointVolume->SetChannelVolumeLevelScalar(1,v*panRight,NULL);

	endpointVolume->SetMasterVolumeLevelScalar(v,NULL);
	 

	//endpointVolume->Release();

	//CoUninitialize();
}

BOOL SetMasterVolumeXP(DWORD dwVal) {
	MMRESULT result;
	static MIXERCONTROL mc = {0};
	
//	HMIXER hMixer;
	if (!hMixer)
		result = mixerOpen(&hMixer, MIXER_OBJECTF_MIXER, 0, 0, 0);

	//dwVal = (DWORD)60000;

	//Next, we need to get the default output line of the mixer device.
	MIXERLINE ml = {0};
	ml.cbStruct = sizeof(MIXERLINE);
	ml.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
	result = mixerGetLineInfo((HMIXEROBJ) hMixer, &ml, MIXER_GETLINEINFOF_COMPONENTTYPE);

	//Next, we need to get the volume control of the speaker line.
	MIXERLINECONTROLS mlc = {0};
	
	mlc.cbStruct = sizeof(MIXERLINECONTROLS);
	mlc.dwLineID = ml.dwLineID;
	mlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
	mlc.cControls = 1;
	mlc.pamxctrl = &mc;
	mlc.cbmxctrl = sizeof(MIXERCONTROL);
	result = mixerGetLineControls((HMIXEROBJ) hMixer, &mlc, MIXER_GETLINECONTROLSF_ONEBYTYPE);

	
	//We now have everything identified. All that's left is to actually set the volume level.
	MIXERCONTROLDETAILS_UNSIGNED mxcdVolume[2];
	//panLeft = 1.0;
	//panRight = 1.0;
	float newLeft = (float)dwVal * panLeft;
	float newRight = (float)dwVal * panRight;
	mxcdVolume[0].dwValue = (unsigned int)newLeft;
	mxcdVolume[1].dwValue = (unsigned int)newRight;

	MIXERCONTROLDETAILS mxcd;
	mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID = mc.dwControlID;
	mxcd.cChannels = 2;
	mxcd.cMultipleItems = 0;
	mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	mxcd.paDetails = mxcdVolume;
	result = mixerSetControlDetails((HMIXEROBJ) hMixer, &mxcd, MIXER_OBJECTF_HMIXER | MIXER_SETCONTROLDETAILSF_VALUE);
	//mixerClose(hMixer);
	return (result);
	
	
}

void SetMasterVolume(float v) {
	if (IsVistaOrLater()) {
		SetMasterVolumeVista(v);
	} else {
		DWORD v1 = (DWORD)(v*65535);
		SetMasterVolumeXP(v1);
	}
}

float GetMasterVolume() {
	if (IsVistaOrLater()) {
		return (GetMasterVolumeVista());
	} else {
		return (GetMasterVolumeXP());
	}
}

void CloseMixer() {
	if (!IsVistaOrLater() && hMixer) {
		mixerClose(hMixer);
	}
}

void getVolumeEndPointVolumeForVista() {
	if (IsVistaOrLater()) {
		HRESULT hr;

		CoInitialize(NULL);
		IMMDeviceEnumerator *deviceEnumerator = NULL;
		hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
		IMMDevice *defaultDevice = NULL;

		hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
		deviceEnumerator->Release();
		deviceEnumerator = NULL;

		// this is the line that sets endpointVolume
		hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
		defaultDevice->Release();
		defaultDevice = NULL; 

		CoUninitialize();
	}
}

void releaseEndPointVolumeForVista() {
	if (IsVistaOrLater() && endpointVolume) { 
		endpointVolume->Release();
		endpointVolume = NULL;
	}
}