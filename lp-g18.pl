/*                  Grupo 18                  */
/* 57842 Filipa Marques - 84708 David Coimbra */

/* Programa principal */

% movs_possiveis/4: dados um labirinto Lab, uma posicao Pos_atual e os movimentos ja efetuados Movs, os movimentos possíveis sao Poss.
movs_possiveis(Lab, Pos_actual, Movs, Poss) :- adjacentes(Pos_actual, Adj), celula_labirinto(Lab, Pos_actual, Paredes), 
											   seleciona_se_sem_paredes(Paredes, Adj, PossAux),
											   seleciona_se_nao_pertence(PossAux, Movs, Poss).

% distancia/3: calcula Dist como a distancia entre (L1, C1) e (L2, C2).
distancia((L1, C1), (L2, C2), Dist) :- Dist is (abs(L1 - L2) + abs(C1 - C2)).

% ordena_poss(Poss, Poss_ord, Pos_inicial, Pos_final).

% resolve1(Lab, Pos_inicial, Pos_final, Movs).

% resolve2(Lab, Pos_inicial, Pos_final, Movs).

/* Predicados auxiliares */

% pos_i/3: determina o valor da lista no indice I dado.
pos_i([Cabeca|_], 1, Cabeca). % caso de paragem
pos_i([Cabeca|Cauda], I, Elemento) :- length([Cabeca|Cauda], Comp), I =< Comp,
									  IAux is (I - 1),
									  pos_i(Cauda, IAux, Elemento).

% adjacentes/2: verdadeiro se a lista Adj for uma lista de pontos adjacentes ao ponto (L, C)
adjacentes((L, C), [AdjH|AdjT]) :- Lcima is L-1, Lbaixo is L+1, Cesq is C-1, Cdir is C+1,
							       AdjH = (c, Lcima, C),
							       AdjT = [(b, Lbaixo, C), (e, L, Cesq), (d, L, Cdir)].

% nao_pertence/2: verdadeiro se um certo movimento nao pertencer a lista de movimentos dada
nao_pertence(_, []). % caso de paragem 
nao_pertence((_, L, C), [(_, Linha, Coluna)|MovsT]) :- (L \== Linha ; C \== Coluna),
													   nao_pertence((_, L, C), MovsT).
													  
%seleciona_se_nao_pertence/3: verdadeiro se Res corresponder a todos os movimentos de Adj cujas posicoes nao pertencem a Movs
seleciona_se_nao_pertence([AdjH|AdjT], Movs, Res) :- seleciona_se_nao_pertence([AdjH|AdjT], Movs, Res, []).
seleciona_se_nao_pertence([],_, Acc, Acc).
seleciona_se_nao_pertence([AdjH|AdjT], Movs, Res, Acc) :- (nao_pertence(AdjH, Movs),
													      concatena(Acc, [AdjH], Acc1),
													      seleciona_se_nao_pertence(AdjT, Movs, Res, Acc1));
													      % ou:
													      (\+(nao_pertence(AdjH, Movs)),
													      seleciona_se_nao_pertence(AdjT, Movs, Res, Acc)).

% concatena/3: corresponde a concatenacao das Listas 1 e 2.
concatena([], NovaCauda, NovaCauda).
concatena([Lista1H|Lista1T], Lista2, [Lista1H|Res]) :- concatena(Lista1T, Lista2, Res).

% celula_labirinto/3: Celula corresponde a posicao (L, C) do labirinto Lab.
celula_labirinto(Lab, (L, C), Celula) :- pos_i(Lab, L, Linha), pos_i(Linha, C, Celula).

% sem_paredes/2: verdadeiro se nao existir uma parede na direcao do movimento dado.
sem_paredes(_, []).
sem_paredes((D,_,_), [CelH|CelT]) :- CelH \== D, sem_paredes((D,_,_), CelT).

%seleciona_se_sem_paredes/3: verdadeiro se Res corresponder a todos os movimentos de MovsPoss cuja direcao nao esta bloqueada por paredes.
seleciona_se_sem_paredes(Paredes, MovsPoss, Res) :- seleciona_se_sem_paredes(Paredes, MovsPoss, Res, []).
seleciona_se_sem_paredes(_, [], Acc, Acc).
seleciona_se_sem_paredes(Paredes, [MovsPossH|MovsPossT], Res, Acc) :- (sem_paredes(MovsPossH, Paredes),
																	  concatena(Acc, [MovsPossH], Acc1),
																	  seleciona_se_sem_paredes(Paredes, MovsPossT, Res, Acc1));
																	  % ou:
																	  (\+(sem_paredes(MovsPossH, Paredes)),
																	  seleciona_se_sem_paredes(Paredes, MovsPossT, Res, Acc)).

% [[[d,e,c],[e,b,c],[b,c],[c],[d,c],[d,e,c]], [[e,b],[b,c],[b,c],[],[b],[d,b]], [[e,c],[b,c],[b,c],[b],[b,c],[d,b,c]], [[d,e],[e,c],[c],[c],[c],[d,c]], [[e,b],[d,b],[e,b],[b],[b],[d,b]]]

% membro(Elem, [Elem|_]). % caso de paragem
% membro(Elem, [_|Cauda]) :- membro(Elem, Cauda).

% remove(Cabeca,[Cabeca|Cauda],Cauda).
% remove(X,[Cabeca|Cauda1],[Cabeca|Cauda2]) :- remove(X,Cauda1,Cauda2).