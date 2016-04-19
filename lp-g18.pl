/*                  Grupo 18                  */
/* 57842 Filipa Marques - 84708 David Coimbra */

/* Programa principal */

% movs_possiveis(Lab, Pos_atual, Movs, Poss). 

% distancia/3: calcula Dist como a distancia entre (L1, C1) e (L2, C2).
distancia((L1, C1), (L2, C2), Dist) :- Dist is (abs(L1-L2) + abs(C1-C2)).

% ordena_poss(Poss, Poss_ord, Pos_inicial, Pos_final).

% resolve1(Lab, Pos_inicial, Pos_final, Movs).

% resolve2(Lab, Pos_inicial, Pos_final, Movs).

/* Predicados auxiliares */

% membro/2: verifica se o elemento Elem faz parte da lista dada.
membro(Elem, [Elem | _]).
membro(Elem, [_ | Cauda]) :- membro(Elem, Cauda).