/*
	Written By Pradipna Nepal
	www.pradsprojects.com

	Copyright (C) 2013 Pradipna Nepal
	Please read COPYING.txt included along with this source code for more detail.
	If not included, see http://www.gnu.org/licenses/
*/

#include <time.h>
#include <cstdlib>
#include"build.h"
#include"declarations.h"
#include "interpreterCore.h"
using namespace std;

DespairTimer DespairCore::timer;

DespairCore::DespairCore(uint8 *codePtr, uint8 *globalDataPtr, uint32 codeStartIndex, uint64 paramAddr, GPUCore *gpuCore, DespairHeader::ExecutableHeader *header, KeyboardManager *keyboardManager)
				: memManager(header->part1.stackSize, header->part1.dataSize, codePtr, globalDataPtr) { 
	regs[0xFF] = (uint64)memManager.dataSpace;
	regs[0xFE] = (uint64)memManager.globalDataSpace;
	if (paramAddr > 0) *(uint64*)&memManager.dataSpace[0] = paramAddr;
	sP = 0;
	pC = codeStartIndex;
	this->gpuCore = gpuCore;
	portManager.initializePortManager(gpuCore, memManager.codeSpace, memManager.globalDataSpace, header, keyboardManager);
	//As far as I know, microsoft compiler needs srand to be called in each thread
#ifdef USING_MICROSOFT_COMPILER
	srand(time(0));
#endif
}

void DespairCore::startCPULoop() {
	while (fdxCycle());
}

#ifdef DEBUG_DESPAIR
void DespairCore::debugGetRegisters(int32 *rOut) {
	for (int i = 0; i < 256; ++i) {
		rOut[i] = regs[i];
	}
}
#endif

bool DespairCore::fdxCycle() {
	if (pC < 0) return false;

	uint16 opcode = *(uint16*)&memManager.codeSpace[pC];
	pC += 2;

	switch(opcode) {
		case _MOV_R_M:
			MOV_R_M();
			pC += 5;
			break;
		case _MOV_M_R:
			MOV_M_R();
			pC += 5;
			break;
		case _MOV_R_R:
			MOV_R_R();
			pC += 2;
			break;
		case _MOV_M_M:
			MOV_M_M();
			pC += 8;
			break;
		case _MOV_MR_R:
			MOV_MR_R();
			pC += 2;
			break;
		case _MOV_R_MR:
			MOV_R_MR();
			pC += 2;
			break;
		case _MOV_MR_M:
			MOV_MR_M();
			pC += 5;
			break;
		case _MOV_M_MR:
			MOV_M_MR();
			pC += 5;
			break;
		case _MOV_MR_MR:
			MOV_MR_MR();
			pC += 2;
			break;
		case _MOV_R_IMMI:
			MOV_R_IMMI();
			pC += 5;
			break;
		case _MOV_M_IMMI:
			MOV_M_IMMI();
			pC += 8;
			break;
		case _MOV_MR_IMMI:
			MOV_MR_IMMI();
			pC += 5;
			break;
		case _ADD_R_M:
			ADD_R_M();
			pC += 5;
			break;
		case _ADD_M_R:
			ADD_M_R();
			pC += 5;
			break;
		case _ADD_R_R:
			ADD_R_R();
			pC += 2;
			break;
		case _ADD_M_M:
			ADD_M_M();
			pC += 8;
			break;
		case _ADD_MR_R:
			ADD_MR_R();
			pC += 2;
			break;
		case _ADD_R_MR:
			ADD_R_MR();
			pC += 2;
			break;
		case _ADD_MR_M:
			ADD_MR_M();
			pC += 5;
			break;
		case _ADD_M_MR:
			ADD_M_MR();
			pC += 5;
			break;
		case _ADD_MR_MR:
			ADD_MR_MR();
			pC += 2;
			break;
		case _ADD_R_IMMI:
			ADD_R_IMMI();
			pC += 5;
			break;
		case _ADD_M_IMMI:
			ADD_M_IMMI();
			pC += 8;
			break;
		case _ADD_MR_IMMI:
			ADD_MR_IMMI();
			pC += 5;
			break;
		case _SUB_R_M:
			SUB_R_M();
			pC += 5;
			break;
		case _SUB_M_R:
			SUB_M_R();
			pC += 5;
			break;
		case _SUB_R_R:
			SUB_R_R();
			pC += 2;
			break;
		case _SUB_M_M:
			SUB_M_M();
			pC += 8;
			break;
		case _SUB_MR_R:
			SUB_MR_R();
			pC += 2;
			break;
		case _SUB_R_MR:
			SUB_R_MR();
			pC += 2;
			break;
		case _SUB_MR_M:
			SUB_MR_M();
			pC += 5;
			break;
		case _SUB_M_MR:
			SUB_M_MR();
			pC += 5;
			break;
		case _SUB_MR_MR:
			SUB_MR_MR();
			pC += 2;
			break;
		case _SUB_R_IMMI:
			SUB_R_IMMI();
			pC += 5;
			break;
		case _SUB_M_IMMI:
			SUB_M_IMMI();
			pC += 8;
			break;
		case _SUB_MR_IMMI:
			SUB_MR_IMMI();
			pC += 5;
			break;
		case _MUL_R_M:
			MUL_R_M();
			pC += 5;
			break;
		case _MUL_M_R:
			MUL_M_R();
			pC += 5;
			break;
		case _MUL_R_R:
			MUL_R_R();
			pC += 2;
			break;
		case _MUL_M_M:
			MUL_M_M();
			pC += 8;
			break;
		case _MUL_MR_R:
			MUL_MR_R();
			pC += 2;
			break;
		case _MUL_R_MR:
			MUL_R_MR();
			pC += 2;
			break;
		case _MUL_MR_M:
			MUL_MR_M();
			pC += 5;
			break;
		case _MUL_M_MR:
			MUL_M_MR();
			pC += 5;
			break;
		case _MUL_MR_MR:
			MUL_MR_MR();
			pC += 2;
			break;
		case _MUL_R_IMMI:
			MUL_R_IMMI();
			pC += 5;
			break;
		case _MUL_M_IMMI:
			MUL_M_IMMI();
			pC += 8;
			break;
		case _MUL_MR_IMMI:
			MUL_MR_IMMI();
			pC += 5;
			break;
		case _DIV_R_M:
			DIV_R_M();
			pC += 5;
			break;
		case _DIV_M_R:
			DIV_M_R();
			pC += 5;
			break;
		case _DIV_R_R:
			DIV_R_R();
			pC += 2;
			break;
		case _DIV_M_M:
			DIV_M_M();
			pC += 8;
			break;
		case _DIV_MR_R:
			DIV_MR_R();
			pC += 2;
			break;
		case _DIV_R_MR:
			DIV_R_MR();
			pC += 2;
			break;
		case _DIV_MR_M:
			DIV_MR_M();
			pC += 5;
			break;
		case _DIV_M_MR:
			DIV_M_MR();
			pC += 5;
			break;
		case _DIV_MR_MR:
			DIV_MR_MR();
			pC += 2;
			break;
		case _DIV_R_IMMI:
			DIV_R_IMMI();
			pC += 5;
			break;
		case _DIV_M_IMMI:
			DIV_M_IMMI();
			pC += 8;
			break;
		case _DIV_MR_IMMI:
			DIV_MR_IMMI();
			pC += 5;
			break;
		case _MOD_R_M:
			MOD_R_M();
			pC += 5;
			break;
		case _MOD_M_R:
			MOD_M_R();
			pC += 5;
			break;
		case _MOD_R_R:
			MOD_R_R();
			pC += 2;
			break;
		case _MOD_M_M:
			MOD_M_M();
			pC += 8;
			break;
		case _MOD_MR_R:
			MOD_MR_R();
			pC += 2;
			break;
		case _MOD_R_MR:
			MOD_R_MR();
			pC += 2;
			break;
		case _MOD_MR_M:
			MOD_MR_M();
			pC += 5;
			break;
		case _MOD_M_MR:
			MOD_M_MR();
			pC += 5;
			break;
		case _MOD_MR_MR:
			MOD_MR_MR();
			pC += 2;
			break;
		case _MOD_R_IMMI:
			MOD_R_IMMI();
			pC += 5;
			break;
		case _MOD_M_IMMI:
			MOD_M_IMMI();
			pC += 8;
			break;
		case _MOD_MR_IMMI:
			MOD_MR_IMMI();
			pC += 5;
			break;
		case _AND_R_M:
			AND_R_M();
			pC += 5;
			break;
		case _AND_M_R:
			AND_M_R();
			pC += 5;
			break;
		case _AND_R_R:
			AND_R_R();
			pC += 2;
			break;
		case _AND_M_M:
			AND_M_M();
			pC += 8;
			break;
		case _AND_MR_R:
			AND_MR_R();
			pC += 2;
			break;
		case _AND_R_MR:
			AND_R_MR();
			pC += 2;
			break;
		case _AND_MR_M:
			AND_MR_M();
			pC += 5;
			break;
		case _AND_M_MR:
			AND_M_MR();
			pC += 5;
			break;
		case _AND_MR_MR:
			AND_MR_MR();
			pC += 2;
			break;
		case _AND_R_IMMI:
			AND_R_IMMI();
			pC += 5;
			break;
		case _AND_M_IMMI:
			AND_M_IMMI();
			pC += 8;
			break;
		case _AND_MR_IMMI:
			AND_MR_IMMI();
			pC += 5;
			break;
		case _OR_R_M:
			OR_R_M();
			pC += 5;
			break;
		case _OR_M_R:
			OR_M_R();
			pC += 5;
			break;
		case _OR_R_R:
			OR_R_R();
			pC += 2;
			break;
		case _OR_M_M:
			OR_M_M();
			pC += 8;
			break;
		case _OR_MR_R:
			OR_MR_R();
			pC += 2;
			break;
		case _OR_R_MR:
			OR_R_MR();
			pC += 2;
			break;
		case _OR_MR_M:
			OR_MR_M();
			pC += 5;
			break;
		case _OR_M_MR:
			OR_M_MR();
			pC += 5;
			break;
		case _OR_MR_MR:
			OR_MR_MR();
			pC += 2;
			break;
		case _OR_R_IMMI:
			OR_R_IMMI();
			pC += 5;
			break;
		case _OR_M_IMMI:
			OR_M_IMMI();
			pC += 8;
			break;
		case _OR_MR_IMMI:
			OR_MR_IMMI();
			pC += 5;
			break;
		case _XOR_R_M:
			XOR_R_M();
			pC += 5;
			break;
		case _XOR_M_R:
			XOR_M_R();
			pC += 5;
			break;
		case _XOR_R_R:
			XOR_R_R();
			pC += 2;
			break;
		case _XOR_M_M:
			XOR_M_M();
			pC += 8;
			break;
		case _XOR_MR_R:
			XOR_MR_R();
			pC += 2;
			break;
		case _XOR_R_MR:
			XOR_R_MR();
			pC += 2;
			break;
		case _XOR_MR_M:
			XOR_MR_M();
			pC += 5;
			break;
		case _XOR_M_MR:
			XOR_M_MR();
			pC += 5;
			break;
		case _XOR_MR_MR:
			XOR_MR_MR();
			pC += 2;
			break;
		case _XOR_R_IMMI:
			XOR_R_IMMI();
			pC += 5;
			break;
		case _XOR_M_IMMI:
			XOR_M_IMMI();
			pC += 8;
			break;
		case _XOR_MR_IMMI:
			XOR_MR_IMMI();
			pC += 5;
			break;
		case _SHL_R_IMMI8:
			SHL_R_IMMI8();
			pC += 2;
			break;
		case _SHL_M_IMMI8:
			SHL_M_IMMI8();
			pC += 5;
			break;
		case _SHL_MR_IMMI8:
			SHL_MR_IMMI8();
			pC += 2;
			break;
		case _SHL_R_R:
			SHL_R_R();
			pC += 2;
			break;
		case _SHL_M_R:
			SHL_M_R();
			pC += 5;
			break;
		case _SHL_MR_R:
			SHL_MR_R();
			pC += 2;
			break;
		case _SHL_R_M:
			SHL_R_M();
			pC += 5;
			break;
		case _SHL_M_M:
			SHL_M_M();
			pC += 8;
			break;
		case _SHL_MR_M:
			SHL_MR_M();
			pC += 5;
			break;
		case _SHL_R_MR:
			SHL_R_MR();
			pC += 2;
			break;
		case _SHL_M_MR:
			SHL_M_MR();
			pC += 5;
			break;
		case _SHL_MR_MR:
			SHL_MR_MR();
			pC += 2;
			break;
		case _SHR_R_IMMI8:
			SHR_R_IMMI8();
			pC += 2;
			break;
		case _SHR_M_IMMI8:
			SHR_M_IMMI8();
			pC += 5;
			break;
		case _SHR_MR_IMMI8:
			SHR_MR_IMMI8();
			pC += 2;
			break;
		case _SHR_R_R:
			SHR_R_R();
			pC += 2;
			break;
		case _SHR_M_R:
			SHR_M_R();
			pC += 5;
			break;
		case _SHR_MR_R:
			SHR_MR_R();
			pC += 2;
			break;
		case _SHR_R_M:
			SHR_R_M();
			pC += 5;
			break;
		case _SHR_M_M:
			SHR_M_M();
			pC += 8;
			break;
		case _SHR_MR_M:
			SHR_MR_M();
			pC += 5;
			break;
		case _SHR_R_MR:
			SHR_R_MR();
			pC += 2;
			break;
		case _SHR_M_MR:
			SHR_M_MR();
			pC += 5;
			break;
		case _SHR_MR_MR:
			SHR_MR_MR();
			pC += 2;
			break;
		case _NOP:
			break;
		case _JMP_IMMI:
			JMP_IMMI();
			break;
		case _JMPR_IMMI:
			JMPR_IMMI();
			pC += 4;
			break;
		case _JC_R_IMMI:
			JC_R_IMMI();
			break;
		case _JCR_R_IMMI:
			JCR_R_IMMI();
			pC += 5;
			break;
		case _MOVP_R_M:
			MOVP_R_M();
			pC += 5;
			break;
		case _MOVP_M_R:
			MOVP_M_R();
			pC += 5;
			break;
		case _MOVP_R_MR:
			MOVP_R_MR();
			pC += 2;
			break;
		case _MOVP_MR_R:
			MOVP_MR_R();
			pC += 2;
			break;
		case _CALL_IMMI:
			CALL_M();
			break;
		case _RET:
			RET();
			break;
		case _PUSH_R:
			PUSH_R();
			++pC;
			break;
		case _POP_R:
			POP_R();
			++pC;
			break;
		case _DRW_R_R_R:
			DRW_R_R_R();
			pC += 3;
			break;
		case _DRW_R_R_M:
			DRW_R_R_M();
			pC += 6;
			break;
		case _OUT_R_IMMI8:
			OUT_R_IMMI8();
			pC += 2;
			break;
		case _OUT_R_IMMI16:
			OUT_R_IMMI16();
			pC += 3;
			break;
		case _OUT_R_IMMI32:
			OUT_R_IMMI32();
			pC += 5;
			break;
		case _OUT_R_IMMI64:
			OUT_R_IMMI64();
			pC += 9;
			break;
		case _OUT_IMMI_R8:
			OUT_IMMI_R8();
			pC += 5;
			break;
		case _OUT_IMMI_R16:
			OUT_IMMI_R16();
			pC += 5;
			break;
		case _OUT_IMMI_R32:
			OUT_IMMI_R32();
			pC += 5;
			break;
		case _OUT_IMMI_R64:
			OUT_IMMI_R64();
			pC += 5;
			break;
		case _OUT_R_R8:
			OUT_R_R8();
			pC += 2;
			break;
		case _OUT_R_R16:
			OUT_R_R16();
			pC += 2;
			break;
		case _OUT_R_R32:
			OUT_R_R32();
			pC += 2;
			break;
		case _OUT_R_R64:
			OUT_R_R64();
			pC += 2;
			break;
		case _OUT_IMMI_IMMI8:
			OUT_IMMI_IMMI8();
			pC += 5;
			break;
		case _OUT_IMMI_IMMI16:
			OUT_IMMI_IMMI16();
			pC += 6;
			break;
		case _OUT_IMMI_IMMI32:
			OUT_IMMI_IMMI32();
			pC += 8;
			break;
		case _OUT_IMMI_IMMI64:
			OUT_IMMI_IMMI64();
			pC += 12;
			break;
		case _IN_R8_IMMI:
			IN_R8_IMMI();
			pC += 5;
			break;
		case _IN_R16_IMMI:
			IN_R16_IMMI();
			pC += 5;
			break;
		case _IN_R32_IMMI:
			IN_R32_IMMI();
			pC += 5;
			break;
		case _IN_R64_IMMI:
			IN_R64_IMMI();
			pC += 5;
			break;
		case _IN_R8_R:
			IN_R8_R();
			pC += 2;
			break;
		case _IN_R16_R:
			IN_R16_R();
			pC += 2;
			break;
		case _IN_R32_R:
			IN_R32_R();
			pC += 2;
			break;
		case _IN_R64_R:
			IN_R64_R();
			pC += 2;
			break;
//////////////////////////////////////////////
		case _FCON_R_FR:
			FCON_R_FR();
			pC += 2;
			break;
		case _FCON_FR_R:
			FCON_FR_R();
			pC += 2;
			break;
		case _FCON_MR_FR:
			FCON_MR_FR();
			pC += 2;
			break;
		case _FCON_FR_MR:
			FCON_FR_MR();
			pC += 2;
			break;
		case _FCON_M_FR:
			FCON_M_FR();
			pC += 5;
			break;
		case _FCON_FR_M:
			FCON_FR_M();
			pC += 5;
			break;
		case _FCON_R_FM:
			FCON_R_FM();
			pC += 5;
			break;
		case _FCON_FM_R:
			FCON_FM_R();
			pC += 5;
			break;
		case _FCON_MR_FM:
			FCON_MR_FM();
			pC += 5;
			break;
		case _FCON_FM_MR:
			FCON_FM_MR();
			pC += 5;
			break;
		case _FCON_M_FM:
			FCON_M_FM();
			pC += 8;
			break;
		case _FCON_FM_M:
			FCON_FM_M();
			pC += 8;
			break;
		case _FCON_R_MFR:
			FCON_R_MFR();
			pC += 2;
			break;
		case _FCON_MFR_R:
			FCON_MFR_R();
			pC += 2;
			break;
		case _FCON_MR_MFR:
			FCON_MR_MFR();
			pC += 2;
			break;
		case _FCON_MFR_MR:
			FCON_MFR_MR();
			pC += 2;
			break;
		case _FCON_M_MFR:
			FCON_M_MFR();
			pC += 5;
			break;
		case _FCON_MFR_M:
			FCON_MFR_M();
			pC += 5;
			break;
		case _FCON_FR_IMMI:
			FCON_FR_IMMI();
			pC += 5;
			break;
		case _FCON_FM_IMMI:
			FCON_FM_IMMI();
			pC += 8;
			break;
		case _FCON_MFR_IMMI:
			FCON_MFR_IMMI();
			pC += 5;
			break;
		case _FMOV_FR_FR:
			FMOV_FR_FR();
			pC += 2;
			break;
		case _FMOV_FR_FM:
			FMOV_FR_FM();
			pC += 5;
			break;
		case _FMOV_FM_FR:
			FMOV_FM_FR();
			pC += 5;
			break;
		case _FMOV_FR_MFR:
			FMOV_FR_MFR();
			pC += 2;
			break;
		case _FMOV_MFR_FR:
			FMOV_MFR_FR();
			pC += 2;
			break;
		case _FMOV_FM_MFR:
			FMOV_FM_MFR();
			pC += 5;
			break;
		case _FMOV_MFR_FM:
			FMOV_MFR_FM();
			pC += 5;
			break;
		case _FMOV_MFR_MFR:
			FMOV_MFR_MFR();
			pC += 2;
			break;
		case _FMOV_FM_FM:
			FMOV_FM_FM();
			pC += 8;
			break;
		case _FMOV_FR_FIMMI:
			FMOV_FR_FIMMI();
			pC += 5;
			break;
		case _FMOV_FM_FIMMI:
			FMOV_FM_FIMMI();
			pC += 8;
			break;
		case _FMOV_MFR_FIMMI:
			FMOV_MFR_FIMMI();
			pC += 5;
			break;
		case _FADD_FR_FR:
			FADD_FR_FR();
			pC += 2;
			break;
		case _FADD_FR_FM:
			FADD_FR_FM();
			pC += 5;
			break;
		case _FADD_FM_FR:
			FADD_FM_FR();
			pC += 5;
			break;
		case _FADD_FR_MFR:
			FADD_FR_MFR();
			pC += 2;
			break;
		case _FADD_MFR_FR:
			FADD_MFR_FR();
			pC += 2;
			break;
		case _FADD_R_FR:
			FADD_R_FR();
			pC += 2;
			break;
		case _FADD_FR_R:
			FADD_FR_R();
			pC += 2;
			break;
		case _FADD_MR_FR:
			FADD_MR_FR();
			pC += 2;
			break;
		case _FADD_FR_MR:
			FADD_FR_MR();
			pC += 2;
			break;
		case _FADD_M_FR:
			FADD_M_FR();
			pC += 5;
			break;
		case _FADD_FR_M:
			FADD_FR_M();
			pC += 5;
			break;
		case _FADD_R_FM:
			FADD_R_FM();
			pC += 5;
			break;
		case _FADD_FM_R:
			FADD_FM_R();
			pC += 5;
			break;
		case _FADD_MR_FM:
			FADD_MR_FM();
			pC += 5;
			break;
		case _FADD_FM_MR:
			FADD_FM_MR();
			pC += 5;
			break;
		case _FADD_M_FM:
			FADD_M_FM();
			pC += 8;
			break;
		case _FADD_FM_M:
			FADD_FM_M();
			pC += 8;
			break;
		case _FADD_R_MFR:
			FADD_R_MFR();
			pC += 2;
			break;
		case _FADD_MFR_R:
			FADD_MFR_R();
			pC += 2;
			break;
		case _FADD_MR_MFR:
			FADD_MR_MFR();
			pC += 2;
			break;
		case _FADD_MFR_MR:
			FADD_MFR_MR();
			pC += 2;
			break;
		case _FADD_M_MFR:
			FADD_M_MFR();
			pC += 5;
			break;
		case _FADD_MFR_M:
			FADD_MFR_M();
			pC += 5;
			break;
		case _FADD_FR_FIMMI:
			FADD_FR_FIMMI();
			pC += 5;
			break;
		case _FADD_FM_FIMMI:
			FADD_FM_FIMMI();
			pC += 8;
			break;
		case _FADD_MFR_FIMMI:
			FADD_MFR_FIMMI();
			pC += 5;
			break;
		case _FADD_R_FIMMI:
			FADD_R_FIMMI();
			pC += 5;
			break;
		case _FADD_M_FIMMI:
			FADD_M_FIMMI();
			pC += 8;
			break;
		case _FADD_MR_FIMMI:
			FADD_MR_FIMMI();
			pC += 5;
			break;
		case _FSUB_FR_FR:
			FSUB_FR_FR();
			pC += 2;
			break;
		case _FSUB_FR_FM:
			FSUB_FR_FM();
			pC += 5;
			break;
		case _FSUB_FM_FR:
			FSUB_FM_FR();
			pC += 5;
			break;
		case _FSUB_FR_MFR:
			FSUB_FR_MFR();
			pC += 2;
			break;
		case _FSUB_MFR_FR:
			FSUB_MFR_FR();
			pC += 2;
			break;
		case _FSUB_R_FR:
			FSUB_R_FR();
			pC += 2;
			break;
		case _FSUB_FR_R:
			FSUB_FR_R();
			pC += 2;
			break;
		case _FSUB_MR_FR:
			FSUB_MR_FR();
			pC += 2;
			break;
		case _FSUB_FR_MR:
			FSUB_FR_MR();
			pC += 2;
			break;
		case _FSUB_M_FR:
			FSUB_M_FR();
			pC += 5;
			break;
		case _FSUB_FR_M:
			FSUB_FR_M();
			pC += 5;
			break;
		case _FSUB_R_FM:
			FSUB_R_FM();
			pC += 5;
			break;
		case _FSUB_FM_R:
			FSUB_FM_R();
			pC += 5;
			break;
		case _FSUB_MR_FM:
			FSUB_MR_FM();
			pC += 5;
			break;
		case _FSUB_FM_MR:
			FSUB_FM_MR();
			pC += 5;
			break;
		case _FSUB_M_FM:
			FSUB_M_FM();
			pC += 8;
			break;
		case _FSUB_FM_M:
			FSUB_FM_M();
			pC += 8;
			break;
		case _FSUB_R_MFR:
			FSUB_R_MFR();
			pC += 2;
			break;
		case _FSUB_MFR_R:
			FSUB_MFR_R();
			pC += 2;
			break;
		case _FSUB_MR_MFR:
			FSUB_MR_MFR();
			pC += 2;
			break;
		case _FSUB_MFR_MR:
			FSUB_MFR_MR();
			pC += 2;
			break;
		case _FSUB_M_MFR:
			FSUB_M_MFR();
			pC += 5;
			break;
		case _FSUB_MFR_M:
			FSUB_MFR_M();
			pC += 5;
			break;
		case _FSUB_FR_FIMMI:
			FSUB_FR_FIMMI();
			pC += 5;
			break;
		case _FSUB_FM_FIMMI:
			FSUB_FM_FIMMI();
			pC += 8;
			break;
		case _FSUB_MFR_FIMMI:
			FSUB_MFR_FIMMI();
			pC += 5;
			break;
		case _FSUB_R_FIMMI:
			FSUB_R_FIMMI();
			pC += 5;
			break;
		case _FSUB_M_FIMMI:
			FSUB_M_FIMMI();
			pC += 8;
			break;
		case _FSUB_MR_FIMMI:
			FSUB_MR_FIMMI();
			pC += 5;
			break;
		case _FMUL_FR_FR:
			FMUL_FR_FR();
			pC += 2;
			break;
		case _FMUL_FR_FM:
			FMUL_FR_FM();
			pC += 5;
			break;
		case _FMUL_FM_FR:
			FMUL_FM_FR();
			pC += 5;
			break;
		case _FMUL_FR_MFR:
			FMUL_FR_MFR();
			pC += 2;
			break;
		case _FMUL_MFR_FR:
			FMUL_MFR_FR();
			pC += 2;
			break;
		case _FMUL_R_FR:
			FMUL_R_FR();
			pC += 2;
			break;
		case _FMUL_FR_R:
			FMUL_FR_R();
			pC += 2;
			break;
		case _FMUL_MR_FR:
			FMUL_MR_FR();
			pC += 2;
			break;
		case _FMUL_FR_MR:
			FMUL_FR_MR();
			pC += 2;
			break;
		case _FMUL_M_FR:
			FMUL_M_FR();
			pC += 5;
			break;
		case _FMUL_FR_M:
			FMUL_FR_M();
			pC += 5;
			break;
		case _FMUL_R_FM:
			FMUL_R_FM();
			pC += 5;
			break;
		case _FMUL_FM_R:
			FMUL_FM_R();
			pC += 5;
			break;
		case _FMUL_MR_FM:
			FMUL_MR_FM();
			pC += 5;
			break;
		case _FMUL_FM_MR:
			FMUL_FM_MR();
			pC += 5;
			break;
		case _FMUL_M_FM:
			FMUL_M_FM();
			pC += 8;
			break;
		case _FMUL_FM_M:
			FMUL_FM_M();
			pC += 8;
			break;
		case _FMUL_R_MFR:
			FMUL_R_MFR();
			pC += 2;
			break;
		case _FMUL_MFR_R:
			FMUL_MFR_R();
			pC += 2;
			break;
		case _FMUL_MR_MFR:
			FMUL_MR_MFR();
			pC += 2;
			break;
		case _FMUL_MFR_MR:
			FMUL_MFR_MR();
			pC += 2;
			break;
		case _FMUL_M_MFR:
			FMUL_M_MFR();
			pC += 5;
			break;
		case _FMUL_MFR_M:
			FMUL_MFR_M();
			pC += 5;
			break;
		case _FMUL_FR_FIMMI:
			FMUL_FR_FIMMI();
			pC += 5;
			break;
		case _FMUL_FM_FIMMI:
			FMUL_FM_FIMMI();
			pC += 8;
			break;
		case _FMUL_MFR_FIMMI:
			FMUL_MFR_FIMMI();
			pC += 5;
			break;
		case _FMUL_R_FIMMI:
			FMUL_R_FIMMI();
			pC += 5;
			break;
		case _FMUL_M_FIMMI:
			FMUL_M_FIMMI();
			pC += 8;
			break;
		case _FMUL_MR_FIMMI:
			FMUL_MR_FIMMI();
			pC += 5;
			break;
		case _FDIV_FR_FR:
			FDIV_FR_FR();
			pC += 2;
			break;
		case _FDIV_FR_FM:
			FDIV_FR_FM();
			pC += 5;
			break;
		case _FDIV_FM_FR:
			FDIV_FM_FR();
			pC += 5;
			break;
		case _FDIV_FR_MFR:
			FDIV_FR_MFR();
			pC += 2;
			break;
		case _FDIV_MFR_FR:
			FDIV_MFR_FR();
			pC += 2;
			break;
		case _FDIV_R_FR:
			FDIV_R_FR();
			pC += 2;
			break;
		case _FDIV_FR_R:
			FDIV_FR_R();
			pC += 2;
			break;
		case _FDIV_MR_FR:
			FDIV_MR_FR();
			pC += 2;
			break;
		case _FDIV_FR_MR:
			FDIV_FR_MR();
			pC += 2;
			break;
		case _FDIV_M_FR:
			FDIV_M_FR();
			pC += 5;
			break;
		case _FDIV_FR_M:
			FDIV_FR_M();
			pC += 5;
			break;
		case _FDIV_R_FM:
			FDIV_R_FM();
			pC += 5;
			break;
		case _FDIV_FM_R:
			FDIV_FM_R();
			pC += 5;
			break;
		case _FDIV_MR_FM:
			FDIV_MR_FM();
			pC += 5;
			break;
		case _FDIV_FM_MR:
			FDIV_FM_MR();
			pC += 5;
			break;
		case _FDIV_M_FM:
			FDIV_M_FM();
			pC += 8;
			break;
		case _FDIV_FM_M:
			FDIV_FM_M();
			pC += 8;
			break;
		case _FDIV_R_MFR:
			FDIV_R_MFR();
			pC += 2;
			break;
		case _FDIV_MFR_R:
			FDIV_MFR_R();
			pC += 2;
			break;
		case _FDIV_MR_MFR:
			FDIV_MR_MFR();
			pC += 2;
			break;
		case _FDIV_MFR_MR:
			FDIV_MFR_MR();
			pC += 2;
			break;
		case _FDIV_M_MFR:
			FDIV_M_MFR();
			pC += 5;
			break;
		case _FDIV_MFR_M:
			FDIV_MFR_M();
			pC += 5;
			break;
		case _FDIV_FR_FIMMI:
			FDIV_FR_FIMMI();
			pC += 5;
			break;
		case _FDIV_FM_FIMMI:
			FDIV_FM_FIMMI();
			pC += 8;
			break;
		case _FDIV_MFR_FIMMI:
			FDIV_MFR_FIMMI();
			pC += 5;
			break;
		case _FDIV_R_FIMMI:
			FDIV_R_FIMMI();
			pC += 5;
			break;
		case _FDIV_M_FIMMI:
			FDIV_M_FIMMI();
			pC += 8;
			break;
		case _FDIV_MR_FIMMI:
			FDIV_MR_FIMMI();
			pC += 5;
			break;
		case _FMOD_FR_FR:
			FMOD_FR_FR();
			pC += 2;
			break;
		case _FMOD_FR_FM:
			FMOD_FR_FM();
			pC += 5;
			break;
		case _FMOD_FM_FR:
			FMOD_FM_FR();
			pC += 5;
			break;
		case _FMOD_FR_MFR:
			FMOD_FR_MFR();
			pC += 2;
			break;
		case _FMOD_MFR_FR:
			FMOD_MFR_FR();
			pC += 2;
			break;
		case _FMOD_R_FR:
			FMOD_R_FR();
			pC += 2;
			break;
		case _FMOD_FR_R:
			FMOD_FR_R();
			pC += 2;
			break;
		case _FMOD_MR_FR:
			FMOD_MR_FR();
			pC += 2;
			break;
		case _FMOD_FR_MR:
			FMOD_FR_MR();
			pC += 2;
			break;
		case _FMOD_M_FR:
			FMOD_M_FR();
			pC += 5;
			break;
		case _FMOD_FR_M:
			FMOD_FR_M();
			pC += 5;
			break;
		case _FMOD_R_FM:
			FMOD_R_FM();
			pC += 5;
			break;
		case _FMOD_FM_R:
			FMOD_FM_R();
			pC += 5;
			break;
		case _FMOD_MR_FM:
			FMOD_MR_FM();
			pC += 5;
			break;
		case _FMOD_FM_MR:
			FMOD_FM_MR();
			pC += 5;
			break;
		case _FMOD_M_FM:
			FMOD_M_FM();
			pC += 8;
			break;
		case _FMOD_FM_M:
			FMOD_FM_M();
			pC += 8;
			break;
		case _FMOD_R_MFR:
			FMOD_R_MFR();
			pC += 2;
			break;
		case _FMOD_MFR_R:
			FMOD_MFR_R();
			pC += 2;
			break;
		case _FMOD_MR_MFR:
			FMOD_MR_MFR();
			pC += 2;
			break;
		case _FMOD_MFR_MR:
			FMOD_MFR_MR();
			pC += 2;
			break;
		case _FMOD_M_MFR:
			FMOD_M_MFR();
			pC += 5;
			break;
		case _FMOD_MFR_M:
			FMOD_MFR_M();
			pC += 5;
			break;
		case _FMOD_FR_FIMMI:
			FMOD_FR_FIMMI();
			pC += 5;
			break;
		case _FMOD_FM_FIMMI:
			FMOD_FM_FIMMI();
			pC += 8;
			break;
		case _FMOD_MFR_FIMMI:
			FMOD_MFR_FIMMI();
			pC += 5;
			break;
		case _FMOD_R_FIMMI:
			FMOD_R_FIMMI();
			pC += 5;
			break;
		case _FMOD_M_FIMMI:
			FMOD_M_FIMMI();
			pC += 8;
			break;
		case _FMOD_MR_FIMMI:
			FMOD_MR_FIMMI();
			pC += 5;
			break;
		case _BMOV_R_BM:
			BMOV_R_BM();
			pC += 5;
			break;
		case _BMOV_BM_R:
			BMOV_BM_R();
			pC += 5;
			break;
		case _BMOV_R_BR:
			BMOV_R_BR();
			pC += 2;
			break;
		case _BMOV_BR_R:
			BMOV_BR_R();
			pC += 2;
			break;
		case _BMOV_BR_BR:
			BMOV_BR_BR();
			pC += 2;
			break;
		case _BMOV_BM_BM:
			BMOV_BM_BM();
			pC += 8;
			break;
		case _BMOV_BR_IMMI8:
			BMOV_BR_IMMI8();
			pC += 2;
			break;
		case _BMOV_BM_IMMI8:
			BMOV_BM_IMMI8();
			pC += 5;
			break;
		case _CMPE_R_R:
			CMPE_R_R();
			pC += 2;
			break;
		case _CMPNE_R_R:
			CMPNE_R_R();
			pC += 2;
			break;
		case _CMPG_R_R:
			CMPG_R_R();
			pC += 2;
			break;
		case _CMPL_R_R:
			CMPL_R_R();
			pC += 2;
			break;
		case _CMPGE_R_R:
			CMPGE_R_R();
			pC += 2;
			break;
		case _CMPLE_R_R:
			CMPLE_R_R();
			pC += 2;
			break;
		case _CMPE_R_FR_FR:
			CMPE_R_FR_FR();
			pC += 3;
			break;
		case _CMPNE_R_FR_FR:
			CMPNE_R_FR_FR();
			pC += 3;
			break;
		case _CMPG_R_FR_FR:
			CMPG_R_FR_FR();
			pC += 3;
			break;
		case _CMPL_R_FR_FR:
			CMPL_R_FR_FR();
			pC += 3;
			break;
		case _CMPGE_R_FR_FR:
			CMPGE_R_FR_FR();
			pC += 3;
			break;
		case _CMPLE_R_FR_FR:
			CMPLE_R_FR_FR();
			pC += 3;
			break;
		case _FOUT_IMMI_FR:
			FOUT_IMMI_FR();
			pC += 5;
			break;
		case _FIN_IMMI_FR:
			FIN_IMMI_FR();
			pC += 5;
			break;
		case _FOUT_IMMI_FIMMI:
			FOUT_IMMI_FIMMI();
			pC += 8;
			break;
		case _CMPE_R_IMMI:
			CMPE_R_IMMI();
			pC += 5;
			break;
		case _CMPNE_R_IMMI:
			CMPNE_R_IMMI();
			pC += 5;
			break;
		case _CMPG_R_IMMI:
			CMPG_R_IMMI();
			pC += 5;
			break;
		case _CMPGE_R_IMMI:
			CMPGE_R_IMMI();
			pC += 5;
			break;
		case _CMPL_R_IMMI:
			CMPL_R_IMMI();
			pC += 5;
			break;
		case _CMPLE_R_IMMI:
			CMPLE_R_IMMI();
			pC += 5;
			break;
		case _TIME:
			TIME();
			break;
		case _SLEEP:
			SLEEP();
			break;
		case _RAND:
			RAND();
	}
	return true;
}