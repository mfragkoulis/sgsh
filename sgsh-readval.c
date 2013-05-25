/*
 * Copyright 2013 Diomidis Spinellis
 *
 * Communicate with the data store specified as a Unix-domain socket.
 * (User interface)
 * By default the command will read a value.
 * Calling it with the -q flag will send the data store a termination
 * command.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

#include "sgsh.h"
#include "keystore.h"

static const char *program_name;

static void
usage(void)
{
	fprintf(stderr, "Usage: %s [-c|l] [-n] [-q] -s path\n"
		"-c"		"\tRead the current value from the store\n"
		"-l"		"\tRead the last (before EOF) value from the store (default)\n"
		"-n"		"\tDo not retry failed connection to write store\n"
		"-q"		"\tAsk the write-end to quit\n"
		"-s path"	"\tSpecify the socket to connect to\n",
		program_name);
	exit(1);
}

int
main(int argc, char *argv[])
{
	int ch;
	bool quit = false;
	char cmd = 0;
	const char *socket_path = NULL;
	bool retry_connection = true;

	program_name = argv[0];

	/* Default if nothing else is specified */
	if (argc == 3)
		cmd = 'L';

	while ((ch = getopt(argc, argv, "clnqs:")) != -1) {
		switch (ch) {
		case 'c':	/* Read current value */
			cmd = 'C';
			break;
		case 'l':	/* Read last value */
			cmd = 'L';
			break;
		case 'n':
			retry_connection = false;
			break;
		case 'q':
			quit = true;
			break;
		case 's':
			socket_path = optarg;
			break;
		case '?':
		default:
			usage();
		}
	}
	argc -= optind;
	argv += optind;

	if (argc != 0 || socket_path == NULL)
		usage();

	sgsh_send_command(socket_path, cmd, retry_connection, quit);

	return 0;
}
