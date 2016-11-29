BUILDDIR=../_build/test
$(shell mkdir -p $(BUILDDIR))
OCAMLDIR=$(shell ocamlopt -where)

CURDIR = $(shell pwd)

GTEST_DIR = $(CURDIR)/googletest/googletest

CC:= $(shell ocamlfind ocamlc -config | awk '/^bytecomp_c_compiler/ {for(i=2;i<=NF;i++) printf "%s " ,$$i}')
LD=$(CC)

all: $(BUILDDIR)/test
clean:
	rm -rf $(BUILDDIR)

.PHONY: $(BUILDDIR)/test.native
$(BUILDDIR)/test.native: $(BUILDDIR)/test.o
	$(LD) -isystem ${GTEST} -o $@ $< -L$(BUILDDIR)/.. -lrelayout libgtest.a -pthread

$(BUILDDIR)/%.o: %.cpp
	$(CC) -c -o $@ -I $(OCAMLDIR) -I ../stub $<

FLAGS = -fno-omit-frame-pointer -fexceptions -Wall -Werror -O3 -std=c++11

$(BUILDDIR)/test: CSSLayoutPaddingTest.cpp CSSLayoutMinMaxDimensionTest.cpp CSSLayoutMeasureTest.cpp CSSLayoutMeasureModeTest.cpp CSSLayoutMeasureCacheTest.cpp CSSLayoutMarginTest.cpp CSSLayoutJustifyContentTest.cpp CSSLayoutFlexWrapTest.cpp CSSLayoutFlexTest.cpp CSSLayoutFlexDirectionTest.cpp CSSLayoutEdgeTest.cpp CSSLayoutDirtyMarkingTest.cpp CSSLayoutDefaultValuesTest.cpp CSSLayoutBorderTest.cpp CSSLayoutAlignSelfTest.cpp CSSLayoutAlignItemsTest.cpp CSSLayoutAlignContentTest.cpp CSSLayoutAbsolutePositionTest.cpp
	g++ -o $@ -isystem ${GTEST_DIR}/include $(FLAGS) -pthread -I $(OCAMLDIR) -I ../stub libgtest.a -L$(BUILDDIR)/.. -lrelayout ${GTEST_DIR}/src/gtest_main.cc $^

gtest:
	rm -rf googletest
	git clone --branch release-1.8.0 --depth 1 https://github.com/google/googletest
	g++ -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

.PHONY: all clean gtest
