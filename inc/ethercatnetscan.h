/**
 ******************************************************************************
 * @file           :  ethercatnetscan.h
 * @brief          :  scans EtherCAT network for slaves and reports info on them
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_ETHERCATNETSCAN_H
#define GBEM_ETHERCATNETSCANH

void ECNetscan(bool apply_pdo_mapping_before_netscan);

#endif