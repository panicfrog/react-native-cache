import { NativeModules, Platform } from 'react-native';

declare interface Global {
  jsimultiply(a: number, b: number): number
  getValueForKey(key: string): boolean | number | string | undefined
  setValueForKey(key: string, value: boolean | number | string): undefined
}
declare const global: Global

const LINKING_ERROR =
  `The package 'react-native-cache' doesn't seem to be linked. Make sure: \n\n` +
  Platform.select({ ios: "- You have run 'pod install'\n", default: '' }) +
  '- You rebuilt the app after installing the package\n' +
  '- You are not using Expo managed workflow\n';

const Cache = NativeModules.Cache
  ? NativeModules.Cache
  : new Proxy(
      {},
      {
        get() {
          throw new Error(LINKING_ERROR);
        },
      }
    );

export function multiply(a: number, b: number): Promise<number> {
  return Cache.multiply(a, b);
}

export function jsimultiply(a: number, b: number): number {
  return global.jsimultiply(a, b)
}

export function get(key: string): boolean | number | string | undefined {
  return global.getValueForKey(key)
}

export function set(key: string, value: boolean | number | string ): undefined {
  return global.setValueForKey(key, value)
}
