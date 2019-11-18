(* 
   Tutorial 9 : Imperative Programming with OCaml
   Week of 29th Oct 2019 

  Q1 : Consider a tail-recursive method.
  Re-implement this method using an iterative loop together
  with two imperative ref type values that are internal to the method.
  You can try the online interpreter below.
  https://repl.it/languages/ocaml
*)

let fib n =
  let rec aux n a b =
    if n<=0 then (a,b)
    else aux (n-1) (a+b) a
 in fst(aux n 1 0);;

let fib_imp n =
  failwith "TBI"


(* 
   Q2 : The for-loop is implemented as follows.
   What is the polymoprhic type of this method?
*)

let for_loop init final stmt =
  let rec aux i =
    if i<=final then (stmt i; aux (i+1))
    else ()
  in aux init


(* 
   Q3 :  Write two higher-order methods that would
   implement a for-down-to loop iterator, and a while loop method *)

let while_loop t1 body =
  failwith "TBI"
;;


let for_loop init final stmt =
  failwith "TBI"
;;


(* 
   Q4 : Implement fib function with memoization 
   using a hash table. 

   How does this compares with your implementation in Q1?
*)

let fib8 n =
  let h = Hashtbl.create 10 in
  let rec aux n =
    if n<=1 then 1
    else 
      aux (n-1) + aux (n-2)
  in aux n;;

