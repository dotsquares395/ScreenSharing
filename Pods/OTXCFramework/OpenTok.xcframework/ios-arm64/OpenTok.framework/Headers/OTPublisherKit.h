//
//  OTPublisherKit.h
//
//  Copyright (c) 2014 Tokbox, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import "OTVideoKit.h"
#import "OTAudioKit.h"

@class OTPublisherKit, OTSession, OTError, OTStream, OTPublisherKitVideoNetworkStats, OTPublisherKitAudioNetworkStats;

@protocol OTVideoCapture;
@protocol OTVideoRender;
@protocol OTPublisherKitDelegate;
@protocol OTPublisherKitAudioLevelDelegate;
@protocol OTPublisherKitNetworkStatsDelegate;
@protocol OTPublisherKitRtcStatsReportDelegate;

/**
 * Video enabled and disabled events are accompanied with a reason code, for
 * determining why the video track was enabled/disabled.
 *
 */
typedef NS_ENUM(int32_t, OTPublisherVideoEventReason) {
    /**
     * The video event was caused by this publisher changing the value
     * for OTPublisherKit.publishVideo.
     */

    OTPublisherVideoEventPublisherPropertyChanged = 1,
    /**
     * The video event was caused by a change in the network congestion level.
     * The <[OTPublisherKitDelegate publisherVideoDisabled:reason:]> message
     * is sent. When conditions improve, the video stream resumes, and the
     * <[OTPublisherKitDelegate publisherVideoEnabled:reason:]> message is
     * sent.
     *
     * When the video stream is dropped, the publisher continues to send
     * the audio stream, if there is one. Moreover, the capturer corresponding
     * to this publisher will not be disabled, and the captured frames will still
     * be seen on the publisher side.
     *
     * When the congestion level deteriorates to a level that
     * is low enough that the video stream is at risk of being disabled, the
     * <[OTPublisherKitDelegate publisherVideoDisableWarning:]> message is
     * sent.
     *
     * You can disable this publisher audio-only fallback feature, by setting the
     * <[OTPublisherKit publisherAudioFallbackEnabled]> property to <code>NO</code>
     * for the publisher of the stream.
     */
    OTPublisherVideoEventQualityChanged = 3
};

/**
* Defines values for the <[OTPublisherKit videoType]> property.
*/
typedef NS_ENUM(int32_t, OTPublisherKitVideoType) {
    /**
     * Defines the camera video mode setting for the
     * <[OTPublisherKit videoType]> property.
     */
    OTPublisherKitVideoTypeCamera = 1,
    /**
     * Defines the screen video mode setting for the
     * <[OTPublisherKit videoType]> property.
     */
    OTPublisherKitVideoTypeScreen = 2,
};

/**
 * Defines settings to be used when initializing a publisher using the
 * <[OTPublisherKit initWithDelegate:settings:]> method.
 */
@interface OTPublisherKitSettings : NSObject

/** @name Defining publisher settings */

/**
 * The name of the publisher video. The <[OTStream name]> property
 * for a stream published by this publisher will be set to this value
 * (on all clients). The default value is `nil`.
 */
@property(nonatomic, copy, nullable) NSString *name;

/**
 * Whether to publish audio (YES, the default) or not (NO).
 *
 * If this property is set to NO, the audio subsystem will not be initialized
 * for the publisher, and setting the <[OTPublisherKit publishAudio]> property
 * will have no effect. If your application does not require the use of audio,
 * it is recommended to set this Builder property rather than use the
 * <[OTPublisherKit publishAudio]> property, which only temporarily disables
 * the audio track.
 */
@property(nonatomic) BOOL audioTrack;

/**
 * Whether to publish video (YES, the default) or not (NO).
 *
 * If this property is set to NO, the video subsystem will not be initialized
 * for the publisher, and setting the <[OTPublisherKit publishVideo]> property
 * will have no effect. If your application does not require the use of video,
 * it is recommended to set this Builder property rather than use the
 * <[OTPublisherKit publishVideo]> property, which only temporarily disables
 * the video track.
 */
@property(nonatomic) BOOL videoTrack;

/**
 * The desired bitrate for the published audio, in bits per second.
 * The supported range of values is 6,000 - 510,000. (Invalid values are
 * ignored.) Set this value to enable high-quality audio (or to reduce
 * bandwidth usage with lower-quality audio).
 *
 * The following are recommended settings:
 *
 * * 8,000 - 12,000 for narrowband (NB) speech
 * * 16,000 - 20,000 for wideband (WB) speech
 * * 28,000 - 40,000 for full-band (FB) speech
 * * 48,000 - 64,000 for full-band (FB) mono music
 * * 64,000 - 128,000 for full-band (FB) stereo music
 *
 * The default value is 40,000.
 *
 * To set other audio settings, see the <OTAudioDeviceManager> interface.
 */
@property(nonatomic) int audioBitrate;

/**
 * Whether to enable [Opus DTX](https://datatracker.ietf.org/doc/html/rfc7587#section-3.1.3)
 * (YES) or not (NO, the default). Enabling Opus DTX can reduce bandwidth usage
 * in streams that have long periods of silence.
 */
@property(nonatomic) BOOL enableOpusDtx;

/**
 * Whether to allow use of
 * <a href="https://tokbox.com/developer/guides/scalable-video/">scalable video</a>
 * for a publisher that has the videoType
 * set to OTStreamVideoTypeScreen (YES) or not (NO, the default).
 */
@property(nonatomic) BOOL scalableScreenshare;

/**
 * Whether the stream will use the subscriber audio-fallback feature (<code>YES</code>) or
 * not (<code>NO</code>), for subscribers to the published stream. The subscriber
 * audio-fallback feature is available in sessions that use the the OpenTok Media Router.
 * With the subscriber audio-fallback feature enabled, when the OpenTok
 * Media Router determines that a stream's quality has degraded significantly
 * for a specific subscriber, it disables the video in that subscriber in order to
 * preserve call quality.
 *
 * The default setting is <code>YES</code> (the audio-fallback feature is
 * enabled) for publishers using the camera. When using the
 * screen video type in a session that uses the OpenTok Media Server, the
 * subscriber audio-fallback property is set to <code>NO</code> by default,
 * so that the video does not drop out in subscribers.
 *
 * For more information, see <[OTPublisherKit videoType]>,
 * <[OTSubscriberKitDelegate subscriberVideoDisabled:reason:]>,
 * <OTSubscriberVideoEventReason>, [the OpenTok Media
 * Router](http://tokbox.com/opentok/tutorials/create-session/#media-mode ).
 */
@property (nonatomic, assign) BOOL subscriberAudioFallbackEnabled;

/**
 * Whether the stream will use the publisher audio-fallback feature (<code>YES</code>) or
 * not (<code>NO</code>). With the publisher audio-fallback feature
 * enabled, when the Publisher determines that a stream's quality has
 * degraded significantly, it disables the video in in order to preserve call quality.
 *
 * The default setting is <code>NO</code> (the audio-fallback feature is
 * disabled).
 *
 * For more information, see <[OTPublisherKit videoType]>,
 * <[OTPublisherKitDelegate publisherVideoDisabled:reason:]>,
 * <OTPublisherVideoEventReason>, [the OpenTok Media
 * Router](http://tokbox.com/opentok/tutorials/create-session/#media-mode ).
 *
 */
@property (nonatomic, assign) BOOL publisherAudioFallbackEnabled;

@end

/**
 * A publisher captures an audio-video stream from the sources you specify. You
 * can then publish the audio-video stream to an OpenTok session by sending the
 * <[OTSession publish:error:]> message.
 *
 * The OpenTok iOS SDK supports publishing on all multi-core iOS devices.
 * See "Developer and client requirements" in the README file for the
 * [OpenTok iOS SDK](http://tokbox.com/opentok/libraries/client/ios).
 */
@interface OTPublisherKit : NSObject

- (_Nonnull instancetype)init NS_UNAVAILABLE;

/** @name Initializing a publisher */

/**
 * Initialize a publisher object and specify the delegate object.
 *
 * When running in the XCode iOS Simulator, this method returns `nil`.
 *
 * @param delegate The delegate (<OTPublisherKitDelegate>) object for the
 * publisher.
 *
 * @return The pointer to the instance, or `nil` if initialization failed.
 */
- (nullable instancetype)initWithDelegate:(nullable id<OTPublisherKitDelegate>)delegate;

/**
 * Initialize the publisher with settings defined by an
 * <OTPublisherKitSettings> object.
 *
 * @param delegate The delegate (<OTPublisherKitDelegate>) object for the
 * publisher.
 *
 * @param settings The (<OTPublisherKitSettings>) object that defines settings
 * for the publisher.
 */
-(nullable instancetype)initWithDelegate:(nullable id<OTPublisherKitDelegate>)delegate
                       settings:(nonnull OTPublisherKitSettings *)settings;

/**
 * Initialize a publisher object, and specify the delegate object and the 
 * stream's name.
 *
 * This method is deprecated. Use <[OTPublisherKit initWithDelegate:settings:]>
 * instead.
 *
 * When running in the XCode iOS Simulator, this method returns `nil`.
 *
 * @param delegate The delegate (<OTPublisherKitDelegate>) object for the 
 * publisher.
 *
 * @param name The name of the publisher video. The <[OTStream name]> property
 * for a stream published by this publisher will be set to this value
 * (on all clients).
 *
 * @return The pointer to the instance, or `nil` if initialization failed.
 */
- (nullable instancetype)initWithDelegate:(nullable id<OTPublisherKitDelegate>)delegate
                                    name:(nullable NSString*)name __attribute((deprecated(("Use initWithDelegate: settings: instead"))));

/**
 * Initialize the publisher, and specify whether audio and video will be
 * enabled for this instance.
 *
 * This method is deprecated. Use <[OTPublisherKit initWithDelegate:settings:]>
 * instead.
 *
 * If either audioTrack or videoTrack are set to NO,
 * the respective subsystem will not be initialized, and setting the
 * corresponding <[OTPublisherKit publishAudio]> and
 * <[OTPublisherKit publishVideo]> properties will have no effect.
 *
 * If your application does not require the use of audio or video, it is
 * recommended to set these values rather than use the
 * <[OTPublisherKit publishAudio]> and <[OTPublisherKit publishVideo]>
 * properties, which only temporarily disable the tracks.
 *
 * @param delegate The delegate (<OTPublisherKitDelegate>) object for the
 * publisher.
 *
 * @param name The name of the publisher video. The <[OTStream name]> property
 * for a stream published by this publisher will be set to this value
 * (on all clients).
 *
 * @param audioTrack Whether to publish audio (YES) or not (NO).
 *
 * @param videoTrack Whether to publish video (YES) or not (NO).
 */
- (nullable instancetype)initWithDelegate:(nullable id<OTPublisherKitDelegate>)delegate
                                    name:(nullable NSString*)name
                              audioTrack:(BOOL)audioTrack
                              videoTrack:(BOOL)videoTrack __attribute((deprecated(("Use initWithDelegate: settings: instead"))));

/** @name Getting information about the publisher */

/**
 * The <OTPublisherDelegate> object, which is the delegate for the OTPublisher
 * object.
 *
 * See also <[OTPublisherKit audioLevelDelegate]>,
 * <[OTPublisherKit networkStatsDelegate]>, and
 * <[OTPublisherKit rtcStatsReportDelegate]>.
 */
@property(nonatomic, weak) id<OTPublisherKitDelegate> _Nullable delegate;

/**
 * Periodically receives reports of audio levels for this publisher.
 *
 * This is a separate delegate object from that set as the delegate property
 * (the OTPublisherKitDelegate object).
 *
 * If you do not set this property, the audio sampling subsystem is disabled.
 */
@property (nonatomic, weak)
id<OTPublisherKitAudioLevelDelegate> _Nullable audioLevelDelegate;

/**
 * The <OTPublisherKitNetworkStatsDelegate> object that periodically
 * receives publisher quality statistics.
 *
 * This delegate object is sent messages reporting the following:
 *
 * * Total audio and video packets lost
 * * Total audio and video packets received
 * * Total audio and video bytes received
 *
 * This is a separate delegate object from that set as the
 * <[OTPublisherKit delegate]> property (the OTPublisherKitDelegate object).
 *
 * Also see <[OTPublisherKit rtcStatsReportDelegate]>.
 */
@property (nonatomic, weak)
id<OTPublisherKitNetworkStatsDelegate> _Nullable networkStatsDelegate;

/**
 * The <OTPublisherKitRtcStatsReportDelegate> that reports RTC stats for
 * the publisher. The <[OTPublisherKitRtcStatsReportDelegate publisher:rtcStatsReport:]>
 * message is sent in reponse to calling the <[OTPublisherKit getRtcStatsReport]>
 * method.
 */
@property (nonatomic, weak)
id<OTPublisherKitRtcStatsReportDelegate> _Nullable rtcStatsReportDelegate;

/**
 * The session that owns this publisher.
 */
@property(readonly) OTSession* _Nullable session;

/**
 * The <OTStream> object associated with the publisher.
 */
@property(readonly) OTStream* _Nullable stream;

/**
 * A string that will be associated with this publisher's stream. This string is
 * displayed at the bottom of subscriber videos associated with the published
 * stream, if an overlay to display the name exists. 
 *
 * The name must be set at initialization, when you when you send the
 * <[OTPublisherKit initWithDelegate:settings:]> message.
 *
 * This value defaults to an empty string.
 */
@property(readonly) NSString* _Nullable name;

/**
 * Gets the RTC stats report for the publisher. This is an asynchronous operation.
 * Set the <[OTPublisherKit rtcStatsReportDelegate]> property and implement the
 * <[OTPublisherKitRtcStatsReportDelegate publisher:rtcStatsReport:]> method
 * prior to calling this method. When the stats are available, the implementation
 * of the <[OTPublisherKitRtcStatsReportDelegate publisher:rtcStatsReport:]>
 * message is sent.
 *
 * Also see <[OTPublisherKit networkStatsDelegate]> and
 * <[OTSubscriberKit getRtcStatsReport]>.
 */
-(void) getRtcStatsReport;

/** @name Controlling audio and video output for a publisher */

/**
 * Whether to publish audio.
 *
 * The default value is TRUE.
 */
@property(nonatomic) BOOL publishAudio;

/**
 * Whether to publish video.
 *
 * The default value is TRUE.
 */
@property(nonatomic) BOOL publishVideo;

/**
 * Whether to make captions active.
 *
 * The default value is FALSE.
 */
@property(nonatomic) BOOL publishCaptions;

/** @name Setting publisher device configuration */

/**
 * The <OTVideoCapture> instance used to capture video to stream to the OpenTok
 * session.
 */
@property(nonatomic, strong) id<OTVideoCapture> _Nullable videoCapture;

/**
 * Specifies the type of video for the published stream.
 *
 * Set this to one of the following values:
 *
 * * <code>OTPublisherKitVideoTypeScreen</code> &mdash; Optimizes the video
 *   encoding for screen sharing. To publish a screen-sharing stream, you
 *   need to implement a custom video capturer for the OTPublisherKit object.
 *   See <[OTPublisherKit videoCapture]>. It is recommended to use a low frame
 *   rate (5 frames per second or lower) with this video type. When using the
 *   screen video type in a session that uses the OpenTok Media Server, the
 *   <[OTPublisherKit audioFallbackEnabled]> property is set to NO by default;
 *   this disables the audio-only fallback feature, so that the video does not
 *   drop out in subscribers. See [the OpenTok Media
 *   Router](http://tokbox.com/opentok/tutorials/create-session/#media-mode ).
 *
 * * <code>OTPublisherKitVideoTypeCamera</code> &mdash;
 *   This is the default setting, which you should use when using a camera
 *   as the video source.
 */
@property (nonatomic, assign) OTPublisherKitVideoType videoType;

/**
 * The <OTVideoRender> instance used to render video to stream to the OpenTok
 * session.
 */
@property(nonatomic, strong) id<OTVideoRender> _Nullable videoRender;

/** @name Setting the audio-only fallback mode */

/**
 * Whether the stream will use the subscriber audio-fallback feature (<code>YES</code>) or
 * not (<code>NO</code>). The audio-fallback feature is available in sessions
 * that use the the OpenTok Media Router. With the audio-fallback feature
 * enabled, when the OpenTok Media Router determines that a stream's quality has
 * degraded significantly for a specific subscriber, it disables the video in
 * that subscriber in order to preserve call quality.
 *
 * This method is deprecated.
 * Use <[OTPublisherKitSettings subscriberAudioFallbackEnabled]> instead.
 *
 * The default setting is <code>YES</code> (the audio-fallback feature is
 * enabled) for publishers using the camera. To turn off the audio-fallback
 * feature, set this property to <code>NO</code> <i>before</i> calling the
 * <[OTSession publish:error:]> method. However, When using the
 * screen video type in a session that uses the OpenTok Media Server, the
 * audioFallbackEnabled property is set to NO by default; this disables the
 * audio-only fallback feature, so that the video does not drop out in
 * subscribers.
 *
 * For more information, see <[OTPublisherKit videoType]>,
 * <[OTSubscriberKitDelegate subscriberVideoDisabled:reason:]>,
 * <OTSubscriberVideoEventReason>, [the OpenTok Media
 * Router](http://tokbox.com/opentok/tutorials/create-session/#media-mode ).
 */
@property (nonatomic, assign) BOOL audioFallbackEnabled;

/** @name Setting media transformers */

/**
 * Array of OTVideoTransformer in the order to be applied to the stream.
 */
@property (nonatomic, strong) NSArray * _Nonnull videoTransformers;

/**
 * Array of OTAudioTransformer in the order to be applied to the stream.
 */
@property (nonatomic, strong) NSArray * _Nonnull audioTransformers;

@end

/**
 * Used for sending messages for an OTPublisher instance. The OTPublisher class
 * includes a `delegate` property. When you send the
 * <[OTPublisherKit initWithDelegate:]> message or the
 * <[OTPublisherKit initWithDelegate:settings:]> message, you specify an
 * OTPublisherKitDelegate object.
 */
@protocol OTPublisherKitDelegate <NSObject>

/**
 * Sent if the publisher encounters an error. After this message is sent,
 * the publisher can be considered fully detached from a session and may
 * be released.
 * @param publisher The publisher that signalled this event.
 * @param error The error (an <OTError> object). The `OTPublisherErrorCode` 
 * enum (defined in the OTError class)
 * defines values for the `code` property of this object.
 */
- (void)publisher:(nonnull OTPublisherKit*)publisher
 didFailWithError:(nonnull OTError*)error;

@optional

/**
 * Sent when a moderator has forced this publisher to mute audio.
 *
 * See <[OTSession forceMuteAll:error:]> and
 * <[OTSession forceMuteStream:error:]>.
 *
 * @param publisher The publisher.
 */
- (void)muteForced:(nonnull OTPublisherKit*)publisher;

/**
 * Sent when the publisher starts streaming.
 *
 * @param publisher The publisher of the stream.
 * @param stream The stream that was created.
 */
- (void)publisher:(nonnull OTPublisherKit*)publisher
    streamCreated:(nonnull OTStream*)stream;

/**
 * Sent when the publisher stops streaming.
 * @param publisher The publisher that stopped sending this stream.
 * @param stream The stream that ended.
 */
- (void)publisher:(nonnull OTPublisherKit*)publisher
  streamDestroyed:(nonnull OTStream*)stream;

/**
 * This message is sent when the publisher stops sending video.
 * Check the reason parameter for the reason why the video stopped.
 *
 * @param publisher The <OTPublisher> that will no longer send video.
 * @param reason The reason that the video track was disabled. See
 * <OTPublisherVideoEventReason>.
 */
- (void)publisherVideoDisabled:(nonnull OTPublisherKit*)publisher
                        reason:(OTPublisherVideoEventReason)reason;

/**
 * This message is sent when the publisher's video stream resumes (after
 * video was disabled). Check the reason parameter for the reason
 * why the video resumed.
 *
 * @param publisher The <OTPublisher> that will send video.
 * @param reason The reason that the video track was enabled. See
 * <OTPublisherVideoEventReason>.
 */
- (void)publisherVideoEnabled:(nonnull OTPublisherKit*)publisher
                       reason:(OTPublisherVideoEventReason)reason;

/**
 * This message is sent when the publisher determines that the stream
 * quality has degraded and the video will be disabled if the quality degrades
 * further. If the quality degrades further, the publisher disables the video
 * and the <[OTPublisherKitDelegate publisherVideoDisabled:reason:]> message
 * is sent. If the network congestion level improves, the
 * <[OTPublisherKitDelegate publisherVideoDisableWarningLifted:]> message is
 * sent.
 *
 * This message is mainly sent when connection quality degrades.
 *
 * @param publisher The <OTPublisher> that may stop sending video soon.
 */
- (void)publisherVideoDisableWarning:(nonnull OTPublisherKit*)publisher;

/**
 * This message is sent when the publisher determines that the network
 * congestion level has improved to the point at which the video being disabled
 * is not an immediate risk. This message is sent after the
 * <[OTPublisherKitDelegate publisherVideoDisableWarning:]> message is
 * sent.
 *
 * This message is mainly sent when connection quality improves.
 *
 * @param publisher The <OTPublisher> instance.
 */
- (void)publisherVideoDisableWarningLifted:(nonnull OTPublisherKit*)publisher;

@end

/**
 * Used for monitoring the audio levels of the publisher.
 */
@protocol OTPublisherKitAudioLevelDelegate <NSObject>

/**
 * Sent on a regular interval with the recent representative audio level.
 *
 * @param publisher The publisher instance being represented.
 * @param audioLevel A value between 0 and 1, representing the audio level.
 * Adjust this value logarithmically for use in a user interface
 * visualization of the volume (such as a volume meter).
 */
- (void)publisher:(nonnull OTPublisherKit*)publisher
audioLevelUpdated:(float)audioLevel;

@end

/**
 * Used to monitor audio and video statistics for the publisher. See
 * <[OTPublisherKit networkStatsDelegate]>.
 */
@protocol OTPublisherKitNetworkStatsDelegate <NSObject>

@optional

/**
 * Sent periodically to report video statistics for the publisher.
 *
 * @param publisher The publisher these statistic apply to.
 *
 * @param stats An array of <OTPublisherKitVideoNetworkStats> objects.
 * For a publisher in a routed session (one that uses the
 * <a href="https://tokbox.com/developer/guides/create-session/#media-mode">OpenTok
 * Media Router</a>), this array includes one object, defining the statistics
 * for the single video media stream that is sent to the OpenTok Media Router.
 * In a relayed session, the array includes an object for each subscriber
 * to the published stream.
 *
 * The <OTPublisherKitVideoNetworkStats> object includes properties.
 * properties for the video bytes received, video packets lost, and video
 * packets sent for the publisher.
 */
- (void)publisher:(nonnull OTPublisherKit*)publisher
videoNetworkStatsUpdated:(nonnull NSArray<OTPublisherKitVideoNetworkStats*>*)stats;

/**
 * Sent periodically to report audio statistics for the publisher.
 *
 * @param publisher The publisher these statistic apply to.
 *
 * @param stats An array of <OTPublisherKitAudioNetworkStats> objects.
 * For a publisher in a routed session (one that uses the
 * <a href="https://tokbox.com/developer/guides/create-session/#media-mode">OpenTok
 * Media Router</a>), this array includes one object, defining the statistics
 * for the single audio media stream that is sent to the OpenTok Media Router.
 * In a relayed session, the array includes an object for each subscriber
 * to the published stream.
 *
 * The <OTPublisherKitAudioNetworkStats> object includes properties.
 * properties for the audio bytes received, audio packets lost, and audio
 * packets sent for the publisher.
 */
- (void)publisher:(nonnull OTPublisherKit*)publisher
audioNetworkStatsUpdated:(nonnull NSArray<OTPublisherKitAudioNetworkStats*>*)stats;

@end

/**
 * Represents RTC statistics for a media stream published by the publisher.
 *
 * See <[OTPublisherKit getRtcStatsReport]>.
 */
@interface OTPublisherRtcStats : NSObject
/**
 * For a relayed session (in which a publisher sends individual media streams
 * to each subscriber), this is the unique ID of the client's connection.
 */
@property (strong) NSString* _Nonnull connectionId;
/**
  A JSON array of RTC stats reports for the media stream.
  The structure of the JSON array is similar to the format of
  the RtcStatsReport object implemented in web browsers (see
  <a href="https://developer.mozilla.org/en-US/docs/Web/API/RTCStatsReport">Mozilla
  docs</a>). Also see <a href="https://w3c.github.io/webrtc-stats/#summary">this
  W3C documentation</a>.

  Here is a partial sample:

  <pre>
   [
      {
          "audioLevel": 0,
          "id": "RTCAudioSource_1",
          "kind": "audio",
          "timestamp": 1603448671532842,
          "totalAudioEnergy": 0,
          "totalSamplesDuration": 4.249999999999954,
          "trackIdentifier": "4nP5yeIDzbUm6IJho5jkTps1lnfabsFvTXjH00",
          "type": "media-source"
      },
      {
          "base64Certificate": "MIIBFjCB...QIhAMIfr/NgvhNp16zaoHxGQGrID1STFmBSSSB4V1bxBrTU",
          "fingerprint": "E7:5F:...:FA:5A:1F:A7:E0:55:60",
          "fingerprintAlgorithm": "sha-256",
          "id": "RTCCertificate_E7:5F:E5...F:1D:FA:5A:1F:A7:E0:55:60",
          "timestamp": 1603448671532842,
          "type": "certificate"
      },
      {
          "channels": 1,
          "clockRate": 8000,
          "id": "RTCCodec_audio_Inbound_0",
          "mimeType": "audio/PCMU",
          "payloadType": 0,
          "timestamp": 1603448671532842,
          "type": "codec"
      },
      {
          "channels": 2,
          "clockRate": 48000,
          "id": "RTCCodec_audio_Inbound_111",
          "mimeType": "audio/opus",
          "payloadType": 111,
          "sdpFmtpLine": "minptime=10;useinbandfec=1",
          "timestamp": 1603448671532842,
          "type": "codec"
      },
   ]
   </pre>
 */
@property (strong) NSString* _Nonnull jsonArrayOfReports;
@end

/**
 * Used for sending messages reporting publisher RTC statistics. Set the
 * <[OTPublisherKit rtcStatsReportDelegate]> property to use this delegate.
 * After you call the <[OTPublisherKit getRtcStatsReport]> method,
 * the <[OTPublisherKitRtcStatsReportDelegate publisher:rtcStatsReport:]>
 * message is sent to report the stats.
 */
@protocol OTPublisherKitRtcStatsReportDelegate <NSObject>

@optional

/**
 * Sent when the RTC statistics for the publisher are available, in response
 * to a call to the <[OTPublisherKit rtcStatsReportDelegate]> method.
 *
 * @param publisher The publisher these statistic apply to.
 *
 * @param stats The RTC statistics for the publisher, represented as an
 * array of <OTPublisherRtcStats> objects. For a routed session (a seesion that uses the
 * <a href="https://tokbox.com/developer/guides/create-session/#media-mode">OpenTok
 * Media Router</a>), this array includes one object, defining the statistics
 * for the single video media stream that is sent to the OpenTok Media Router.
 * In a relayed session, the array includes an object for each subscriber
 * to the published stream.
 */
- (void)publisher:(nonnull OTPublisherKit*)publisher
   rtcStatsReport:(nonnull NSArray<OTPublisherRtcStats *> *)stats;

@end

/**
 * Defines a custom transformer for a video stream, to be passed into the
 * <[OTVideoTransformer initWithName:transformer:]> method.
 */
@protocol OTCustomVideoTransformer <NSObject>
/**
 * Called when a video frame is available to be transformed. Transform the
 * data in your implementation of the method.
 * @param frame The frame to be transformed.
 */
- (void)transform:(nonnull OTVideoFrame*)frame;

@end

/**
 * Defines a custom transformer for an audio stream, to be passed into the
 * <[OTAudioTransformer initWithName:transformer:]> method.
 */
@protocol OTCustomAudioTransformer <NSObject>
/**
 * Called when audio data is available to be transformed. Transform the
 * data in your implementation of the method.
 * @param audioData The audio data to be transformed.
 */
- (void)transform:(nonnull OTAudioData*)audioData;

@end

/**
 *  Defines a transformer of a video stream.
 */
@interface OTVideoTransformer : NSObject

/**
* Creates an instance of a Vonage video transformer. For more information,
* see <a href="https://tokbox.com/developer/guides/vonage-media-processor/ios/">Using
* the Vonage Media Processor library</a>. <b>Note:</b> Media transformers
* are not supported on all devices or iOS versions — see
* <a href="https://tokbox.com/developer/guides/vonage-media-processor/ios/#supported-devices-and-ios-versions">this documentation</a>.
* @param name The name of the Vonage transformer. Currently,
* "BackgroundBlur" and "BackgroundReplacement" are supported.
* @param properties A JSON string with the properties of the Vonage video transformer.
* For a background blur transformer, the format of the JSON is
* "{\"radius\":\"None\"}". Valid values for the <code>radius</code> property
* a None", "High", and "Low".
* For a custom background blur transformer, the format of the JSON is
* "{\"radius\":\"Custom\",\"custom_radius\":\"value\"}", where <code>custom_radius</code> can
* be any positive integer.
* For a background replacement transformer, the format of the JSON is
* "{\"image_file_path\":\"path/to/image\"}", where <code>image_file_path</code> is the
* absolute file path of a local image to use as virtual background.  Supported image formats
* are PNG and JPEG.
*/
- (nullable id)initWithName:(nonnull NSString*)name properties:(nonnull NSString *)properties;

/**
* Creates an instance of a custom video transformer.
* @param name The identifier for the transformer.
* @param transformer The transformer.
* @return NULL if an error occurs.
*/
- (nullable id)initWithName:(nonnull NSString*)name transformer:(nonnull id<OTCustomVideoTransformer>)transformer;

@end

/**
 *  Defines a transformer of an audio stream.
 */
@interface OTAudioTransformer : NSObject

/**
* Creates an instance of a custom audio transformer.
* @param name The identifier for the transformer.
* @param transformer The transformer.
* @return NULL if error occurs.
*/
- (nullable id)initWithName:(nonnull NSString*)name transformer:(nonnull id<OTCustomAudioTransformer>)transformer;

@end
