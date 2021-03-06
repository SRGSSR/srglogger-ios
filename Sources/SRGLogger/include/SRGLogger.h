//
//  Copyright (c) SRG SSR. All rights reserved.
//
//  License information is available from the LICENSE file.
//

@import Foundation;

NS_ASSUME_NONNULL_BEGIN

// Oficial version number.
FOUNDATION_EXPORT NSString *SRGLoggerMarketingVersion(void);

/**
 *  Logging levels
 */
typedef NS_ENUM(NSUInteger, SRGLogLevel) {
    /**
     *  Level to capture detailed technical information.
     */
    SRGLogLevelVerbose,
    /**
     *  Level to capture information useful for debugging.
     */
    SRGLogLevelDebug,
    /**
     *  Level to capture information that may be helpful for troubleshooting errors.
     */
    SRGLogLevelInfo,
    /**
     *  Level to capture information about conditions which might lead to a failure.
     */
    SRGLogLevelWarning,
    /**
     *  Level to capture information about failures.
     */
    SRGLogLevelError
};

// Block signatures.
typedef void (^SRGLogHandler)(NSString * (^message)(void), SRGLogLevel level, NSString * _Nullable const subsystem, NSString * _Nullable const category, const char *file, const char *function, NSUInteger line);

/**
 *  @name Built-in log handlers
 */

/**
 *  CocoaLumberjack handler. Return `nil` if CocoaLumberjack is not available at runtime.
 */
OBJC_EXTERN SRGLogHandler _Nullable SRGCocoaLumberjackHandler(void);

/**
 *  Apple unified logging handler. Return `nil` on non-supported platforms (< iOS 10).
 */
OBJC_EXTERN SRGLogHandler _Nullable SRGUnifiedLoggingHandler(void);

/**
 *  `NSLog`-based handler. Always available.
 */
OBJC_EXTERN SRGLogHandler SRGNSLogHandler(void);

/**
 *  `SRGLogger` is a small generic logging facility. It can be used by all SSRG SSR apps and libraries to provide for a consistent
 *  way of logging and interfacing with logging frameworks like [CocoaLumberjack](https://github.com/CocoaLumberjack/CocoaLumberjack)
 *  or [Apple unified logging](https://developer.apple.com/reference/os/1891852-logging), for example.
 *
 *  ## Logging
 *
 *  To log a message, call the macro matching the desired level. You should provide an optional subsystem (identifying
 *  your library or application) and / or category (identifying to which part of the code the log is related), e.g.
 *
 *  SRGLogInfo(@"com.myapp", @"Weather", @"The temperature is %@", @(temperature));
 *
 *  This information is forwarded to a log handler, which is a global block through which logging requests are sent. By
 *  default this handler sends messages to [CocoaLumberjack](https://github.com/CocoaLumberjack/CocoaLumberjack) if found
 *  at runtime, otherwise to Apple Unified logging. If neither is found, no loggging will take place. You can use a built-in
 *  `NSLog` based logger by calling `+setLogHandler:` with `SRGNSLogHandler` as parameter if you need a quick way to setup
 *  a logger. Be careful, though, as this logger logs everything (and thus can be quite verbose and slow down your application).
 *
 *  ## Interfacing with other loggers
 *
 *  If the default log handler does not suit your needs (or if you simply want to inhibit logging), call the `+setLogHandler:`
 *  method to set a new handler (or `nil`). Then implement the handler block to forward the messages and contextual
 *  information to your other logger.
 *
 *  The standard CocoLumberjack, Unified logging and `NSLog` handlers are available publicly (@see Built-in log handlers).
 *  You can force the use of one of them using `+setLogHandler:` if you want (provided the logger actually is available, 
 *  otherwise logging will be disabled).
 *
 *  ## Defining convenience macros in your application or library
 *
 *  To avoid specifiying the subsystem in your application or library each time you call the macro, you can define you
 *  own set of macros which always set this value consistently, for example:
 *
 *  #define MyAppLogVerbose(category, format, ...) SRGLogVerbose(@"com.myapp", category, format, ##__VA_ARGS__)
 *  #define MyAppLogDebug(category, format, ...)   SRGLogDebug(@"com.myapp", category, format, ##__VA_ARGS__)
 *  #define MyAppLogInfo(category, format, ...)    SRGLogInfo(@"com.myapp", category, format, ##__VA_ARGS__)
 *  #define MyAppLogWarning(category, format, ...) SRGLogWarning(@"com.myapp", category, format, ##__VA_ARGS__)
 *  #define MyAppLogError(category, format, ...)   SRGLogError(@"com.myapp", category, format, ##__VA_ARGS__)
 *
 *  You can of course proceed similarly for categories if needed.
 *
 *  ## Use from Swift code
 *
 *  `SRGLogger` uses macros to log contextual information (file name, line number, etc.) which cannot be used from Swift
 *  source files. If you need to use `SRGLogger` from a Swift source file, use the functions declared by the companion
 *  `SRGLoggerSwift` framework.
 *
 *  ## Credits
 *
 *  This implementation is entirely based on the following idea from Cédric Luthi, with minor adjustements along the way:
 *    http://stackoverflow.com/questions/34732814/how-should-i-handle-logs-in-an-objective-c-library/34732815#
 */
@interface SRGLogger : NSObject

/**
 *  Call to replace the current log handler.
 *
 *  @param logHandler The new log handler.
 *
 *  @return The previously installed log handler.
 */
+ (nullable SRGLogHandler)setLogHandler:(nullable SRGLogHandler)logHandler;

/**
 *  Log a message. Not meant to be called directly, use macros below instead.
 *
 *  @param message   A message block for message construction.
 *  @param level     The logging level to use.
 *  @param subsystem The subsystem with which the message must be associated.
 *  @param category  The category with which the message must be associated.
 *  @param file      File name information.
 *  @param function  Function name information.
 *  @param line      Line information.
 */
+ (void)logMessage:(NSString * (^)(void))message
             level:(SRGLogLevel)level
         subsystem:(nullable NSString * const)subsystem
          category:(nullable NSString * const)category
              file:(const char *)file
          function:(const char *)function
              line:(NSUInteger)line;

@end

/**
 *  Generic macro for logging messages with a specific level.
 */
#define SRGLog(_subsystem, _category, _level, _message) [SRGLogger logMessage:(_message) level:(_level) subsystem:(_subsystem) category:(_category) file:__FILE__ function:__PRETTY_FUNCTION__ line:__LINE__]

/**
 *  Convenience macros for logging at all available levels.
 */
#define SRGLogVerbose(subsystem, category, format, ...) SRGLog(subsystem, category, SRGLogLevelVerbose, (^{ return [NSString stringWithFormat:(format), ##__VA_ARGS__]; }))
#define SRGLogDebug(subsystem, category, format, ...)   SRGLog(subsystem, category, SRGLogLevelDebug,   (^{ return [NSString stringWithFormat:(format), ##__VA_ARGS__]; }))
#define SRGLogInfo(subsystem, category, format, ...)    SRGLog(subsystem, category, SRGLogLevelInfo,    (^{ return [NSString stringWithFormat:(format), ##__VA_ARGS__]; }))
#define SRGLogWarning(subsystem, category, format, ...) SRGLog(subsystem, category, SRGLogLevelWarning, (^{ return [NSString stringWithFormat:(format), ##__VA_ARGS__]; }))
#define SRGLogError(subsystem, category, format, ...)   SRGLog(subsystem, category, SRGLogLevelError,   (^{ return [NSString stringWithFormat:(format), ##__VA_ARGS__]; }))

NS_ASSUME_NONNULL_END
