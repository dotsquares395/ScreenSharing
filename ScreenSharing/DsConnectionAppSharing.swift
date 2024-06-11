//
//  DsConnectionAppSharing.swift
//  ScreenSharing
//
//  Created by Hemraj Yogi on 06/06/24.
//

import Foundation
import OpenTok

public struct ConnectionSharing{
    public static var session: OTSession?
    public var publisher: OTPublisher?
    var subscriber: OTSubscriber?
   
    
    public static func connectScreenSharing(kApiKey:String,kSessionId:String,kToken:String,delegate:UIViewController) {
        session = OTSession(apiKey: kApiKey, sessionId: kSessionId, delegate: delegate as? OTSessionDelegate)
        var error: OTError?
        session?.connect(withToken: kToken, error: &error)
        if let error = error {
            print("Error connecting to session: \(error.localizedDescription)")
        }
        
    }
    
    func sessionDidConnect(_ session: OTSession) {
        print("Session connected")
    }

    func sessionDidDisconnect(_ session: OTSession) {
        print("Session disconnected")
    }

    func session(_ session: OTSession, didFailWithError error: OTError) {
        print("Session failed with error: \(error.localizedDescription)")
    }

    func session(_ session: OTSession, streamCreated stream: OTStream) {
        print("Stream created")
        // Handle new streams (subscribers) here if needed
    }

    func session(_ session: OTSession, streamDestroyed stream: OTStream) {
        print("Stream destroyed")
        // Handle stream destruction here if needed
    }

    func publisher(_ publisher: OTPublisherKit, didFailWithError error: OTError) {
        print("Publisher failed with error: \(error.localizedDescription)")
    }
    
}
