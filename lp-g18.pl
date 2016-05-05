/**********************************************/
/*                  Grupo 18                  */
/* 57842 Filipa Marques - 84708 David Coimbra */
/**********************************************/

/**********************/
/* Programa principal */
/**********************/

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% movs_possiveis/4((Lab, Pos_actual, Movs, Poss):                                  % 
% dados um labirinto Lab, uma posicao Pos_atual e os movimentos ja efetuados Movs, %
% os movimentos possiveis sao Poss, apresentados pela ordem (c, b, e, d).          %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
movs_possiveis(Lab, Pos_actual, Movs, Poss) :- 
	adjacentes(Pos_actual, Adj), celula_labirinto(Lab, Pos_actual, Paredes), 
	seleciona_se_sem_paredes(Paredes, Adj, PossAux),
	seleciona_se_nao_pertence(PossAux, Movs, Poss).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% distancia/3:((L1,C1), (L2,C2), Dist):                  %
% calcula Dist como a distancia entre (L1,C1) e (L2,C2). %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
distancia((L1,C1), (L2,C2), Dist) :- 
	Dist is (abs(L1 - L2) + abs(C1 - C2)).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% ordena_poss/4(Poss, Poss_ord, Pos_inicial, Pos_final):                %
% Poss_ord e' o resultado de ordenar os movimentos possiveis Poss,      %
% por ordem crescente de distancia a Pos_final e, em caso de igualdade, %
% por ordem decrescente de distancia a Pos_inicial.                     %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
ordena_poss([], [],_,_).
ordena_poss([PossH|PossT], Poss_ord, Pos_inicial, Pos_final) :-
	ordena_poss(PossT, Poss_ordAux, Pos_inicial, Pos_final),
	insere_em_lista_ordenada(Poss_ordAux, PossH, Poss_ord, Pos_inicial, Pos_final).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	
% resolve1/4(Lab, Pos_inicial, Pos_final, Movs):             %
% Movs e' a sequencia de movimentos correspondente a solucao %
% do labirinto Lab desde a posicao inicial Pos_inicial a     %
% posicao final Pos_final, nunca passando mais que uma vez   %
% pela mesma celula e seguindo a ordem (c, b, e, d).         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
resolve1(Lab, (Xi,Yi), Pos_final, Movs) :- 
	movs_possiveis(Lab, (Xi,Yi), [(i,Xi,Yi)], Poss),
	resolve1(Lab, Poss, Pos_final, [(i,Xi,Yi)], Movs). /* Avalia o resto do labirinto */
	
	/* caso de paragem: quando a posicao atual for igual a Pos_final. */
resolve1(_, [(D, Xf, Yf)|_], (Xf, Yf), Movs_atuais, Movs) :-
	append(Movs_atuais, [(D,Xf,Yf)], Movs). 

resolve1(Lab, [(D_prox_pos, X_prox_pos, Y_prox_pos)|Outras_poss], Pos_final, Movs_atuais, Movs):-
	(	/* Adiciona o primeiro movimento a Movs e 
		   avalia o resto do labirinto a partir dai. */
		append(Movs_atuais, [(D_prox_pos, X_prox_pos, Y_prox_pos)], Movs_mais_actuais),
		movs_possiveis(Lab, (X_prox_pos, Y_prox_pos), Movs_mais_actuais, Movs1),
		resolve1(Lab, Movs1, Pos_final, Movs_mais_actuais, Movs_res),
			Movs = Movs_res
	);
		/* caso o primeiro movimento nao resultar numa solucao, entao avalia os movimentos possiveis seguintes.*/
	resolve1(Lab, Outras_poss, Pos_final, Movs_atuais, Movs).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% resolve2/4(Lab, Pos_inicial, Pos_final, Movs):                          %
% Movs e' a sequencia de movimentos correspondente a solucao              %
% do labirinto Lab desde a posicao inicial Pos_inicial a                  %
% posicao final Pos_final, pelos criterios de resolve1/4 e ordena_poss/4. %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
resolve2(Lab, (Xi, Yi), Pos_final, Movs) :- 
	movs_possiveis(Lab, (Xi,Yi), [(i,Xi,Yi)], Poss),   /* Comeca por avaliar os movimentos possiveis a partir da posicao inicial */
	ordena_poss(Poss, Poss_ord, (Xi,Yi), Pos_final),
	resolve2(Lab, Poss_ord, (Xi,Yi), Pos_final, [(i,Xi,Yi)], Movs).
	
	/* caso de paragem: quando a posicao atual for igual a Pos_final. */
resolve2(_, [(D,Xf,Yf)|_],_, (Xf,Yf), Movs_atuais, Movs) :-
	append(Movs_atuais, [(D,Xf,Yf)], Movs). /* Nesse caso, os Movs serao todos os ja determinados, mais o correspondente a posicao final.*/

resolve2(Lab, [(D_prox_pos, X_prox_pos, Y_prox_pos)|Outras_poss], Pos_inicial, Pos_final, Movs_atuais, Movs) :-
	(	/* Adiciona o primeiro movimento a Movs e 
		   avalia o resto do labirinto a partir dai. */
		append(Movs_atuais, [(D_prox_pos, X_prox_pos, Y_prox_pos)], Movs_mais_actuais),
		movs_possiveis(Lab, (X_prox_pos, Y_prox_pos), Movs_mais_actuais, Movs1),
		ordena_poss(Movs1, Poss_ord1, Pos_inicial, Pos_final),
		resolve2(Lab, Poss_ord1, Pos_inicial, Pos_final, Movs_mais_actuais, Movs_res),
			Movs = Movs_res
	);
		/* caso o primeiro movimento nao resultar numa solucao, entao avalia os movimentos possiveis seguintes.*/
	resolve2(Lab, Outras_poss, Pos_inicial, Pos_final, Movs_atuais, Movs).

/*************************/
/* Predicados auxiliares */
/*************************/

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% adjacentes/2((L, C), Adj):                      							    %
% verdadeiro se a lista Adj for uma lista de pontos adjacentes ao ponto (L, C). %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
adjacentes((L, C), Adj) :- 
	Lcima is L-1, Lbaixo is L+1, Cesq is C-1, Cdir is C+1,
	Adj = [(c, Lcima, C), (b, Lbaixo, C), (e, L, Cesq), (d, L, Cdir)].

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% seleciona_se_nao_pertence/3(Adj, Movs, Res):         %
% verdadeiro se Res corresponder a todos os movimentos %
% de Adj cujas posicoes nao pertencem a Movs.          %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
seleciona_se_nao_pertence([AdjH|AdjT], Movs, Res) :- 
	seleciona_se_nao_pertence([AdjH|AdjT], Movs, Res, []).

seleciona_se_nao_pertence([],_, Acc, Acc):- !.

seleciona_se_nao_pertence([(AdjD, AdjX, AdjY)|AdjT], Movs, Res, Acc) :- 
  ( \+(memberchk((_, AdjX, AdjY), Movs)), append(Acc, [(AdjD, AdjX, AdjY)], Acc1),
	seleciona_se_nao_pertence(AdjT, Movs, Res, Acc1), ! );
  % ou:
	seleciona_se_nao_pertence(AdjT, Movs, Res, Acc).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% celula_labirinto/3(Lab, (L, C), Celula):              %
% Celula corresponde a posicao (L, C) do labirinto Lab. %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
celula_labirinto(Lab, (L,C), Celula) :- 
	nth1(L, Lab, Linha,_), nth1(C, Linha, Celula,_).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
% sem_paredes/2(Mov, Cel):  										 %
% verdadeiro se nao existir uma parede na direcao do movimento dado. %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
sem_paredes(_, []).

sem_paredes((D,_,_), [CelH|CelT]) :- 
	CelH \== D, sem_paredes((D,_,_), CelT).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% seleciona_se_sem_paredes/3(Paredes, MovsPoss, Res):                  %
% verdadeiro se Res corresponder a todos os         				   %
% movimentos de MovsPoss cuja direcao nao esta bloqueada por paredes.  %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
seleciona_se_sem_paredes(Paredes, MovsPoss, Res) :- 
	seleciona_se_sem_paredes(Paredes, MovsPoss, Res, []).

seleciona_se_sem_paredes(_, [], Acc, Acc):- !.

seleciona_se_sem_paredes(Paredes, [MovsPossH|MovsPossT], Res, Acc) :- 
	(sem_paredes(MovsPossH, Paredes),
	append(Acc, [MovsPossH], Acc1),
	seleciona_se_sem_paredes(Paredes, MovsPossT, Res, Acc1), !);
  % ou:
	(\+(sem_paredes(MovsPossH, Paredes)),
	seleciona_se_sem_paredes(Paredes, MovsPossT, Res, Acc)).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% dist2pontos/4((L, C), (_, L1, C1), (_, L2, C2), Dist): %
% calcula a diferenca entre as distancias                %
% de dois movimentos relativamente a uma posicao.		 %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
dist2pontos((L,C), (_, L1,C1), (_, L2,C2), Dist) :-
	distancia((L1,C1), (L,C), Dist1),
	distancia((L2, C2), (L, C), Dist2),
	Dist is (Dist1 - Dist2).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% avalia_letra/3((D1,L1,C1), (D2,L2,C2), Letras):                               %
% verdadeiro se o movimento (D2,L2,C2) for prioritario ao movimento (D2,L2,C2), %
% relativamente a direcao do movimento.                                         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
avalia_letra((D1,L1,C1), (D2,L2,C2), [LetrasH|LetrasT]) :-
	(D1  == LetrasH, !);
	(D2 \== LetrasH, avalia_letra((D1,L1,C1),(D2,L2,C2), LetrasT)).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% e_prioritario/4(M1, M2, Pos_inicial, Pos_final): %
% verdadeiro se se M1 for prioriario a M2.         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
e_prioritario(M1, M2, Pos_inicial, Pos_final) :-
	dist2pontos(Pos_final, M1, M2, Dist_fin),
   ( (Dist_fin <  0, !);
	
   ( (Dist_fin == 0, dist2pontos(Pos_inicial, M1, M2, Dist_ini)),
   (  Dist_ini >  0;
	( Dist_ini == 0, avalia_letra(M1, M2, [c,b,e,d]))
	) ) ).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	
% insere_em_lista_ordenada/5([H|T], Elem, Res, Pos_inicial, Pos_final):          %
% dadas uma Lista [H|T], um elemento Elem e um predicado de prioridades,         %
% Res e' o resultado da insercao do elemento Elem na lista, na posicao correcta. %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
insere_em_lista_ordenada([], Elem, [Elem],_,_).
insere_em_lista_ordenada([H|T], Elem, Res, Pos_inicial, Pos_final):-
	(e_prioritario(Elem, H, Pos_inicial, Pos_final), Res = [Elem|[H|T]], !);
	(insere_em_lista_ordenada(T, Elem, Res1, Pos_inicial, Pos_final), Res = [H|Res1] ).