/*
 * Copyright (C) 2021 Amlogic, Inc. All rights reserved.
 *
 * All information contained herein is Amlogic confidential.
 *
 * This software is provided to you pursuant to Software License
 * Agreement (SLA) with Amlogic Inc ("Amlogic"). This software may be
 * used only in accordance with the terms of this agreement.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification is strictly prohibited without prior written permission
 * from Amlogic.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tee_secpu_fw_loader.h"

int main(int argc, char **argv)
{
	int res = -1;
	unsigned int status=0, debug = 0;
	char* fw_path = "bl40.bin";

	if (argc > 1) {
		if (argv[1]) {
			if (!strcmp(argv[1], "-d"))
				debug = 1;
			else
				fw_path = argv[1];
		}

		if (argc > 2 && argv[2])
			fw_path = argv[2];
	}

	res = get_secpu_fw_load_status(&status);
	printf("get_secpu_fw_load_status status=%#x res=%#x\n", status, res);

	printf("fw_path=%s\n",fw_path);
	res = load_secpu_firmware(fw_path, debug);
	if (res == 0)
		printf("tee load secpu fw ok\n");
	else
		printf("tee load secpu fw fail\n");

	status = 0;
	res = get_secpu_fw_load_status(&status);
	printf("get_secpu_fw_load_status status=%#x res=%#x\n", status, res);

	return res;
}
