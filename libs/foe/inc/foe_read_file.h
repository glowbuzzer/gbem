/**
 ******************************************************************************
 * @file           :  foe_read_file
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef GBEM_FOE_READ_FILE_H
#define GBEM_FOE_READ_FILE_H

#include "std_headers.h"

gberror_t foe_read_file(char *filename, uint32_t password, uint16_t slave, char *buffer, size_t buffer_size);


#endif //GBEM_FOE_READ_FILE_H
