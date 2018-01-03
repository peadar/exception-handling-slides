COMMFLAGS = -Os -static -fno-stack-protector -Wall -pedantic
CXXFLAGS += $(COMMFLAGS) -std=c++11
CFLAGS += $(COMMFLAGS)

LDFLAGS +=
.SUFFIXES: .es .ec .ecc .eh

TARGETS = htmlfilt foo.es foo32 foo32.es sjlj foo.dwarftables foo foo.ecc sjljeg.ec sjljbasic.ec foo.es foo.ecc sjljbasic.ec sjljeg.ec foo-explicit.ec foo.freebsd4.es foo32.s foo.s __cxa_throw.ecc libgcc/unwind-dw2.ec foo-g.ecc sjlj.eh sjlj.ec foo-g.es foo-h.es foo-f.es eh_personality.ecc

all: $(TARGETS)

htmlfilt: htmlfilt.c
	$(CC) -o $@ $<

foo.cc: foo-hdr.cc foo-h.cc foo-g.cc foo-f.cc foo-main.cc
	cat foo-hdr.cc foo-h.cc foo-g.cc foo-f.cc foo-main.cc > foo.cc

foo: foo.o
	c++ -o $@ $(CXXFLAGS) $^

foo-g.s: foo.s
	./extract-func-asm _Z1gv < $^ > $@

foo-f.s: foo.s
	./extract-func-asm _Z1fv < $^ > $@

foo-h.s: foo.s
	./extract-func-asm _Z1hv < $^ > $@



foo32: foo32.o
	c++ -m32 -o $@ $(CXXFLAGS) $^

foo.s: foo.cc
	c++ -S $(CXXFLAGS) -o $@ $^

foo32.s: foo.cc
	c++ -S -m32 $(CXXFLAGS) -o $@ $^

foo32.o: foo.cc
	c++ -c -m32 $(CXXFLAGS) -o $@ $^


foo.dwarftables: foo
	( nm foo.o && readelf --debug-dump=frames foo.o ) | c++filt | ./htmlfilt > $@


sjlj: sjljeg.o sjlj.o
	cc -o $@ $^

clean:
	rm -f *.o $(TARGETS) *.ecc *.ec *.eh *.es foo.cc foo-[fgh].s

.cc.ecc:
	c++filt < $^ | ./htmlfilt > $@

.c.ec:
	c++filt < $^ | ./htmlfilt > $@
.h.eh:
	c++filt < $^ | ./htmlfilt > $@
.s.es:
	c++filt < $^ | ./htmlfilt > $@
