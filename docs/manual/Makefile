all:
	rm -Rf out/
	mkdir -p out/
	cd v0.20/ && mkdocs build --clean && mkdocs2pandoc > ceu-sdl-v0.20.md && pandoc ceu-sdl-v0.20.md -o ceu-sdl-v0.20.pdf && mv site/ ../out/v0.20/
