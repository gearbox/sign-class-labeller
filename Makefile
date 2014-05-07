.DEFAULT_GOAL := release

QPROJECT=sign-class-labeller.pro

release:
	qmake -o build.make $(QPROJECT)
	$(MAKE) -f build.make

.PHONY: clean

clean:
	@rm -f moc_* *.o sign-class-labeller build.make
