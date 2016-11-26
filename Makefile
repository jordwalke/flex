OCAMLDEP=ocamldep
BUILDDIR=_build
SRCDIR=src
CAML_INIT=$(BUILDDIR)/stub/init.o

EXTDLL:=.o

$(shell mkdir -p $(BUILDDIR) $(BUILDDIR)/stub $(BUILDDIR)/$(SRCDIR) $(BUILDDIR)/test)

$(shell nopam)

GENERATOR_FILES=$(BUILDDIR)/stub/bindings.re

# The files used to build the stub generator.
LIBFILES=$(BUILDDIR)/stub/bindings.cmx $(BUILDDIR)/stub/CSSLayout.o

SOURCES := $(shell find $(SRCDIR) -name '*.re')

SOURCES_IN_BUILD := $(addprefix $(BUILDDIR)/,$(SOURCES))

MODULES=Spec HardCodedEncoding FixedEncoding FloatEncoding ReJsPrelude LayoutTypes LayoutSupport LayoutPrint Layout LayoutTestUtils

CMXS=$(addsuffix .cmx,$(MODULES))

CMXS_IN_BUILD=$(addprefix $(BUILDDIR)/src/,$(CMXS))

GENERATOR = $(BUILDDIR)/generate

PACKAGES=

TOOLCHAIN=

all: sharedlib

android: TOOLCHAIN=-toolchain android
android: $(BUILDDIR)/librelayout$(EXTDLL)

android64: $(BUILDDIR)/librelayout$(EXTDLL)

sharedlib: $(BUILDDIR)/librelayout.so

$(BUILDDIR)/%.re: %.re
	cp $< $@

$(BUILDDIR)/%.ml: %.ml
	cp $< $@

$(BUILDDIR)/%.re: %.ml
	refmt -print re -parse ml $< > $@

%.cmx: %.re $(SOURCES_IN_BUILD)
	ocamlfind $(TOOLCHAIN) opt -w -40 -pp refmt -c -o $@ -I $(BUILDDIR)/src  $(PACKAGES) -impl $<

$(BUILDDIR)/%.o: %.c
	ocamlfind $(TOOLCHAIN) opt -ccopt -std=c11 -g -c $< -o $@
	mv $(notdir $@) $@

$(BUILDDIR)/librelayout.so: $(CAML_INIT) $(CMXS_IN_BUILD) $(LIBFILES)
	ocamlfind opt -o $@ -linkpkg -runtime-variant _pic -verbose -ccopt -dynamiclib $(PACKAGES) $^
	# ocamlfind $(TOOLCHAIN) opt -o $(BUILDDIR)/librelayout$(EXTDLL) -linkpkg -output-complete-obj -linkall -runtime-variant _pic -verbose -output-obj $(PACKAGES) $^
	@echo "sharedlib genereated at: $@"

$(BUILDDIR)/librelayout$(EXTDLL): $(CAML_INIT) $(CMXS_IN_BUILD) $(LIBFILES)
	ocamlfind $(TOOLCHAIN) opt -o $@ -linkpkg -output-complete-obj -linkall -runtime-variant _pic -verbose -output-obj $(PACKAGES) $^
	@echo "lib genereated at: $@"

clean:
	rm -rf $(BUILDDIR)

test: all
	$(MAKE) -C stub_test

depend: $(SOURCES_IN_BUILD) $(GENERATOR_FILES)
	$(OCAMLDEP) -I _build/src -I -pp refmt -ml-synonym .re -mli-synonym .rei $^ > .depend

include .depend
