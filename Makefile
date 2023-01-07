all:
	make build

run:build
	cd build && ./ray-tracing $(mode)

rebuild:
	make clean
	make build

build:
	d=$$(date +%s) && \
	cmake -S ./src -B ./build && \
	cmake --build build && \
	echo "==================================\n \
	Build took $$(($$(date +%s)-d)) seconds"

clean:
	rm -rf build
