build:
	g++ -o ./out/program -I ./include $(shell find ./src -name "*.cpp" -o -name "*.cc" -o -name "*.cxx" -o -name "*.c++" -o -name "*.C") -L ./lib -lcpr -lz -lpthread -lssl -lcurl -lcrypto
run:
	./out/program
