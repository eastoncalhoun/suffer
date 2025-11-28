build:
	g++ -o ./out/suffer -I ./include $(shell find ./src -name "*.cpp")

run:
	./out/suffer

install:
	sudo cp ./out/suffer /bin