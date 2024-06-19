![ImagePicker](https://github.com/dotsquares395/ScreenSharing/blob/main/ScreenSharing//screenshare.png)
![ImagePicker](https://github.com/dotsquares395/ScreenSharing/blob/main/ScreenSharing/screensharecode.png)


## Description

**ScreenSharing** ScreenSharing allows real-time sharing of your computer screen with others, facilitating remote collaboration, presentations, and technical support..


## Usage

**ScreenSharing** works as a normal controller, just instantiate it and present it.

```swift
    import ScreenSharing
```

**ScreenSharing** "Use the connectScreenSharing function in the OpenTokManager class, providing kApiKey, sessionId, token, and appView as parameters. Verify that appView is correctly set to your screen-sharing interface."

```swift
    let kApiKey = ""
    let kSessionId = ""
    let kToken = ""
    OpenTokManager.connectScreenSharing(apiKey:kApiKey , sessionId: kSessionId, token: kToken, appView: UIView())
```

**ScreenSharing** please allow permission

```swift
  - Camera 
  - Micro-phone
```

## Installation

**ScreenSharing** is available through [CocoaPods](https://cocoapods.org/pods/ScreenSharing). To install
it, simply add the following line to your Podfile:

```ruby
pod 'ScreenSharing'
```


```ruby
GitHub "dotsquares395/ScreenSharing"
```

## Author

[dotsquares395] made this with ❤️


## License

**ScreenSharing** is available under the MIT license. See the [LICENSE](https://github.com/dotsquares395/ScreenSharing/?tab=MIT-1-ov-file) file for more info.
Copyright (c) 2024 


**ScreenSharing** version vs Swift version.

ScreenSharing 13.0+ is Swift 5 ready! 🎉

If you use an earlier version of Swift - refer to the table below:

| Swift version | ImageViewer version               |
| ------------- | --------------------------------- |
| 5.x           | >= 13.0                       |



