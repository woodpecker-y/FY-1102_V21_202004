/**
 *  UNPUBLISHED PROPRIETARY SOURCE CODE
 *  Copyright (c) 2016 MXCHIP Inc.
 *
 *  The contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of MXCHIP Corporation.
 *
 */


#ifndef __RTX_CLI_H__
#define __RTX_CLI_H__


#define CLI_R_OK 0
#define CLI_R_ERR 1

/** Structure for registering CLI commands */
struct cli_command 
{
	/** The name of the CLI command */
	const char *name;
	/** The help text associated with the command */
	const char *help;
	/** The function that should be invoked for this command. */
	void (*function) (char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
};


#define cmd_printf(...) do	{\
                            if (xWriteBufferLen > 0) \
                            {\
                                snprintf(pcWriteBuffer, xWriteBufferLen, __VA_ARGS__);\
                                xWriteBufferLen-=strlen(pcWriteBuffer);\
                                pcWriteBuffer+=strlen(pcWriteBuffer);\
                            }\
						}while(0)


#define CLI_ARGS char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv

/** Register a CLI command
 *
 * This function registers a command with the command-line interface.
 * 
 * \param[in] command The structure to register one CLI command
 * \return 0 on success
 * \return 1 on failure
 */
int cli_register_command(const struct cli_command *command);

/** Unregister a CLI command
 *
 * This function unregisters a command from the command-line interface.
 * 
 * \param[in] command The structure to unregister one CLI command
 * \return 0 on success
 * \return 1 on failure
 */
int cli_unregister_command(const struct cli_command *command);



/** Stop the CLI thread and carry out the cleanup
 *
 * \return kNoErr on success
 * \return error code otherwise.
 *
 */
int cli_stop(void);

/** Register a batch of CLI commands
 *
 * Often, a module will want to register several commands.
 * 
 * \param[in] commands Pointer to an array of commands.
 * \param[in] num_commands Number of commands in the array.
 * \return 0 on success
 * \return 1 on failure
 */
int cli_register_commands(const struct cli_command *commands, int num_commands);

/** Unregister a batch of CLI commands
 *
 * \param[in] commands Pointer to an array of commands.
 * \param[in] num_commands Number of commands in the array.
 * \return 0 on success
 * \return 1 on failure
 */
int cli_unregister_commands(const struct cli_command *commands,int num_commands);

/* Get a CLI msg
 *
 * If an external input task wants to use the CLI, it can use
 * cli_get_cmd_buffer() to get a command buffer that it can then
 * submit to the CLI later using cli_submit_cmd_buffer().
 *
 * \param buff Pointer to a char * to place the buffer pointer in.
 * \return 0 on success
 * \return error code otherwise.
 */
int cli_getchar(char *inbuf);

/* Send CLI output msg
 *
 * \param buff Pointer to a char * buffer.
 * \return 0 on success
 * \return error code otherwise.
 */
#define cli_printf(M, ...) os_printf(M, ##__VA_ARGS__)

int cli_init(void);
void cli_main(void);

#endif

