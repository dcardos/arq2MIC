# arq2MIC
Trabalho final da matéria arquitetura de computadores 2 utilizando o algoritmo de Mandelbrot implementado de diferentes maneiras.

# Implementações utilizadas
- mandelbrot.c - arquivo orignal.  
- mandelbrotInnerLoopFuncrion.c - algoritmo implementado funcionalmente, os dois loops aninhados viraram uma função, bem  como a parte referente a persistência.  
- mandelbrotILFwPthread.c - algoritmo implementado através de POSIX Threads.  
- mandelbrotInnerLoopFunctionwOffload.c - algoritmo implementado com offload para placa MIC  

Todas as versões citadas anteriormente possuem implementação tanto com persistência, gerando a imagem, tanto quanto sem. A sigla 'NP' no nome dos arquivos significa não persistente.

# Instruções para execução dos arquivos
- Makefile-P irá compilar e executar as verões persistentes mostrando seu tempo de execução. **Atenção** para o fato de que **serão geradas 3 imagens de aproximadamente 800MB cada**, elas devem ser iguais em conteúdo, mas seus nomes são dados de acordo com o algoritmo utilizado.
- Makefile-NP irá compilar e executar as versões não persistentes mostrando seu tempo de execução.
- Makefile-threads irá compilar e executar 11 modos do algoritmo paralelo usando threads, começando por 2 até 4096 threads. Tempo de execução também será computado.

# Referências
https://software.intel.com/en-us/articles/programming-and-compiling-for-intel-many-integrated-core-architecture

https://software.intel.com/sites/default/files/managed/26/d6/Intel_Xeon_Phi_Quick_Start_Developers_Guide-MPSS-3.4.pdf

https://software.intel.com/en-us/node/522683

https://software.intel.com/sites/products/documentation/doclib/iss/2013/compiler/cpp-lin/GUID-44F5B8E2-8EFD-4C51-ACF8-357900798834.htm

http://www.training.prace-ri.eu/uploads/tx_pracetmo/xeonPhiProg.pdf

https://computing.llnl.gov/tutorials/pthreads/
