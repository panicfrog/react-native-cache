#
#  Be sure to run `pod spec lint SQLiteCpp.podspec' to ensure this is a
#  valid spec and to remove all comments including this before submitting the spec.
#
#  To learn more about Podspec attributes see https://guides.cocoapods.org/syntax/podspec.html
#  To see working Podspecs in the CocoaPods repo see https://github.com/CocoaPods/Specs/
#

Pod::Spec.new do |spec|

  spec.name         = "SQLiteCpp"
  spec.version      = "3.1.1"
  spec.summary      = "A short description of SQLiteCpp. sqlite with cpp"

  spec.description  = <<-DESC
  sqlite cpp
                   DESC

  spec.homepage     = "http://github.com/panicfrog/SQLiteCpp"
  spec.license      = "MIT"

  spec.author             = { "yeyongping" => "burnedfrog@gmail.com" }
  spec.platform     = :ios, "10.0"

  spec.source       = { :git => "git@github.com/panicfrog/SQLiteCpp.git", :tag => "#{spec.version}" }
  spec.ios.vendored_libraries = 'libSQLiteCpp.a', 'libsqlite3.a'
  
  spec.source_files  = "include/SQLiteCpp/*.h"
  # spec.exclude_files = "Classes/Exclude"

  # spec.public_header_files = "Classes/**/*.h"
  spec.xcconfig = { "HEADER_SEARCH_PATHS" => "$(SDKROOT)/include/SQLiteCpp" }

end
