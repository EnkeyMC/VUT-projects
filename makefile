HOST=merlin.fit.vutbr.cz
USER=xomach00
FILENAME=proj5
HOST_MAKE=makefile-merlin
DIRECTORY=ITY_project_5
SSH_COMMAND="cd $(DIRECTORY) && make"
SSH_COMMAND_CLEAN="cd $(DIRECTORY) && make clean"
EPS=$(wildcard *.eps)
STY=$(wildcard *.sty)
ZIP_NAME=xomach00-fit.zip

convert: copy-tex copy-makefile copy-sty
	ssh $(USER)@$(HOST) $(SSH_COMMAND)
	scp $(USER)@$(HOST):~/$(DIRECTORY)/$(FILENAME).pdf ./$(FILENAME).pdf

copy-tex: $(FILENAME).tex
	scp $< $(USER)@$(HOST):~/$(DIRECTORY)/

copy-eps: $(EPS)
	scp $(EPS) $(USER)@$(HOST):~/$(DIRECTORY)/

copy-sty: $(STY)
	scp $^ $(USER)@$(HOST):~/$(DIRECTORY)/

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
