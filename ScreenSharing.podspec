Pod::Spec.new do |spec|

  spec.name         = "ScreenSharing"
  spec.version      = "1.1.0"
  spec.summary      = "ScreenSharing enabling collaborative work, presentations, and remote assistance seamlessly."
  spec.description  = "ScreenSharing allows real-time sharing of your computer screen with others, facilitating remote collaboration, presentations, and technical support"
  spec.homepage     = "https://github.com/dotsquares395/ScreenSharing"
  spec.license      =  "MIT"
  spec.authors      = { "dotsquares395" => "rohan.sharma@dotsquares.com" }
  spec.platform     = :ios, "13.0"
  spec.source       = {:git => "https://github.com/dotsquares395/ScreenSharing.git", :tag => spec.version}
  spec.source_files =  'ScreenSharing/*.{swift}'
  spec.swift_versions = '5.0'
  spec.xcconfig     = { "ENABLE_BITCODE" => "NO" }
  spec.screenshots = 'https://ibb.co/hd6rHtJ','https://ibb.co/R31zQf5'
  spec.dependency   = "OpenTok"
 
 end
