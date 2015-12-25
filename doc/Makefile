all:
	doxygen doxygen.cfg
	$(MAKE) -f sphinx.mk html

doxygen:
	doxygen doxygen.cfg

sphinx:
	$(MAKE) -f sphinx.mk html

clean:
	rm -rf xml
	$(MAKE) -f sphinx.mk clean
