.PHONY: all tests install uninstall gcov_report dist dvi run linter vg clean_dvi clean clean_dist

all: install tests
	./bin/3dViewer

install:
	mkdir -p build
	cd build; cmake ..; make;
	mkdir -p bin
	mv build/3dViewer bin/
	mv build/model_test bin/

uninstall:
	rm -rf bin/ build/ settings.conf

tests:
	./bin/model_test

gcov_report:
	gcovr -r ./ --object-directory ./build --exclude 'sources/tests/.*' --exclude 'sources/s21_matrix*' --html --html-details -o build/coverage_report.html
	open build/coverage_report.html

dist:
	mkdir -p "3dViewer-2.0"
	cp -r ./*Make* ./doc ./include ./sources ./shaders 3dViewer-2.0/
	tar -zcf 3dViewer-2.0.tar.gz 3dViewer-2.0
	rm -rf 3dViewer-2.0

dvi:
	mkdir -p "dvi"
	doxygen Doxyfile
	cd dvi; make;

run:
	./bin/3dViewer

linter:
	find ./ -iname "*.h" -o -iname "*.cc" | xargs clang-format --style=google -n

vg:
	valgrind --quiet --gen-suppressions=yes ./bin/model_test

clean_dist:
	rm -f 3dViewer-2.0.tar.gz

clean_dvi:
	rm -rf dvi

clean: uninstall clean_dist clean_dvi