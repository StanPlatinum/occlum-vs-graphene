THIS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
APP_DIR ?= /root/graphene/Examples/pipe

ARCH_LIBDIR ?= /lib/$(shell $(CC) -dumpmachine)

SGX_SIGNER_KEY ?= ../../Pal/src/host/Linux-SGX/signer/enclave-key.pem

ifeq ($(DEBUG),1)
GRAPHENE_LOG_LEVEL = debug
else
GRAPHENE_LOG_LEVEL = error
endif

.PHONY: all
all: pipe.manifest pipe
ifeq ($(SGX),1)
all: pipe.manifest.sgx pipe.sig pipe.token pipe
endif

pipe.manifest: pipe.manifest.template
	graphene-manifest \
		-Dlog_level=$(GRAPHENE_LOG_LEVEL) \
		-Dhome=$(HOME) \
		-Darch_libdir=$(ARCH_LIBDIR) \
		-Dapp_dir=$(APP_DIR) \
		$< >$@

pipe.manifest.sgx: pipe.manifest
	@test -s $(SGX_SIGNER_KEY) || \
	    { echo "SGX signer private key was not found, please specify SGX_SIGNER_KEY!"; exit 1; }
	graphene-sgx-sign \
		--key $(SGX_SIGNER_KEY) \
		--manifest pipe.manifest \
		--output $@

pipe.sig: pipe.manifest.sgx

pipe.token: pipe.sig
	graphene-sgx-get-token --output $@ --sig $^

ifeq ($(SGX),)
GRAPHENE = graphene-direct
else
GRAPHENE = graphene-sgx
endif

pipe:
	gcc -O3 -fPIC dev_null.c -o dev_null
	gcc -O3 -fPIC pipe_throughput.c -o pipe

.PHONY: check
check: all pipe
	$(GRAPHENE) ./pipe 64

.PHONY: clean
clean:
	$(RM) *.manifest *.manifest.sgx *.token *.sig OUTPUT pipe dev_null

.PHONY: distclean
distclean: clean
