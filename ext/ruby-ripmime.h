#ifndef __RUBY_RIPMIME_H__
#define __RUBY_RIPMIME_H__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ripmime-api.h>
#include <ruby.h>

#ifndef RSTRING_PTR
#define RSTRING_PTR(s) (RSTRING(s)->ptr)
#endif
#ifndef RSTRING_LEN
#define RSTRING_LEN(s) (RSTRING(s)->len)
#endif

int MIME_set_name_by_type(int level);

#define _LOGGER_FILE 3
int LOGGER_set_output_mode(int modechoice);
int LOGGER_set_output_file(FILE *f);

#endif //__RUBY_RIPMIME_H__
