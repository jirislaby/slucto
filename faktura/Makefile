TEX=latex
PROJ=faktura

all: dvi

dvi: $(PROJ).dvi
pdf: $(PROJ).pdf
ps: $(PROJ).ps

$(PROJ).dvi: $(PROJ).tex FORCE

%.pdf: %.tex FORCE
	pdflatex $<

%.ps: %.tex FORCE
	pslatex $<
	dvips $(subst .tex$,.dvi,$<) -o$@
	rm $(subst .tex$,.dvi,$<)

clean:
	rm -f $(PROJ).{pdf,dvi,toc,aux,log,bbl,blg,ps,out}

FORCE:

.PHONY: FORCE
