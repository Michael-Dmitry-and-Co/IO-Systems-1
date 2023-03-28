SRCDIR = src
BUILDDIR = build

obj-m += fan_driver.o

all: $(SRCDIR) $(BUILDDIR)
	cp $(SRCDIR)/*.c $(BUILDDIR)
	cd $(BUILDDIR)
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	cd ..

clean:
	rm -r $(BUILDDIR)

$(BUILDDIR):
	mkdir $(BUILDDIR)

$(SRCDIR):
	$(error Can not find sources dir)
