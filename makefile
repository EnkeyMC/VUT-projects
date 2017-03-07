HOST=merlin.fit.vutbr.cz
USER=xomach00
FILENAME=proj2
DIRECTORY=ITY_project_2
SSH_COMMAND="cd $(DIRECTORY) && make"

convert:
	scp $(FILENAME).tex $(USER)@$(HOST):~/$(DIRECTORY)/
	ssh $(USER)@$(HOST) $(SSH_COMMAND)
	scp $(USER)@$(HOST):~/$(DIRECTORY)/$(FILENAME).pdf ./$(FILENAME).pdf

