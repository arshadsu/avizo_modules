
# The configuration and architecture.
MAKE_CFG ?= Debug

uname_s := $(shell sh -c 'uname -s 2>/dev/null || echo unknown')
uname_m := $(shell sh -c 'uname -m 2>/dev/null || echo unknown')

ifeq ($(uname_s),Linux)
    ifeq ($(uname_m),x86_64)
        MAKE_UNAME = LinuxAMD64
    else
        ifeq ($(uname_m),i686)
            MAKE_UNAME = Linux
        else
            $(error Unknown Linux machine type '$(uname_m)'.)
        endif
    endif
else
    ifeq ($(uname_s),Darwin)
        MAKE_UNAME = MacX
    else
        $(error Unknown operating system '$(uname_s)'.)
    endif
endif

ARCH=arch-$(MAKE_UNAME)-$(MAKE_CFG)


# Canceling implicit rules
%.o : %.c
%.o : %.cpp
%.o : %.cc
%.o : %.C
%.o : %.p
%.o : %.r
%.o : %.f
%.o : %.F
%.sym : %.def
%.o : %.mod
%.o : %.s
%.s : %.S
% : %.o
%.c : %.y
%.c : %.l
%.r : %.l
%.nl : %c
%.dvi : %.tex
%.tex : %.web
%.tex : %.w
%.p : %.web
%.c : %.w
%.dvi : %.texinfo
%.dvi : %.texi
%.dvi : %.txinfo
%.info : %.texinfo
%.info : %.texi
%.info : %.txinfo



.PHONY: default
default: fast

.PHONY: all fast full
all fast full: alldevelop3
	@echo '    OK all'

.PHONY: pedantic
pedantic:
	@rm -f $(filter-out $(wildcard intermediate/$(ARCH)/*-docrun.lastbuild), $(wildcard intermediate/$(ARCH)/*.lastbuild))
	$(MAKE) all

.PHONY: myPickingSlice
myPickingSlice:
	$(MAKE) obj/$(ARCH)/myPickingSlice.lastbuild

obj/$(ARCH)/myPickingSlice.lastbuild: src/myPickingSlice/GNUmakefile src/myPickingSlice/api.h src/myPickingSlice/myPickingSlice.h src/myPickingSlice/share/resources/myPickingSlice.rc src/myPickingSlice/myPickingSlice.cpp src/myPickingSlice/version.cpp src/myPickingSlice/init.cpp
	$(MAKE) -C src/myPickingSlice
	@mkdir -p obj/$(ARCH)
	@touch obj/$(ARCH)/myPickingSlice.lastbuild
	@echo '    OK myPickingSlice'

.PHONY: docdevelop3
docdevelop3:
	$(MAKE) obj/$(ARCH)/docdevelop3.lastbuild

obj/$(ARCH)/docdevelop3.lastbuild: $(wildcard  src/myPickingSlice/doc)
	@$(MAKE) obj/$(ARCH)/docdevelop3-docrun.lastbuild
	@mkdir -p obj/$(ARCH)
	@touch obj/$(ARCH)/docdevelop3.lastbuild
	@echo '    OK docdevelop3'

obj/$(ARCH)/docdevelop3-docrun.lastbuild: $(wildcard  src/myPickingSlice/doc)
	mkdir -p product/share/doc/avizo
	/var/remote/software/avizo/current/bin/doc2html -A -skin AvizoSkin -local product -d product/share/doc/. -P src/myPickingSlice
	@mkdir -p obj/$(ARCH)
	@touch obj/$(ARCH)/docdevelop3-docrun.lastbuild

.PHONY: doc
doc: docdevelop3
	@echo '    OK doc'

.PHONY: alldevelop3
alldevelop3:
	$(MAKE) obj/$(ARCH)/alldevelop3.lastbuild

obj/$(ARCH)/alldevelop3.lastbuild: obj/$(ARCH)/myPickingSlice.lastbuild obj/$(ARCH)/docdevelop3.lastbuild
	@mkdir -p obj/$(ARCH)
	@touch obj/$(ARCH)/alldevelop3.lastbuild
	@echo '    OK alldevelop3'


.PHONY: clean
clean:
	for dir in src/myPickingSlice; do $(MAKE) -C $$dir clean ; done
	rm -rf product/share/doc

.PHONY: dep
dep: 
	for dir in src/myPickingSlice; do $(MAKE) -C $$dir dep ; done

.PHONY: depclean
depclean:
	for dir in src/myPickingSlice; do $(MAKE) -C $$dir depclean ; done

