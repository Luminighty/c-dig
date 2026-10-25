.PHONY: debug release clean

TARGET_OS=Linux
ifeq ($(OS),Windows_NT)
	TARGET_OS = Windows
else
	TARGET_OS = $(shell uname -s)
endif

debug:
	@echo "The detected OS is: $(TARGET_OS)"
	lua ./luamake/make.lua $(TARGET_OS) debug

release:
	lua ./luamake/make.lua $(TARGET_OS) release

clean:
	rm -rf build bin

