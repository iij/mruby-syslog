MRuby::Gem::Specification.new('mruby-syslog') do |spec|
  spec.license = 'MIT'
  spec.authors = 'Internet Initiative Japan Inc.'

  spec.cc.include_paths << "#{build.root}/src"
end
