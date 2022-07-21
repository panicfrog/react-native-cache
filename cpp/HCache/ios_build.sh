rm -rf output 
rm -rf build
rm -rf tem

mkdir -p build/devices
mkdir -p build/simulator

sims=("SIMULATOR" "SIMULATOR64" "SIMULATORARM64")

for SIM in ${sims[@]}
do 
P="build/simulator/${SIM}"
mkdir -p "${P}"
done

mkdir tem && cd tem

cmake .. -G Xcode \
-DCMAKE_TOOLCHAIN_FILE=../thirdparty/ios-cmake/ios.toolchain.cmake \
-DPLATFORM=OS \
-DDEPLOYMENT_TARGET=10.0

cmake --build . --config Release

cp Release-iphoneos/* ../build/devices/
cp thirdparty/SQLiteCpp/Release-iphoneos/* ../build/devices/
cp thirdparty/SQLiteCpp/sqlite3/Release-iphoneos/* ../build/devices/


for SIM in ${sims[@]}
do 
P="../build/simulator/${SIM}"
rm -rf ./*

cmake .. -G Xcode \
-DCMAKE_TOOLCHAIN_FILE=../thirdparty/ios-cmake/ios.toolchain.cmake \
-DPLATFORM=${SIM} \
-DDEPLOYMENT_TARGET=10.0

cmake --build . --config Release

cp Release-iphonesimulator/* ${P}/
cp thirdparty/SQLiteCpp/Release-iphonesimulator/* ${P}/
cp thirdparty/SQLiteCpp/sqlite3/Release-iphonesimulator/* ${P}/
done

libs=("libHCache.a" "libSQLiteCpp.a" "libsqlite3.a")

for LIB in ${libs[@]} 
do 
lipo -create ../build/simulator/SIMULATOR/${LIB} \
  ../build/simulator/SIMULATOR64/${LIB} \
  ../build/simulator/SIMULATORARM64/${LIB} \
  -output ../build/simulator/${LIB}
done

for SIM in ${sims[@]}
do
rm -rf ../build/simulator/${SIM}
done

# merge static library
mkdir -p ../build/merge/simulator
mkdir -p ../build/merge/devices

libtool -static ../build/simulator/*.a -o ../build/merge/simulator/libHCache.a
libtool -static ../build/devices/*.a -o ../build/merge/devices/libHCache.a

xcodebuild -create-xcframework \
  -library ../build/merge/simulator/libHCache.a \
  -headers ../include \
  -library ../build/merge/devices/libHCache.a \
  -headers ../include \
  -output ../build/HCache.xcframework

# clean 
rm -rf ../build/devices
rm -rf ../build/merge
rm -rf ../build/simulator
rm -rf ../tem

