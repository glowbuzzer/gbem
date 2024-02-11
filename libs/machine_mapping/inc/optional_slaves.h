/**
 ******************************************************************************
 * @file           :  optional_slaves
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef OPTIONAL_SLAVES_H
#define OPTIONAL_SLAVES_H

void test(void);

gberror_t os_disable_slaves(bool *which_slaves_are_optional);


#endif //OPTIONAL_SLAVES_H
