/*
	Written By Pradipna Nepal
	www.pradsprojects.com

	Copyright (C) 2013 Pradipna Nepal
	Please read COPYING.txt included along with this source code for more detail.
	If not included, see http://www.gnu.org/licenses/
*/

#ifndef THREAD_PARAMETER_H
#define THREAD_PARAMETER_H

#include <string>
#include "build.h"
#include "declarations.h"
#include "gpuCore.h"
#include "keyboardManager.h"
#include "despairHeader.h"

struct ThreadParameter {
	uint32 codeStartIndex;
	volatile bool *threadStopped, threadInitialized;
	uint64 paramAddr;
	uint8 *codePtr, *globalDataPtr;
	GPUCore *gpuCore;
	KeyboardManager *keyboardManager;
	DespairHeader::ExecutableHeader *header;

	ThreadParameter() {
		threadStopped = 0;
		threadInitialized = false;
	}
};

#endif
