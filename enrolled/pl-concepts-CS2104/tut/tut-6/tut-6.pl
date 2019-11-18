father(john, mary).
father(john, tom).
father(kevin, john).
mother(eva, mary).
mother(eva, tom).
mother(cristina, john).
mother(cristina, kelly).
mother(kelly, alloy).
male(john).
male(kevin).
male(tom).
female(X):-atom(X),not(male(X)). /* works for known X */
parent(X,Y) :- father(X,Y).
parent(X,Y) :- mother(X,Y).
grandparent(X,Y) :- parent(X,Z),parent(Z,Y).
daughter(X,Y) :- female(X),parent(Y,X).
sibling(X,Y) :- parent(Z,X),parent(Z,Y),X\==Y.
true_sibling(X,Y) :- father(Z,X),father(Z,Y),X\==Y,mother(M,X),mother(M,Y).
ancestor(X,Y) :- parent(X,Y).
ancestor(X,Y) :- parent(X,Z),ancestor(Z,Y).

/* Q1 */
/* Q1. (Answer and explanation)
grandparent(G,mary).


ancestor(A,mary).


sibling(S,mary).        


*/

/* Q2
   aunt(X,Y) means X is aunt of Y
   cousin(X,Y) means X is cousin of Y
   nephew(X,Y) means X is nephew of Y
 Many possible answers since conjunction is
 in general commutative.
 Below is one of the possible answers.
*/
aunt(X,Y) :- fail.
cousin(X,Y) :- fail.
nephew(X,Y) :- fail.


sel(X,[X|_]).
sel(X,[_|T]) :- sel(X,T).
remDupl([],[]).
remDupl([H|T],R) :- sel(H,T), remDupl(T,R).
remDupl([H|T],[H|R]) :- remDupl(T,R).


append([],Y,Y).
append([X|Xs],Y,[X|Rs]):-append(Xs,Y,Rs).
rev([],[]).
rev([X|Xs],Y) :- rev(Xs,Y2),append(Y2,[X],Y).
rev3([],[]).
rev3([X|Xs],Y) :- append(Y2,[X],Y),rev3(Xs,Y2).
rev2([],[]).
rev2(.(X,Xs),Y) :- rev2(Xs,Y2),append(Y2,[X],Y).
fact(0,1).
fact(N,R) :- N>0, M is N-1, fact(M,R1), R is N*R1.
select2(X,[X]).
select2(X,[_|T]):-select2(X,T).

/* Q3
 last(Xs,X) :- X is the last element of Xs
 len(Xs,N) - N is the length of Xs
 nth(Xs,I,X) - X is the I-th element of Xs, starting from 0.
 occurs(Ys,X,N) - X occurs N-times in Ys.
 You may use the finite constraint solving package.

   last([X]) ==> X
   last([X|T]) ==> last(T)
*/

last(X,Y):-fail.

len(X, N) :- fail.

nth(XS,N,Z) :- fail.

occurs(Xs,X,N) :- fail.

/* Q4
*/

gcd(X,T,Z) :- fail.

/* Q5
   Hint: you may use mod operation
   ?- divisors(30,X).
	X = [1,2,3,5,6,10,15,30]
*/


divisors(X, L) :- fail.









