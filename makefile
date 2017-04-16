HOST=merlin.fit.vutbr.cz
USER=xomach00
FILENAME=proj4
HOST_MAKE=makefile-merlin
DIRECTORY=ITY_project_4
SSH_COMMAND="cd $(DIRECTORY) && make"
SSH_COMMAND_CLEAN="cd $(DIRECTORY) && make clean"
EPS=$(wildcard *.eps)
BIB=$(wildcard *.bib)
BST=$(wildcard *.bst)
ZIP_NAME=xomach00-fit.zip

convert: copy-tex copy-bib copy-bst copy-makefile 
	ssh $(USER)@$(HOST) $(SSH_COMMAND)
	scp $(USER)@$(HOST):~/$(DIRECTORY)/$(FILENAME).pdf ./$(FILENAME).pdf

copy-tex: $(FILENAME).tex
	scp $< $(USER)@$(HOST):~/$(DIRECTORY)/

copy-eps:
	scp $(EPS) $(USER)@$(HOST):~/$(DIRECTORY)/

copy-bib:
	scp $(BIB) $(USER)@$(HOST):~/$(DIRECTORY)/

copy-bst:
	scp $(BST) $(USER)@$(HOST):~/$(DIRECTORY)/		

copy-makefile: $(HOST_MAKE)
	scp $< $(USER)@$(HOST):~/$(DIRECTORY)/makefile

clean-merlin:
	ssh $(USER)@$(HOST) $(SSH_COMMAND_CLEAN)

clean:
	rm -f $(FILENAME).dvi
	rm -f $(FILENAME).ps
	rm -f $(FILENAME).pdf
	rm -f $(FILENAME).log
	rm -f *.bbl
	rm -f *.blg

create-directory:
	ssh $(USER)@$(HOST) "mkdir $(DIRECTORY)"

pack: $(FILENAME).tex makefile $(EPS) $(BIB) $(BST)
	mv makefile makefile_
	mv makefile-odevzdani makefile
	zip $(ZIP_NAME) $+
	mv makefile makefile-odevzdani
	mv makefile_ makefile
