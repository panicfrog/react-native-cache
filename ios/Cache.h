#import <React/RCTBridgeModule.h>

#ifdef __cplusplus

#import "react-native-cache.h"

#endif

@interface Cache : NSObject <RCTBridgeModule>

@property (nonatomic, assign) BOOL setBridgeOnMainQueue;

@end
