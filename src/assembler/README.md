# Montador

### How to use

#### Compilar

```
$ make
```

#### Compilar DEBUG

Se for utilizar o GDB

```
$ make debug
```

#### Executar

Executar abrindo o arquivo all.a, media.a e pa.a

```
$ make run
```

Caso deseje especificar um arquivo:

```
$ ./montador [arquivo.a]
```

Se desejar especificar o arquivo de saída utilize "-o"

```
$ ./montador -o [arquivo_saida.mif]
```


#### Executar em verbose

Executar abrindo o arquivo W2-1.a

```
$ make v
```

#### Limpar arquivos compilados

Serve para garantir a compilação sem reaproveitamento.

```
$ make clean
```
