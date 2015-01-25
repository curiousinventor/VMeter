#include "stdafx.h"
#include <mmdeviceapi.h>
#include <endpointvolume.h>

void SetMasterVolume(float v);
float GetMasterVolume();
void CloseMixer();
void releaseEndPointVolumeForVista();
void getVolumeEndPointVolumeForVista();
BOOL IsVistaOrLater();
