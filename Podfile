## Uncomment the next line to define a global platform for your project
## platform :ios, '9.0'
#
#target 'ScreenSharing' do
#  use_frameworks!
#  pod 'OTXCFramework'
#
#  # Pods for ScreenSharing
#
#  target 'ScreenSharingTests' do
#    # Pods for testing
#  end
#
#end

use_frameworks!

target 'ScreenSharing' do
  pod 'OTXCFramework', :modular_headers => true
end

post_install do |installer|
  installer.pods_project.targets.each do |target|
    if target.name == 'ScreenSharing'
      target.build_configurations.each do |config|
        config.build_settings['EXCLUDED_ARCHS[sdk=iphonesimulator*]'] = 'arm64'
      end
    end
  end
end
