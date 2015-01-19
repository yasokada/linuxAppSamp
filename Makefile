ifndef PETALINUX
$(error "Error: PETALINUX environment variable not set.  Change to the root of your PetaLinux install, and source the settings.sh file")
endif

include apps.common.mk

APP = myapp

# Add any other object files to this list below
#
#APP_OBJS = myapp.o 
#for zynq+linux demo (150107)
APP_OBJS = myapp.o func_gpio.o func_term.o func_file.o socket_util.o func_ipaddrs.o

## 141007 added to avoid optimization
## http://tidsreise.com/div/Xilinx_undervisning/embedded_linux_xupv5_lab_manual.pdf
#CFLAGS += -O0

all: build install

build: $(APP)

$(APP): $(APP_OBJS)
	$(CC) $(LDFLAGS) -lpthread -lrt -o $@ $(APP_OBJS) $(LDLIBS)

clean:
	-rm -f $(APP) *.elf *.gdb *.o

.PHONY: install image

install: $(APP)
#added for auto-startup
#	$(TARGETINST) -d -p 0755 mystartup /etc/init.d/mystartup
#	$(TARGETINST) -s /etc/init.d/mystartup /etc/rc5.d/S99mystartup

	$(TARGETINST) -d $(APP) /bin/$(APP)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

help:
	@echo ""
	@echo "Quick reference for various supported build targets for $(INSTANCE)."
	@echo "----------------------------------------------------"
	@echo "  clean                  clean out build objects"
	@echo "  all                    build $(INSTANCE) and install to rootfs host copy"
	@echo "  build                  build subsystem"
	@echo "  install                install built objects to rootfs host copy"
