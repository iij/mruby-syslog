MRuby::Gem::Specification.new('mruby-syslog') do |spec|
  spec.license = 'MIT'
  spec.author  = 'Internet Initiative Japan Inc.'

  spec.add_dependency 'mruby-sprintf'

  spec.cc.include_paths << "#{build.root}/src"

  # tests depend on...
  #spec.add_dependency 'mruby-io'
end
