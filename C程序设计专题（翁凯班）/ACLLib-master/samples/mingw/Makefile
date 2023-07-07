SRC = ..\..\src\acllib.c
INCLUDE_DIR = ..\..\src
LIB = gdi32 ole32 oleaut32 uuid winmm msimg32

all : keyboard mouse_move mouse_click image clock char 

keyboard :
	gcc $(SRC) $@.c -I$(INCLUDE_DIR) $(LIB:%=-l%) -DWINVER=0x0501 -o $@
	
mouse_move :
	gcc $(SRC) $@.c -I$(INCLUDE_DIR) $(LIB:%=-l%) -DWINVER=0x0501 -o $@
	
mouse_click :
	gcc $(SRC) $@.c -I$(INCLUDE_DIR) $(LIB:%=-l%) -DWINVER=0x0501 -o $@

image :
	gcc $(SRC) $@.c -I$(INCLUDE_DIR) $(LIB:%=-l%) -DWINVER=0x0501 -o $@
	
clock :
	gcc $(SRC) $@.c -I$(INCLUDE_DIR) $(LIB:%=-l%) -DWINVER=0x0501 -o $@
	
char :
	gcc $(SRC) $@.c -I$(INCLUDE_DIR) $(LIB:%=-l%) -DWINVER=0x0501 -o $@
deno :
	gcc $(SRC) $@.c -I$(INCLUDE_DIR) $(LIB:%=-l%) -DWINVER=0x0501 -o $@
mmm :
	gcc $(SRC) print_function.c -I$(INCLUDE_DIR) $(LIB:%=-l%) -DWINVER=0x0501 -o paint_a_function
mm :
	gcc $(SRC) print_function_copy.c -I$(INCLUDE_DIR) $(LIB:%=-l%) -DWINVER=0x0501 -o paint
ee :
	gcc $(SRC) editor1.c -I$(INCLUDE_DIR) $(LIB:%=-l%) -DWINVER=0x0501 -o print
eee :
	gcc $(SRC) editor.c -I$(INCLUDE_DIR) $(LIB:%=-l%) -DWINVER=0x0501 -o print1
