#************************************************
#*                                              *
#*             (c) 2019 J. FABRIZIO             *
#*                                              *
#*                               LRDE EPITA     *
#*                                              *
#************************************************

CC = g++

CPP_FILES = src/utils.cc src/camera.cc src/input.cc src/program.cc src/obj.cc
CPP_FILES +=
HXX_FILES = src/utils.hh src/camera.hh src/input.hh src/program.hh objects/bunny.hh src/obj.hh
HXX_FILES +=
OBJ_FILES = $(CPP_FILES:.cc=.o)

CXX_FLAGS += -Wall -Wextra -O3 -g -std=c++11
CXX_FLAGS +=
CXX_FLAGS += -m64 -march=native
CXX_FLAGS += -fopt-info-vec-optimized 
LDXX_FLAGS = -lGL  -lGLEW -lglut -lpthread -lglfw -lX11 -lXrandr -lrt -lm

MAIN_FILE = main.cc
DIST = grass_simulation

SKEL_DIST_DIR = pogl_skel_tp
SKEL_FILES = $(CPP_FILES) $(HXX_FILES) $(MAIN_FILE) Makefile vertex.shd fragment.shd


#For gcc 4.9
export GCC_COLORS=1

define color
    if test -n "${TERM}" ; then\
	if test `tput colors` -gt 0 ; then \
	    tput setaf $(1); \
        fi;\
    fi
endef

define default_color
    if test -n "${TERM}" ; then\
	if test `tput colors` -gt 0 ; then  tput sgr0 ; fi; \
    fi
endef


all: post-build

pre-build:
	@$(call color,4)
	@echo "******** Starting Compilation ************"
	@$(call default_color)

post-build:
	@make --no-print-directory main-build ; \
	sta=$$?;	  \
	$(call color,4); \
	echo "*********** End Compilation **************"; \
	$(call default_color); \
	exit $$sta;

main-build: pre-build build

build: $(OBJ_FILES)
	$(CC) $(MAIN_FILE) -o $(DIST) $(OBJ_FILES) $(CXX_FLAGS) $(LDXX_FLAGS)


%.o: %.cc %.hh
	@$(call color,2)
	@echo "[$@] $(CXX_FLAGS)"
	@$(call default_color)
	@$(CC) -c -o $@ $< $(CXX_FLAGS) ; \
	sta=$$?;	  \
	if [ $$sta -eq 0 ]; then  \
	  $(call color,2) ; \
	  echo "[$@ succes]" ; \
	  $(call default_color) ; \
	else  \
	  $(call color,1) ; \
	  echo "[$@ failure]" ; \
	  $(call default_color) ; \
	fi ;\
	exit $$sta

.PHONY: all clean pre-build post-build main-build build skel

clean:
	rm -f $(OBJ_FILES)
	rm -f $(DIST)
	rm -rf $(SKEL_DIST_DIR).tar.bz2


skel:
	rm -rf $(SKEL_DIST_DIR)
	mkdir $(SKEL_DIST_DIR)
	cp $(SKEL_FILES) $(SKEL_DIST_DIR)
	tar -cjvf $(SKEL_DIST_DIR).tar.bz2 $(SKEL_DIST_DIR)
	rm -rf $(SKEL_DIST_DIR)
