HOST=merlin.fit.vutbr.cz
USER=xomach00
FILENAME=document
DIRECTORY=ITY_project_1
SSH_COMMAND="cd $(DIRECTORY) && make"

convert:
	scp $(FILENAME).tex $(USER)@$(HOST):~/$(DIRECTORY)/
	ssh $(USER)@$(HOST) $(SSH_COMMAND)
	scp $(USER)@$(HOST):~/$(DIRECTORY)/$(FILENAME).pdf ./$(FILENAME).pdf

