build:
	mkdir -p ./out
	g++ -O2 -o ./out/suffer -I ./include $(shell find ./src -name "*.cpp")

run:
	./out/suffer

install:
	@if [ "$$UID" -ne 0 ]; then \
		echo "Copying Suffer's binary to /usr/bin ..."; \
		sudo cp ./out/suffer /usr/bin; \
		mkdir -p $(HOME)/.suffer/cache; \
		mkdir -p $(HOME)/.suffer/libs; \
		cp ./data/known.json $(HOME)/.suffer/; \
		echo "Suffer has been installed!"; \
	else \
		echo "You gotta run \`make install\` without root perms, otherwise the installation is broken"; \
	fi