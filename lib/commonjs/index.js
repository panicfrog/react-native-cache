"use strict";

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports.get = get;
exports.set = set;

var _reactNative = require("react-native");

const LINKING_ERROR = `The package 'react-native-cache' doesn't seem to be linked. Make sure: \n\n` + _reactNative.Platform.select({
  ios: "- You have run 'pod install'\n",
  default: ''
}) + '- You rebuilt the app after installing the package\n' + '- You are not using Expo managed workflow\n';
const Cache = _reactNative.NativeModules.Cache ? _reactNative.NativeModules.Cache : new Proxy({}, {
  get() {
    throw new Error(LINKING_ERROR);
  }

});

if (Cache) {
  if (typeof Cache.install === 'function') {
    Cache.install();
  }
}

function get(key) {
  return global.getValueForKey(key);
}

function set(key, value) {
  return global.setValueForKey(key, value);
}
//# sourceMappingURL=index.js.map