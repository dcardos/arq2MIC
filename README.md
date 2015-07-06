# arq2MIC
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
- Makefile-threads irá compilar e executar 11 modos do algoritmo paralelo usando threads, começando por 2 até 4096 threads. Tempo de execução também será computado.

# Resultados
Utilizando o compilador da Intel em um notebook ASUS com Intel i7 4700hq [(especificações)](http://ark.intel.com/products/75116/Intel-Core-i7-4700HQ-Processor-6M-Cache-up-to-3_40-GHz), 8GB DDR3L 1600 MHz SDRAM e SSD 500GB foram obtidos os resultados apresentados no gráfico

![Notebook Asus N550J com SSD](https://github.com/dcardos/arq2MIC/blob/master/Graficos/PvsNP.jpg)

![Notebook Asus N550J com SSD threads](https://github.com/dcardos/arq2MIC/blob/master/Graficos/threads.jpg)

# Conclusão
A versão funcional -Inner Loop Function- teve um leve ganho de desempenho provavelmente por uma possibilidade de vetorização ou melhor utilização da memória cache. Todavia criar uma função de persistência aumentou o número de acesso vetorial não aproveitando o loop original, além de aumentar significamente o tempo de persistência dos dados.

A versão paralela usando POSIX threads é sem dúvidas a melhor, mesmo com a lenta função de persistência seu ganho chega a ser de 30%, sem persistência obteve-se um algoritmo quase 85% mais rápido que o original utilizando 512 threads.


# Referências
https://software.intel.com/en-us/articles/programming-and-compiling-for-intel-many-integrated-core-architecture

https://software.intel.com/sites/default/files/managed/26/d6/Intel_Xeon_Phi_Quick_Start_Developers_Guide-MPSS-3.4.pdf

https://software.intel.com/en-us/node/522683

https://software.intel.com/sites/products/documentation/doclib/iss/2013/compiler/cpp-lin/GUID-44F5B8E2-8EFD-4C51-ACF8-357900798834.htm

http://www.training.prace-ri.eu/uploads/tx_pracetmo/xeonPhiProg.pdf

https://computing.llnl.gov/tutorials/pthreads/
