BUILDDIR=../_build/test
$(shell mkdir -p $(BUILDDIR))
OCAMLDIR=$(shell ocamlopt -where)

CC:= $(shell ocamlfind ocamlc -config | awk '/^bytecomp_c_compiler/ {for(i=2;i<=NF;i++) printf "%s " ,$$i}')
LD=$(CC)

all: $(BUILDDIR)/test.native
clean:
	rm -rf $(BUILDDIR)

.PHONY: $(BUILDDIR)/test.native
$(BUILDDIR)/test.native: $(BUILDDIR)/test.o
	$(LD) -o $@ $< -L$(BUILDDIR)/.. -lrelayout

$(BUILDDIR)/test.o: test.c
	$(CC) -c -o $@ -I $(OCAMLDIR) -I ../stub $<


.PHONY: all clean
