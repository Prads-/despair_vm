/*
	Written By Pradipna Nepal
	www.pradsprojects.com

	Copyright (C) 2013 Pradipna Nepal
	Please read COPYING.txt included along with this source code for more detail.
	If not included, see http://www.gnu.org/licenses/
*/

#include <string>
#include "portManager.h"
#include "memoryManager.h"
#include "fileManager.h"
#include "stringManager.h"
#include "threadManager.h"
#include "threadParameter.h"
#include "memoryDMAController.h"
#include "gpuCore.h"
using namespace FileManager;
using namespace StringManager;
using namespace ThreadManager;
using namespace MemoryDMAController;
using namespace std;

template uint8 PortManager::readPort<uint8>(uint32 address);
template uint16 PortManager::readPort<uint16>(uint32 address);
template uint32 PortManager::readPort<uint32>(uint32 address);
template uint64 PortManager::readPort<uint64>(uint32 address);
template void PortManager::writePort(uint8 val, uint32 address);
template void PortManager::writePort(uint16 val, uint32 address);
template void PortManager::writePort(uint32 val, uint32 address);
template void PortManager::writePort(uint64 val, uint32 address);

void PortManager::initializePortManager(GPUCore *gpuCore, uint8 *codePtr, uint8 *globalDataPtr, DespairHeader::ExecutableHeader *header, KeyboardManager *keyboardManager) {
	memset(ports, 0, PORTS_NUMBER);
	this->gpuCore = gpuCore;
	this->codePtr = codePtr;
	this->globalDataPtr = globalDataPtr;
	this->header = header;
	this->keyboardManager = keyboardManager;
}

template<typename Type>
Type PortManager::readPort(uint32 address) {
	return *(Type*)&ports[address];
}

template<typename Type>
void PortManager::writePort(Type val, uint32 address) {
	switch (address) {
		case PORT_GPU_FB_IN_DMA:
			gpuCore->gpuDMA_In((uint32*)val);
			return;
		case PORT_GPU_FB_OUT_DMA:
			gpuCore->gpuDMA_Out((uint32*)val);
			return;
		case PORT_KEYBOARD:
			ports[address] = keyboardManager->getKeyStatus(val);
			return;
		case PORT_MEMORY_MAKE_HEAP:
			*(uint64*)&ports[address] = (uint64)MemoryManager::makeHeap(val);
			return;
		case PORT_MEMORY_DESTROY_HEAP:
			MemoryManager::destroyHeap(val);
			return;
		case PORT_FILE_COMMAND:
			decodeFileCommands(val, ports, &header->exeFolder);
			return;
		case PORT_STRING_COMMAND:
			decodeStringCommands(val, ports);
			return;
		case PORT_THREAD_CREATE:
			{
				ThreadParameter param;
				param.paramAddr = *(uint64*)&ports[PORT_THREAD_PARAMETER];
				param.codeStartIndex = val;
				param.codePtr = codePtr;
				param.globalDataPtr = globalDataPtr;
				param.gpuCore = gpuCore;
				param.header = header;
				param.keyboardManager = keyboardManager;

				ports[PORT_THREAD_CREATE] = (int)createNewThread(&param);
				return;
			}
		case PORT_DMA_SIZE:
			memoryDMATransfer(val, ports);
			return;
		case PORT_GPU_COMMAND:
			gpuCore->gpuDecodeCommand(val);
			return;
	}

	*(Type*)&ports[address] = val;
}

float32 PortManager::readPortAsFloat(uint32 address) {
	return *(float32*)&ports[address];
}

void PortManager::writePortAsFloat(float32 val, uint32 address) {
	*(float32*)&ports[address] = val;
}