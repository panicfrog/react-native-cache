import * as React from 'react';

import { StyleSheet, View, Text } from 'react-native';
import { multiply, jsimultiply } from 'react-native-cache';

export default function App() {
  const [result, setResult] = React.useState<number | undefined>();
  const [jsiresult, setJSIResult] = React.useState<number | undefined>();

  React.useEffect(() => {
    let start = performance.now()
    multiply(3, 7).then((res) => {
      let end = performance.now()
      console.log(`非JSI调用耗时: ${end-start} ms`)
      setResult(res)
    });
    let jsiStart = performance.now()
    let jsiRes = jsimultiply(3, 7)
    let jsiEnd = performance.now()
    console.log(`JSI调用耗时: ${jsiEnd-jsiStart} ms`)
    setJSIResult(jsiRes)
  }, []);

  return (
    <View style={styles.container}>
      <Text>Result: {result}</Text>
      <Text>JSI Result: {jsiresult}</Text>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
  },
  box: {
    width: 60,
    height: 60,
    marginVertical: 20,
  },
});
