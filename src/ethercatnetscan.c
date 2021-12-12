/**
 ******************************************************************************
 * @file           :  ethercatnetscan.c
 * @brief          :  scans EtherCAT network for slaves and reports info on them
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include <string.h>
#include <inttypes.h>
#include "shared.h"
#include "ethercat.h"
#include "ethercatprinteeprom.h"
#include "log.h"
#include "user_message.h"

#include "main.h"

//char IOmap[4096];
ec_ODlistt ODlist;
ec_OElistt OElist;

/* These are important: if you set printSDO to true you get the whole SDO map from the slaves ( takes 1 minute to scan)
 * Or set printMAP to just print the PDO mapping
 * */
boolean printSDO = FALSE;
boolean printMAP = TRUE;
char usdo[128];
char hstr[1024];

char* dtype2string(uint16 dtype);
char* SDO2string(uint16 slave, uint16 index, uint8 subidx, uint16 dtype);
int si_PDOassign(uint16 slave, uint16 PDOassign, int mapoffset, int bitoffset);
void si_sdo(int cnt);
int si_map_sdo(int slave);
int si_siiPDO(uint16 slave, uint8 t, int mapoffset, int bitoffset);

//---- convert Ethercat types to string ----------------------------------------

char* dtype2string(uint16 dtype) {
	switch (dtype) {
	case ECT_BOOLEAN:
		sprintf(hstr, "BOOLEAN");
		break;
	case ECT_INTEGER8:
		sprintf(hstr, "INTEGER8");
		break;
	case ECT_INTEGER16:
		sprintf(hstr, "INTEGER16");
		break;
	case ECT_INTEGER32:
		sprintf(hstr, "INTEGER32");
		break;
	case ECT_INTEGER24:
		sprintf(hstr, "INTEGER24");
		break;
	case ECT_INTEGER64:
		sprintf(hstr, "INTEGER64");
		break;
	case ECT_UNSIGNED8:
		sprintf(hstr, "UNSIGNED8");
		break;
	case ECT_UNSIGNED16:
		sprintf(hstr, "UNSIGNED16");
		break;
	case ECT_UNSIGNED32:
		sprintf(hstr, "UNSIGNED32");
		break;
	case ECT_UNSIGNED24:
		sprintf(hstr, "UNSIGNED24");
		break;
	case ECT_UNSIGNED64:
		sprintf(hstr, "UNSIGNED64");
		break;
	case ECT_REAL32:
		sprintf(hstr, "REAL32");
		break;
	case ECT_REAL64:
		sprintf(hstr, "REAL64");
		break;
	case ECT_BIT1:
		sprintf(hstr, "BIT1");
		break;
	case ECT_BIT2:
		sprintf(hstr, "BIT2");
		break;
	case ECT_BIT3:
		sprintf(hstr, "BIT3");
		break;
	case ECT_BIT4:
		sprintf(hstr, "BIT4");
		break;
	case ECT_BIT5:
		sprintf(hstr, "BIT5");
		break;
	case ECT_BIT6:
		sprintf(hstr, "BIT6");
		break;
	case ECT_BIT7:
		sprintf(hstr, "BIT7");
		break;
	case ECT_BIT8:
		sprintf(hstr, "BIT8");
		break;
	case ECT_VISIBLE_STRING:
		sprintf(hstr, "VISIBLE_STRING");
		break;
	case ECT_OCTET_STRING:
		sprintf(hstr, "OCTET_STRING");
		break;
	default:
		sprintf(hstr, "Type 0x%4.4X", dtype);
	}
	return hstr;
}

char* SDO2string(uint16 slave, uint16 index, uint8 subidx, uint16 dtype) {
	int l = sizeof(usdo) - 1, i;
	uint8 *u8;
	int8 *i8;
	uint16 *u16;
	int16 *i16;
	uint32 *u32;
	int32 *i32;
	uint64 *u64;
	int64 *i64;
	float *sr;
	double *dr;
	char es[32];

	memset(&usdo, 0, 128);
	ec_SDOread(slave, index, subidx, FALSE, &l, &usdo, EC_TIMEOUTRXM);
	if (EcatError) {
		return ec_elist2string();
	} else {
		switch (dtype) {
		case ECT_BOOLEAN:
			u8 = (uint8*) &usdo[0];
			if (*u8)
				sprintf(hstr, "TRUE");
			else
				sprintf(hstr, "FALSE");
			break;
		case ECT_INTEGER8:
			i8 = (int8*) &usdo[0];
			sprintf(hstr, "0x%2.2x %d", *i8, *i8);
			break;
		case ECT_INTEGER16:
			i16 = (int16*) &usdo[0];
			sprintf(hstr, "0x%4.4x %d", *i16, *i16);
			break;
		case ECT_INTEGER32:
		case ECT_INTEGER24:
			i32 = (int32*) &usdo[0];
			sprintf(hstr, "0x%8.8x %d", *i32, *i32);
			break;
		case ECT_INTEGER64:
			i64 = (int64*) &usdo[0];
			sprintf(hstr, "0x%16.16"PRIx64" %"PRId64, *i64, *i64);
			break;
		case ECT_UNSIGNED8:
			u8 = (uint8*) &usdo[0];
			sprintf(hstr, "0x%2.2x %u", *u8, *u8);
			break;
		case ECT_UNSIGNED16:
			u16 = (uint16*) &usdo[0];
			sprintf(hstr, "0x%4.4x %u", *u16, *u16);
			break;
		case ECT_UNSIGNED32:
		case ECT_UNSIGNED24:
			u32 = (uint32*) &usdo[0];
			sprintf(hstr, "0x%8.8x %u", *u32, *u32);
			break;
		case ECT_UNSIGNED64:
			u64 = (uint64*) &usdo[0];
			sprintf(hstr, "0x%16.16"PRIx64" %"PRIu64, *u64, *u64);
			break;
		case ECT_REAL32:
			sr = (float*) &usdo[0];
//			todo no float printf support = what to do??
            sprintf(hstr, "%f", *sr);
			break;
		case ECT_REAL64:
			dr = (double*) &usdo[0];
            sprintf(hstr, "%f", *dr);
			break;
		case ECT_BIT1:
		case ECT_BIT2:
		case ECT_BIT3:
		case ECT_BIT4:
		case ECT_BIT5:
		case ECT_BIT6:
		case ECT_BIT7:
		case ECT_BIT8:
			u8 = (uint8*) &usdo[0];
			sprintf(hstr, "0x%x", *u8);
			break;
		case ECT_VISIBLE_STRING:
			strcpy(hstr, usdo);
			break;
		case ECT_OCTET_STRING:
			hstr[0] = 0x00;
			for (i = 0; i < l; i++) {
				sprintf(es, "0x%2.2x ", usdo[i]);
				strcat(hstr, es);
			}
			break;
		default:
			sprintf(hstr, "Unknown type");
		}
		return hstr;
	}
}

/** Read PDO assign structure */
int si_PDOassign(uint16 slave, uint16 PDOassign, int mapoffset, int bitoffset) {
	uint16 idxloop, nidx, subidxloop, rdat, idx, subidx;
	uint8 subcnt;
	int siwkc, bsize = 0, rdl;
	int32 rdat2;
	uint8 bitlen, obj_subidx;
	uint16 obj_idx;
	int abs_offset, abs_bit, byteoffset;

	rdl = sizeof(rdat);
	rdat = 0;
	/* read PDO assign subindex 0 ( = number of PDO's) */
	siwkc = ec_SDOread(slave, PDOassign, 0x00, FALSE, &rdl, &rdat, EC_TIMEOUTRXM);
	rdat = etohs(rdat);
	/* positive result from slave ? */
	if ((siwkc > 0) && (rdat > 0)) {
		/* number of available sub indexes */
		nidx = rdat;
		bsize = 0;
		/* read all PDO's */
		for (idxloop = 1; idxloop <= nidx; idxloop++) {
			rdl = sizeof(rdat);
			rdat = 0;
			/* read PDO assign */
			siwkc = ec_SDOread(slave, PDOassign, (uint8) idxloop, FALSE, &rdl, &rdat, EC_TIMEOUTRXM);
			/* result is index of PDO */
			idx = etohs(rdat);
			if (idx > 0) {
				rdl = sizeof(subcnt);
				subcnt = 0;
				/* read number of subindexes of PDO */
				siwkc = ec_SDOread(slave, idx, 0x00, FALSE, &rdl, &subcnt, EC_TIMEOUTRXM);
				subidx = subcnt;
				/* for each subindex */
				for (subidxloop = 1; subidxloop <= subidx; subidxloop++) {
					rdl = sizeof(rdat2);
					rdat2 = 0;
					/* read SDO that is mapped in PDO */
					siwkc = ec_SDOread(slave, idx, (uint8) subidxloop, FALSE, &rdl, &rdat2, EC_TIMEOUTRXM);
					rdat2 = etohl(rdat2);
					/* extract bitlength of SDO */
					bitlen = LO_BYTE(rdat2);
					bsize += bitlen;
					obj_idx = (uint16) (rdat2 >> 16);
					obj_subidx = (uint8) ((rdat2 >> 8) & 0x000000ff);
					abs_offset = mapoffset + (bitoffset / 8);
					abs_bit = bitoffset % 8;
					ODlist.Slave = slave;
					ODlist.Index[0] = obj_idx;
					OElist.Entries = 0;
					wkc = 0;
					byteoffset = (bitoffset / 8);
					/* read object entry from dictionary if not a filler (0x0000:0x00) */
					if (obj_idx || obj_subidx)
						siwkc = ec_readOEsingle(0, obj_subidx, &ODlist, &OElist);
					//>> this is where we print addrb
					printf("\t[%-11u] [0x%4.4X.%1d] [0x%4.4X:0x%2.2X] [0x%2.2X]", byteoffset, abs_offset, abs_bit, obj_idx, obj_subidx, bitlen);
					if ((siwkc > 0) && OElist.Entries) {
						printf(" [%-12s] [%-40s]\n", dtype2string(OElist.DataType[obj_subidx]), OElist.Name[obj_subidx]);
					} else
						printf("\n");
					bitoffset += bitlen;
				};
			};
		};
	};
	/* return total found bitlength (PDO) */
	return bsize;
}

//---- PDO mapping according to CoE --------------------------------------------

int si_map_sdo(int slave) {
	int siwkc, rdl;
	int retVal = 0;
	uint8 nSM, iSM, tSM;
	int Tsize, outputs_bo, inputs_bo;
	uint8 SMt_bug_add;

	printf("PDO mapping according to CoE :\n");
	SMt_bug_add = 0;
	outputs_bo = 0;
	inputs_bo = 0;
	rdl = sizeof(nSM);
	nSM = 0;
	/* read SyncManager Communication Type object count */
	siwkc = ec_SDOread(slave, ECT_SDO_SMCOMMTYPE, 0x00, FALSE, &rdl, &nSM, EC_TIMEOUTRXM);
	/* positive result from slave ? */
	if ((siwkc > 0) && (nSM > 2)) {
		/* make nSM equal to number of defined SM */
		nSM--;
		/* limit to maximum number of SM defined, if true the slave can't be configured */
		if (nSM > EC_MAXSM)
			nSM = EC_MAXSM;
		/* iterate for every SM type defined */
		for (iSM = 2; iSM <= nSM; iSM++) {
			rdl = sizeof(tSM);
			tSM = 0;
			/* read SyncManager Communication Type */
			siwkc = ec_SDOread(slave, ECT_SDO_SMCOMMTYPE, iSM + 1, FALSE, &rdl, &tSM, EC_TIMEOUTRXM);
			if (siwkc > 0) {
				if ((iSM == 2) && (tSM == 2)) // SM2 has type 2 == mailbox out, this is a bug in the slave!
						{
					SMt_bug_add = 1; // try to correct, this works if the types are 0 1 2 3 and should be 1 2 3 4
					printf("Activated SM type workaround, possible incorrect mapping.\n");
				}
				if (tSM)
					tSM += SMt_bug_add; // only add if SMt > 0

				if (tSM == 3) // outputs
						{
					/* read the assign RXPDO */
					printf("  SM%1d outputs\n \t[byte_offset] [addr b  ] [index:sub  ] [bitl] [data_type   ] [name                                    ]\n", iSM);
					Tsize = si_PDOassign(slave, ECT_SDO_PDOASSIGN + iSM, (int) (ec_slave[slave].outputs - (uint8*) &IOmap[0]), outputs_bo);
					outputs_bo += Tsize;
				}
				if (tSM == 4) // inputs
						{
					/* read the assign TXPDO */
					printf("  SM%1d inputs\n \t[byte_offset] [addr b  ] [index: sub ] [bitl] [data_type   ] [name                                    ]\n", iSM);
					Tsize = si_PDOassign(slave, ECT_SDO_PDOASSIGN + iSM, (int) (ec_slave[slave].inputs - (uint8*) &IOmap[0]), inputs_bo);
					inputs_bo += Tsize;
				}
			}
		}
	}

	/* found some I/O bits ? */
	if ((outputs_bo > 0) || (inputs_bo > 0)){
        retVal = 1;
	}else{
        printf("  No CoE PDO mapping found\n");
	}

	return retVal;
}

//---- CoE objects description -------------------------------------------------

void si_sdo(int cnt) {
	int i, j;

	ODlist.Entries = 0;
	memset(&ODlist, 0, sizeof(ODlist));
	if (ec_readODlist(cnt, &ODlist)) {
		printf(" CoE Object Description found, %d entries.\n", ODlist.Entries);
		for (i = 0; i < ODlist.Entries; i++) {
			ec_readODdescription(i, &ODlist);
			while (EcatError)
				printf("%s", ec_elist2string());
			printf(" Index: %4.4x Datatype: %4.4x Objectcode: %2.2x Name: %s\n", ODlist.Index[i], ODlist.DataType[i], ODlist.ObjectCode[i], ODlist.Name[i]);
			memset(&OElist, 0, sizeof(OElist));
			ec_readOE(i, &ODlist, &OElist);
			while (EcatError)
				printf("%s", ec_elist2string());
			for (j = 0; j < ODlist.MaxSub[i] + 1; j++) {
				if ((OElist.DataType[j] > 0) && (OElist.BitLength[j] > 0)) {
					printf("  Sub: %2.2x Datatype: %4.4x Bitlength: %4.4x Obj.access: %4.4x Name: %s\n", j, OElist.DataType[j], OElist.BitLength[j], OElist.ObjAccess[j], OElist.Name[j]);
					if ((OElist.ObjAccess[j] & 0x0007)) {
						printf("          Value :%s\n", SDO2string(cnt, ODlist.Index[i], j, OElist.DataType[j]));
					}
				}
			}
		}
	} else {
		while (EcatError)
			printf("%s", ec_elist2string());
	}
}

int si_siiPDO(uint16 slave, uint8 t, int mapoffset, int bitoffset) {
	uint16 a, w, c, e, er, Size;
	uint8 eectl;
	uint16 obj_idx;
	uint8 obj_subidx;
	uint8 obj_name;
	uint8 obj_datatype;
	uint8 bitlen;
	int totalsize;
	ec_eepromPDOt eepPDO;
	ec_eepromPDOt *PDO;
	int abs_offset, abs_bit;
	char str_name[EC_MAXNAME + 1];

	eectl = ec_slave[slave].eep_pdi;
	Size = 0;
	totalsize = 0;
	PDO = &eepPDO;
	PDO->nPDO = 0;
	PDO->Length = 0;
	PDO->Index[1] = 0;
	for (c = 0; c < EC_MAXSM; c++)
		PDO->SMbitsize[c] = 0;
	if (t > 1)
		t = 1;
	PDO->Startpos = ec_siifind(slave, ECT_SII_PDO + t);
	if (PDO->Startpos > 0) {
		a = PDO->Startpos;
		w = ec_siigetbyte(slave, a++);
		w += (ec_siigetbyte(slave, a++) << 8);
		PDO->Length = w;
		c = 1;
		/* traverse through all PDOs */
		do {
			PDO->nPDO++;
			PDO->Index[PDO->nPDO] = ec_siigetbyte(slave, a++);
			PDO->Index[PDO->nPDO] += (ec_siigetbyte(slave, a++) << 8);
			PDO->BitSize[PDO->nPDO] = 0;
			c++;
			/* number of entries in PDO */
			e = ec_siigetbyte(slave, a++);
			PDO->SyncM[PDO->nPDO] = ec_siigetbyte(slave, a++);
			a++;
			obj_name = ec_siigetbyte(slave, a++);
			a += 2;
			c += 2;
			if (PDO->SyncM[PDO->nPDO] < EC_MAXSM) /* active and in range SM? */
			{
				str_name[0] = 0;
				if (obj_name)
					ec_siistring(str_name, slave, obj_name);
				if (t)
					printf("\tSM%1d RXPDO 0x%4.4X %s\n", PDO->SyncM[PDO->nPDO], PDO->Index[PDO->nPDO], str_name);
				else
					printf("\tSM%1d TXPDO 0x%4.4X %s\n", PDO->SyncM[PDO->nPDO], PDO->Index[PDO->nPDO], str_name);
				printf("\t\t[addr b  ] [index:sub  ] [bitl] [data_type ]   [name                                    ]\n");
				/* read all entries defined in PDO */
				for (er = 1; er <= e; er++) {
					c += 4;
					obj_idx = ec_siigetbyte(slave, a++);
					obj_idx += (ec_siigetbyte(slave, a++) << 8);
					obj_subidx = ec_siigetbyte(slave, a++);
					obj_name = ec_siigetbyte(slave, a++);
					obj_datatype = ec_siigetbyte(slave, a++);
					bitlen = ec_siigetbyte(slave, a++);
					abs_offset = mapoffset + (bitoffset / 8);
					abs_bit = bitoffset % 8;

					PDO->BitSize[PDO->nPDO] += bitlen;
					a += 2;

					/* skip entry if filler (0x0000:0x00) */
					if (obj_idx || obj_subidx) {
						str_name[0] = 0;
						if (obj_name)
							ec_siistring(str_name, slave, obj_name);

						printf("\t\t[0x%4.4X.%1d] [0x%4.4X:0x%2.2X] [0x%2.2X] ", abs_offset, abs_bit, obj_idx, obj_subidx, bitlen);
						printf("[%-12s] [%-40s]\n", dtype2string(obj_datatype), str_name);
					}
					bitoffset += bitlen;
					totalsize += bitlen;
				}
				PDO->SMbitsize[PDO->SyncM[PDO->nPDO]] += PDO->BitSize[PDO->nPDO];
				Size += PDO->BitSize[PDO->nPDO];
				c++;
			} else /* PDO deactivated because SM is 0xff or > EC_MAXSM */
			{
				c += 4 * e;
				a += 8 * e;
				c++;
			}
			if (PDO->nPDO >= (EC_MAXEEPDO - 1))
				c = PDO->Length; /* limit number of PDO entries in buffer */
		} while (c < PDO->Length);
	}
	if (eectl)
		ec_eeprom2pdi(slave); /* if eeprom control was previously pdi then restore */
	return totalsize;
}

int si_map_sii(int slave) {
	int retVal = 0;
	int Tsize, outputs_bo, inputs_bo;

	printf("PDO mapping according to SII :\n");

	outputs_bo = 0;
	inputs_bo = 0;
	/* read the assign RXPDOs */

	//DG this is where we calc mapoffset (int) (ec_slave[slave].outputs - (uint8*) &IOmap
	Tsize = si_siiPDO(slave, 1, (int) (ec_slave[slave].outputs - (uint8*) &IOmap), outputs_bo);
	outputs_bo += Tsize;
	/* read the assign TXPDOs */
	Tsize = si_siiPDO(slave, 0, (int) (ec_slave[slave].inputs - (uint8*) &IOmap), inputs_bo);
	inputs_bo += Tsize;
	/* found some I/O bits ? */
	if ((outputs_bo > 0) || (inputs_bo > 0)){
		retVal = 1;
	}else{
        printf("  No SII PDO mapping found\n");
	}

	return retVal;
}

void ecm_netscan(char *ifname1, bool apply_pdo_mapping) {
	int cnt, i, j, nSM;
	uint16 ssigen;
	int nsexpectedWKC;

	ecm_status.net_scan_state = ECM_NET_SCAN_START;


    UM_INFO(GBEM_UM_EN, "GBEM: **************************************************************************");
    UM_INFO(GBEM_UM_EN, "GBEM: ***                          Network scanning                          ***");
    UM_INFO(GBEM_UM_EN, "GBEM: **************************************************************************");

    if (apply_pdo_mapping) {
        UM_INFO(GBEM_UM_EN, "GBEM: We are going to apply the configured PDO mapping to all slaves");
    }else{
        UM_INFO(GBEM_UM_EN, "GBEM: We are NOT going to apply any PDO mapping to the slaves. Slaves will be scanned in their un-configured state");
    }

    //   /* initialise SOEM, bind socket to ifname */
	if (ec_init(ifname1)) {
		UM_INFO(GBEM_UM_EN, "GBEM: ec_init succeeded");
		/* find and auto-config slaves */
		bool pdo_map_failure = false;

		if ( ec_config(FALSE, &IOmap, sizeof(IOmap)) > 0 ){

		    while (EcatError) {
		        ecm_status.net_scan_state = ECM_NET_SCAN_ERROR;
		        UM_ERROR(GBEM_UM_EN, "GBEM: netscan error [%s]", ec_elist2string());
		        osal_usleep(1000000);
		    }


		    UM_INFO(GBEM_UM_EN, "GBEM: netscan found [%d] slaves and configured them",ec_slavecount);
		    expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
		    UM_INFO(GBEM_UM_EN, "GBEM: netscan calculated a workcounter of [%d]", expectedWKC);

            if (apply_pdo_mapping) {

                for (int slave = 1; slave < MAP_NUM_SLAVES + 1; slave++) {
                    if (*map_slave_pdo_map_function_ptr[slave - 1] != NULL) {
                        if ((*map_slave_pdo_map_function_ptr[slave - 1])(slave) == E_SUCCESS) {
                            UM_INFO(GBEM_UM_EN, "GBEM: PDO mapping by netscan succeeded for slave [%u]", slave);
                        } else {
                            UM_ERROR(GBEM_UM_EN, "GBEM: PDO mapping by netscan  failed for slave [%u]", slave);
                            pdo_map_failure = true;
                        }
                    }
                }

//                ec_slave[0].state = EC_STATE_PRE_OP;
//                ec_statecheck(0, EC_STATE_PRE_OP, EC_TIMEOUTSTATE * 3);
//                if (ec_slave[0].state != EC_STATE_PRE_OP) {
//                    UM_ERROR(GBEM_UM_EN, "GBEM: netscan failed to transition all slaves PRE-OP state");
//                    ec_readstate();
//                    for (i = 1; i <= ec_slavecount; i++) {
//                        if (ec_slave[i].state != EC_STATE_PRE_OP) {
//                            UM_INFO(GBEM_UM_EN, "GBEM: netscan found a slave not in pre-op - Slave [%d] State [%s] StatusCode [%4x : %s]", i, ec_state_to_string[ec_slave[i].state], ec_slave[i].ALstatuscode, ec_ALstatuscode2string(ec_slave[i].ALstatuscode));
//                        }
//                    }
//                }




                if (pdo_map_failure) {
                    UM_ERROR(GBEM_UM_EN, "GBEM: NOT all slaves successfully had their PDO map applied by netscan");
                    exit(0);
                } else {
                    UM_INFO(GBEM_UM_EN, "GBEM: All slaves successfully had their PDO map applied by netscan");
                }
            }



			//EcatError is global error saying error is available in error stack
			while (EcatError) {
				ecm_status.net_scan_state = ECM_NET_SCAN_ERROR;
				UM_ERROR(GBEM_UM_EN, "GBEM: netscan found an EtherCAT error [%s]", ec_elist2string());
                osal_usleep(1000000);
			}

//todo crit - not sure we want this transition at all here
//			ec_slave[0].state = (EC_STATE_SAFE_OP);
//			ec_writestate(0);
//
//			/* wait for all slaves to reach SAFE_OP state */
//			ec_statecheck(0, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE * 3);
//			if (ec_slave[0].state != EC_STATE_SAFE_OP) {
//				UM_ERROR(GBEM_UM_EN, "GBEM: netscan failed to transition all slaves SAFE-OP state");
//				ec_readstate();
//				for (i = 1; i <= ec_slavecount; i++) {
//					if (ec_slave[i].state != EC_STATE_SAFE_OP) {
//					    UM_INFO(GBEM_UM_EN, "GBEM: netscan found a slave not in safe-op - Slave [%d] State [%s] StatusCode [%4x : %s]", i, ec_state_to_string[ec_slave[i].state], ec_slave[i].ALstatuscode, ec_ALstatuscode2string(ec_slave[i].ALstatuscode));
//					    UM_ERROR(GBEM_UM_EN, "GBEM: netscan error [%s]", ec_elist2string());
//					}
//				}
//			}

            ec_readstate();





			printf("*** Starting network scanning output ***");
			for (cnt = 1; cnt <= ec_slavecount; cnt++) {
				printf("\nSlave: %d\n Name: %s\n Output size: %dbits\n Input size: %dbits\n State: %d\n Delay: %d[ns]\n Has DC: %d\n", cnt, ec_slave[cnt].name, ec_slave[cnt].Obits, ec_slave[cnt].Ibits,
						ec_slave[cnt].state, ec_slave[cnt].pdelay, ec_slave[cnt].hasdc);
				if (ec_slave[cnt].hasdc)
					printf(" DCParentport: %d\n", ec_slave[cnt].parentport);
				printf(" Activeports: %d.%d.%d.%d\n", (ec_slave[cnt].activeports & 0x01) > 0, (ec_slave[cnt].activeports & 0x02) > 0, (ec_slave[cnt].activeports & 0x04) > 0,
						(ec_slave[cnt].activeports & 0x08) > 0);
				printf(" Configured address: %4.4x\n", ec_slave[cnt].configadr);
				printf(" Man: %8.8x ID: %8.8x Rev: %8.8x\n", (int) ec_slave[cnt].eep_man, (int) ec_slave[cnt].eep_id, (int) ec_slave[cnt].eep_rev);
				for (nSM = 0; nSM < EC_MAXSM; nSM++) {
					if (ec_slave[cnt].SM[nSM].StartAddr > 0)
						printf(" SM%1d A:%4.4x L:%4d F:%8.8x Type:%d\n", nSM, etohs(ec_slave[cnt].SM[nSM].StartAddr), etohs(ec_slave[cnt].SM[nSM].SMlength), etohl(ec_slave[cnt].SM[nSM].SMflags),
								ec_slave[cnt].SMtype[nSM]);
				}
				for (j = 0; j < ec_slave[cnt].FMMUunused; j++) {
					printf(" FMMU%1d Ls:%8.8x Ll:%4d Lsb:%d Leb:%d Ps:%4.4x Psb:%d Ty:%2.2x Act:%2.2x\n", j, etohl(ec_slave[cnt].FMMU[j].LogStart), etohs(ec_slave[cnt].FMMU[j].LogLength),
							ec_slave[cnt].FMMU[j].LogStartbit, ec_slave[cnt].FMMU[j].LogEndbit, etohs(ec_slave[cnt].FMMU[j].PhysStart), ec_slave[cnt].FMMU[j].PhysStartBit,
							ec_slave[cnt].FMMU[j].FMMUtype, ec_slave[cnt].FMMU[j].FMMUactive);
				}
				printf(" FMMUfunc 0:%d 1:%d 2:%d 3:%d\n", ec_slave[cnt].FMMU0func, ec_slave[cnt].FMMU1func, ec_slave[cnt].FMMU2func, ec_slave[cnt].FMMU3func);
				printf(" MBX length wr: %d rd: %d MBX protocols : %2.2x\n", ec_slave[cnt].mbx_l, ec_slave[cnt].mbx_rl, ec_slave[cnt].mbx_proto);
				ssigen = ec_siifind(cnt, ECT_SII_GENERAL);
				/* SII general section */
				if (ssigen) {
					ec_slave[cnt].CoEdetails = ec_siigetbyte(cnt, ssigen + 0x07);
					ec_slave[cnt].FoEdetails = ec_siigetbyte(cnt, ssigen + 0x08);
					ec_slave[cnt].EoEdetails = ec_siigetbyte(cnt, ssigen + 0x09);
					ec_slave[cnt].SoEdetails = ec_siigetbyte(cnt, ssigen + 0x0a);
					if ((ec_siigetbyte(cnt, ssigen + 0x0d) & 0x02) > 0) {
						ec_slave[cnt].blockLRW = 1;
						ec_slave[0].blockLRW++;
					}
					ec_slave[cnt].Ebuscurrent = ec_siigetbyte(cnt, ssigen + 0x0e);
					ec_slave[cnt].Ebuscurrent += ec_siigetbyte(cnt, ssigen + 0x0f) << 8;
					ec_slave[0].Ebuscurrent += ec_slave[cnt].Ebuscurrent;
				}
				printf(" CoE details: %2.2x FoE details: %2.2x EoE details: %2.2x SoE details: %2.2x\n", ec_slave[cnt].CoEdetails, ec_slave[cnt].FoEdetails, ec_slave[cnt].EoEdetails,
						ec_slave[cnt].SoEdetails);
				printf(" Ebus current: %d[mA]\n only LRD/LWR:%d\n", ec_slave[cnt].Ebuscurrent, ec_slave[cnt].blockLRW);
				if ((ec_slave[cnt].mbx_proto & ECT_MBXPROT_COE) && printSDO)
					si_sdo(cnt);
				if (printMAP) {
					if (ec_slave[cnt].mbx_proto & ECT_MBXPROT_COE)
						si_map_sdo(cnt);
					else
						si_map_sii(cnt);
				}
			}
			printf("*** End of network scanning output ***\n");

            printf("*** Start of eeprom output ***\n");
            for (i = 1; i <= ec_slavecount; i++) {
                eeprom_print(i);
            }
            printf("*** End of eeprom output ***\n");

			ecm_status.net_scan_state = ECM_NET_SCAN_FINISHED;

		} else {
			ecm_status.net_scan_state = ECM_NET_SCAN_NO_SLAVES_FOUND;
			UM_ERROR(GBEM_UM_EN, "GBEM: netscan found no slaves found!");
		}
		//todo we dont have a socket
		UM_INFO(GBEM_UM_EN, "GBEM: netscan has finished, closing socket");
		/* stop SOEM, close socket */
		ec_close();
	} else {
		UM_ERROR(GBEM_UM_EN, "GBEM: netscan network initialisation failed");
		ecm_status.net_scan_state = ECM_NET_SCAN_ERROR;
	}
}