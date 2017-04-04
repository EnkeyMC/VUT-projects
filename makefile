HOST=merlin.fit.vutbr.cz
USER=xomach00
FILENAME=proj4
HOST_MAKE=makefile-merlin
DIRECTORY=ITY_project_4
SSH_COMMAND="cd $(DIRECTORY) && make"
SSH_COMMAND_CLEAN="cd $(DIRECTORY) && make clean"
EPS=$(wildcard *.eps)
ZIP_NAME=xomach00-fit.zip

convert: copy-tex copy-eps copy-makefile 
	ssh $(USER)@$(HOST) $(SSH_COMMAND)
	scp $(USER)@$(HOST):~/$(DIRECTORY)/$(FILENAME).pdf ./$(FILENAME).pdf

copy-tex: $(FILENAME).tex
	scp $< $(USER)@$(HOST):~/$(DIRECTORY)/

copy-eps:
	scp $(EPS) $(USER)@$(HOST):~/$(DIRECTORY)/

copy-makefile: $(HOST_MAKE)
	scp $< $(USER)@$(HOST):~/$(DIRECTORY)/makefile

clean-merlin:
	ssh $(USER)@$(HOST) $(SSH_COMMAND_CLEAN)

clean:
	rm $(FILENAME).dvi
	rm $(FILENAME).ps
	rm $(FILENAME).pdf
	rm $(FILENAME).log

create-directory:
	ssh $(USER)@$(HOST) "mkdir $(DIRECTORY)"

pack: $(FILENAME).tex makefile $(EPS)
	mv makefile makefile_
	mv makefile-odevzdani makefile
	zip $(ZIP_NAME) $+
	mv makefile makefile-odevzdani
	mv makefile_ makefile
