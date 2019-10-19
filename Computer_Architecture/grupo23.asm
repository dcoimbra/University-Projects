; *********************************************************************
; * IST-UL
; * Introdução à Arquitetura de Computadores
; * Projeto: 'Sistema de controlo de comboios'
; * Grupo 23
; * 85254 - Alexandre Dinis Pedro Seixas Caldeira
; * 84708 - David Ferreira Coimbra
; * 84713 - Fábio Rafael Gaudino Caetano Pereira Pimenta
; *********************************************************************

; *********************************************************************
; * Módulo:    grupo23.asm
; * Descrição: Este programa, destinado a ser executado no PEPE,
;              regula o controlo automático do sistema de controlo
;              de comboios. As suas funcionalidades são as seguintes:
;                
;                * Ao premir os botões de pressão, as agulhas devem
;                  trocar de posição.
;                 
;                * Quando um comboio sai de um troço, coloca o semáforo
;                  respetivo a vermelho, e pára quando encontra um sinal
;                  vermelho.
;                
;                * Quando um comboio chega a uma estação, pára.
;     
;                * Quando um comboio passa nas passagens de nível, os
;                  respetivos semáforos trocam entre cinzento e vermelho
;                  ao ritmo do relógio de tempo real, que interage com o PEPE
;                  por meio da interrupção 1.
;
;                * Quando os comboios azul e verde passam por um sensor,
;                colocam nos LCD de cima e baixo, respetivamente,
;                o numero desse sensor (i.e. número do semáforo 
;                correspondente), através da interrupção 0.
; *********************************************************************

; **********************************************************************
; * Constantes
; **********************************************************************

; endereços dos portos do módulo dos comboios 
PLACE 3000H                     ;inicio da zona de dados
LCD_CIMA        EQU 8000H       ;endereço do LCD de cima
LCD_BAIXO       EQU 8002H       ;endereço do LCD de baixo
BOTOES_PRESSAO  EQU 800CH       ;endereço dos botões de pressão
SEMAFOROS       EQU 8012H       ;endereço dos semáforos bi-colores
PASS_NIVEL      EQU 8014H       ;endereço dos semáforos de passagem de nível
AGULHAS         EQU 8016H       ;endereço das agulhas
SEL_COMB        EQU 8018H       ;endereço de seleção de comboio e comando
OP_COMB	        EQU 801AH       ;endereço de operação sobre comboio
SENSOR_EVENTOS  EQU 801CH	      ;endereço do porto dos sensores (número de eventos)		
SENSOR_INFO	    EQU 801EH	      ;endereço do porto dos sensores (informação)
NAO_HA_EVENTO   EQU 0FFFFH      ;indica que não houve passagem por sensor

; **********************************************************************
; * Dados
; **********************************************************************

;informação relativa aos objetos da interface
COMB_0          EQU 0000H       ;indica comboio 0 (azul) no porto de seleção
COMB_1          EQU 0010H       ;indica comboio 1 (verde) no porto de seleção
VEL_0           EQU 00H         ;velocidade 0 (parado)
VEL_3           EQU 03H         ;velocidade 3 (máxima)  
SEM_CINZENTO    EQU 0           ;o semáforo está cinzento
SEM_VERMELHO    EQU 1           ;o semáforo está vermelho
SEM_VERDE       EQU 2           ;o semáforo está verde
ESQUERDA        EQU 1           ;a agulha está apontada para a esquerda
DIREITA         EQU 2           ;a agulha está apontada para a direita
SENSOR_8        EQU 08H
SENSOR_9        EQU 09H
 
;valor a que a stack deve inicializar
STACK_POINTER   EQU 2000H

;estado atual dos botões de pressão;
estado_botoes_pressao:
     WORD 0

;tabela das interrupções
tabela_interrupcoes:
     WORD      rot0                ;rotina de tratamento da interrupção 0
     WORD      rot1                ;rotina de tratamento da interrupção 1

;informacao do comboio que passou pelo sensor lido 
comboio_lido:
     WORD  NAO_HA_EVENTO      

;número do sensor lido pela rotina de interrupção 0
sensor_lido:
     WORD  NAO_HA_EVENTO                     

;esta variável é 1 se o comboio 0 estiver numa estação e 0 caso contrário
estacao_ocupada_0:
    WORD 0

;esta variável é 1 se o comboio 1 estiver numa estação e 0 caso contrário
estacao_ocupada_1: 
    WORD 0

;numero de ciclos de relógio ocorridos
estado_clock:
    WORD 0

;tabelas para as cores dos semáforos (VERDE, CINZENTO ou VERMELHO).
;Por omissão, todos os semáforos são mostrados inicialmente a VERDE,
;exceto os de passagem de nível, que se mostram a CINZENTO
cores_semaforos:                                    
      STRING    SEM_VERDE          ;cor do semáforo 0
      STRING    SEM_VERDE          ;cor do semáforo 1
      STRING    SEM_VERDE          ;cor do semáforo 2
      STRING    SEM_VERDE          ;cor do semáforo 3
      STRING    SEM_VERDE          ;cor do semáforo 4
      STRING    SEM_VERDE          ;cor do semáforo 5
      STRING    SEM_VERDE          ;cor do semáforo 6
      STRING    SEM_VERDE          ;cor do semáforo 7

cores_nivel:
     STRING     SEM_CINZENTO       ;cor do semáforo 8
     STRING     SEM_CINZENTO       ;cor do semáforo 9

;tabela para as posicoes das agulhas (ESQUERDA OU DIREITA).
posicoes_agulhas:                                    
      STRING    DIREITA            ;posição da agulha 0
      STRING    DIREITA            ;posição da agulha 1
      STRING    DIREITA            ;posição da agulha 2
      STRING    ESQUERDA           ;posição da agulha 3

; **********************************************************************
; * Código
; **********************************************************************
PLACE 0000H                     ;inicio da zona de codigo

;inicialização da stack
MOV SP, STACK_POINTER

;inicialização BTE
MOV BTE, tabela_interrupcoes   ;tabela das rotinas de interrupção

;configuração interrupções
PUSH R0
MOV R0, 2                  
MOV RCN, R0                ;interrupção 0 sensível ao nível 1                           
EI0                        ;permite interrupções 0
EI1
EI                         ;permite interrupções
POP R0

;correção dos semáforos de passagem de nível
PUSH R0
PUSH R1
PUSH R2
MOV R0, PASS_NIVEL             ;endereço dos semáforos de passagem de nível
MOV R1, 00H                    ;semáforo 8 a cinzento
MOV R2, 02H                    ;semáforo 9 a cinzento
MOVB [R0], R1   
MOVB [R0], R2                  ;muda ambos os semáforos de passagem de nível para cinzento
POP R2
POP R1
POP R0

;inicialização de movimento
comeca_comboio:
   PUSH R1
   PUSH R2
   PUSH R3
   PUSH R4
   MOV R1, SEL_COMB            ;endereço do porto de seleção de comboios
   MOV R2, OP_COMB             ;endereço do porto de operação dos comboios
   MOV R3, COMB_1              ;endereço do comboio 1 (verde)
   MOV R4, VEL_3               ;sentido para a frente, velocidade 3 (maxima)
   MOVB [R1], R3               ;seleciona comboio verde
   MOVB [R2], R4               ;põe comboio verde a andar
   MOV R3, COMB_0              ;endereço do comboio 0 (azul)
   MOVB [R1], R3               ;seleciona comboio azul
   MOV R4, VEL_3               ;sentido para a frente, velocidade 3 (maxima)
   MOVB [R2], R4               ;põe comboio azul a andar
   POP R4
   POP R3
   POP R2
   POP R1

;programa principal
ciclo:                 
   CALL agulhas               ;regula a troca de posição das agulhas
   CALL semaforos             ;regula a troca de cor dos semáforos e a reação do maquinista à mesma
   CALL nivel                 ;regula a troca de cor dos semáforos de passagem de nível
   CALL gestao_eventos        ;certifica-se dos sensores lidos através dos LCDs, mantém a variável sensor_lido limpa entre interrupções, e regula a paragem nas estações 
	 JMP  ciclo                 ;repete	

; **************************************************************************************************************
; Processo agulhas - Troca a posição das agulhas através de cliques nos botões de pressão.
; ARGUMENTOS: nenhum
; RETORNA:    nada
; **************************************************************************************************************
agulhas:
     PUSH R7
     PUSH R10
     PUSH R9
     PUSH R8
     PUSH R1
     MOV R10, estado_botoes_pressao
     MOV R7, [R10]               ;obtém estado atual dos botoes de pressao

;estado 0 - À espera que um botão seja premido
pressao_a_0:                  
     CMP R7, 0                    ;estamos no estado 0?
     JNZ pressao_a_1              ;se não, estamos no estado 1, à espera que um botão teclado seja libertado 

;espera que seja premido um botão no teclado
espera_pressao_premido:
     MOV R9, BOTOES_PRESSAO       ;endereço do porto do teclado 0 a 7
     MOVB R1, [R9]                ;lê o estado dos botoes
     CMP R1, 0                    ;testa o estado dos botoes
     JZ agulhas_fim               ;se o teclado está a zeros, não há botões carregados. Tem de esperar que seja premido algum

;foi premido um botao! Pode trocar a posição e passar ao estado 1
pressao_premido:                     
       CALL obtem_numero             ;obtém o número da agulha que se quer trocar 
       CALL troca_posicao_agulha     ;troca a posição da agulha
       MOV R7, 1                   
       MOV [R10], R7                ;passa ao estado 1 (atualiza na variável)
       JMP agulhas_fim               ;mas só na próxima iteração. Agora sai

;estado 1 - à espera que o botão seja libertado
pressao_a_1:                  
       CMP R7, 1                   ;o teclado tem um botão carregado?
       JNZ agulhas_fim              ;se não, o estado é desconhecido. sai e ignora

espera_pressao_livre:
       MOV R9, BOTOES_PRESSAO       ;endereço do porto dos botões de pressão
       MOVB R1, [R9]                ;lê o estado dos botões
       CMP R1, 0                    ;testa os botoes
       JNZ agulhas_fim              ;se o teclado não está a zeros, há um botão premido. Tem de esperar que esteja não premido
       MOV R7, 0
       MOV [R10], R7                ;passa ao estado 0 (atualiza na variável)
       JMP agulhas_fim              ;sai para atualizar na próxima iteração
     
agulhas_fim:
       POP R1
       POP R8
       POP R9
       POP R10
       POP R7
       RET

; **************************************************************************************************************
; Processo obtem_numero - Determina o número do semáforo ou agulha ao qual se pretende trocar a cor ou posição
; ARGUMENTOS: R1 - estado do teclado
; RETORNA:    R8 - número do semáforo/agulha
; **************************************************************************************************************
obtem_numero:
   SHR R1, 1                    ;desloca o valor do teclado para a direita       
   ADD R8, 1                    ;contador do ciclo - no final, será o número do semáforo/agulha
   CMP R1, 0                    ;o valor está a zero?
   JNZ obtem_numero             ;se não, repete o ciclo até estar a zero
   SUB R8, 1
   RET

; **************************************************************************************************************
; troca_posicao_agulha - Troca a posição de uma dada agulha (de ESQUERDA para DIREITA e vice-versa).
; ARGUMENTOS: R8 - número da agulha
; RETORNA:    nada
; **************************************************************************************************************
troca_posicao_agulha:
   PUSH R2
   CALL obtem_posicao_agulha      ;obtém posição da agulha (em R2)
   CMP  R2, DIREITA
   JZ   poe_esquerda              ;se a agulha está à direita, põe à esquerda, caso contrário põe à direita
     
poe_direita:
   MOV  R2, DIREITA            
   JMP  atualiza_posicao          ;agulha vai ficar à direita
     
poe_esquerda:
   MOV  R2, ESQUERDA              ;agulha vai ficar à esquerda
     
atualiza_posicao:
   CALL atualiza_posicao_agulha   ;atualiza posicao da agulha na tabela e na interface
   POP R2
   RET

; **************************************************************************************************************
; obtem_posicao_agulha - Obtém a posicao atual de uma dada agulha (por leitura da tabela de posicoes das agulhas).
; ARGUMENTOS: R8 - número da agulha
; RETORNA:    R2 - posicao da agulha (ESQUERDA, DIREITA)
; **************************************************************************************************************
obtem_posicao_agulha:
   PUSH R10                      
   MOV R10, posicoes_agulhas     ;endereço da tabela das posicoes das agulhas
   ADD R10, R8                   ;obtém endereço do byte de posicao da agulha (soma o número da agulha à base da tabela)
   MOVB R2, [R10]                ;lê a posicao da agulha
   POP R10                      
   RET
     
; **************************************************************************************************************
; atualiza_posicao_agulha - Atualiza a posicao de uma dada agulha (quer na tabela de posicoes das agulhas,
;                         quer na agulha propriamente dita, na interface de visualização do módulo dos comboios).
; ARGUMENTOS: R8 - número da agulha
;             R2 - nova posicao da agulha (ESQUERDA ou DIREITA)
; RETORNA:    nada
; **************************************************************************************************************
atualiza_posicao_agulha:
     PUSH R8                       
     PUSH R10
     PUSH R7
     MOV R10, posicoes_agulhas     ;endereço da tabela das posicoes das varias agulhas
     ADD R10, R8                   ;obtém endereço do byte de posicao da agulha (soma o número da agulha à base da tabela)
     MOVB [R10], R2                ;atualiza a posicao da agulha na tabela de posicoes das agulhas
     SHL R8, 2                     ;formato do porto das agulhas (número da agulha tem de estar nos bits 7 a 2, posicao nos bits 1 e 0)
     ADD R8, R2                    ;junta posicao da agulha (que fica nos bits 1 e 0)
     MOV R7, AGULHAS               ;endereço do porto das agulhas no módulo dos comboios
     MOVB [R7], R8                 ;atualiza posicao na agulha propriamente dita
     POP R7                       
     POP R10
     POP R8
     RET

; **************************************************************************************************************
; Processo semaforos - Troca a cor dos semáforos para vermelho quando o troço seguinte é ocupado
;                      e pára os comboios quando encontram um sinal vermelho.
; ARGUMENTOS: R10 - numero do sensor lido pela interrupção 0
;             R9  - numero do comboio que provocou a interrupção 0
; RETORNA:    nada
; **************************************************************************************************************
semaforos:
     PUSH R0
     PUSH R1
     PUSH R2
     PUSH R7
     PUSH R8
     PUSH R9
     PUSH R10
     MOV R10, sensor_lido ;endereço da variável que guarda o numero dos sensores
     MOV R9, comboio_lido ;endereço da variável que guarda a informação do comboio
     MOV R7, [R10]        ;obtém número do sensor pelo que o comboio passou
     MOV R8, [R9]         ;obtém informação sobre o comboio que passou

espera_semaforo:
     MOV R0, NAO_HA_EVENTO ;houve evento?
     CMP R7, R0
     JZ semaforos_fim      ;se não, não há semáforos a trocar. pode sair.
     MOV R1, 8             
     MOV R2, 9
     CMP R7, R1            ;passou no sensor 8?
     JZ semaforos_fim      ;se a interrupção foi provocada aí, ignora e prossegue   
     CMP R7, R2            ;passou no sensor 9?
     JZ semaforos_fim      ;se a interrupção foi provocada aí, ignora e prossegue

semaforo_aproximado:     
     CALL troca_cor_semaforo  ;vê a cor do semáforo: se vermelho, pára, se não, põe vermelho e prossegue.
   
semaforos_fim:
     POP R10
     POP R9
     POP R8
     POP R7
     POP R2
     POP R1
     POP R0
     RET
     
; **************************************************************************************************************
; troca_cor_semaforo - Troca a cor de um dado semáforo para SEM_VERMELHO.
; ARGUMENTOS: R7 - número do semáforo
; RETORNA:    nada
; **************************************************************************************************************
troca_cor_semaforo:
     PUSH R2
     PUSH R8
     PUSH R10
     CALL obtem_cor_semaforo       ;obtém cor do semáforo (em R2)
     CMP  R2, SEM_VERDE
     JZ   poe_vermelho             ;se o semáforo está a verde, resume o movimento, põe a vermelho, caso contrário pára
     MOV R9, comboio_lido ;endereço da variável que guarda a informação do comboio
     MOV R7, [R10]        ;obtém número do sensor pelo que o comboio passou
     MOV R8, [R9]         ;obtém informação sobre o comboio que passou
     BIT R8, 1                   ;qual o comboio a parar?
     JZ para_comboio_0           ;pára comboio 0 se tiver sido esse
     JNZ para_comboio_1          ;pára comboio 1

para_comboio_0:
     MOV R8, COMB_0              ;endereço do comboio 0 (azul)
     MOV R10, VEL_0               ;velocidade 0 (parado)
     CALL opera_comboios
     JMP troca_cor_semaforo_fim

para_comboio_1:
     MOV R8, COMB_1               ;endereço do comboio 1 (verde)
     MOV R10, VEL_0               ;velocidade 0 (parado)
     CALL opera_comboios
     JMP troca_cor_semaforo_fim
    
poe_vermelho:
     MOV  R2, SEM_VERMELHO         ;semáforo do troço a ocupar vai ficar vermelho
     
atualiza_cor:
     CALL atualiza_cor_semaforo    ;atualiza cor do semáforo na tabela e na interface
 
troca_cor_semaforo_fim:     
     POP R10
     POP R8
     POP R2
     RET

; **************************************************************************************************************
; obtem_cor_semaforo - Obtém a cor atual de um dado semáforo (por leitura da tabela de cores dos semáforos).
; ARGUMENTOS: R7 - número do semáforo
; RETORNA:    R2 - cor do semáforo (SEM_VERDE, SEM_VERMELHO)
; **************************************************************************************************************
obtem_cor_semaforo:
     PUSH R8                      
     MOV R8, cores_semaforos      ;endereço da tabela das cores dos vários semáforos
     ADD R8, R7                   ;obtém endereço do byte de cor do semáforo (soma o número do semáforo à base da tabela)
     MOVB R2, [R8]                ;lê a cor do semáforo
     POP R8                      
     RET

; **************************************************************************************************************
; atualiza_cor_semaforo - Atualiza a cor de um dado semáforo (quer na tabela de cores dos semáforos,
;                         quer no semáforo propriamente dito, na interface de visualização do módulo dos comboios).
; ARGUMENTOS: R7 - número do semáforo
;             R2 - nova cor do semáforo (SEM_VERDE, SEM_VERMELHO)
; RETORNA:    nada
; **************************************************************************************************************
atualiza_cor_semaforo:
     PUSH R8                       
     PUSH R10
     MOV R8, cores_semaforos       ;endereço da tabela das cores dos vários semáforos
     ADD R8, R7                    ;obtém endereço do byte de cor do semáforo (soma o número do semáforo à base da tabela)
     MOVB [R8], R2                 ;atualiza a cor do semáforo na tabela de cores dos semáforos
     SHL R7, 2                     ;formato do porto dos semáforos
     ADD R7, R2                    ;junta cor do semáforo
     MOV R10, SEMAFOROS            ;endereço do porto dos semáforos no módulo dos comboios
     MOVB [R10], R7                ;atualiza cor no semaforo propriamente dito                     
     POP R10
     POP R8
     RET  

; **************************************************************************************************************
; nivel - Regula a troca de cor dos semáforos de passagem de nível, através da interrupção 1 (ciclos de clock).
; ARGUMENTOS: R0 - sensor lido pela interrupção 0
; RETORNA:    nada
; **************************************************************************************************************
nivel:
     PUSH R0
     PUSH R1 
     PUSH R2
     PUSH R3
     PUSH R4
     MOV R0, sensor_lido
     MOV R1, [R0]                ;leitura do sensor do ultimo evento 
     MOV R2, SENSOR_8
     CMP R2, R1                  ;se o sensor_lido for o sensor 8, vê o estado do clock para alterar os semáforos conforme.
     JZ  ver_estado_do_clock    
     MOV R2, SENSOR_9
     CMP R2, R1                  ;se o sensor_lido for o sensor 9 entao ele poe ambos os semaforos a cinzento
     JZ sem8e9_cinzentos 
     JMP fim_nivel
   
ver_estado_do_clock:             ;funçao que vê o valor do clock e consoante se é par ou impar (bit de menor peso), intercala as cores dos semaforos 
     MOV R1, estado_clock        ; entre cinzento e vermelho, e vermelho e cinzento
     MOV R2, [R1]
     BIT R2, 0
     JNZ sem8_cinzento_sem9_vermelho
   
sem8_vermelho_sem9_cinzento:
     MOV R1, PASS_NIVEL                ;acesso ao endrenço da passagem de nivel, em relaçao aos semaforos
     MOV R2, 01H                       ;semaforo 8 a vermelho
     MOV R3, 02H                       ;semaforo 9 a cinzento
     MOVB [R1], R2
     MOVB [R1], R3
     JMP fim_nivel
     
sem8_cinzento_sem9_vermelho:
     MOV R1, PASS_NIVEL                ;acesso ao enderenço da passagem de nivel, em relaçao aos semaforos
     MOV R2, 00H                       ;semaforo 8 a cinzento
     MOV R3, 03H                       ;semaforo 9 a vermelho
     MOVB [R1], R2
     MOVB [R1], R3
     JMP fim_nivel
     
sem8e9_cinzentos:
     MOV R1, PASS_NIVEL                ;acesso ao endrenço da passagem de nivel, em relaçao aos semaforos
     MOV R2, 00H                       ;semaforo 8 a cinzento
     MOV R3, 02H                       ;semaforo 9 a cinzento
     MOVB [R1], R2
     MOVB [R1], R3
   
fim_nivel:
     POP R4
     POP R3
     POP R2
     POP R1
     POP R0
     RET

; *************************************
; gestao_eventos - Mantém registo nos LCDs dos sensores lidos. Adicionalmente, gere a paragem dos
;                  comboios nas estações e certifica-se que as variáveis sensor_lido e comboio_lido
;                  não são constantemente substituídas.
; ARGUMENTOS: R9 - Porto de informação de evento, que contém o número e informação do evento que provocou a interrupção
; RETORNA:    R1 - informação do comboio, a colocar na variável em memória
;             R2 - numero do sensor, a colocar na variável em memória
; *************************************
gestao_eventos:
     PUSH R10
     PUSH R9
     PUSH R8
     PUSH R7
     PUSH R6
     PUSH R5
     PUSH R2
     PUSH R1
     PUSH R0

espera_eventos:
     MOV  R9, sensor_lido          ; variável para comunicação entre a interrupção 0 e o código principal
     MOV  R2, [R9]                 ; lê o número do sensor pelo qual o comboio passou
     MOV R1, NAO_HA_EVENTO
     CMP R2, R1
     JZ  gestao_eventos_fim            ; se for NAO_HA_EVENTO, não há eventos ainda e pode sair

;quando houver eventos, determina qual o comboio que os provocou
houve_evento:   
     MOV R5, comboio_lido         ;porto dos sensores (informação)
     MOV R7, [R5]
     BIT R7, 1                        ;testa o bit que identifica o comboio
     JNZ escreve_numero_sensor_baixo  ;se for o comboio verde, escreve no LCD de baixo. se for o azul, escreve no de cima.

;escreve o sensor passado pelo comboio azul no LCD de cima
escreve_numero_sensor_cima:
     MOV R7, 30H                      ;'0' em ASCII. O LCD apenas reconhece carateres ASCII.
     ADD R2, R7                       ;converte número do sensor (0 a 9) para caracter ASCII ('0' a '9') 
     MOV R6, LCD_CIMA                 ;porto da linha de cima do LCD
     MOVB [R6], R2                    ;escreve número do sensor no LCD
     MOV [R9], R1
     SUB R2, R7
     CMP R2, 2
     JZ foi_estacao
     CMP R2, 5
     JZ foi_estacao
     JMP gestao_eventos_fim

;escreve o sensor passado pelo comboio verde no LCD de baixo
escreve_numero_sensor_baixo:
     MOV R7, 30H                      ;'0' em ASCII. O LCD apenas reconhece carateres ASCII.
     ADD R2, R7                       ;converte número do sensor (0 a 9) para caracter ASCII ('0' a '9') 
     MOV R0, LCD_BAIXO              ;porto da linha de baixo do LCD
     MOVB [R0], R2                    ;escreve número do sensor no LCD
     MOV [R9], R1                   ;limpa variáveis para que não estejam sempre a ser substituídas
     SUB R2, R7                     ;restaura numero do sensor em hexadecimal (e não em ASCII)
     CMP R2, 2                      
     JZ foi_estacao                 ;se passou por uma estação, pára lá os comboios
     CMP R2, 5
     JZ foi_estacao                 ;se passou por uma estação, pára lá os comboios
     JMP gestao_eventos_fim         ;se não, sai

foi_estacao:
     BIT R7,1                      ;decide qual o comboio a parar
     JNZ interrompe_comboio_1      ;pára o comboio que provocou a interrupção

interrompe_comboio_0:
     MOV R1, estacao_ocupada_0     ;indica na variável que a estação se encontra ocupada pelo comboio 0
     MOV R2, 1                      
     MOV [R1], R2
     MOV R8, COMB_0
     MOV R10, VEL_0
     CALL opera_comboios           ;pára o comboio 0 na estação
     JMP gestao_eventos_fim        ;sai

interrompe_comboio_1:
     MOV R1, estacao_ocupada_1     ;indica na variável que a estação se encontra ocupada pelo comboio 1
     MOV R2, 1                     
     MOV [R1], R2
     MOV R8, COMB_1
     MOV R10, VEL_0
     CALL opera_comboios          ;pára o comboio 1 na estação
 
gestao_eventos_fim:
     POP R0
     POP R1
     POP R2
     POP R5
     POP R6
     POP R7
     POP R8
     POP R9
     POP R10
     RET

; *************************************
; opera_comboios - Aplica operações de velocidade ao comboio selecionado
; ARGUMENTOS: R8 - Comboio a operar
; RETORNA:    R10 - Operação a aplicar
; *************************************
opera_comboios:
     PUSH R0
     MOV R0, SEL_COMB
     MOV [R0], R8        ;seleciona comboio
     MOV R0, OP_COMB
     MOV [R0], R10       ;aplica operação
     POP R0
     RET

; *************************************
; rot0 - Rotina de tratamento da interrupção 0 (eventos de passagem pelos sensores)
; ARGUMENTOS: R9 - Porto de informação de evento, que contém o número e informação do evento que provocou a interrupção
; RETORNA:    R1 - informação do comboio, a colocar na variável em memória
;             R2 - numero do sensor, a colocar na variável em memória
; *************************************
rot0:
   PUSH R1
   PUSH R2
   PUSH R4
   PUSH R9
   MOV R9, SENSOR_INFO               ;porto dos sensores (informação)
   MOVB R1, [R9]                     ;lê 1º byte (informação sobre o comboio que passou)
   MOVB R2, [R9]                     ;lê 2º byte (número do sensor)
   BIT  R1, 0                        ;verifica qual o topo lido
   JNZ termina_rot0                  ;se tiver sido a traseira, termina a interrupção e ignora
   MOV  R4, comboio_lido             ;endereço da variável que guarda a informação do comboio que passa por um sensor
   MOV  [R4], R1                     ;guarda informação na variável, para ser comunicada ao código principal
   MOV  R9, sensor_lido              ;endereço da variável que guarda o número do sensor
   MOV  [R9], R2                     ;guarda numero na variável, para ser comunicada ao código principal

termina_rot0:
   POP  R9
   POP  R4
   POP  R2
   POP  R1   
   RFE

; *************************************
; rot1 - Rotina de tratamento da interrupção 0 (contagem de ciclos de clock)
; ARGUMENTOS: nenhum
; RETORNA: R2 - contagem atual de ciclos de relógio
; *************************************
rot1:
     PUSH R1
     PUSH R2
     MOV R1, estado_clock                   
     MOV R2, [R1]                    ;aceder a estado_clock, variavel que conta o numero de ciclos no clock
     ADD R2,1                        ;acrecento de 1, de modo a manter contagem de ciclos
     MOV [R1],R2                 
     POP R1
     POP R2    
     RFE