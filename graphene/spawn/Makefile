THIS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
APP_DIR ?= /root/graphene/Examples/spawn

ARCH_LIBDIR ?= /lib/$(shell $(CC) -dumpmachine)

SGX_SIGNER_KEY ?= ../../Pal/src/host/Linux-SGX/signer/enclave-key.pem

ifeq ($(DEBUG),1)
GRAPHENE_LOG_LEVEL = debug
else
GRAPHENE_LOG_LEVEL = error
endif

.PHONY: all
all: spawn.manifest spawn
ifeq ($(SGX),1)
all: spawn.manifest.sgx spawn.sig spawn.token spawn
endif

spawn.manifest: spawn.manifest.template
	graphene-manifest \
		-Dlog_level=$(GRAPHENE_LOG_LEVEL) \
		-Dhome=$(HOME) \
		-Darch_libdir=$(ARCH_LIBDIR) \
		-Dapp_dir=$(APP_DIR) \
		$< >$@

spawn.manifest.sgx: spawn.manifest
	@test -s $(SGX_SIGNER_KEY) || \
	    { echo "SGX signer private key was not found, please specify SGX_SIGNER_KEY!"; exit 1; }
	graphene-sgx-sign \
		--key $(SGX_SIGNER_KEY) \
		--manifest spawn.manifest \
		--output $@

spawn.sig: spawn.manifest.sgx

spawn.token: spawn.sig
	graphene-sgx-get-token --output $@ --sig $^

ifeq ($(SGX),)
GRAPHENE = graphene-direct
else
GRAPHENE = graphene-sgx
endif

spawn:
	gcc -O3 -fPIC spawn_victim.c -o spawn_victim
	gcc -O3 -fPIC spawn_child.c -o spawn

.PHONY: check
check: all
	$(GRAPHENE) ./spawn 100 0

.PHONY: clean
clean:
	$(RM) *.manifest *.manifest.sgx *.token *.sig OUTPUT spawn spawn_victim

.PHONY: distclean
distclean: clean
