.DEFAULT_GOAL=ReleaseNoOF

# Builds a chocon (shared JS binding library) using the openFrameworks makefile
# infrastructure: addon parsing, include paths, compile rules — without linking
# libopenFrameworks.a. Undefined oF symbols are resolved at runtime by ofJsRuntime.

OF_SHARED_MAKEFILES_PATH=$(OF_ROOT)/libs/openFrameworksCompiled/project/makefileCommon

ifndef APPNAME
	APPNAME = $(shell basename "`pwd`")
endif

include $(OF_SHARED_MAKEFILES_PATH)/config.shared.mk

# choc headers (resolved after OF_ROOT is known)
OFXCHOC_ROOT ?= $(OF_ROOT)/addons/ofxChoc
PROJECT_CFLAGS += -I$(OFXCHOC_ROOT)/libs/choc/javascript -I$(OFXCHOC_ROOT)/libs/choc

ifeq ($(PLATFORM_OS),Darwin)
	CHOCON_EXT = dylib
	PROJECT_LDFLAGS += -dynamiclib -undefined dynamic_lookup
else
	CHOCON_EXT = so
	PROJECT_CFLAGS += -fPIC
	PROJECT_LDFLAGS += -shared -Wl,--allow-shlib-undefined
endif

ifndef PLATFORM_PROJECT_RELEASE_TARGET
	PLATFORM_PROJECT_RELEASE_TARGET = $(APPNAME).$(CHOCON_EXT)
endif
ifndef PLATFORM_PROJECT_DEBUG_TARGET
	PLATFORM_PROJECT_DEBUG_TARGET = $(APPNAME)_debug.$(CHOCON_EXT)
endif

# Name TARGET (output in project root, not bin/)
ifeq ($(findstring Debug,$(MAKECMDGOALS)),Debug)
	TARGET_NAME = Debug
	ifndef PLATFORM_PROJECT_DEBUG_TARGET
		TARGET = $(APPNAME)_debug.$(CHOCON_EXT)
	else
		TARGET = $(PLATFORM_PROJECT_DEBUG_TARGET)
	endif
	BIN_NAME = $(APPNAME)_debug
else ifeq ($(findstring Release,$(MAKECMDGOALS)),Release)
	TARGET_NAME = Release
	ifndef PLATFORM_PROJECT_RELEASE_TARGET
		TARGET = $(APPNAME).$(CHOCON_EXT)
	else
		TARGET = $(PLATFORM_PROJECT_RELEASE_TARGET)
	endif
	BIN_NAME = $(APPNAME)
else ifeq ($(MAKECMDGOALS),)
	TARGET_NAME = Release
	ifndef PLATFORM_PROJECT_RELEASE_TARGET
		TARGET = $(APPNAME).$(CHOCON_EXT)
	else
		TARGET = $(PLATFORM_PROJECT_RELEASE_TARGET)
	endif
	BIN_NAME = $(APPNAME)
endif

ABIS_TO_COMPILE =

ifeq ($(findstring Release,$(TARGET_NAME)),Release)
	ifdef ABIS_TO_COMPILE_RELEASE
		ABIS_TO_COMPILE += $(ABIS_TO_COMPILE_RELEASE)
	endif
endif

ifeq ($(findstring Debug,$(TARGET_NAME)),Debug)
	ifdef ABIS_TO_COMPILE_DEBUG
		ABIS_TO_COMPILE += $(ABIS_TO_COMPILE_DEBUG)
	endif
endif

ifeq ($(MAKECMDGOALS),clean)
	TARGET = $(APPNAME)_debug.$(CHOCON_EXT) $(APPNAME).$(CHOCON_EXT)
	TARGET_NAME = Release
endif

ifdef TARGET_NAME
	CLEANTARGET = $(addprefix Clean,$(TARGET_NAME))
endif

ifeq ($(findstring ABI,$(MAKECMDGOALS)),ABI)
	include $(OF_SHARED_MAKEFILES_PATH)/config.project.mk
	# chocons resolve oF symbols from the host at runtime — do not link oF
	OF_CORE_LIBS :=
	TARGET_LIBS :=
	OF_PROJECT_LIBS :=
	LDFLAGS := $(strip $(PROJECT_LDFLAGS))
	CFLAGS += -I$(OFXCHOC_ROOT)/libs/choc/javascript -I$(OFXCHOC_ROOT)/libs/choc
	-include $(OF_PROJECT_DEPENDENCY_FILES)
endif

.PHONY: all Debug Release ReleaseNoOF DebugNoOF after clean CleanDebug CleanRelease help force

JOBS = -j2

ReleaseNoOF:
	@echo Compiling chocon $(APPNAME) for Release
ifndef ABIS_TO_COMPILE_RELEASE
	@$(MAKE) $(JOBS) ReleaseABI
else
	@$(foreach abi,$(ABIS_TO_COMPILE_RELEASE),$(MAKE) $(JOBS) ReleaseABI ABI=$(abi) &&) echo
endif

DebugNoOF:
	@echo Compiling chocon $(APPNAME) for Debug
ifndef ABIS_TO_COMPILE_DEBUG
	@$(MAKE) $(JOBS) DebugABI
else
	@$(foreach abi,$(ABIS_TO_COMPILE_DEBUG),$(MAKE) DebugABI ABI=$(abi) &&) echo
endif

Release: ReleaseNoOF

Debug: DebugNoOF

ReleaseABI: $(TARGET)
	@$(PROJECT_AFTER)

DebugABI: $(TARGET)
	@$(PROJECT_AFTER)

all:
	$(MAKE) $(JOBS) ReleaseNoOF

$(OF_PROJECT_OBJ_OUTPUT_PATH).compiler_flags: force
	@mkdir -p $(OF_PROJECT_OBJ_OUTPUT_PATH)
	@if [ "$(strip $(OPTIMIZATION_CFLAGS) $(CFLAGS) $(CXXFLAGS) $(PROJECT_INCLUDE_CFLAGS) $(OPTIMIZATION_LDFLAGS) $(LDFLAGS))" != "$(strip $$(cat $@ 2>/dev/null))" ]; then echo $(strip $(OPTIMIZATION_CFLAGS) $(CFLAGS) $(CXXFLAGS) $(PROJECT_INCLUDE_CFLAGS) $(OPTIMIZATION_LDFLAGS) $(LDFLAGS)) > $@; fi

$(OF_ADDONS_PATH)/$(OF_PROJECT_OBJ_OUTPUT_PATH).compiler_flags: force
	@mkdir -p $(OF_PROJECT_OBJ_OUTPUT_PATH)
	@mkdir -p $(OF_ADDONS_PATH)/$(OF_PROJECT_OBJ_OUTPUT_PATH)
	@if [ "$(strip $(OPTIMIZATION_CFLAGS) $(CFLAGS) $(CXXFLAGS))" != "$(strip $$(cat $@ 2>/dev/null))" ]; then echo $(strip $(OPTIMIZATION_CFLAGS) $(CFLAGS) $(CXXFLAGS)) > $@; fi

$(OF_PROJECT_OBJ_OUTPUT_PATH)%.o: $(PROJECT_ROOT)/%.cpp $(OF_PROJECT_OBJ_OUTPUT_PATH).compiler_flags
	@echo "Compiling" $<
	@mkdir -p $(@D)
	$(CXX) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) $(CXXFLAGS) $(PROJECT_INCLUDE_CFLAGS) -MMD -MP -MF $(OF_PROJECT_OBJ_OUTPUT_PATH)$*.d -MT $(OF_PROJECT_OBJ_OUTPUT_PATH)$*.o -o $@ -c $<

$(OF_PROJECT_OBJ_OUTPUT_PATH)%.o: $(PROJECT_ROOT)/%.cxx $(OF_PROJECT_OBJ_OUTPUT_PATH).compiler_flags
	@echo "Compiling" $<
	@mkdir -p $(@D)
	$(CXX) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) $(CXXFLAGS) $(PROJECT_INCLUDE_CFLAGS) -MMD -MP -MF $(OF_PROJECT_OBJ_OUTPUT_PATH)$*.d -MT $(OF_PROJECT_OBJ_OUTPUT_PATH)$*.o -o $@ -c $<

$(OF_PROJECT_OBJ_OUTPUT_PATH)%.o: $(PROJECT_ROOT)/%.cc $(OF_PROJECT_OBJ_OUTPUT_PATH).compiler_flags
	@echo "Compiling" $<
	@mkdir -p $(@D)
	$(CXX) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) $(CXXFLAGS) $(PROJECT_INCLUDE_CFLAGS) -MMD -MP -MF $(OF_PROJECT_OBJ_OUTPUT_PATH)$*.d -MT $(OF_PROJECT_OBJ_OUTPUT_PATH)$*.o -o $@ -c $<

PROJECT_ADDONS_OBJ_PATH=./$(OF_PROJECT_OBJ_OUTPUT_PATH)addons/

$(OF_ADDONS_PATH)/$(OF_PROJECT_OBJ_OUTPUT_PATH)%.o: $(OF_ADDONS_PATH)/%.cpp $(OF_ADDONS_PATH)/$(OF_PROJECT_OBJ_OUTPUT_PATH).compiler_flags
	@echo "Compiling" $<
	@mkdir -p $(@D)
	$(CXX) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) $(CXXFLAGS) $(ADDON_INCLUDE_CFLAGS) -MMD -MP -MF $(OF_ADDONS_PATH)/$(OF_PROJECT_OBJ_OUTPUT_PATH)$*.d -MT $(OF_ADDONS_PATH)/$(OF_PROJECT_OBJ_OUTPUT_PATH)$*.o -o $@ -c $<

$(OF_ADDONS_PATH)/$(OF_PROJECT_OBJ_OUTPUT_PATH)%.o: $(OF_ADDONS_PATH)/%.cxx $(OF_ADDONS_PATH)/$(OF_PROJECT_OBJ_OUTPUT_PATH).compiler_flags
	@echo "Compiling" $<
	@mkdir -p $(@D)
	$(CXX) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) $(CXXFLAGS) $(ADDON_INCLUDE_CFLAGS) -MMD -MP -MF $(OF_ADDONS_PATH)/$(OF_PROJECT_OBJ_OUTPUT_PATH)$*.d -MT $(OF_ADDONS_PATH)/$(OF_PROJECT_OBJ_OUTPUT_PATH)$*.o -o $@ -c $<

$(OF_ADDONS_PATH)/$(OF_PROJECT_OBJ_OUTPUT_PATH)%.o: $(OF_ADDONS_PATH)/%.cc $(OF_ADDONS_PATH)/$(OF_PROJECT_OBJ_OUTPUT_PATH).compiler_flags
	@echo "Compiling" $<
	@mkdir -p $(@D)
	$(CXX) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) $(CXXFLAGS) $(ADDON_INCLUDE_CFLAGS) -MMD -MP -MF $(OF_ADDONS_PATH)/$(OF_PROJECT_OBJ_OUTPUT_PATH)$*.d -MT $(OF_ADDONS_PATH)/$(OF_PROJECT_OBJ_OUTPUT_PATH)$*.o -o $@ -c $<

$(TARGET): $(OF_PROJECT_OBJS) $(OF_PROJECT_ADDONS_OBJS) $(OF_PROJECT_OBJ_OUTPUT_PATH).compiler_flags
	@echo '🔗 Linking chocon $(TARGET) for $(ABI_LIB_SUBPATH)'
	@mkdir -p $(@D)
	$(CXX) -o $@ $(OPTIMIZATION_LDFLAGS) $(OF_PROJECT_OBJS) $(OF_PROJECT_ADDONS_OBJS) $(LDFLAGS)

clean:
	@$(MAKE) CleanRelease

$(CLEANTARGET)ABI:
ifneq ($(OF_PROJECT_ADDONS_OBJS),)
	rm -f $(OF_PROJECT_ADDONS_OBJS)
endif
	rm -rf $(OF_PROJECT_OBJ_OUTPUT_PATH)
	rm -f $(TARGET)

$(CLEANTARGET):
ifndef ABIS_TO_COMPILE
	@$(MAKE) $(CLEANTARGET)ABI
else
ifeq ($(TARGET_NAME),Debug)
	@$(foreach abi,$(ABIS_TO_COMPILE_DEBUG),$(MAKE) $(CLEANTARGET)ABI ABI=$(abi) &&) echo done
else
	@$(foreach abi,$(ABIS_TO_COMPILE_RELEASE),$(MAKE) $(CLEANTARGET)ABI ABI=$(abi) &&) echo done
endif
endif

after: $(TARGET_NAME)
	@echo
	@echo "     chocon built: $(TARGET)"
	@echo "     drop into chocons/ next to your script or the runtime data/chocons/ folder"
	@echo

help:
	@echo
	@echo ofxChoc chocon makefile
	@echo
	@echo "make / make ReleaseNoOF   build the shared library (default)"
	@echo "make clean              remove build artifacts"
	@echo
	@echo "List oF addons in addons.make; put binding code in src/"
	@echo

force:
