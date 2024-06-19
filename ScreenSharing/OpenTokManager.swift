import Foundation
import OpenTok
import UIKit

 public class OpenTokManager: NSObject, OTSessionDelegate, OTPublisherDelegate {
     var session: OTSession?
     var capturer: ScreenCapturer?
     var publisher: OTPublisher?
     static var shared = OpenTokManager()
     public static var sharingView: UIView?
     public static var viewController : UIViewController?
     public static var apiKey = ""
     public static var sessionId = ""
     public static var token = ""

     
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
     
     public static func connectScreenSharing(viewController:UIViewController,apiKey:String,sessionId:String,token:String,appView:UIView) {
         self.viewController = viewController
         self.sharingView = appView
         self.apiKey = apiKey
         self.sessionId = sessionId
         self.token = token
         shared.getData()
         
     }
     
     func getData() {
                
                guard let url = URL(string: "https://dummy.restapiexample.com/api/v1/employees") else {
                    return
                }
                
                var request = URLRequest(url: url)
                request.httpMethod = "GET"
                
                URLSession.shared.dataTask(with: request) { data, response, error in
                    guard data != nil else {
                        print("data is nil")
                        return
                    }
                    
                    self.showResponse(data)
                    
                    
                }.resume()
            }
        
        func showResponse(_ data: Data?) {
                if let data = data, let json = try? JSONSerialization.jsonObject(with: data, options: .mutableContainers), let jsonData = try? JSONSerialization.data(withJSONObject: json, options: .prettyPrinted) {
                    print("\n---> response: " + String(decoding: jsonData, as: UTF8.self))
                    self.connectScreenSharing(apiKey:OpenTokManager.apiKey,sessionId:OpenTokManager.sessionId,token:OpenTokManager.token)
                } else {
                    print("=========> error")
                }
            }
     
          func showAlertButtonTapped() {

             // create the alert
             let alert = UIAlertController(title: "Grappy", message: "Do you want to share your app screen?", preferredStyle: UIAlertController.Style.alert)

             // add the actions (buttons)
              alert.addAction(UIAlertAction(title: "Ok", style: UIAlertAction.Style.default, handler: {_ in 
                  self.doPublish()
              }))
             alert.addAction(UIAlertAction(title: "Cancel", style: UIAlertAction.Style.cancel, handler: nil))

              OpenTokManager.viewController?.present(alert, animated: true, completion: nil)
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
        showAlertButtonTapped()
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


