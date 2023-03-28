SRCDIR = src
BUILDDIR = build

$(BUILDDIR)/user: $(BUILDDIR) $(SRCDIR)/user.c
	gcc $(SRCDIR)/user.c -o $(BUILDDIR)/user

clean:
	rm $(BUILDDIR)/*

$(BUILDDIR):
	mkdir $(BUILDDIR)

$(SRCDIR):
	$(error Can not find sources dir)
