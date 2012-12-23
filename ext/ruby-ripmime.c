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

  fp_err = tmpfile();

  if (fp_err == NULL) {
    rb_raise(rb_eRuntimeError, strerror(errno));
  }

  fd_stdout = dup(fileno(stdout));

  if (fd_stdout == -1) {
    rb_raise(rb_eRuntimeError, strerror(errno));
  }

  fd_stderr = dup(fileno(stderr));

  if (dup2(fileno(fp_err), fileno(stdout)) == -1) {
    rb_raise(rb_eRuntimeError, strerror(errno));
  }

  if (dup2(fileno(fp_err), fileno(stderr)) == -1) {
    rb_raise(rb_eRuntimeError, strerror(errno));
  }

  RIPMIME_init(&rm);

  ret = RIPMIME_decode(&rm, StringValuePtr(mailpack), StringValuePtr(outputdir));

  if (dup2(fd_stdout, fileno(stdout)) == -1) {
    rb_raise(rb_eRuntimeError, strerror(errno));
  }

  if (dup2(fd_stdout, fileno(stderr)) == -1) {
    rb_raise(rb_eRuntimeError, strerror(errno));
  }

  if (close(fd_stdout) == -1) {
    rb_warn(strerror(errno));
  }

  if (close(fd_stderr) == -1) {
    rb_warn(strerror(errno));
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
  
  return 0;
}

void Init_ripmime() {
  rb_mRipmime = rb_define_module("Ripmime");
  rb_eRipmimeError = rb_define_class_under(rb_mRipmime, "Error", rb_eStandardError);

  rb_define_module_function(rb_mRipmime, "decode", ruby_ripmime_decode, 2);
}
