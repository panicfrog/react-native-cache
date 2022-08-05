#import "Cache.h"
#import "react-native-cache.h"

#import <React/RCTBridge+Private.h>
#import <React/RCTUtils.h>
#import <jsi/jsi.h>

@implementation Cache

RCT_EXPORT_MODULE()

// @synthesize bridge = _bridge;
// @synthesize methodQueue = _methodQueue;

RCT_EXPORT_BLOCKING_SYNCHRONOUS_METHOD(install) {
  RCTBridge *bridge = [RCTBridge currentBridge];
  RCTCxxBridge *cxxBridge = (RCTCxxBridge *)bridge;
  if (cxxBridge == nil) {
    return @false;
  }
  using namespace facebook;
  
  auto jsiRuntime = (jsi::Runtime *)cxxBridge.runtime;
  if (jsiRuntime == nil) {
    return @false;
  }
  auto &runtime = *jsiRuntime;
  
 NSString *db_path = [NSString stringWithFormat:@"%@/jsi_cache.db", NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, true)[0]];
  const char *path = [db_path UTF8String];
  cache::install(runtime, path);
  
  return @true;
}

//RCT_EXPORT_METHOD(multiply:(nonnull NSNumber*)a withB:(nonnull NSNumber*)b
//                  withResolver:(RCTPromiseResolveBlock)resolve
//                  withReject:(RCTPromiseRejectBlock)reject)
//{
//    NSNumber *result = @(cache::multiply([a floatValue], [b floatValue]));
//
//    resolve(result);
//}
//
//+ (BOOL)requiresMainQueueSetup {
//    return YES;
//}
//
//- (void)setBridge:(RCTBridge *)bridge {
//  _bridge = bridge;
//  _setBridgeOnMainQueue = RCTIsMainQueue();
//
//  RCTCxxBridge *cxxBridge = (RCTCxxBridge *)self.bridge;
//  if (!cxxBridge.runtime) {
//    return;
//  }
//
// NSString *dir = [NSString stringWithFormat:@"%@/jsi_cache.db", NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, true)[0]];
//  const char *path = [dir UTF8String];
//  cache::install(*(facebook::jsi::Runtime *)cxxBridge.runtime, path);
//}

@end
