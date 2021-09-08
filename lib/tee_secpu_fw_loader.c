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
#include <tee_client_api.h>
#include <ta_secpu_fw_loader.h>
#include <tee_secpu_fw_loader.h>

static char *buf = NULL;
static TEEC_Context ctx;
static TEEC_Session sess;
static int tee_inited = 0;


static int get_firmware_from_sys(char **firmware, char *fw_file, uint32_t *size)
{
	FILE *fp = NULL;
	long len = 0;

	fp = fopen(fw_file, "r");
    if (NULL == fp) {
        printf("Open video firmware fail!\n");
        return -1;
    }

	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if (buf) {
		free(buf);
		buf = NULL;
	}
	buf = (char *)malloc(len);
	if (NULL == buf) {
        printf("no enough memory!\n");
		fclose(fp);
        return -1;
	}

	if (fread(buf, len, 1, fp) > 0)
		*size = len;

	fclose(fp);
	*firmware = buf;

	return 0;
}

/*
 * Called when a new session is opened to the TA. *sess_ctx can be updated
 * with a value to be able to identify this session in subsequent calls to the
 * TA.
 */
static int tee_session_load_init()
{
    TEEC_Result res;
	TEEC_Operation op;
	TEEC_UUID uuid = TA_SECPU_FW_LOADER_UUID;
	uint32_t err_origin;

	/* Initialize a context connecting us to the TEE */
	memset(&op, 0, sizeof(TEEC_Operation));
	res = TEEC_InitializeContext(NULL, &ctx);
	if (res != TEEC_SUCCESS) {
		printf("TEEC_InitializeContext failed with code 0x%x\n", res);
		return -1;
	}

	/*Open a session to TA */
	res = TEEC_OpenSession(&ctx, &sess, &uuid,
			       TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
	if (res != TEEC_SUCCESS) {
		printf("TEEC_Opensession failed with code 0x%x origin 0x%x\n",
			res, err_origin);
		return -1;
	}

	tee_inited = 1;
	return 0;
}

/*
 * Called when a session is closed, sess_ctx hold the value that was
 * assigned by TA_OpenSessionEntryPoint().
 */
static void tee_session_load_close()
{
	/*
	 * We're done with the TA, close the session and
	 * destroy the context.
    */
    if (tee_inited == 1)
    {
	    TEEC_CloseSession(&sess);
	    TEEC_FinalizeContext(&ctx);
	} else {
        printf("not init yet!!!\n");
	}
	if (buf) {
		free(buf);
		buf = NULL;
	}
}

static int tee_fw_send_to_ta(char * fw_file, unsigned int debug)
{
	char *firmware = NULL;
	uint32_t len = 0;
	TEEC_Operation op;
	TEEC_Result res;
	uint32_t err_origin;

	if (tee_inited != 1)
	{
		printf("tee session not inityet\n");
		return -1;
	}

	if (get_firmware_from_sys(&firmware, fw_file, &len))
	{
		printf("get_firmware_from_sys fail\n");
		return -1;
	}
	/*
	 * Execute a function in the TA by invoking it, in this case
	 * we're incrementing a number.
	 *
	 * The value of command ID part and how the parameters are
	 * interpreted is part of the interface provided by the TA.
	 */

	/*
	 * Prepare the argument. Pass a value in the first parameter,
	 * the remaining three parameters are unused.
	 */
	memset(&op, 0, sizeof(TEEC_Operation));
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
					 TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE);
	op.params[0].tmpref.buffer = firmware;
	op.params[0].tmpref.size = len;
	op.params[1].value.a = debug;

	res = TEEC_InvokeCommand(&sess, TA_CMD_LOAD_SECPU_FW, &op,
				 &err_origin);
	if (res != TEEC_SUCCESS) {
		printf("TEEC_InvokeCommand failed with code 0x%x origin 0x%x\n",
			res, err_origin);
		return -1;
	}
    return 0;
}


//for external call
int load_secpu_firmware(char * fw_file, unsigned int debug)
{
    int res;
    res = tee_session_load_init();
    if (res != 0)
        return res;

    res = tee_fw_send_to_ta(fw_file, debug);
    tee_session_load_close();
	return res;
}
