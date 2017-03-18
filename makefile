FILE = proj2

all: $(FILE).ps
	ps2pdf $<

$(FILE).ps: $(FILE).dvi
	dvips -t a4 $<

$(FILE).dvi: $(FILE).tex
	latex $<

clear-all:
	rm $(FILE).dvi
	rm $(FILE).ps
	rm $(FILE).pdf
	rm $(FILE).log
