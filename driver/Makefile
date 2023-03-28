SRCDIR = src
BUILDDIR = build

obj-m += $(BUILDDIR)/fan_driver.o

all: $(SRCDIR) $(BUILDDIR)
	cp $(SRCDIR)/*.c $(BUILDDIR)
	cd $(BUILDDIR)
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	cd ..
	rm .Module.symvers.cmd     \
       .fan_driver.ko.cmd      \
       .fan_driver.mod.cmd     \
       .fan_driver.mod.o.cmd   \
       .fan_driver.o.cmd       \
       .modules.order.cmd

clean:
	rm -r $(BUILDDIR)

$(BUILDDIR):
	mkdir $(BUILDDIR)

$(SRCDIR):
	$(error Can not find sources dir)
