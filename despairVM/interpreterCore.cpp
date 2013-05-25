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
		case _MOV_R_MR_IMMI:
			MOV_R_MR_IMMI();
			pC += 6;
			break;
		case _MOV_MR_IMMI_R:
			MOV_MR_IMMI_R();
			pC += 6;
			break;
		case _MOV_MR_IMMI_MR_IMMI:
			MOV_MR_IMMI_MR_IMMI();
			pC += 10;
			break;
		case _MOV_MR_IMMI_IMMI:
			MOV_MR_IMMI_IMMI();
			pC += 9;
			break;
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
		case _ADD_R_R:
			ADD_R_R();
			pC += 2;
			break;
		case _ADD_R_IMMI:
			ADD_R_IMMI();
			pC += 5;
			break;
		case _SUB_R_R:
			SUB_R_R();
			pC += 2;
			break;
		case _SUB_R_IMMI:
			SUB_R_IMMI();
			pC += 5;
			break;
		case _MUL_R_R:
			MUL_R_R();
			pC += 2;
			break;
		case _MUL_R_IMMI:
			MUL_R_IMMI();
			pC += 5;
			break;
		case _DIV_R_R:
			DIV_R_R();
			pC += 2;
			break;
		case _DIV_R_IMMI:
			DIV_R_IMMI();
			pC += 5;
			break;
		case _MOD_R_R:
			MOD_R_R();
			pC += 2;
			break;
		case _MOD_R_IMMI:
			MOD_R_IMMI();
			pC += 5;
			break;
		case _AND_R_R:
			AND_R_R();
			pC += 2;
			break;
		case _AND_R_IMMI:
			AND_R_IMMI();
			pC += 5;
			break;
		case _OR_R_R:
			OR_R_R();
			pC += 2;
			break;
		case _OR_R_IMMI:
			OR_R_IMMI();
			pC += 5;
			break;
		case _XOR_R_R:
			XOR_R_R();
			pC += 2;
			break;
		case _XOR_R_IMMI:
			XOR_R_IMMI();
			pC += 5;
			break;
		case _SHL_R_IMMI8:
			SHL_R_IMMI8();
			pC += 2;
			break;
		case _SHL_R_R:
			SHL_R_R();
			pC += 2;
			break;
		case _SHR_R_IMMI8:
			SHR_R_IMMI8();
			pC += 2;
			break;
		case _SHR_R_R:
			SHR_R_R();
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
		case _MOVP_R_MR_IMMI:
			MOVP_R_MR_IMMI();
			pC += 6;
			break;
		case _MOVP_MR_IMMI_R:
			MOVP_MR_IMMI_R();
			break;
		case _MOVP_MR_IMMI_MR_IMMI:
			MOVP_MR_IMMI_MR_IMMI();
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
		case _DRW_R_R_MR:
			DRW_R_R_MR();
			pC += 3;
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
		case _FCON_R_FR:
			FCON_R_FR();
			pC += 2;
			break;
		case _FCON_FR_R:
			FCON_FR_R();
			pC += 2;
			break;
		case _FCON_FR_IMMI:
			FCON_FR_IMMI();
			pC += 5;
			break;
		case _FMOV_FR_MFR_IMMI:
			FMOV_FR_MFR_IMMI();
			pC += 6;
			break;
		case _FMOV_MFR_IMMI_FR:
			FMOV_MFR_IMMI_FR();
			pC += 6;
			break;
		case _FMOV_MFR_IMMI_MFR_IMMI:
			FMOV_MFR_IMMI_MFR_IMMI();
			pC += 10;
			break;
		case _FMOV_MFR_IMMI_FIMMI:
			FMOV_MFR_IMMI_FIMMI();
			pC += 9;
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
		case _FADD_R_FR:
			FADD_R_FR();
			pC += 2;
			break;
		case _FADD_FR_R:
			FADD_FR_R();
			pC += 2;
			break;
		case _FADD_FR_FIMMI:
			FADD_FR_FIMMI();
			pC += 5;
			break;
		case _FADD_R_FIMMI:
			FADD_R_FIMMI();
			pC += 5;
			break;
		case _FSUB_FR_FR:
			FSUB_FR_FR();
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
		case _FSUB_FR_FIMMI:
			FSUB_FR_FIMMI();
			pC += 5;
			break;
		case _FSUB_R_FIMMI:
			FSUB_R_FIMMI();
			pC += 5;
			break;
		case _FMUL_FR_FR:
			FMUL_FR_FR();
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
		case _FMUL_FR_FIMMI:
			FMUL_FR_FIMMI();
			pC += 5;
			break;
		case _FMUL_R_FIMMI:
			FMUL_R_FIMMI();
			pC += 5;
			break;
		case _FDIV_FR_FR:
			FDIV_FR_FR();
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
		case _FDIV_FR_FIMMI:
			FDIV_FR_FIMMI();
			pC += 5;
			break;
		case _FDIV_R_FIMMI:
			FDIV_R_FIMMI();
			pC += 5;
			break;
		case _FMOD_FR_FR:
			FMOD_FR_FR();
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
		case _FMOD_FR_FIMMI:
			FMOD_FR_FIMMI();
			pC += 5;
			break;
		case _FMOD_R_FIMMI:
			FMOD_R_FIMMI();
			pC += 5;
			break;
		case _BMOV_R_BR_IMMI:
			BMOV_R_BR_IMMI();
			pC += 6;
			break;
		case _BMOV_BR_IMMI_R:
			BMOV_BR_IMMI_R();
			pC += 6;
			break;
		case _BMOV_BR_IMMI_BR_IMMI:
			BMOV_BR_IMMI_BR_IMMI();
			pC += 10;
			break;
		case _BMOV_BR_IMMI_IMMI8:
			BMOV_BR_IMMI_IMMI8();
			pC += 6;
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
			break;
		case _JMP_R:
			JMP_R();
			break;
		case _JMPR_R:
			JMPR_R();
			pC += 1;
			break;
		case _JC_R_R:
			JC_R_R();
			break;
		case _JCR_R_R:
			JCR_R_R();
			pC += 2;
	}
	return true;
}