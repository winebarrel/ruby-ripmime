Gem::Specification.new do |spec|
  spec.name              = 'ruby-ripmime'
  spec.version           = '0.1.4'
  spec.summary           = 'Ruby binding for ripMIME.'
  spec.description       = 'Ruby binding for ripMIME.'
  spec.files             = Dir.glob('ext/{*.c,*.h,extconf.rb}') + %w(README)
  spec.author            = 'winebarrel'
  spec.email             = 'sgwr_dts@yahoo.co.jp'
  spec.homepage          = 'https://bitbucket.org/winebarrel/ruby-ripmime'
  spec.extensions        = 'ext/extconf.rb'
  spec.has_rdoc          = true
  spec.rdoc_options      << '--title' << 'ruby-ripmime - Ruby binding for ripMIME.'
  spec.extra_rdoc_files  = %w(README)
end
