/*

Copyright (c) 2015, Christian Schwarz 
All rights reserved. 

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met: 

 * Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer. 
 * Redistributions in binary form must reproduce the above copyright 
   notice, this list of conditions and the following disclaimer in the 
   documentation and/or other materials provided with the distribution. 
 * Neither the name of  nor the names of its contributors may be used to 
   endorse or promote products derived from this software without specific 
   prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE. 

 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <err.h>

static char* sysfs_path = "/sys/class/backlight/gmux_backlight/brightness";

long int get_brightness(FILE *sysfs_fp) {

	rewind(sysfs_fp);
		
	size_t string_length = 5;
	char *brightness_string = (char*)malloc(string_length);
	if (brightness_string == NULL) {
		return -1;
	}

	if (fgets(brightness_string, string_length, sysfs_fp) == NULL) {
		return -1;
	}

	long int brightness = strtol(brightness_string, NULL, 10);

	return brightness;
}

void set_brightness(FILE *sysfs_fp, long int brightness_value) {

	rewind(sysfs_fp);

	if (fprintf(sysfs_fp, "%ld", brightness_value) < 0) {
		err(1, "Error writing brightness to sysfs");
	}

}

typedef enum {
	bl_action_set,
	bl_action_increment,
	bl_action_decrement
} bl_action;

bl_action parse_action(char *argstring, long int *num_p) {

	bl_action action = bl_action_set;

	switch (argstring[0]) {
		case '+':
			action = bl_action_increment;
			argstring++;
			break;
		case '-':
			action = bl_action_decrement;
			argstring++;
			break;
		default:
			break;
	}

	long int number = strtol(argstring, NULL, 10);
	if (errno == EINVAL) {
		err(1, "Error parsing brightness number argument. Allowed: (+-)?[0-9]+");
	}

	*num_p= number;

	return action;

}

int main(int argc, char* args[]) {

	FILE *f = fopen(sysfs_path, "r+");
	if (f == NULL) {
		err(1, "Error opening the sysfs control path.");
	}

	if (argc != 2) {
		fprintf(stderr, "USAGE: %s ( +num | -num | num )\n", args[0]);
		return 1;
	}

	long int amount = 0;
	bl_action action = parse_action(args[1], &amount);

	switch (action) {
		case bl_action_set:
			set_brightness(f, amount);
			break;
		case bl_action_increment:
			set_brightness(f, get_brightness(f) + amount);
			break;
		case bl_action_decrement:
			set_brightness(f, get_brightness(f) - amount);
			break;
		default:
			err(1, "Unsupported case");

	}

	fclose(f);
}
