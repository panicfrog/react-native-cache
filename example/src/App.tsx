import * as React from 'react';

import { StyleSheet, View, Text, TextInput, Button } from 'react-native';
import { get, set } from 'react-native-cache';

export default function App() {

  const [setKey, onSetKeyChanged] = React.useState('')
  const [value, onValueChanged] = React.useState('')
  const [getKey, onGetKeyChanged] = React.useState('')

  const [kvResult, setKVResult] = React.useState('')

  React.useEffect(() => {
  }, []);

  const onSetPress = () => {
    let start = performance.now()
    set(setKey, value)
    let end = performance.now()
    console.log(`set value: ${end - start}ms`)
  }

  const onGetPress = () => {
    let start = performance.now()
    let v = get(getKey)
    let end = performance.now()
    setKVResult(v)
    console.log(`get value: ${end - start}ms`)
  }

  return (
    <View style={styles.container}>
      <View style={{height: 20}}/>
      <View style={styles.row}>
        <TextInput 
          style={styles.textInput} 
          onChangeText={onSetKeyChanged} 
          placeholder={"key"} />
        <View style={{width: 15}}/>
        <TextInput 
          style={styles.textInput} 
          onChangeText={onValueChanged} 
          placeholder={"value"} />
        <Button style={{width: 35}} onPress={onSetPress} title={'set'} />
      </View>
      <View style={{height: 20}}/>
      <View style={styles.row}>
        <TextInput 
          style={styles.textInput} 
          onChangeText={onGetKeyChanged} 
          placeholder={"key"} />
        <View style={{width: 15}}/>
        <Text style={{flex: 1}} >{kvResult}</Text>
        <Button style={{width: 35}} onPress={onGetPress} title={'get'} />
      </View>
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
  textInput: {
    flex: 1, 
    borderWidth: 1, 
    paddingHorizontal: 5, 
    height: 35,
    borderRadius: 5
  },
  row: {
    flexDirection: 'row', 
    justifyContent: 'center', 
    alignItems: 'center', 
    width: '100%', 
    paddingHorizontal: 15
  }
});
