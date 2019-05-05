all: vi2

vi2: vi_main.o vi_normal.o vi_insert.o vi_cline.o vi_ds_impl.o
	 g++ vi_main.o vi_normal.o vi_insert.o vi_cline.o vi_ds_impl.o -o vi2 

vi_main.o: vi_main.cpp
		   g++ -c vi_main.cpp

vi_normal.o: vi_normal.cpp
	         g++ -c vi_normal.cpp


vi_insert.o: vi_insert.cpp
		     g++ -c vi_insert.cpp 


vi_cline.o: vi_cline.cpp
		    g++ -c vi_cline.cpp 

vi_ds_impl.o : vi_ds_impl.cpp
		  	   g++ -c vi_ds_impl.cpp 


