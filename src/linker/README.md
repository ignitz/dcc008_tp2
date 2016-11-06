# Linker

> Mais informações você encontra na [documentação](../../doc/).

Inicialmente você deve compilar o montador na pasta **assembler**. Se tudo der certo, aparecerá como abaixo.
```
yuri@Lenovo-IdeaPad-S400U:~/Documents/code/tp/dcc008_tp2/src/assembler$ make
Creating directories
Beginning release build
Compiling: symbol_table.cpp -> build/release/symbol_table.o
	 Compile time: 00:00:01
Compiling: table_opcodes.cpp -> build/release/table_opcodes.o
	 Compile time: 00:00:01
Compiling: passOne.cpp -> build/release/passOne.o
	 Compile time: 00:00:02
Compiling: passTwo.cpp -> build/release/passTwo.o
	 Compile time: 00:00:03
Compiling: util.cpp -> build/release/util.o
	 Compile time: 00:00:00
Compiling: montador.cpp -> build/release/montador.o
	 Compile time: 00:00:02
Linking: bin/release/montador
	 Link time: 00:00:00
Making symlink: montador -> bin/release/montador
Total build time: 00:00:09
```

Depois é a vez do **linker**.
```
yuri@Lenovo-IdeaPad-S400U:~/Documents/code/tp/dcc008_tp2/src/linker$ make
Creating directories
Beginning release build
Compiling: main.cpp -> build/release/main.o
	 Compile time: 00:00:02
Compiling: Programa.cpp -> build/release/Programa.o
	 Compile time: 00:00:02
Compiling: Linker.cpp -> build/release/Linker.o
	 Compile time: 00:00:02
Compiling: SymbolTable.cpp -> build/release/SymbolTable.o
	 Compile time: 00:00:00
Compiling: util.cpp -> build/release/util.o
	 Compile time: 00:00:02
Linking: bin/release/linker
	 Link time: 00:00:00
Making symlink: linker -> bin/release/linker
Total build time: 00:00:09
```

Execute `make ass` na pasta **linker** para o montador compilar os módulos.
```
yuri@Lenovo-IdeaPad-S400U:~/Documents/code/tp/dcc008_tp2/src/linker$ make ass
../assembler/montador ../tst/main.a -o ../tst/main.mif
../assembler/montador ../tst/max.a -o ../tst/max.mif
../assembler/montador ../tst/min.a -o ../tst/min.mif
../assembler/montador ../tst/soma.a -o ../tst/soma.mif
../assembler/montador ../tst/prod.a -o ../tst/prod.mif
../assembler/montador ../tst/media.a -o ../tst/media.mif
```

Por fim, execute `make run` para criar o **MIF** (programa.mif) final que estará na pasta **tst**.

O `make v` executa em modo verbose mostrando todos os passos do linkador.
