/*
 *   This file is part of DroneBridge: https://github.com/DroneBridge/ESP32
 *
 *   Copyright 2018 Wolfgang Christl
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *
 */

#ifndef DB_ESP32_DB_COMM_H
#define DB_ESP32_DB_COMM_H

#define MAX_ERR_MSG_LENGTH 2048

int crc_ok(uint8_t *buf, int msg_length);

int gen_db_comm_sys_ident_json(uint8_t *message_buffer, int new_id, int new_fw_id);

int gen_db_comm_err_resp(uint8_t *message_buffer, int id, char error_message[MAX_ERR_MSG_LENGTH]);

int gen_db_comm_ping_resp(uint8_t *message_buffer, int id);

#endif //DB_ESP32_DB_COMM_H
