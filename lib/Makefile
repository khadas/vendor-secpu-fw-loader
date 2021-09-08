CC      = $(CROSS_COMPILE)gcc
LD      = $(CROSS_COMPILE)ld
AR      = $(CROSS_COMPILE)ar
NM      = $(CROSS_COMPILE)nm
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
READELF = $(CROSS_COMPILE)readelf

OBJS = tee_secpu_fw_loader.o

ifeq ($(ARCH), arm64)
TEEC_EXPORT=$(TDK_DIR)/ca_export_arm64
endif

ifeq ($(ARCH), arm)
TEEC_EXPORT=$(TDK_DIR)/ca_export_arm
endif

CFLAGS += -Wall -I../ta/include -I$(TEEC_EXPORT)/include -I./include
CFLAGS += -fPIC
#Add/link other required libraries here
LDADD += -L$(TEEC_EXPORT)/lib -lteec
LIB = libtee_secpu_fw_loader.so

.PHONY: all
all: $(LIB)

$(LIB): $(OBJS)
	mkdir -p $(O)
	$(CC) $(LDADD) -fPIC -shared -o $(O)/$@ $^

.PHONY: clean
clean:
	rm -rf $(O)

