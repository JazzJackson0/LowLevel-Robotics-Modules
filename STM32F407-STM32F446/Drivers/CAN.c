#include "CAN.h"


//Static Prototypes------------------------------------------------------
static void CAN_PinInit(uint8_t canNum);
static CANx* Get_CAN(uint8_t canNum);
static MAIL_FILTERSx* Get_MailBoxANDFilters(uint8_t canNum);
static FILTER_BANKSx* Get_FilterBanks(uint8_t canNum);
static void Set_FilterNumber_AssignFIFO_SetFilterMode(MAIL_FILTERSx *const CANMailFilters, 
	uint8_t filterNumber, E_FilterSize filterSize, uint8_t fifoNumber, E_FilterMode filterMode);

//Global Variables-------------------------------------------------------
CAN_CLOCK *const CANClock = ADDR_CAN_CLOCK;

//CAN
CANx *const CAN1 = ADDR_CAN1;
MAIL_FILTERSx *const CAN1MailFilters = ADDR_CAN1_MAIL_FILTERS;
FILTER_BANKSx *const CAN1Bank = ADDR_CAN2_FILTERBANKS;
CANx *const CAN2 = ADDR_CAN2;
MAIL_FILTERSx *const CAN2MailFilters = ADDR_CAN2_MAIL_FILTERS;
FILTER_BANKSx *const CAN2Bank = ADDR_CAN2_FILTERBANKS;


void CAN_SetBitTime(uint8_t canNum, uint8_t baudRatePrescale, uint8_t resyncJump, 
	uint8_t timeSeg1, uint8_t timeSeg2) {
	
	CANx *CAN = Get_CAN(canNum);
	MAIL_FILTERSx *CANMailFilters = Get_MailBoxANDFilters(canNum);
	
	CAN->BitTimingReg.rw_BaudRatePrescaler = baudRatePrescale;
	CAN->BitTimingReg.rw_ReSyncJumpWidth = resyncJump;
	CAN->BitTimingReg.rw_TimeSegment1 = timeSeg1;
	CAN->BitTimingReg.rw_TimeSegment2 = timeSeg2;
}

void CAN_SetAcceptanceFilter(uint8_t canNum, Filter *filterInfo) {

	CANx *const CAN = Get_CAN(canNum);
	MAIL_FILTERSx *const CANMailFilters = Get_MailBoxANDFilters(canNum);
	FILTER_BANKSx *const CANBank = Get_FilterBanks(canNum);
	uint8_t bankNum = filterInfo->filterBankRegNum;
	uint64_t identiferORMaskVal = filterInfo->identiferORMask;

	CANMailFilters->FilterMasterReg.filterActive0_filterReadyToInitialize1 = 1;
	
	Set_FilterNumber_AssignFIFO_SetFilterMode(CANMailFilters, filterInfo->filterNum, filterInfo->filterSize, 
		filterInfo->fifoNum, filterInfo->filterMode);
	
	CANBank->Banks[bankNum].bit0_IdentiferORMask = identiferORMaskVal % 10;
	identiferORMaskVal /= 10;
	CANBank->Banks[bankNum].bit1_IdentiferORMask = identiferORMaskVal % 10;
	identiferORMaskVal /= 10;
	CANBank->Banks[bankNum].bit2_IdentiferORMask = identiferORMaskVal % 10;
	identiferORMaskVal /= 10;
	CANBank->Banks[bankNum].bit3_IdentiferORMask = identiferORMaskVal % 10;
	identiferORMaskVal /= 10;
	CANBank->Banks[bankNum].bit4_IdentiferORMask = identiferORMaskVal % 10;
	identiferORMaskVal /= 10;
	CANBank->Banks[bankNum].bit5_IdentiferORMask = identiferORMaskVal % 10;
	identiferORMaskVal /= 10;
	CANBank->Banks[bankNum].bit6_IdentiferORMask = identiferORMaskVal % 10;
	identiferORMaskVal /= 10;
	CANBank->Banks[bankNum].bit7_IdentiferORMask = identiferORMaskVal % 10;
	identiferORMaskVal /= 10;
	CANBank->Banks[bankNum].bit8_IdentiferORMask = identiferORMaskVal % 10;
	identiferORMaskVal /= 10;
	CANBank->Banks[bankNum].bit9_IdentiferORMask = identiferORMaskVal % 10;
	identiferORMaskVal /= 10;
	CANBank->Banks[bankNum].bit10_IdentiferORMask = identiferORMaskVal % 10;
	identiferORMaskVal /= 10;
	CANBank->Banks[bankNum].bit11_IdentiferORMask = identiferORMaskVal % 10;
	identiferORMaskVal /= 10;
	CANBank->Banks[bankNum].bit12_IdentiferORMask = identiferORMaskVal % 10;
	identiferORMaskVal /= 10;
	CANBank->Banks[bankNum].bit13_IdentiferORMask = identiferORMaskVal % 10;
	identiferORMaskVal /= 10;
	CANBank->Banks[bankNum].bit14_IdentiferORMask = identiferORMaskVal % 10;
	identiferORMaskVal /= 10;
	CANBank->Banks[bankNum].bit15_IdentiferORMask = identiferORMaskVal % 10;

	CANMailFilters->FilterMasterReg.filterActive0_filterReadyToInitialize1 = 0;
	CANMailFilters->FilterActivationReg.enable_Filter0 = 1;
}

void CAN_SetTXMailbox(uint8_t canNum, uint8_t mailboxNum, uint64_t standardID, uint8_t numOfBytes) {
	
	MAIL_FILTERSx *const CANMailFilters = Get_MailBoxANDFilters(canNum);
	
	switch (canNum) {
		case 1 :
			CANClock->can1_StartTick = 1;
		case 2 : 
			CANClock->can2_StartTick = 1;
	}
	
	
	if (numOfBytes < 0 || numOfBytes > 8) numOfBytes = 0; 
	
	switch (mailboxNum) {

		case 0:
			CANMailFilters->TXMailBox0IDReg.StandardIdentifier.rw_StandardIdentifier = standardID;
			CANMailFilters->TXMaiBox0DataLenCtrlTimeStampReg.rw_DataLen_InBytes = numOfBytes;
			break;
		case 1:
			CANMailFilters->TXMailBox1IDReg.StandardIdentifier.rw_StandardIdentifier = standardID;
			CANMailFilters->TXMaiBox0DataLenCtrlTimeStampReg.rw_DataLen_InBytes = numOfBytes;
			break;
		case 2:
			CANMailFilters->TXMailBox2IDReg.StandardIdentifier.rw_StandardIdentifier = standardID;
			CANMailFilters->TXMaiBox0DataLenCtrlTimeStampReg.rw_DataLen_InBytes = numOfBytes;
			break;
	}
	
}

void CAN_SetRXMailbox(uint8_t canNum, uint8_t mailboxNum, uint64_t standardID, uint8_t numOfBytes) {
	
	MAIL_FILTERSx *const CANMailFilters = Get_MailBoxANDFilters(canNum);
	
	switch (canNum) {
		case 1 :
			CANClock->can1_StartTick = 1;
		case 2 : 
			CANClock->can2_StartTick = 1;
	}
	
	
	if (numOfBytes < 0 || numOfBytes > 8) numOfBytes = 0; 
	
	switch (mailboxNum) {

		case 0:
			CANMailFilters->RXMailBox0IDReg.StandardIdentifier.read_StandardIdentifier = standardID;
			CANMailFilters->RXMaiBox0DataLenCtrlTimeStampReg.read_DataLen_InBytes = numOfBytes;
			break;
		case 1:
			CANMailFilters->RXMailBox1IDReg.StandardIdentifier.read_StandardIdentifier = standardID;
			CANMailFilters->RXMaiBox1DataLenCtrlTimeStampReg.read_DataLen_InBytes = numOfBytes;
			break;
	}
}


void CAN_Init_and_Start(uint8_t canNum){

	CAN_PinInit(canNum);
	CANx *const CAN = Get_CAN(canNum);
	CAN->MasterControlReg.enable_AutoExitBusOffState = 1;
	CAN->MasterControlReg.enable_AutoWakeUp = 1;
	CAN->MasterControlReg.enable_LockRXWhenFull = 1;
	CAN->MasterControlReg.txOrderIdentifier0_txOrderChronological1 = 1;
	
	//Starting CAN.....
	CAN->MasterControlReg.send_InitializeModeRequest = 0; //Sends Request that Hardware enter 'Normal Mode;
	while (CAN->MasterStatusReg.initializeModeRequestAccepted == 1); //Syncing to CAN Bus (Waiting for 11 Recessive Bits)
}

uint8_t CAN_Transmit(uint8_t canNum, uint8_t mailboxNum, uint8_t *dataBuffer, uint8_t numOfBytes) {

	MAIL_FILTERSx *const CANMailFilters = Get_MailBoxANDFilters(canNum);
	
	if (numOfBytes > 8) { numOfBytes = 8; }

	if (numOfBytes > 0) {
		if (mailboxNum == 0) {
			for (int i = 0; i < numOfBytes; i++) {
				if (i > 3) {
					CANMailFilters->TXMailBox0DataHighReg.dataIterator[i] = *dataBuffer++;
				}
				else {
					CANMailFilters->TXMailBox0DataLowReg.dataIterator[i] = *dataBuffer++;
				}
			}
		}
		else if (mailboxNum == 1) {
			for (int i = 0; i < numOfBytes; i++) {
				if (i > 3) {
					CANMailFilters->TXMailBox0DataHighReg.dataIterator[i] = *dataBuffer++;
				}
				else {
					CANMailFilters->TXMailBox0DataLowReg.dataIterator[i] = *dataBuffer++;
				}
			}
		}

		else if (mailboxNum == 2) {
			for (int i = 0; i < numOfBytes; i++) {
				if (i > 3) {
					CANMailFilters->TXMailBox0DataHighReg.dataIterator[i] = *dataBuffer++;
					dataBuffer++;
				}
				else {
					CANMailFilters->TXMailBox0DataLowReg.dataIterator[i] = *dataBuffer++;
				}
			}
		}

		else { return 0; }
	}
	
	switch (mailboxNum) {

		case 0:
			CANMailFilters->TXMailBox0IDReg.StandardIdentifier.request_TransmissionFromMailbox = 1;
			break;
		case 1:
			CANMailFilters->TXMailBox1IDReg.StandardIdentifier.request_TransmissionFromMailbox = 1;
			break;
		case 2:
			CANMailFilters->TXMailBox2IDReg.StandardIdentifier.request_TransmissionFromMailbox = 1;
			break;
	}

	//Check Status Reg for any transmission errors.
	
	return 1;
}


Received_Data* CAN_Receive(uint8_t canNum, uint8_t mailboxNum, uint8_t numOfBytes) {

	MAIL_FILTERSx *const CANMailFilters = Get_MailBoxANDFilters(canNum);
	Received_Data *R_Data = malloc(sizeof(Received_Data));
	
	//uint8_t id = rxMailboxID->StandardIdentifier.read_StandardIdentifier;
	//uint8_t len = txDataCtrlTimeStamp->read_DataLen_InBytes

	if (numOfBytes > 0) {
		if (mailboxNum == 0) {
			for (int i = 0; i < numOfBytes; i++) {
				
				if (i > 3) {
					R_Data->dataIterator[i] = CANMailFilters->TXMailBox0DataHighReg.dataIterator[i];
				}
				else {
					R_Data->dataIterator[i] = CANMailFilters->TXMailBox0DataLowReg.dataIterator[i];
				}
			}
		}
		else if (mailboxNum == 1) {
			for (int i = 0; i < numOfBytes; i++) {
				
				if (i > 3) {
					R_Data->dataIterator[i] = CANMailFilters->TXMailBox0DataHighReg.dataIterator[i];
				}
				else {
					R_Data->dataIterator[i] = CANMailFilters->TXMailBox0DataLowReg.dataIterator[i];
				}
			}
		}
		else { return ((void*)0); }
	}
	return R_Data;
}


//Helper Functions--------------------------------------------------------------------------------------------------------------
static void CAN_PinInit(uint8_t canNum) {

	switch (canNum) {

		case 1 :
			//CAN TX
			Pin_Init('A', 12, AF9);
			//CAN RX
			Pin_Init('A', 11, AF9);
			break;
		case 2 : 
			//CAN TX
			Pin_Init('B', 6, AF9);
			//CAN RX
			Pin_Init('B', 5, AF9);
			break;
	}		
}

static CANx* Get_CAN(uint8_t canNum) {

	switch(canNum) {

		case 1:
			return CAN1;
		case 2:
			return CAN2;
		default:
			return ((void*)0);
	}
}

static MAIL_FILTERSx* Get_MailBoxANDFilters(uint8_t canNum) {

	switch(canNum) {
		case 1:
			return CAN1MailFilters;
		case 2:
			return CAN2MailFilters;
		default:
			return ((void*)0);
	}
}

static FILTER_BANKSx* Get_FilterBanks(uint8_t canNum) {

	switch(canNum) {
		case 1:
			return CAN1Bank;
		case 2:
			return CAN2Bank;
		default:
			return ((void*)0);
	}
}

static void Set_FilterNumber_AssignFIFO_SetFilterMode(MAIL_FILTERSx *const CANMailFilters, 
	uint8_t filterNumber, E_FilterSize filterSize, uint8_t fifoNumber, E_FilterMode filterMode) {

	switch(filterNumber) {
		
		case 0 :
			CANMailFilters->FilterScaleReg.filter0Size16B0_filter0Size32B1 = filterSize;
			CANMailFilters->FilterFIFOAssignReg.filter0ToFIFO0_filter0ToFIFO1 = fifoNumber;
			CANMailFilters->FilterModeReg.filter0IDMaskMode0_filter0IDListMode1 = filterMode;
			break;
		case 1 :
			CANMailFilters->FilterScaleReg.filter1Size16B0_filter1Size32B1 = filterSize;
			CANMailFilters->FilterFIFOAssignReg.filter1ToFIFO0_filter1ToFIFO1 = fifoNumber;
			CANMailFilters->FilterModeReg.filter1IDMaskMode0_filter1IDListMode1 = filterMode;
			break;
		case 2 :
			CANMailFilters->FilterScaleReg.filter2Size16B0_filter2Size32B1 = filterSize;
			CANMailFilters->FilterFIFOAssignReg.filter2ToFIFO0_filter2ToFIFO1 = fifoNumber;
			CANMailFilters->FilterModeReg.filter2IDMaskMode0_filter2IDListMode1 = filterMode;
			break;
		case 3 :
			CANMailFilters->FilterScaleReg.filter3Size16B0_filter3Size32B1 = filterSize;
			CANMailFilters->FilterFIFOAssignReg.filter3ToFIFO0_filter3ToFIFO1 = fifoNumber;
			CANMailFilters->FilterModeReg.filter3IDMaskMode0_filter3IDListMode1 = filterMode;
			break;
		case 4 :
			CANMailFilters->FilterScaleReg.filter4Size16B0_filter4Size32B1 = filterSize;
			CANMailFilters->FilterFIFOAssignReg.filter4ToFIFO0_filter4ToFIFO1 = fifoNumber;
			CANMailFilters->FilterModeReg.filter4IDMaskMode0_filter4IDListMode1 = filterMode;
			break;
		case 5 :
			CANMailFilters->FilterScaleReg.filter5Size16B0_filter5Size32B1 = filterSize;
			CANMailFilters->FilterFIFOAssignReg.filter5ToFIFO0_filter5ToFIFO1 = fifoNumber;
			CANMailFilters->FilterModeReg.filter5IDMaskMode0_filter5IDListMode1 = filterMode;
			break;
		case 6 :
			CANMailFilters->FilterScaleReg.filter6Size16B0_filter6Size32B1 = filterSize;
			CANMailFilters->FilterFIFOAssignReg.filter6ToFIFO0_filter6ToFIFO1 = fifoNumber;
			CANMailFilters->FilterModeReg.filter6IDMaskMode0_filter6IDListMode1 = filterMode;
			break;
		case 7 :
			CANMailFilters->FilterScaleReg.filter7Size16B0_filter7Size32B1 = filterSize;
			CANMailFilters->FilterFIFOAssignReg.filter7ToFIFO0_filter7ToFIFO1 = fifoNumber;
			CANMailFilters->FilterModeReg.filter7IDMaskMode0_filter7IDListMode1 = filterMode;
			break;
		case 8 :
			CANMailFilters->FilterScaleReg.filter8Size16B0_filter8Size32B1 = filterSize;
			CANMailFilters->FilterFIFOAssignReg.filter8ToFIFO0_filter8ToFIFO1 = fifoNumber;
			CANMailFilters->FilterModeReg.filter8IDMaskMode0_filter8IDListMode1 = filterMode;
			break;
		case 9 :
			CANMailFilters->FilterScaleReg.filter9Size16B0_filter9Size32B1 = filterSize;
			CANMailFilters->FilterFIFOAssignReg.filter9ToFIFO0_filter9ToFIFO1 = fifoNumber;
			CANMailFilters->FilterModeReg.filter9IDMaskMode0_filter9IDListMode1 = filterMode;
			break;
		case 10 :
			CANMailFilters->FilterScaleReg.filter10Size16B0_filter10Size32B1 = filterSize;
			CANMailFilters->FilterFIFOAssignReg.filter10ToFIFO0_filter10ToFIFO1 = fifoNumber;
			CANMailFilters->FilterModeReg.filter10IDMaskMode0_filter10IDListMode1 = filterMode;
			break;			
		case 11 :
			CANMailFilters->FilterScaleReg.filter11Size16B0_filter11Size32B1 = filterSize;
			CANMailFilters->FilterFIFOAssignReg.filter11ToFIFO0_filter11ToFIFO1 = fifoNumber;	
			CANMailFilters->FilterModeReg.filter11IDMaskMode0_filter11IDListMode1 = filterMode;
			break;
		case 12 :
			CANMailFilters->FilterScaleReg.filter12Size16B0_filter12Size32B1 = filterSize;
			CANMailFilters->FilterFIFOAssignReg.filter12ToFIFO0_filter12ToFIFO1 = fifoNumber;	
			CANMailFilters->FilterModeReg.filter12IDMaskMode0_filter12IDListMode1 = filterMode;
			break;
		case 13 :
			CANMailFilters->FilterScaleReg.filter13Size16B0_filter13Size32B1 = filterSize;
			CANMailFilters->FilterFIFOAssignReg.filter13ToFIFO0_filter13ToFIFO1 = fifoNumber;	
			CANMailFilters->FilterModeReg.filter13IDMaskMode0_filter13IDListMode1 = filterMode;
			break;
		case 14 :
			CANMailFilters->FilterScaleReg.filter14Size16B0_filter14Size32B1 = filterSize;
			CANMailFilters->FilterFIFOAssignReg.filter14ToFIFO0_filter14ToFIFO1 = fifoNumber;	
			CANMailFilters->FilterModeReg.filter14IDMaskMode0_filter14IDListMode1 = filterMode;
			break;
		case 15 :
			CANMailFilters->FilterScaleReg.filter15Size16B0_filter15Size32B1 = filterSize;
			CANMailFilters->FilterFIFOAssignReg.filter15ToFIFO0_filter15ToFIFO1 = fifoNumber;	
			CANMailFilters->FilterModeReg.filter15IDMaskMode0_filter15IDListMode1 = filterMode;
			break;
		case 16 :
			CANMailFilters->FilterScaleReg.filter16Size16B0_filter16Size32B1 = filterSize;
			CANMailFilters->FilterFIFOAssignReg.filter16ToFIFO0_filter16ToFIFO1 = fifoNumber;
			CANMailFilters->FilterModeReg.filter16IDMaskMode0_filter16IDListMode1 = filterMode;
			break;			
		case 17 :
			CANMailFilters->FilterScaleReg.filter17Size16B0_filter17Size32B1 = filterSize;
			CANMailFilters->FilterFIFOAssignReg.filter17ToFIFO0_filter17ToFIFO1 = fifoNumber;	
			CANMailFilters->FilterModeReg.filter17IDMaskMode0_filter17IDListMode1 = filterMode;
			break;
		case 18 :
			CANMailFilters->FilterScaleReg.filter18Size16B0_filter18Size32B1 = filterSize;
			CANMailFilters->FilterFIFOAssignReg.filter18ToFIFO0_filter18ToFIFO1 = fifoNumber;	
			CANMailFilters->FilterModeReg.filter18IDMaskMode0_filter18IDListMode1 = filterMode;
			break;
		case 19 :
			CANMailFilters->FilterScaleReg.filter19Size16B0_filter19Size32B1 = filterSize;
			CANMailFilters->FilterFIFOAssignReg.filter19ToFIFO0_filter19ToFIFO1 = fifoNumber;	
			CANMailFilters->FilterModeReg.filter19IDMaskMode0_filter19IDListMode1 = filterMode;
			break;
		case 20 :
			CANMailFilters->FilterScaleReg.filter20Size16B0_filter20Size32B1 = filterSize;
			CANMailFilters->FilterFIFOAssignReg.filter20ToFIFO0_filter20ToFIFO1 = fifoNumber;	
			CANMailFilters->FilterModeReg.filter20IDMaskMode0_filter20IDListMode1 = filterMode;
			break;
		case 21 :
			CANMailFilters->FilterScaleReg.filter21Size16B0_filter21Size32B1 = filterSize;
			CANMailFilters->FilterFIFOAssignReg.filter21ToFIFO0_filter21ToFIFO1 = fifoNumber;	
			CANMailFilters->FilterModeReg.filter21IDMaskMode0_filter21IDListMode1 = filterMode;
			break;
		case 22 :
			CANMailFilters->FilterScaleReg.filter22Size16B0_filter22Size32B1 = filterSize;
			CANMailFilters->FilterFIFOAssignReg.filter22ToFIFO0_filter22ToFIFO1 = fifoNumber;	
			CANMailFilters->FilterModeReg.filter22IDMaskMode0_filter22IDListMode1 = filterMode;
			break;
		case 23 :
			CANMailFilters->FilterScaleReg.filter23Size16B0_filter23Size32B1 = filterSize;
			CANMailFilters->FilterFIFOAssignReg.filter23ToFIFO0_filter23ToFIFO1 = fifoNumber;	
			CANMailFilters->FilterModeReg.filter23IDMaskMode0_filter23IDListMode1 = filterMode;
			break;
		case 24 :
			CANMailFilters->FilterScaleReg.filter24Size16B0_filter24Size32B1 = filterSize;
			CANMailFilters->FilterFIFOAssignReg.filter24ToFIFO0_filter24ToFIFO1 = fifoNumber;
			CANMailFilters->FilterModeReg.filter24IDMaskMode0_filter24IDListMode1 = filterMode;
			break;
		case 25 :
			CANMailFilters->FilterScaleReg.filter25Size16B0_filter25Size32B1 = filterSize;
			CANMailFilters->FilterFIFOAssignReg.filter25ToFIFO0_filter25ToFIFO1 = fifoNumber;
			CANMailFilters->FilterModeReg.filter25IDMaskMode0_filter25IDListMode1 = filterMode;
			break;
		case 26 :
			CANMailFilters->FilterScaleReg.filter26Size16B0_filter26Size32B1 = filterSize;
			CANMailFilters->FilterFIFOAssignReg.filter26ToFIFO0_filter26ToFIFO1 = fifoNumber;	
			CANMailFilters->FilterModeReg.filter26IDMaskMode0_filter26IDListMode1 = filterMode;
			break;
		case 27 :
			CANMailFilters->FilterScaleReg.filter27Size16B0_filter27Size32B1 = filterSize;
			CANMailFilters->FilterFIFOAssignReg.filter27ToFIFO0_filter27ToFIFO1 = fifoNumber;	
			CANMailFilters->FilterModeReg.filter27IDMaskMode0_filter27IDListMode1 = filterMode;
			break;
	}	
}
