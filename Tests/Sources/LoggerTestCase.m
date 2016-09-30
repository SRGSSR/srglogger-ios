//
//  Copyright (c) SRG. All rights reserved.
//
//  License information is available from the LICENSE file.
//

#import <SRGLogger/SRGLogger.h>
#import <XCTest/XCTest.h>

@interface LoggerTestCase : XCTestCase

@end

@implementation LoggerTestCase

- (void)testLogging
{
    SRGLogError(@"ch.srgssr.logger-tests", @"Test", @"Error!");
    SRGLogWarning(@"ch.srgssr.logger-tests", @"Test", @"Warning!");
}

@end