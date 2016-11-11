OCAMLDEP=ocamldep
BUILDDIR=_build
SRCDIR=src
CAML_INIT=$(BUILDDIR)/stub/init.o

EXTDLL:=.so

$(shell mkdir -p $(BUILDDIR) $(BUILDDIR)/stub $(BUILDDIR)/$(SRCDIR) $(BUILDDIR)/test)

$(shell nopam)

GENERATOR_FILES=$(BUILDDIR)/stub/bindings.re

# The files used to build the stub generator.
LIBFILES=$(BUILDDIR)/stub/bindings.cmx $(BUILDDIR)/stub/CSSLayout.o $(BUILDDIR)/stub/CSSNodeList.o

SOURCES := $(shell find $(SRCDIR) -name '*.re')

SOURCES_IN_BUILD := $(addprefix $(BUILDDIR)/,$(SOURCES))

MODULES=LayoutValue ReJsPrelude LayoutTypes LayoutSupport LayoutPrint Layout LayoutTestUtils

CMXS=$(addsuffix .cmx,$(MODULES))

CMXS_IN_BUILD=$(addprefix $(BUILDDIR)/src/,$(CMXS))

GENERATOR = $(BUILDDIR)/generate

PACKAGES=str

all: sharedlib

sharedlib: $(BUILDDIR)/librelayout$(EXTDLL)

$(BUILDDIR)/%.re: %.re
	cp $< $@

$(BUILDDIR)/%.ml: %.ml
	cp $< $@

$(BUILDDIR)/%.re: %.ml
	refmt -print re -parse ml $< > $@

%.cmx: %.re $(SOURCES_IN_BUILD)
	ocamlfind ocamlopt -w -40 -pp refmt -c -o $@ -I $(BUILDDIR)/src -package $(PACKAGES) -impl $<

$(BUILDDIR)/%.o: %.c
	ocamlopt -g -c $< -o $@
	mv $(notdir $@) $@

$(BUILDDIR)/librelayout$(EXTDLL): $(CAML_INIT) $(CMXS_IN_BUILD) $(LIBFILES)
	ocamlfind opt -o $@ -linkpkg -runtime-variant _pic -verbose -ccopt -dynamiclib -package $(PACKAGES) $^
	@echo "shared lib genereated at: $@"

clean:
	rm -rf $(BUILDDIR)

test: all
	$(MAKE) -C stub_test

depend: $(SOURCES_IN_BUILD) $(GENERATOR_FILES)
	$(OCAMLDEP) -I _build/src -I -pp refmt -ml-synonym .re -mli-synonym .rei $^ > .depend

include .depend
