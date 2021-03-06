# Computer Architecture Final Assignment - English Version
Final assignment of computer architecture class: optimizing Mandelbrot algorithm using different approaches. 

# Implemented versions
- mandelbrot.c - original algorithm.  
- mandelbrotInnerLoopFuncrion.c - loops modified into functions – this later will allow threads approach.  
- mandelbrotILFwPthread.c - version using POSIX Threads.  
- mandelbrotInnerLoopFunctionwOffload.c - version that sends offload to be processed in a Many Integrated Core (MIC) architecture.

When in the name of a “c” file you see the acronym “NP” – Non Persistent- it means that this version of the algorithm does not save the generated image into the hard disk, just in memory (RAM).

# How to run the algorithms
After cloning this repository in a Linux machine with Intel compiler installed, you will have to execute the desired Makefile using the command:  
*$make -f ChosenMakeFile*
- Makefile-P will run the persistent version of the aforementioned algorithms, at the end it will show the time taken to run those algorithms. **Note that it will be created 3 images of approximately 800MB and saved on the hard disk**.
- Makefile-NP will run the non-persistent version of the aforementioned algorithms, showing the time taken to run them.
- Makefile-threads threads will run the parallel algorithms using 11 variation if the threads number from 4 to 4096. It will also display the time taken to run each variation.
- Makefile-offload will run two different versions of the MIC algorithm: one persistent and the other non-persistent. This has to be executed in a machine with a Xeon-Phi architecture.

# Results
The next two graphics show how the Mandelbrot algorithm versions performed in a Asus laptop using Intel i7 4700hq [(specs)](http://ark.intel.com/products/75116/Intel-Core-i7-4700HQ-Processor-6M-Cache-up-to-3_40-GHz), 8GB DDR3L 1600 MHz SDRAM and SSD 500GB.

![Notebook Asus N550J com SSD](https://github.com/dcardos/arq2MIC/blob/master/Graficos/PvsNP2English.jpg)

The next graphic shows how the non-persistent thread version performed **non-persistent**

![Notebook Asus N550J com SSD threads](https://github.com/dcardos/arq2MIC/blob/master/Graficos/threads2English.jpg)

Now you can see on the next graphic how the same algorithm performed using the MIC architecture.

![MIC threads](https://github.com/dcardos/arq2MIC/blob/master/Graficos/threads-MICEnglish.jpg)

All the time presented is actually the average value of 3 executions.

# Conclusion
The version using functions – Inner Loop Function (ILF) – had little performance gain when it was non-persistent, probably due to vectorization or cache manipulation by the compiler. The persistent version, on the other hand, had increased the number of vector access not taking advantage of the original loop, which made the algorithm much slower in the persistent version.

The parallel algorithm version using POSIX threads it is by far the best regarding performance. Even the slow persistent version had a 30% gain in performance! The non-persistent version was 85% faster (512 threads) than the original, and can yet be 50% faster using the MIC architecture and 1024 threads.

# References
https://software.intel.com/en-us/articles/programming-and-compiling-for-intel-many-integrated-core-architecture

https://software.intel.com/sites/default/files/managed/26/d6/Intel_Xeon_Phi_Quick_Start_Developers_Guide-MPSS-3.4.pdf

https://software.intel.com/en-us/node/522683

https://software.intel.com/sites/products/documentation/doclib/iss/2013/compiler/cpp-lin/GUID-44F5B8E2-8EFD-4C51-ACF8-357900798834.htm

http://www.training.prace-ri.eu/uploads/tx_pracetmo/xeonPhiProg.pdf

https://computing.llnl.gov/tutorials/pthreads/



# arq2MIC - Portuguese version
Trabalho final da matéria arquitetura de computadores 2 utilizando o algoritmo de Mandelbrot implementado de diferentes maneiras.

# Implementações utilizadas
- mandelbrot.c - arquivo orignal.  
- mandelbrotInnerLoopFuncrion.c - algoritmo implementado funcionalmente, os dois loops aninhados viraram uma função, bem  como a parte referente a persistência.  
- mandelbrotILFwPthread.c - algoritmo implementado através de POSIX Threads.  
- mandelbrotInnerLoopFunctionwOffload.c - algoritmo implementado com offload para placa MIC

Todas as versões citadas anteriormente possuem implementação tanto com persistência, gerando a imagem, quanto sem. A sigla 'NP' no nome dos arquivos significa não persistente.

# Instruções para execução dos arquivos
Em uma máquina linux com compilador da Intel instalado execute o Makefile especificado abaixo utilizando  
*$make -f MakeFileEscolhido*
- Makefile-P irá compilar e executar as verões persistentes mostrando seu tempo de execução. **Atenção** para o fato de que **serão geradas 3 imagens de aproximadamente 800MB cada**, elas devem ser iguais em conteúdo, mas seus nomes são dados de acordo com o algoritmo utilizado.
- Makefile-NP irá compilar e executar as versões não persistentes mostrando seu tempo de execução.
- Makefile-threads irá compilar e executar 11 modos do algoritmo paralelo usando threads, começando por 4 até 4096 threads. Tempo de execução também será computado.
- Makefile-offload irá compilar e executar duas versões do offload, uma com persistência e outra sem, mas este só deve ser executado na máquina do Xeon Phi (porém não nativamente).

Há também um script que pode ser executado localizado na placa *mic0*
- threads-script - arquivo encontrado somente na pasta *danilo*, é a versão Makefile-threads para MIC.

# Resultados
Utilizando o compilador da Intel em um notebook ASUS com Intel i7 4700hq [(especificações)](http://ark.intel.com/products/75116/Intel-Core-i7-4700HQ-Processor-6M-Cache-up-to-3_40-GHz), 8GB DDR3L 1600 MHz SDRAM e SSD 500GB foram obtidos os resultados apresentados no gráfico

![Notebook Asus N550J com SSD](https://github.com/dcardos/arq2MIC/blob/master/Graficos/PvsNP2.jpg)

Os próximos dois gráficos apresentam uma análise do algoritmo paralelo usando POSIX threads **sem persistência**

![Notebook Asus N550J com SSD threads](https://github.com/dcardos/arq2MIC/blob/master/Graficos/threads2.jpg)

A execução do mesmo algoritmo utilizando o Xeon Phi compilação nativa na placa resultou nos dados apresentados no gráfico abaixo

![MIC threads](https://github.com/dcardos/arq2MIC/blob/master/Graficos/threads-MIC.jpg)

**Utilizando persistência** com 1024 threads, o tempo de execução na placa passa a ser **2m36s**.

**A versão com Offload** apresentou um tempo de aproximadamente 200s sem persistência e 220s gerando a imagem. Atenção para o fato de que **a imagem não foi averiguada** dada a limitação do terminal.

Todos os tempos apresentados são a média de 3 execuções.

# Conclusão
A versão funcional -Inner Loop Function- teve um leve ganho de desempenho provavelmente por uma possibilidade de vetorização ou melhor utilização da memória cache. Todavia, criar uma função de persistência aumentou o número de acesso vetorial não aproveitando o loop original, além de aumentar significamente o tempo de persistência dos dados.

A versão paralela usando POSIX threads é sem dúvidas a melhor, mesmo com a lenta função de persistência seu ganho chega a ser de 30%, sem persistência obteve-se um algoritmo quase 85% mais rápido que o original utilizando 512 threads. Essa versão paralela sem persistência ainda pode ser 50% mais eficiente se executada nativamente na placa MIC da Intel Xeon Phi com 1024 threads, uma performace incrível apesar de não ser eficiente na persistência dos dados. A persistência é lenta devido a utilização de apenas um core, ou seja, não é paralelizável.

Por fim a versão com offload não apresentou bons resultados, muito provavelmente porque este parâmetro por si só é apenas útil para algoritmos com alto grau de vetorização, que não é o caso. Mesmo assim, offload é muito utilizado junto com técnicas CilkPlus e OpenMP. Infelizmente o algoritmo também não é um bom candidato para CilkPlus, ele gera muito dados a partir de poucos e não o contrário como deveria ser para o uso da ferramenta. Quanto ao OpenMP segue a explicação do porquê não foi possível a implementação.

# Sobre as versões com OpenMP
Infelizmente todas elas de alguma forma deformavam a imagem, o compilador em si não conseguiu fazer com que o loop, mesmo em sua forma funcional, se comportasse adequadamente, em geral um problema no valor da variável *Cy* foi notado. Talvez uma maior exploração dos parâmetros possa contornar este problema, contudo é pouco provável um desempenho melhor que utilizando POSIX threads. Dado tal inconsistência, todos os arquivos que usavam OpenMP foram descartados, mas se encontram no histórico de commits.

PS: relatórios do compilador intel no Wiki


# Referências
https://software.intel.com/en-us/articles/programming-and-compiling-for-intel-many-integrated-core-architecture

https://software.intel.com/sites/default/files/managed/26/d6/Intel_Xeon_Phi_Quick_Start_Developers_Guide-MPSS-3.4.pdf

https://software.intel.com/en-us/node/522683

https://software.intel.com/sites/products/documentation/doclib/iss/2013/compiler/cpp-lin/GUID-44F5B8E2-8EFD-4C51-ACF8-357900798834.htm

http://www.training.prace-ri.eu/uploads/tx_pracetmo/xeonPhiProg.pdf

https://computing.llnl.gov/tutorials/pthreads/
