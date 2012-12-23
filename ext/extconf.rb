require 'mkmf'

if have_header('ripmime-api.h') and have_library('ripmime')
  create_makefile('ripmime')
end
