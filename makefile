CC = icc
CFLAGS = -Wall -std=c99 -lrt -D_XOPEN_SOURCE
CTARGET=happy
TEXSOURCE = assign4

default: pdf th mp cleanup

mp:
	${CC} ${CFLAGS} -o happy_mp happy_mp.c -lm

th: 
	${CC} -pthread ${CFLAGS} -o happy_th ${LDFLAGS} happy_th.c 

dvi: ${TEXSOURCE}.tex 
#	pygmentize the input source file -- THIS NAME SHOULD BE SAFE
#	pygmentize -f latex -o __${TARGET}.tex ${TARGET}.tex
#	run latex twice to get references correct
	latex ${TEXSOURCE}.tex
#	you can also have a bibtex line here
#	bibtex $(TARGET)
	latex $(TEXSOURCE).tex
#	remove the pygmentized output to avoid cluttering up the directory
#	rm __${TARGET}.tex

ps: dvi
	dvips -R -Poutline -t letter ${TEXSOURCE}.dvi -o ${TEXSOURCE}.ps

pdf: ps
	ps2pdf ${TEXSOURCE}.ps

cleanup:
	rm *.aux *.log *.dvi *.ps *.out
