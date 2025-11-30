git clone myLib

cd ./myLib && cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cd ./myLib && cmake --build build --config Release

mkdir -p ./project/lib ./project/include ./project/out 

cp build/libmylib.a ~/project/lib/
cp -r include/* ~/project/include/

cd ~/project

g++ -o ./out/program -I./include $(find ./src -name "*.cpp") -L./lib -lmylib