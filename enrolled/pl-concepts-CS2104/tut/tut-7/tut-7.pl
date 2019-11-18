:- use_module(library(clpfd)).

appendall([],[]).
appendall([L|Ls],R) :- appendall(Ls,R1),append(L,R1,R).

/*
1.	The Sagittarius owes more money than Landyn. 
2.	The person who ordered soup of the day as an appetizer owes less money than the Aries. 
3.	The patient with the $550 hospital bill didn't order soup of the day. 
4.	The 5 people were the patient with the $760 hospital bill, the person who ordered soup of the day as an appetizer, Skylar, the Pisces, and the patient with the $960 hospital bill. 
5.	Either the Sagittarius or the Virgo ordered fried pickles. 
6.	The Pisces didn't order buffalo wings. 
7.	The person who ordered fried pickles as an appetizer is Kayla. 
8.	The patient with the $960 hospital bill is the Virgo. 
9.	Kayla owes less money than Skylar. 
10.	Of Perla and the person who ordered potato skins as an appetizer, one has the $1610 hospital bill and the other has the $960 hospital bill. 
*/

main(Answer) :-
    Answer    = [ [ 'Jorge'=Jorge,
                    'Kayla'=Kayla,
                    'Landyn'=Landyn,
                    'Perla'=Perla,
                    'Skylar'=Skylar],
                  [ 'BuffaloWings'=Wings,
                    'FriedMozarella'=Mozarella,
                    'FriedPickles'=Pickles,
                    'PotatoSkins'=PSkins,
                    'SoupofDay'=Soup],
                  [ 'Aries'=Aries,
                    'Leo'=Leo,
                    'Pisces'=Pisces,
                    'Sagittarius'=Sagittarius,
                    'Virgo'=Virgo]],
    Names = [Jorge,Kayla,Landyn,Perla,Skylar],
    Appetizers = [Wings,Mozarella,Pickles,PSkins,Soup],
    Signs = [Aries,Leo,Pisces,Sagittarius,Virgo],
    Bills=[B550,B760,B960,B1610,B2500],
    Bills=[1,2,3,4,5],
    /*B550=1,B760=2,B960=3,B1610=4,B2500=5,*/
    L=[Names,Appetizers,Signs],
    appendall(L,All),
    All ins 1..5,
    all_different(Names), all_different(Appetizers),
    all_different(Signs),
    Sagittarius #> Landyn,
    label(All).

   
/*   
sortanswer(L,R) :- sort(2,<,L,R).
:- main(L), 
   maplist(sortanswer,L,A),
   print_list(A), 
   halt.
*/
