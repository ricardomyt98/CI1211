#/bin/make

SRC=src
INC=include
OBJ=obj

EXECUTABLE=compilador
CCFLAGS=-ll -ly -lc -I$(INC)

TEMP_FILES=	lex.yy \
			compilador.tab
FILES=	utilities \
		symbol_table \
		compiler

OBJECTS=$(addprefix $(OBJ)/, $(addsuffix .o, $(FILES) $(TEMP_FILES)))
SRC_FILES=$(addprefix $(SRC)/, $(addsuffix .c, $(FILES) $(TEMP_FILES)))
SRC_TEMP_FILES=$(addprefix $(SRC)/, $(addsuffix .c, $(TEMP_FILES)))

main: flex bison $(OBJECTS)
	gcc $(OBJECTS) $(CCFLAGS) -o $(EXECUTABLE)

flex:
	cd $(SRC) && flex compilador.l

bison:
	cd $(SRC) && bison compilador.y -d -v

$(OBJ)/%.o: $(SRC)/%.c
	gcc -c $< -o $@ $(CCFLAGS)

clean:
	rm -f $(OBJECTS)
	rm -f	compilador \
			$(SRC)/compilador.output \
			$(SRC)/compilador.tab.h \
			$(SRC_TEMP_FILES) \
			MEPA
