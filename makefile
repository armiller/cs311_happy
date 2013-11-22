CC = icc
CFLAGS = -Wall -std=c99 -D_XOPEN_SOURCE
CTARGET = happy

TEXSOURCE = assign4
default: pdf cleanup
	${CC} ${CFLAGS} -o ${CTARGET} ${LDFLAGS} ${CTARGET}

debug: ${SOURCE} ${INCLUDES}
	${CC} ${CFLAGS} ${CSOURCE} -o ${CTARGET} ${LDFLAGS} -DDEBUG

mp:
	${CC} ${CFLAGS} -o ${CTARGET}_mp.out ${LDFLAGS} ${CTARGET}_mp.c -DDEBUG

th: 
	${CC} ${CFLAGS} -o ${CTARGET}_th.out ${LDFLAGS} ${CTARGET}_th.c -DDEBUG

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
