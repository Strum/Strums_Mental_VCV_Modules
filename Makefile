
SOURCES = $(wildcard src/*.cpp)

include ../../plugin.mk


dist: all
	mkdir -p dist/mental
	cp LICENSE* dist/mental/
	cp plugin.* dist/mental/
	cp -R res dist/mental/
