/*                  Grupo 18                  */
/* 57842 Filipa Marques - 84708 David Coimbra */

/* Programa principal */

% movs_possiveis/4: dados um labirinto Lab, uma posicao Pos_atual e os movimentos ja efetuados Movs,
% os movimentos possiveis sao Poss.
movs_possiveis(Lab, Pos_actual, Movs, Poss) :- 
	adjacentes(Pos_actual, Adj), celula_labirinto(Lab, Pos_actual, Paredes), 
	seleciona_se_sem_paredes(Paredes, Adj, PossAux),
	seleciona_se_nao_pertence(PossAux, Movs, Poss).

% distancia/3: calcula Dist como a distancia entre (L1, C1) e (L2, C2).
distancia((L1, C1), (L2, C2), Dist) :- 
	Dist is (abs(L1 - L2) + abs(C1 - C2)).

% ordena_poss/4:.
ordena_poss([], [], _, _).
ordena_poss([PossH|PossT], Poss_ord, Pos_inicial, Pos_final):-
	ordena_poss(PossT, Poss_ordAux, Pos_inicial, Pos_final),
	insere_em_lista_ordenada(Poss_ordAux, PossH, Poss_ord, Pos_inicial, Pos_final).
	
% resolve1(Lab, Pos_inicial, Pos_final, Movs).

% resolve2(Lab, Pos_inicial, Pos_final, Movs).

/* Predicados auxiliares */

% pos_i/3: determina o valor da lista no indice I dado.
pos_i([Cabeca|_], 1, Cabeca).

pos_i([Cabeca|Cauda], I, Elemento) :- 
	length([Cabeca|Cauda], Comp), I =< Comp,
	IAux is (I - 1),
	pos_i(Cauda, IAux, Elemento).

% adjacentes/2: verdadeiro se a lista Adj for uma lista de pontos adjacentes ao ponto (L, C).
adjacentes((L, C), Adj) :- 
	Lcima is L-1, Lbaixo is L+1, Cesq is C-1, Cdir is C+1,
	Adj = [(c, Lcima, C), (b, Lbaixo, C), (e, L, Cesq), (d, L, Cdir)].

% nao_pertence/2: verdadeiro se um certo movimento nao pertencer a lista de movimentos dada.
nao_pertence(_, []). 

nao_pertence((_, L, C), [(_, Linha, Coluna)|MovsT]) :- 
	(L \== Linha ; C \== Coluna),
	nao_pertence((_, L, C), MovsT).

%seleciona_se_nao_pertence/3: verdadeiro se Res corresponder a todos os 
%movimentos de Adj cujas posicoes nao pertencem a Movs.
seleciona_se_nao_pertence([AdjH|AdjT], Movs, Res) :- 
	seleciona_se_nao_pertence([AdjH|AdjT], Movs, Res, []).

seleciona_se_nao_pertence([],_, Acc, Acc).

seleciona_se_nao_pertence([AdjH|AdjT], Movs, Res, Acc) :- 
	(nao_pertence(AdjH, Movs),
	concatena(Acc, [AdjH], Acc1),
	seleciona_se_nao_pertence(AdjT, Movs, Res, Acc1));
  % ou:
	(\+(nao_pertence(AdjH, Movs)),
	seleciona_se_nao_pertence(AdjT, Movs, Res, Acc)).

% concatena/3: corresponde a concatenacao das Listas 1 e 2.
concatena([], NovaCauda, NovaCauda).

concatena([Lista1H|Lista1T], Lista2, [Lista1H|Res]) :- 
	concatena(Lista1T, Lista2, Res).

% celula_labirinto/3: Celula corresponde a posicao (L, C) do labirinto Lab.
celula_labirinto(Lab, (L, C), Celula) :- 
	pos_i(Lab, L, Linha), pos_i(Linha, C, Celula).

% sem_paredes/2: verdadeiro se nao existir uma parede na direcao do movimento dado.
sem_paredes(_, []).

sem_paredes((D,_,_), [CelH|CelT]) :- 
	CelH \== D, sem_paredes((D,_,_), CelT).

%seleciona_se_sem_paredes/3: verdadeiro se Res corresponder a todos os 
%movimentos de MovsPoss cuja direcao nao esta bloqueada por paredes.
seleciona_se_sem_paredes(Paredes, MovsPoss, Res) :- 
	seleciona_se_sem_paredes(Paredes, MovsPoss, Res, []).

seleciona_se_sem_paredes(_, [], Acc, Acc).

seleciona_se_sem_paredes(Paredes, [MovsPossH|MovsPossT], Res, Acc) :- 
	(sem_paredes(MovsPossH, Paredes),
	concatena(Acc, [MovsPossH], Acc1),
	seleciona_se_sem_paredes(Paredes, MovsPossT, Res, Acc1));
  % ou:
	(\+(sem_paredes(MovsPossH, Paredes)),
	seleciona_se_sem_paredes(Paredes, MovsPossT, Res, Acc)).

% dist2pontos/4: calcula as distancias de dois movimentos relativamente a 
%uma posicao, e devolve a diferenca das mesmas.
dist2pontos((L, C), (_, L1, C1), (_, L2, C2), Dist) :-
	distancia((L1, C1), (L, C), Dist1),
	distancia((L2, C2), (L, C), Dist2),
	Dist is Dist1-Dist2.

% avalia_letra/3: devolve true caso o mov1 = (D1,L1,C1) seja prioritario ao mov2 = (D2, L2,C2).
avalia_letra((D1,L1,C1), (D2,L2,C2), [LetrasH|LetrasT]) :-
	(D1 == LetrasH, !);
	(D2 \== LetrasH, avalia_letra((D1,L1,C1),(D2,L2,C2), LetrasT)).

%e_prioritario/4: retorna true se M1 for prioriario a M2 e false caso contrario.
e_prioritario(M1, M2, Pos_inicial, Pos_final) :-
	dist2pontos(Pos_final, M1, M2, Dist_fin),
   ( (Dist_fin < 0, !);
	
   ( (Dist_fin == 0, dist2pontos(Pos_inicial, M1, M2, Dist_ini)),
   ( Dist_ini > 0;
	(Dist_ini == 0, avalia_letra(M1, M2, [c,b,e,d]))
	) ) ).
	
%insere_em_lista_ordenada/5: dadas uma lista, um elemento e um predicado de prioridades,
%devolve em Res o resultado da insercao do elemento X na lista, na posicao correcta.
insere_em_lista_ordenada([], X, [X],_,_).
insere_em_lista_ordenada([H|T], X, Res, Pos_inicial, Pos_final):-
	(e_prioritario(X, H, Pos_inicial, Pos_final), Res = [X|[H|T]], !);
	(insere_em_lista_ordenada(T, X, Res1, Pos_inicial, Pos_final), Res = [H|Res1] ).

%[[[d,e,c],[e,b,c],[b,c],[c],[d,c],[d,e,c]], [[e,b],[b,c],[b,c],[],[b],[d,b]], [[e,c],[b,c],[b,c],[b],[b,c],[d,b,c]], [[d,e],[e,c],[c],[c],[c],[d,c]], [[e,b],[d,b],[e,b],[b],[b],[d,b]]].

% membro(Elem, [Elem|_]). 
% membro(Elem, [_|Cauda]) :- membro(Elem, Cauda).

% remove(Cabeca,[Cabeca|Cauda],Cauda).
% remove(X,[Cabeca|Cauda1],[Cabeca|Cauda2]) :- remove(X,Cauda1,Cauda2).