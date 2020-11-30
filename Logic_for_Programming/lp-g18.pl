/**********************************************/
/*                  Group 18                  */
/* 57842 Filipa Marques - 84708 David Coimbra */
/**********************************************/

/**********************/
/* Main program */
/**********************/

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% movs_possiveis/4(Lab, Pos_current, Movs, Poss):                                   % 
% given a labyrinth Lab, a position Pos_atual and the movements already done Movs, %
% possible movements are Poss, printed in the order (up, down, left, right).       %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
movs_possiveis(Lab, Pos_curr, Movs, Poss) :- 
	adjacent(Pos_curr, Adj), cell_labyrinth(Lab, Pos_curr, Walls), 
	selects_if_no_walls(Walls, Adj, PossAux),
	selects_if_not_included(PossAux, Movs, Poss).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% distancia/3:((L1,C1), (L2,C2), Dist):                       %
% calculates Dist as the distance between (L1,C1) and (L2,C2). %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
distancia((L1,C1), (L2,C2), Dist) :- 
	Dist is (abs(L1 - L2) + abs(C1 - C2)).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% ordena_poss/4(Poss, Poss_srt, Pos_beg, Pos_end):                        %
% Poss_srt is the result of sorting the possible movements Poss,        %
% by ascending order of distance to Pos_end and, if equal,              %
% by descending order of distance to Pos_beg.                           %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
ordena_poss([], [],_,_).
ordena_poss([PossH|PossT], Poss_srt, Pos_beg, Pos_end) :-
	ordena_poss(PossT, Poss_srtAux, Pos_beg, Pos_end),
	inserts_in_sorted_list(Poss_srtAux, PossH, Poss_srt, Pos_beg, Pos_end).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	
% resolve1/4(Lab, Pos_beg, Pos_end, Movs):                        %
% Movs is the sequence of movements correspondent to the solution %
% of the labyrinth Lab from the starting position Pos_beg to the  %
% final position Pos_end, never repeating                         %
% a cell and following the order (up, down, left, right).         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
resolve1(Lab, (Xi,Yi), Pos_end, Movs) :- 
	movs_possiveis(Lab, (Xi,Yi), [(i,Xi,Yi)], Poss),
	resolve1(Lab, Poss, Pos_end, [(i,Xi,Yi)], Movs). /* evaluates the rest of the labyrinth */
	
	/* base case: when the current position is equal to Pos_end. */
resolve1(_, [(D, Xf, Yf)|_], (Xf, Yf), Movs_curr, Movs) :-
	append(Movs_curr, [(D,Xf,Yf)], Movs). 

resolve1(Lab, [(D_next_pos, X_next_pos, Y_next_pos)|Other_poss], Pos_end, Movs_curr, Movs):-
	(	/* Adds the first movement to Movs and 
		   evaluates the rest of the labyrinth from there. */
		append(Movs_curr, [(D_next_pos, X_next_pos, Y_next_pos)], Movs_most_recent),
		movs_possiveis(Lab, (X_next_pos, Y_next_pos), Movs_most_recent, Movs1),
		resolve1(Lab, Movs1, Pos_end, Movs_most_recent, Movs_res),
			Movs = Movs_res
	);
		/* if the first movement does not result in a solution, evaluate the remaining possible movements.*/
	resolve1(Lab, Other_poss, Pos_end, Movs_curr, Movs).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% resolve2/4(Lab, Pos_beg, Pos_end, Movs):                                        %
% Movs is the sequence of movements corresponding to the solution                 %
% of the labyrinth Lab from the initial position Pos_inicial to the               %
% final position Pos_end, by the criteria of resolve1/4 and ordena_poss/4.          %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
resolve2(Lab, (Xi, Yi), Pos_end, Movs) :- 
	movs_possiveis(Lab, (Xi,Yi), [(i,Xi,Yi)], Poss),   /* Begins by evaluating the possible movements from the initial position */
	ordena_poss(Poss, Poss_srt, (Xi,Yi), Pos_end),
	resolve2(Lab, Poss_srt, (Xi,Yi), Pos_end, [(i,Xi,Yi)], Movs).
	
	/* base case: when the current position is equal to Pos_end. */
resolve2(_, [(D,Xf,Yf)|_],_, (Xf,Yf), Movs_curr, Movs) :-
	append(Movs_curr, [(D,Xf,Yf)], Movs). /* In this case, Movs will be all movements already determined, plus the correspondent to the final position.*/

resolve2(Lab, [(D_next_pos, X_next_pos, Y_next_pos)|Other_poss], Pos_beg, Pos_end, Movs_curr, Movs) :-
	(	/* Adds the first movement to Movs and 
		   evaluates the rest of the labyrinth from there. */
		append(Movs_curr, [(D_next_pos, X_next_pos, Y_next_pos)], Movs_most_recent),
		movs_possiveis(Lab, (X_next_pos, Y_next_pos), Movs_most_recent, Movs1),
		ordena_poss(Movs1, Poss_srt1, Pos_beg, Pos_end),
		resolve2(Lab, Poss_srt1, Pos_beg, Pos_end, Movs_most_recent, Movs_res),
			Movs = Movs_res
	);
		/* if the first movement does not result in a solution, evaluate the remaining possible movements.*/
	resolve2(Lab, Other_poss, Pos_beg, Pos_end, Movs_curr, Movs).

/*************************/
/*  Auxilliary predicates */
/*************************/

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% adjacent/2((L, C), Adj):                      							    %
% true if the list Adj is a list of adjacent cells to the cell (L, C)           %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
adjacent((L, C), Adj) :- 
	Lup is L-1, Ldown is L+1, Cleft is C-1, Cright is C+1,
	Adj = [(c, Lup, C), (b, Ldown, C), (e, L, Cleft), (d, L, Cright)].

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% selects_if_not_included/3(Adj, Movs, Res):           %
% true if Res corresponds to all movements             %
% in Adj that are not included in Movs.                %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
selects_if_not_included([AdjH|AdjT], Movs, Res) :- 
	selects_if_not_included([AdjH|AdjT], Movs, Res, []).

selects_if_not_included([],_, Acc, Acc):- !.

selects_if_not_included([(AdjD, AdjX, AdjY)|AdjT], Movs, Res, Acc) :- 
  ( \+(memberchk((_, AdjX, AdjY), Movs)), append(Acc, [(AdjD, AdjX, AdjY)], Acc1),
	selects_if_not_included(AdjT, Movs, Res, Acc1), ! );
  % or:
	selects_if_not_included(AdjT, Movs, Res, Acc).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% cell_labyrinth/3(Lab, (L, C), Cell):                  %
% Cell corresponds to position (L, C) of labyrinth Lab. %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
cell_labyrinth(Lab, (L,C), Cell) :- 
	nth1(L, Lab, Line,_), nth1(C, Line, Cell,_).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
% no_walls/2(Mov, Cel):  										     %
% true if there is no wall in the direction of the given movement.   %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
no_walls(_, []).

no_walls((D,_,_), [CelH|CelT]) :- 
	CelH \== D, no_walls((D,_,_), CelT).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% selects_if_no_walls/3(Walls, MovsPoss, Res):                         %
% true if Res corresponds to all          				               %
% moviments of MovsPoss that are not blocked by walls.                 %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
selects_if_no_walls(Walls, MovsPoss, Res) :- 
	selects_if_no_walls(Walls, MovsPoss, Res, []).

selects_if_no_walls(_, [], Acc, Acc):- !.

selects_if_no_walls(Walls, [MovsPossH|MovsPossT], Res, Acc) :- 
	(no_walls(MovsPossH, Walls),
	append(Acc, [MovsPossH], Acc1),
	selects_if_no_walls(Walls, MovsPossT, Res, Acc1), !);
  % or:
	(\+(no_walls(MovsPossH, Walls)),
	selects_if_no_walls(Walls, MovsPossT, Res, Acc)).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% dist2cells/4((L, C), (_, L1, C1), (_, L2, C2), Dist):  %
% calculates the difference between the distances        %
% of two movements relatively to a cell.		         % 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
dist2cells((L,C), (_, L1,C1), (_, L2,C2), Dist) :-
	distancia((L1,C1), (L,C), Dist1),
	distancia((L2, C2), (L, C), Dist2),
	Dist is (Dist1 - Dist2).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% eval_letter/3((D1,L1,C1), (D2,L2,C2), Letters):                                   %
% true if the movement (D2,L2,C2) has priority relative to the movement (D2,L2,C2), %
% with respect to the direction of the movement.                                    %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
eval_letter((D1,L1,C1), (D2,L2,C2), [LettersH|LettersT]) :-
	(D1  == LettersH, !);
	(D2 \== LettersH, eval_letter((D1,L1,C1),(D2,L2,C2), LettersT)).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% has_priority/4(M1, M2, Pos_beg, Pos_end):        % 
% true if M1 has priority relative to M2.          %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
has_priority(M1, M2, Pos_beg, Pos_end) :-
	dist2cells(Pos_end, M1, M2, Dist_end),
   ( (Dist_end <  0, !);
	
   ( (Dist_end == 0, dist2cells(Pos_beg, M1, M2, Dist_beg)),
   (  Dist_beg >  0;
	( Dist_beg == 0, eval_letter(M1, M2, [c,b,e,d]))
	) ) ).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	
% inserts_in_sorted_list/5([H|T], Elem, Res, Pos_beg, Pos_end):                       %
% given a list [H|T], an element Elem and a predicate of priorities,                  %
% Res is the resultat of inserting element Elem in the list, in the correct position. %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
inserts_in_sorted_list([], Elem, [Elem],_,_).
inserts_in_sorted_list([H|T], Elem, Res, Pos_beg, Pos_end):-
	(has_priority(Elem, H, Pos_beg, Pos_end), Res = [Elem|[H|T]], !);
	(inserts_in_sorted_list(T, Elem, Res1, Pos_beg, Pos_end), Res = [H|Res1] ).
