require 'formula'

class Libripmime < Formula
  url 'http://www.pldaniels.com/ripmime/ripmime-1.4.0.9.tar.gz'
  homepage 'http://www.pldaniels.com/ripmime/'
  sha1 '883fbed480807c2425965f1c1a96d4c207ae8634'

  def install
    system "/usr/bin/sed -i -e 's|-soname|-install_name|' Makefile"
    system 'make solib'
    ln_s 'libripmime.so.1.4.0', 'libripmime.so'

    lib.install 'libripmime.so.1.4.0'
    lib.install 'libripmime.so'
    include.install 'ripmime-api.h'
  end
end
