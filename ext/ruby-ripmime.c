#include "ruby-ripmime.h"

static VALUE rb_mRipmime;
static VALUE rb_eRipmimeError;

static VALUE ruby_ripmime_decode(VALUE self, VALUE mailpack, VALUE outputdir) {
  struct RIPMIME_object rm;
  int ret, fd_stdout, fd_stderr;
  FILE *fp_err;
  VALUE errmsg;
  char buf[BUFSIZ];

  Check_Type(mailpack, T_STRING);
  Check_Type(outputdir, T_STRING);

  if (RSTRING_LEN(mailpack) < 1) {
    rb_raise(rb_eArgError, "mailpack is empty");
  }

  if (RSTRING_LEN(outputdir) < 1) {
    rb_raise(rb_eArgError, "outputdir is empty");
  }

  fp_err = tmpfile();

  if (fp_err == NULL) {
    rb_raise(rb_eRuntimeError, strerror(errno));
  }

  RIPMIME_init(&rm);

  MIME_set_name_by_type(1);
  LOGGER_set_output_mode(_LOGGER_FILE);
  LOGGER_set_output_file(fp_err);

  // XXX: for strdup memory leak (ripMIME 1.4.0.10)
  rm.outputdir = NULL;
  rm.mailpack = NULL;

  ret = RIPMIME_decode(&rm, StringValuePtr(mailpack), StringValuePtr(outputdir));

  // XXX: for strdup memory leak (ripMIME 1.4.0.10)
  if (rm.outputdir) {
    free(rm.outputdir);
  }

  // XXX: for strdup memory leak (ripMIME 1.4.0.10)
  if (rm.mailpack) {
    free(rm.mailpack);
  }

  rewind(fp_err);

  errmsg = rb_str_new(0, 0);

  while (fgets(buf, BUFSIZ, fp_err) != NULL) {
    rb_str_append(errmsg, rb_str_new2(buf));
  }

  if (fclose(fp_err) != 0) {
    rb_warn(strerror(errno));
  }

  if (ret != 0 || RSTRING_LEN(errmsg) != 0) {
    if (RSTRING_LEN(errmsg) > 0) {
      rb_raise(rb_eRipmimeError, RSTRING_PTR(errmsg));
    } else {
      rb_raise(rb_eRipmimeError, "ripMIME decode errer (retval: %d)", ret);
    }
  }
  
  return Qnil;
}

void Init_ripmime() {
  rb_mRipmime = rb_define_module("Ripmime");
  rb_eRipmimeError = rb_define_class_under(rb_mRipmime, "Error", rb_eStandardError);

  rb_define_module_function(rb_mRipmime, "decode", ruby_ripmime_decode, 2);
}
