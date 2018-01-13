OCAMLDEP=ocamldep
BUILDDIR=_build
SRCDIR=src
CAML_INIT=$(BUILDDIR)/stub/init.o

EXTDLL:=.o

$(shell mkdir -p $(BUILDDIR) $(BUILDDIR)/stub $(BUILDDIR)/$(SRCDIR) $(BUILDDIR)/test)

$(shell nopam)

GENERATOR_FILES=$(BUILDDIR)/stub/bindings.re

# The files used to build the stub generator.
LIBFILES=$(BUILDDIR)/stub/bindings.cmx $(BUILDDIR)/stub/Yoga.o

SOURCES := $(shell find $(SRCDIR) -name '*.re')

SOURCES_IN_BUILD := $(addprefix $(BUILDDIR)/,$(SOURCES))

MODULES=Spec HardCodedEncoding FixedEncoding FloatEncoding ReJsPrelude LayoutTypes LayoutSupport LayoutPrint Layout LayoutTestUtils

CMXS=$(addsuffix .cmx,$(MODULES))

CMXS_IN_BUILD=$(addprefix $(BUILDDIR)/src/,$(CMXS))

GENERATOR = $(BUILDDIR)/generate

PACKAGES=

TOOLCHAIN=

ARMV7BIN=

OCAMLFLAGS=-noassert -inline 100

EXTRAOCAMLCCOPTFLAGS=

all: sharedlib

android: TOOLCHAIN=-toolchain android
android: $(BUILDDIR)/librelayout$(EXTDLL)

android64: $(BUILDDIR)/librelayout$(EXTDLL)

sharedlib: $(CAML_INIT) $(CMXS_IN_BUILD) $(LIBFILES)
	ocamlfind opt $(OCAMLFIND) -o $(BUILDDIR)/librelayout.so -thread unix.cmxa threads.cmxa -linkpkg -runtime-variant _pic -verbose -ccopt -dynamiclib $(EXTRAOCAMLCCOPTFLAGS) $(PACKAGES) $^
	@echo "sharedlib genereated at: $@"

$(BUILDDIR)/%.re: %.re
	mkdir -p $(dir $@)
	cp $< $@

$(BUILDDIR)/%.ml: %.ml
	mkdir -p $(dir $@)
	cp $< $@

$(BUILDDIR)/%.re: %.ml
	refmt -print re -parse ml $< > $@

%.cmx: %.re $(SOURCES_IN_BUILD)
	ocamlfind $(TOOLCHAIN) opt $(OCAMLFLAGS) $(EXTRAOCAMLCCOPTFLAGS) -ccopt -fPIC -thread -w -40 -pp 'refmt --print binary' -c -o $@ -I $(BUILDDIR)/src  $(PACKAGES) -impl $<

$(BUILDDIR)/%.o: %.c
	ocamlfind $(TOOLCHAIN) opt $(OCAMLFLAGS) $(EXTRAOCAMLCCOPTFLAGS) -ccopt -fPIC -ccopt -std=c11 -c $< -o $@
	mkdir -p $(dir $@)
	mv $(notdir $@) $@

$(BUILDDIR)/librelayout.o: $(CAML_INIT) $(CMXS_IN_BUILD) $(LIBFILES)
	ocamlfind $(TOOLCHAIN) opt $(OCAMLFLAGS) $(EXTRAOCAMLCCOPTFLAGS) -thread unix.cmxa threads.cmxa -ccopt -fno-omit-frame-pointer -ccopt -fPIC -ccopt -O3 -o $(BUILDDIR)/librelayout.o -linkpkg -output-complete-obj -linkall -runtime-variant _pic -output-obj -verbose $(PACKAGES) $^

android-armv7: clean android
	$(ANDROID_NDK)/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-ar rc $(BUILDDIR)/librelayout.a $(BUILDDIR)/librelayout.o
	$(ANDROID_NDK)/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-ranlib $(BUILDDIR)/librelayout.a
	@echo "lib genereated at: $(BUILDDIR)/librelayout.a"
	@echo "lib type:"
	@file $(BUILDDIR)/librelayout.a

android-x86: clean android
	$(ANDROID_NDK)/toolchains/x86-4.9/prebuilt/linux-x86_64/bin/i686-linux-android-ar rc $(BUILDDIR)/librelayout.a $(BUILDDIR)/librelayout.o
	$(ANDROID_NDK)/toolchains/x86-4.9/prebuilt/linux-x86_64/bin/i686-linux-android-ranlib $(BUILDDIR)/librelayout.a
	@echo "lib genereated at: $(BUILDDIR)/librelayout.a"
	@echo "lib type:"
	@file $(BUILDDIR)/librelayout.a

clean:
	rm -rf $(BUILDDIR)

test: all
	$(MAKE) -C stub_test

testperf: OCAMLFLAGS=-nodynlink -noassert -inline 100 -S -g -p
testperf: EXTRAOCAMLCCOPTFLAGS=-ccopt -g -ccopt -pg
testperf: all
	$(MAKE) testperf -C stub_test

depend: $(SOURCES_IN_BUILD) $(GENERATOR_FILES)
	$(OCAMLDEP) -I _build/src -I -pp 'refmt --print binary' -ml-synonym .re -mli-synonym .rei $^ > .depend

include .depend
