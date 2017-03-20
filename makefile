HOST=merlin.fit.vutbr.cz
USER=xomach00
FILENAME=proj2
HOST_MAKE=makefile-merlin
DIRECTORY=ITY_project_2
SSH_COMMAND="cd $(DIRECTORY) && make"
SSH_COMMAND_CLEAN="cd $(DIRECTORY) && make clean"

convert: copy-tex copy-makefile 
	ssh $(USER)@$(HOST) $(SSH_COMMAND)
	scp $(USER)@$(HOST):~/$(DIRECTORY)/$(FILENAME).pdf ./$(FILENAME).pdf

copy-tex: $(FILENAME).tex
	scp $< $(USER)@$(HOST):~/$(DIRECTORY)/

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
