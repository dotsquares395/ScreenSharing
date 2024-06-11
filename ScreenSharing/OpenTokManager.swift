import Foundation
import OpenTok
import UIKit

 public class OpenTokManager: NSObject, OTSessionDelegate, OTPublisherDelegate {
     var session: OTSession?
     var capturer: ScreenCapturer?
     var publisher: OTPublisher?
     static var shared = OpenTokManager()
     public static var sharingView: UIView?
     
    override init() {
        super.init()
       // connectToSession()
    }
    
    fileprivate func doPublish() {
        var error: OTError? = nil
        defer {
            print("error>>>>>>",error as Any)
        }
        let settings = OTPublisherSettings()
        settings.name = UIDevice.current.name
        self.publisher = OTPublisher(delegate: nil, settings: settings)
        self.publisher?.videoType = .screen
        self.publisher?.audioFallbackEnabled = false
        capturer = ScreenCapturer(withView:OpenTokManager.sharingView ?? UIView())
        self.publisher?.videoCapture = capturer
        self.publisher?.videoCapture?.videoContentHint = .text
        self.session?.publish(self.publisher!, error: &error)
    }
     
     public static func connectScreenSharing(apiKey:String,sessionId:String,token:String,appView:UIView) {
         OpenTokManager.sharingView = appView
         shared.connectScreenSharing(apiKey:apiKey,sessionId:sessionId,token:token)
     }
          
     private func connectScreenSharing(apiKey:String,sessionId:String,token:String) {

         self.session = OTSession(apiKey: apiKey, sessionId: sessionId, delegate: self)
         var error: OTError?
         self.session?.connect(withToken: token, error: &error)
         if let error = error {
            print("Error connecting to session: \(error.localizedDescription)")
        }
    }
    public func sessionDidConnect(_ session: OTSession) {
        doPublish()
        print("Session connected")
    }

    public func sessionDidDisconnect(_ session: OTSession) {
        print("Session disconnected")
    }

    public func session(_ session: OTSession, didFailWithError error: OTError) {
        print("Session failed with error: \(error.localizedDescription)")
    }

    public func session(_ session: OTSession, streamCreated stream: OTStream) {
        print("Stream created")
        // Handle new streams (subscribers) here if needed
    }

    public func session(_ session: OTSession, streamDestroyed stream: OTStream) {
        print("Stream destroyed")
        // Handle stream destruction here if needed
    }

    public func publisher(_ publisher: OTPublisherKit, didFailWithError error: OTError) {
        print("Publisher failed with error: \(error.localizedDescription)")
    }
}


