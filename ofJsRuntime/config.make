# Project-level build configuration for ofJsRuntime.
# Picked up automatically by oF's compile.project.mk.

# Export all symbols from the executable so that chocons loaded via dlopen
# can resolve oF symbols (e.g. ofLog) at runtime. Linux only; no-op on macOS.
PROJECT_LDFLAGS = -rdynamic
