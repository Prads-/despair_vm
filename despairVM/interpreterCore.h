/*
	Written By Pradipna Nepal
	www.pradsprojects.com

	Copyright (C) 2013 Pradipna Nepal
	Please read COPYING.txt included along with this source code for more detail.
	If not included, see http://www.gnu.org/licenses/
*/

#ifndef INTERPRETER_CORE_H
#define INTERPRETER_CORE_H

#ifdef BUILD_FOR_WINDOWS
#include <Windows.h>
#endif
#ifdef BUILD_FOR_UNIX
#include <unistd.h>
#endif

#include <cstdlib>
#include <string>
#include <cmath>
#include "build.h"
#include "declarations.h"
#include "instructionsSet.h"
#include "memoryManager.h"
#include "gpuCore.h"
#include "portManager.h"
#include "timer.h"
#include "keyboardManager.h"
#include "despairHeader.h"

class DespairCore {
private:
	int64 pC;
	int32 sP;
	int64 regs[256];
	float32 fRegs[256];
	MemoryManager memManager;
	static DespairTimer timer;
	GPUCore *gpuCore;
	PortManager portManager;
	
	inline void MOV_MR_IMMI_R() {
		*(uint32*)(regs[memManager.codeSpace[pC + 1]] + *(uint32*)&memManager.codeSpace[pC + 2]) = regs[memManager.codeSpace[pC]];
	}

	inline void MOV_R_MR_IMMI() {
		regs[memManager.codeSpace[pC]] = *(uint32*)(regs[memManager.codeSpace[pC + 1]] + *(uint32*)&memManager.codeSpace[pC + 2]);
	}

	inline void MOV_MR_IMMI_MR_IMMI() {
		*(uint32*)(regs[memManager.codeSpace[pC]] + *(uint32*)&memManager.codeSpace[pC + 2]) = *(uint32*)(regs[memManager.codeSpace[pC + 1]] + *(uint32*)&memManager.codeSpace[pC + 6]);
	}

	inline void MOV_MR_IMMI_IMMI() {
		*(uint32*)(regs[memManager.codeSpace[pC]] + *(uint32*)&memManager.codeSpace[pC + 1]) = *(uint32*)&memManager.codeSpace[pC + 5];
	}

	inline void MOV_R_M() {
		regs[memManager.codeSpace[pC]] = *(uint32*)&memManager.globalDataSpace[*(uint32*)&memManager.codeSpace[pC + 1]];
	}

	inline void MOV_M_R() {
		*(uint32*)&memManager.globalDataSpace[*(uint32*)&memManager.codeSpace[pC + 1]] = regs[memManager.codeSpace[pC]];
	}

	inline void MOV_R_R() {
		regs[memManager.codeSpace[pC]] = regs[memManager.codeSpace[pC + 1]];
	}

	inline void MOV_M_M() {
		*(uint32*)&memManager.globalDataSpace[*(uint32*)&memManager.codeSpace[pC]] = *(uint32*)&memManager.globalDataSpace[*(uint32*)&memManager.codeSpace[pC + 4]];
	}

	inline void MOV_MR_R() {
		*(uint32*)regs[memManager.codeSpace[pC + 1]] = regs[memManager.codeSpace[pC]];
	}

	inline void MOV_R_MR() {
		regs[memManager.codeSpace[pC]] = *(uint32*)regs[memManager.codeSpace[pC + 1]];
	}

	inline void MOV_MR_M() {
		*(uint32*)regs[memManager.codeSpace[pC]] = *(uint32*)&memManager.globalDataSpace[*(uint32*)&memManager.codeSpace[pC + 1]];
	}

	inline void MOV_M_MR() {
		*(uint32*)&memManager.globalDataSpace[*(uint32*)&memManager.codeSpace[pC + 1]] = *(uint32*)regs[memManager.codeSpace[pC]];
	}

	inline void MOV_MR_MR() {
		*(uint32*)regs[memManager.codeSpace[pC]] = *(uint32*)regs[memManager.codeSpace[pC + 1]];
	}

	inline void MOV_R_IMMI() {
		regs[memManager.codeSpace[pC]] = *(uint32*)&memManager.codeSpace[pC + 1];
	}

	inline void MOV_M_IMMI() {
		*(uint32*)&memManager.globalDataSpace[*(uint32*)&memManager.codeSpace[pC]] = *(uint32*)&memManager.codeSpace[pC + 4];
	}

	inline void MOV_MR_IMMI() {
		*(uint32*)regs[memManager.codeSpace[pC]] = *(uint32*)&memManager.codeSpace[pC + 1];
	}

	inline void ADD_R_R() {
		regs[memManager.codeSpace[pC]] += regs[memManager.codeSpace[pC + 1]];
	}

	inline void ADD_R_IMMI() {
		regs[memManager.codeSpace[pC]] += *(int32*)&memManager.codeSpace[pC + 1];
	}

	inline void SUB_R_R() {
		regs[memManager.codeSpace[pC]] -= regs[memManager.codeSpace[pC + 1]];
	}

	inline void SUB_R_IMMI() {
		regs[memManager.codeSpace[pC]] -= *(int32*)&memManager.codeSpace[pC + 1];
	}

	inline void MUL_R_R() {
		regs[memManager.codeSpace[pC]] *= regs[memManager.codeSpace[pC + 1]];
	}

	inline void MUL_R_IMMI() {
		regs[memManager.codeSpace[pC]] *= *(int32*)&memManager.codeSpace[pC + 1];
	}

	inline void DIV_R_R() {
		regs[memManager.codeSpace[pC]] /= regs[memManager.codeSpace[pC + 1]];
	}

	inline void DIV_R_IMMI() {
		regs[memManager.codeSpace[pC]] /= *(int32*)&memManager.codeSpace[pC + 1];
	}

	inline void MOD_R_R() {
		regs[memManager.codeSpace[pC]] %= regs[memManager.codeSpace[pC + 1]];
	}

	inline void MOD_R_IMMI() {
		regs[memManager.codeSpace[pC]] %= *(int32*)&memManager.codeSpace[pC + 1];
	}

	inline void AND_R_R() {
		regs[memManager.codeSpace[pC]] &= regs[memManager.codeSpace[pC + 1]];
	}

	inline void AND_R_IMMI() {
		regs[memManager.codeSpace[pC]] &= *(uint32*)&memManager.codeSpace[pC + 1];
	}

	inline void OR_R_R() {
		regs[memManager.codeSpace[pC]] |= regs[memManager.codeSpace[pC + 1]];
	}

	inline void OR_R_IMMI() {
		regs[memManager.codeSpace[pC]] |= *(uint32*)&memManager.codeSpace[pC + 1];
	}

	inline void XOR_R_R() {
		regs[memManager.codeSpace[pC]] ^= regs[memManager.codeSpace[pC + 1]];
	}

	inline void XOR_R_IMMI() {
		regs[memManager.codeSpace[pC]] ^= *(uint32*)&memManager.codeSpace[pC + 1];
	}

	inline void SHL_R_IMMI8() {
		regs[memManager.codeSpace[pC]] <<= memManager.codeSpace[pC + 1];
	}

	inline void SHL_R_R() {
		regs[memManager.codeSpace[pC]] <<= regs[memManager.codeSpace[pC + 1]];
	}

	inline void SHR_R_IMMI8() {
		regs[memManager.codeSpace[pC]] >>= memManager.codeSpace[pC + 1];
	}

	inline void SHR_R_R() {
		regs[memManager.codeSpace[pC]] >>= regs[memManager.codeSpace[pC + 1]];
	}

	inline void JMP_IMMI() {
		pC = *(uint32*)&memManager.codeSpace[pC];
	}

	inline void JMPR_IMMI() {
		pC += *(int32*)&memManager.codeSpace[pC];
	}

	inline void JC_R_IMMI() {
		if (regs[memManager.codeSpace[pC]] == 0) {
			pC = *(uint32*)&memManager.codeSpace[pC + 1];
		} else {
			pC += 5;
		}
	}

	inline void JCR_R_IMMI() {
		if (regs[memManager.codeSpace[pC]] == 0) pC += *(int32*)&memManager.codeSpace[pC + 1];
	}

	inline void MOVP_R_MR_IMMI() {
		regs[memManager.codeSpace[pC]] = *(uint64*)(regs[memManager.codeSpace[pC + 1]] + *(uint32*)&memManager.codeSpace[pC + 2]);
	}

	inline void MOVP_MR_IMMI_R() {
		*(uint64*)(regs[memManager.codeSpace[pC + 1]] + *(uint32*)&memManager.codeSpace[pC + 2]) = regs[memManager.codeSpace[pC]];
	}

	inline void MOVP_MR_IMMI_MR_IMMI() {
		*(uint64*)(regs[memManager.codeSpace[pC]] + *(uint32*)&memManager.codeSpace[pC + 2]) = *(uint64*)(regs[memManager.codeSpace[pC + 1]] + *(uint32*)&memManager.codeSpace[pC + 6]);;
	}

	inline void MOVP_R_M() {
		regs[memManager.codeSpace[pC]] = *(uint64*)&memManager.globalDataSpace[*(uint32*)&memManager.codeSpace[pC + 1]];
	}

	inline void MOVP_M_R() {
		*(uint64*)&memManager.globalDataSpace[*(uint32*)&memManager.codeSpace[pC + 1]] = regs[memManager.codeSpace[pC]];
	}

	inline void MOVP_R_MR() {
		regs[memManager.codeSpace[pC]] = *(uint64*)regs[memManager.codeSpace[pC + 1]];
	}

	inline void MOVP_MR_R() {
		*(uint64*)regs[memManager.codeSpace[pC + 1]] = regs[memManager.codeSpace[pC]];
	}

	inline void CALL_M() {
		*(uint32*)&memManager.stackSpace[sP] = pC + 4;
		sP += 4;
		pC = *(uint32*)&memManager.codeSpace[pC];
	}

	inline void RET() {
		sP -= 4;
		if (sP < 0)
			pC = -1;
		else
			pC = *(uint32*)&memManager.stackSpace[sP];
	}

	inline void PUSH_R() {
		*(int64*)&memManager.stackSpace[sP] = regs[memManager.codeSpace[pC]];
		sP += 8;
	}

	inline void POP_R() {
		sP -= 8;
		regs[memManager.codeSpace[pC]] = *(int64*)&memManager.stackSpace[sP];
	}

	inline void DRW_R_R_MR() {
		gpuCore->draw(regs[memManager.codeSpace[pC]], regs[memManager.codeSpace[pC + 1]], regs[memManager.codeSpace[pC + 2]], portManager.readPort<uint8>(PORT_GPU_EFFECTS), portManager.readPort<uint16>(PORT_GPU_ROTATION));
	}

	inline void OUT_R_IMMI8() {
		portManager.writePort<uint8>(memManager.codeSpace[pC + 1], regs[memManager.codeSpace[pC]]);
	}

	inline void OUT_R_IMMI16() {
		portManager.writePort<uint16>(*(uint16*)&memManager.codeSpace[pC + 1], regs[memManager.codeSpace[pC]]);
	}

	inline void OUT_R_IMMI32() {
		portManager.writePort<uint32>(*(uint32*)&memManager.codeSpace[pC + 1], regs[memManager.codeSpace[pC]]);
	}

	inline void OUT_R_IMMI64() {
		portManager.writePort<uint64>(*(uint64*)&memManager.codeSpace[pC + 1], regs[memManager.codeSpace[pC]]);
	}

	inline void OUT_IMMI_R8() {
		portManager.writePort<uint8>(regs[memManager.codeSpace[pC]], *(uint32*)&memManager.codeSpace[pC + 1]);
	}

	inline void OUT_IMMI_R16() {
		portManager.writePort<uint16>(regs[memManager.codeSpace[pC]], *(uint32*)&memManager.codeSpace[pC + 1]);
	}

	inline void OUT_IMMI_R32() {
		portManager.writePort<uint32>(regs[memManager.codeSpace[pC]], *(uint32*)&memManager.codeSpace[pC + 1]);
	}

	inline void OUT_IMMI_R64() {
		portManager.writePort<uint64>(regs[memManager.codeSpace[pC]], *(uint32*)&memManager.codeSpace[pC + 1]);
	}

	inline void OUT_R_R8() {
		portManager.writePort<uint8>(regs[memManager.codeSpace[pC + 1]], regs[memManager.codeSpace[pC]]);
	}

	inline void OUT_R_R16() {
		portManager.writePort<uint16>(regs[memManager.codeSpace[pC + 1]], regs[memManager.codeSpace[pC]]);
	}

	inline void OUT_R_R32() {
		portManager.writePort<uint32>(regs[memManager.codeSpace[pC + 1]], regs[memManager.codeSpace[pC]]);
	}

	inline void OUT_R_R64() {
		portManager.writePort<uint64>(regs[memManager.codeSpace[pC + 1]], regs[memManager.codeSpace[pC]]);
	}

	inline void OUT_IMMI_IMMI8() {
		portManager.writePort<uint8>(memManager.codeSpace[pC + 4], *(uint32*)&memManager.codeSpace[pC]);
	}

	inline void OUT_IMMI_IMMI16() {
		portManager.writePort<uint16>(*(uint16*)&memManager.codeSpace[pC + 4], *(uint32*)&memManager.codeSpace[pC]);
	}

	inline void OUT_IMMI_IMMI32() {
		portManager.writePort<uint32>(*(uint32*)&memManager.codeSpace[pC + 4], *(uint32*)&memManager.codeSpace[pC]);
	}

	inline void OUT_IMMI_IMMI64() {
		portManager.writePort<uint64>(*(uint64*)&memManager.codeSpace[pC + 4], *(uint32*)&memManager.codeSpace[pC]);
	}

	inline void IN_R8_IMMI() {
		regs[memManager.codeSpace[pC]] = portManager.readPort<uint8>(*(uint32*)&memManager.codeSpace[pC + 1]);
	}

	inline void IN_R16_IMMI() {
		regs[memManager.codeSpace[pC]] = portManager.readPort<uint16>(*(uint32*)&memManager.codeSpace[pC + 1]);
	}

	inline void IN_R32_IMMI() {
		regs[memManager.codeSpace[pC]] = portManager.readPort<uint32>(*(uint32*)&memManager.codeSpace[pC + 1]);
	}

	inline void IN_R64_IMMI() {
		regs[memManager.codeSpace[pC]] = portManager.readPort<uint64>(*(uint32*)&memManager.codeSpace[pC + 1]);
	}

	inline void IN_R8_R() {
		regs[memManager.codeSpace[pC]] = portManager.readPort<uint8>(regs[memManager.codeSpace[pC + 1]]);
	}

	inline void IN_R16_R() {
		regs[memManager.codeSpace[pC]] = portManager.readPort<uint16>(regs[memManager.codeSpace[pC + 1]]);
	}

	inline void IN_R32_R() {
		regs[memManager.codeSpace[pC]] = portManager.readPort<uint32>(regs[memManager.codeSpace[pC + 1]]);

	}

	inline void IN_R64_R() {
		regs[memManager.codeSpace[pC]] = portManager.readPort<uint64>(regs[memManager.codeSpace[pC + 1]]);
	}

	inline void FCON_R_FR() {
		regs[memManager.codeSpace[pC]] = fRegs[memManager.codeSpace[pC + 1]];
	}

	inline void FCON_FR_R() {
		fRegs[memManager.codeSpace[pC + 1]] = regs[memManager.codeSpace[pC]];
	}

	inline void FCON_FR_IMMI() {
		fRegs[memManager.codeSpace[pC]] = *(int32*)&memManager.codeSpace[pC + 1];
	}

	inline void FCON_R_FIMMI() {
		regs[memManager.codeSpace[pC]] = *(float32*)&memManager.codeSpace[pC + 1];
	}

	inline void FMOV_FR_MFR_IMMI() {
		fRegs[memManager.codeSpace[pC + 1]] = *(float32*)(regs[memManager.codeSpace[pC]] + *(uint32*)&memManager.codeSpace[pC + 2]);
	}

	inline void FMOV_MFR_IMMI_FR() {
		*(float32*)(regs[memManager.codeSpace[pC]] + *(uint32*)&memManager.codeSpace[pC + 2]) = fRegs[memManager.codeSpace[pC + 1]];
	}

	inline void FMOV_MFR_IMMI_MFR_IMMI() {
		*(float32*)(regs[memManager.codeSpace[pC]] + *(uint32*)&memManager.codeSpace[pC + 2]) = *(float32*)(regs[memManager.codeSpace[pC + 1]] + *(uint32*)&memManager.codeSpace[pC + 6]);
	}

	inline void FMOV_MFR_IMMI_FIMMI() {
		*(float32*)(regs[memManager.codeSpace[pC]] + *(uint32*)&memManager.codeSpace[pC + 1]) = *(float32*)&memManager.codeSpace[pC + 5];
	}

	inline void FMOV_FR_FR() {
		fRegs[memManager.codeSpace[pC]] = fRegs[memManager.codeSpace[pC + 1]];
	}

	inline void FMOV_FR_FM() {
		fRegs[memManager.codeSpace[pC]] = *(float32*)&memManager.globalDataSpace[*(uint32*)&memManager.codeSpace[pC + 1]];
	}

	inline void FMOV_FM_FR() {
		*(float32*)&memManager.globalDataSpace[*(uint32*)&memManager.codeSpace[pC + 1]] = fRegs[memManager.codeSpace[pC]];
	}

	inline void FMOV_FR_MFR() {
		fRegs[memManager.codeSpace[pC + 1]] = *(float32*)regs[memManager.codeSpace[pC]];
	}

	inline void FMOV_MFR_FR() {
		*(float32*)regs[memManager.codeSpace[pC]] = fRegs[memManager.codeSpace[pC + 1]];
	}

	inline void FMOV_FM_MFR() {
		*(float32*)&memManager.globalDataSpace[*(uint32*)&memManager.codeSpace[pC + 1]] = *(float32*)regs[memManager.codeSpace[pC]];
	}

	inline void FMOV_MFR_FM() {
		*(float32*)regs[memManager.codeSpace[pC]] = *(float32*)&memManager.globalDataSpace[*(uint32*)&memManager.codeSpace[pC + 1]];
	}

	inline void FMOV_MFR_MFR() {
		*(float32*)regs[memManager.codeSpace[pC]] = *(float32*)regs[memManager.codeSpace[pC + 1]];
	}

	inline void FMOV_FM_FM() {
		*(float32*)&memManager.globalDataSpace[*(uint32*)&memManager.codeSpace[pC]] = *(float32*)&memManager.globalDataSpace[*(uint32*)&memManager.codeSpace[pC + 4]];
	}

	inline void FMOV_FR_FIMMI() {
		fRegs[memManager.codeSpace[pC]] = *(float32*)&memManager.codeSpace[pC + 1];
	}

	inline void FMOV_FM_FIMMI() {
		*(float32*)&memManager.globalDataSpace[*(uint32*)&memManager.codeSpace[pC]] = *(float32*)&memManager.codeSpace[pC + 4];
	}

	inline void FMOV_MFR_FIMMI() {
		*(float32*)regs[memManager.codeSpace[pC]] = *(float32*)&memManager.codeSpace[pC + 1];
	}

	inline void FADD_FR_FR() {
		fRegs[memManager.codeSpace[pC]] += fRegs[memManager.codeSpace[pC + 1]];
	}

	inline void FADD_R_FR() {
		regs[memManager.codeSpace[pC]] += fRegs[memManager.codeSpace[pC + 1]];
	}

	inline void FADD_FR_R() {
		fRegs[memManager.codeSpace[pC + 1]] += regs[memManager.codeSpace[pC]];
	}

	inline void FADD_FR_FIMMI() {
		fRegs[memManager.codeSpace[pC]] += *(float32*)&memManager.codeSpace[pC + 1];
	}

	inline void FADD_R_FIMMI() {
		regs[memManager.codeSpace[pC]] += *(float32*)&memManager.codeSpace[pC + 1];
	}

	inline void FSUB_FR_FR() {
		fRegs[memManager.codeSpace[pC]] -= fRegs[memManager.codeSpace[pC + 1]];
	}

	inline void FSUB_R_FR() {
		regs[memManager.codeSpace[pC]] -= fRegs[memManager.codeSpace[pC + 1]];
	}

	inline void FSUB_FR_R() {
		fRegs[memManager.codeSpace[pC + 1]] -= regs[memManager.codeSpace[pC]];
	}

	inline void FSUB_FR_FIMMI() {
		fRegs[memManager.codeSpace[pC]] -= *(float32*)&memManager.codeSpace[pC + 1];
	}

	inline void FSUB_R_FIMMI() {
		regs[memManager.codeSpace[pC]] -= *(float32*)&memManager.codeSpace[pC + 1];
	}

	inline void FMUL_FR_FR() {
		fRegs[memManager.codeSpace[pC]] *= fRegs[memManager.codeSpace[pC + 1]];
	}

	inline void FMUL_R_FR() {
		regs[memManager.codeSpace[pC]] *= fRegs[memManager.codeSpace[pC + 1]];
	}

	inline void FMUL_FR_R() {
		fRegs[memManager.codeSpace[pC + 1]] *= regs[memManager.codeSpace[pC]];
	}

	inline void FMUL_FR_FIMMI() {
		fRegs[memManager.codeSpace[pC]] *= *(float32*)&memManager.codeSpace[pC + 1];
	}

	inline void FMUL_R_FIMMI() {
		regs[memManager.codeSpace[pC]] *= *(float32*)&memManager.codeSpace[pC + 1];
	}

	inline void FDIV_FR_FR() {
		fRegs[memManager.codeSpace[pC]] /= fRegs[memManager.codeSpace[pC + 1]];
	}

	inline void FDIV_R_FR() {
		regs[memManager.codeSpace[pC]] /= fRegs[memManager.codeSpace[pC + 1]];
	}

	inline void FDIV_FR_R() {
		fRegs[memManager.codeSpace[pC + 1]] /= regs[memManager.codeSpace[pC]];
	}

	inline void FDIV_FR_FIMMI() {
		fRegs[memManager.codeSpace[pC]] /= *(float32*)&memManager.codeSpace[pC + 1];
	}

	inline void FDIV_R_FIMMI() {
		regs[memManager.codeSpace[pC]] /= *(float32*)&memManager.codeSpace[pC + 1];
	}

	inline void FMOD_FR_FR() {
		fRegs[memManager.codeSpace[pC]] = fmod(fRegs[memManager.codeSpace[pC]], fRegs[memManager.codeSpace[pC + 1]]);
	}

	inline void FMOD_R_FR() {
		regs[memManager.codeSpace[pC]] = fmod(regs[memManager.codeSpace[pC]], fRegs[memManager.codeSpace[pC + 1]]);
	}

	inline void FMOD_FR_R() {
		fRegs[memManager.codeSpace[pC + 1]] = fmod(fRegs[memManager.codeSpace[pC + 1]], regs[memManager.codeSpace[pC]]);
	}

	inline void FMOD_FR_FIMMI() {
		fRegs[memManager.codeSpace[pC]] = fmod(fRegs[memManager.codeSpace[pC]], *(float32*)&memManager.codeSpace[pC + 1]);
	}

	inline void FMOD_R_FIMMI() {
		regs[memManager.codeSpace[pC]] = fmod(regs[memManager.codeSpace[pC]], *(float32*)&memManager.codeSpace[pC + 1]);
	}

	inline void BMOV_R_BR_IMMI() {
		regs[memManager.codeSpace[pC]] = *(uint8*)(regs[memManager.codeSpace[pC + 1]] + *(uint32*)&memManager.codeSpace[pC + 2]);
	}

	inline void BMOV_BR_IMMI_R() {
		*(uint8*)(regs[memManager.codeSpace[pC + 1]] + *(uint32*)&memManager.codeSpace[pC + 2]) = regs[memManager.codeSpace[pC]];
	}

	inline void BMOV_BR_IMMI_BR_IMMI() {
		*(uint8*)(regs[memManager.codeSpace[pC]] + *(uint32*)&memManager.codeSpace[pC + 2]) = *(uint8*)(regs[memManager.codeSpace[pC + 1]] + *(uint32*)&memManager.codeSpace[pC + 6]);
	}

	inline void BMOV_BR_IMMI_IMMI8() {
		*(uint8*)(regs[memManager.codeSpace[pC]] + *(uint32*)&memManager.codeSpace[pC + 1]) = *(uint8*)&memManager.codeSpace[pC + 5];
	}

	inline void BMOV_R_BM() {
		regs[memManager.codeSpace[pC]] = memManager.globalDataSpace[*(uint32*)&memManager.codeSpace[pC + 1]];
	}

	inline void BMOV_BM_R() {
		memManager.globalDataSpace[*(uint32*)&memManager.codeSpace[pC + 1]] = regs[memManager.codeSpace[pC]];
	}

	inline void BMOV_R_BR() {
		regs[memManager.codeSpace[pC]] = *(uint8*)regs[memManager.codeSpace[pC + 1]];
	}

	inline void BMOV_BR_R() {
		*(uint8*)regs[memManager.codeSpace[pC + 1]] = regs[memManager.codeSpace[pC]];
	}

	inline void BMOV_BR_BR() {
		*(uint8*)regs[memManager.codeSpace[pC]] = *(uint8*)regs[memManager.codeSpace[pC + 1]];
	}

	inline void BMOV_BM_BM() {
		memManager.globalDataSpace[*(uint32*)&memManager.codeSpace[pC]] = memManager.globalDataSpace[*(uint32*)&memManager.codeSpace[pC + 4]];
	}

	inline void BMOV_BR_IMMI8() {
		*(uint8*)regs[memManager.codeSpace[pC]] = memManager.codeSpace[pC + 1];
	}

	inline void BMOV_BM_IMMI8() {
		memManager.globalDataSpace[*(uint32*)&memManager.codeSpace[pC]] = memManager.codeSpace[pC + 4];
	}

	inline void CMPE_R_R() {
		regs[memManager.codeSpace[pC]] = *(int32*)&regs[memManager.codeSpace[pC]] == *(int32*)&regs[memManager.codeSpace[pC + 1]];
	}

	inline void CMPNE_R_R() {
		regs[memManager.codeSpace[pC]] = *(int32*)&regs[memManager.codeSpace[pC]] != *(int32*)&regs[memManager.codeSpace[pC + 1]];
	}

	inline void CMPG_R_R() {
		regs[memManager.codeSpace[pC]] = *(int32*)&regs[memManager.codeSpace[pC]] > *(int32*)&regs[memManager.codeSpace[pC + 1]];
	}

	inline void CMPL_R_R() {
		regs[memManager.codeSpace[pC]] = *(int32*)&regs[memManager.codeSpace[pC]] < *(int32*)&regs[memManager.codeSpace[pC + 1]];
	}

	inline void CMPGE_R_R() {
		regs[memManager.codeSpace[pC]] = *(int32*)&regs[memManager.codeSpace[pC]] >= *(int32*)&regs[memManager.codeSpace[pC + 1]];
	}

	inline void CMPLE_R_R() {
		regs[memManager.codeSpace[pC]] = *(int32*)&regs[memManager.codeSpace[pC]] <= *(int32*)&regs[memManager.codeSpace[pC + 1]];
	}

	inline void CMPE_R_FR_FR() {
		regs[memManager.codeSpace[pC]] = fRegs[memManager.codeSpace[pC + 1]] == fRegs[memManager.codeSpace[pC + 2]];
	}

	inline void CMPNE_R_FR_FR() {
		regs[memManager.codeSpace[pC]] = fRegs[memManager.codeSpace[pC + 1]] != fRegs[memManager.codeSpace[pC + 2]];
	}

	inline void CMPG_R_FR_FR() {
		regs[memManager.codeSpace[pC]] = fRegs[memManager.codeSpace[pC + 1]] > fRegs[memManager.codeSpace[pC + 2]];
	}

	inline void CMPL_R_FR_FR() {
		regs[memManager.codeSpace[pC]] = fRegs[memManager.codeSpace[pC + 1]] < fRegs[memManager.codeSpace[pC + 2]];
	}

	inline void CMPGE_R_FR_FR() {
		regs[memManager.codeSpace[pC]] = fRegs[memManager.codeSpace[pC + 1]] >= fRegs[memManager.codeSpace[pC + 2]];
	}

	inline void CMPLE_R_FR_FR() {
		regs[memManager.codeSpace[pC]] = fRegs[memManager.codeSpace[pC + 1]] <= fRegs[memManager.codeSpace[pC + 2]];
	}

	inline void FOUT_IMMI_FR() {
		portManager.writePortAsFloat(fRegs[memManager.codeSpace[pC]], *(uint32*)&memManager.codeSpace[pC + 1]);
	}

	inline void FIN_IMMI_FR() {
		portManager.readPortAsFloat(*(uint32*)&memManager.codeSpace[pC + 1]);
	}

	inline void FOUT_IMMI_FIMMI() {
		portManager.writePortAsFloat(*(float32*)&memManager.codeSpace[pC + 4], *(uint32*)&memManager.codeSpace[pC]);
	}

	inline void PUSHES_R_R() {
		int r1 = memManager.codeSpace[pC], r2 = memManager.codeSpace[pC + 1];
		int size = (r2 - r1) * 8 + 8;
		memcpy(&memManager.stackSpace[sP], &regs[r1], size);
		sP += size;
	}
	
	inline void POPS_R_R() {
		int r1 = memManager.codeSpace[pC], r2 = memManager.codeSpace[pC + 1];
		int size = (r2 - r1) * 8 + 8;
		sP -= size;
		memcpy(&regs[r1], &memManager.stackSpace[sP], size);
	}

	inline void FPUSHES_FR_FR() {
		int fr1 = memManager.codeSpace[pC], fr2 = memManager.codeSpace[pC + 1];
		int size = (fr2 - fr1) * 4 + 4;
		memcpy(&memManager.stackSpace[sP], &fRegs[fr1], size);
		sP += size;
	}

	inline void FPOPS_FR_FR() {
		int fr1 = memManager.codeSpace[pC], fr2 = memManager.codeSpace[pC + 1];
		int size = (fr2 - fr1) * 4 + 4;
		sP -= size;
		memcpy(&fRegs[fr1], &memManager.stackSpace[sP], size);
	}
	
	inline void FPUSH_FR() {
		*(float32*)&memManager.stackSpace[sP] = fRegs[memManager.codeSpace[pC]];
		sP += 4;
	}

	inline void FPOP_FR() {
		sP -= 4;
		fRegs[memManager.codeSpace[pC]] = *(float32*)&memManager.stackSpace[sP];
	}

	inline void CMPE_R_IMMI() {
		regs[memManager.codeSpace[pC]] = *(int32*)&regs[memManager.codeSpace[pC]] == *(int32*)&memManager.codeSpace[pC + 1];
	}

	inline void CMPNE_R_IMMI() {
		regs[memManager.codeSpace[pC]] = *(int32*)&regs[memManager.codeSpace[pC]] != *(int32*)&memManager.codeSpace[pC + 1];
	}

	inline void CMPG_R_IMMI() {
		regs[memManager.codeSpace[pC]] = *(int32*)&regs[memManager.codeSpace[pC]] > *(int32*)&memManager.codeSpace[pC + 1];
	}

	inline void CMPGE_R_IMMI() {
		regs[memManager.codeSpace[pC]] = *(int32*)&regs[memManager.codeSpace[pC]] >= *(int32*)&memManager.codeSpace[pC + 1];	
	}

	inline void CMPL_R_IMMI() {
		regs[memManager.codeSpace[pC]] = *(int32*)&regs[memManager.codeSpace[pC]] < *(int32*)&memManager.codeSpace[pC + 1];	
	}

	inline void CMPLE_R_IMMI() {
		regs[memManager.codeSpace[pC]] = *(int32*)&regs[memManager.codeSpace[pC]] <= *(int32*)&memManager.codeSpace[pC + 1];	
	}

	inline void TIME() {
		regs[0] = timer.getMilliseconds();
	}

	inline void SLEEP() {
#ifdef BUILD_FOR_WINDOWS
		Sleep(1);
#endif
#ifdef BUILD_FOR_UNIX
        usleep(1000);
#endif
	}

	inline void RAND() {
		regs[0] = rand();
	}

	inline void JMP_R() {
		pC = regs[memManager.codeSpace[pC]];	
	}

	inline void JMPR_R() {
		pC += regs[memManager.codeSpace[pC]];
	}

	inline void JC_R_R() {
		if (regs[memManager.codeSpace[pC]] == 0) {
			pC = regs[memManager.codeSpace[pC + 1]];
		} else {
			pC += 2;
		}
	}

	inline void JCR_R_R() {
		if (regs[memManager.codeSpace[pC]] == 0) {
			pC += regs[memManager.codeSpace[pC + 1]];
		}
	}

	bool fdxCycle();
public:
	DespairCore(uint8 *codePtr, uint8 *globalDataPtr, uint32 codeStartIndex, uint64 paramAddr, GPUCore *gpuCore, DespairHeader::ExecutableHeader *header, KeyboardManager *keyboardManager);
	void startCPULoop();	//despair "Infinite" CPU Loop
#ifdef DEBUG_DESPAIR
	void debugGetRegisters(int32 *rOut);
#endif
};

#endif
