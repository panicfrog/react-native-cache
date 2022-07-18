Pod::Spec.new do |spec|

  spec.name         = "HCache"
  spec.version      = "0.0.1"
  spec.summary      = "A short description of HCache cpp."

  spec.description  = <<-DESC
  HCache cpp
                   DESC

  spec.homepage     = "http://github.com/panicfrog/HCache"

  spec.license      = "MIT"

  spec.author             = { "yeyongping" => "burnedfrog@gmail.com" }
  spec.platform     = :ios, "10.0"

  spec.source       = { :git => "http://github.com/panicfrog/HCach.git", :tag => "#{spec.version}" }

  spec.ios.vendored_frameworks = 'build/HCache.xcframework'

end
