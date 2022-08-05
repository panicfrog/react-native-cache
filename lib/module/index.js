import { NativeModules, Platform } from 'react-native';
const LINKING_ERROR = `The package 'react-native-cache' doesn't seem to be linked. Make sure: \n\n` + Platform.select({
  ios: "- You have run 'pod install'\n",
  default: ''
}) + '- You rebuilt the app after installing the package\n' + '- You are not using Expo managed workflow\n';
const Cache = NativeModules.Cache ? NativeModules.Cache : new Proxy({}, {
  get() {
    throw new Error(LINKING_ERROR);
  }

});

if (Cache) {
  if (typeof Cache.install === 'function') {
    Cache.install();
  }
}

export function get(key) {
  return global.getValueForKey(key);
}
export function set(key, value) {
  return global.setValueForKey(key, value);
}
//# sourceMappingURL=index.js.map