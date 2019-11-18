(* 

   Week of 4 Nov 2019 - to do Q1 and discuss Lab4
   
   Tutorial 11 : OOP/Modules with OCaml
   
   Week of 12 Nov 2019

*)

(*
Q1.

 Consider a generic memoizing function

    memo_fn : ('a -> 'b) -> 'a -> 'a

  Show how this may be used to memoize fibonacci function

*)

let memo_fn f =
  let h = Hashtbl.create 10 in
  (fun n ->
      try
       Hashtbl.find h n 
      with _ ->
       let r = f n in
       let () = print_endline ("hash add "^(string_of_int n)) in
       let () = Hashtbl.add h n r in
       r)
(*

Comment on the Objects and Modules for OCaml below.

You have been given a imperative stack of integers,
called stack_int, as an OO type class.

(1) Study the stack carefully, and describe which are
    pure vs imperative methods. Take note of mutable
    variables inside objects

(2) You are tasked to make a new polymorphic stack by
    declaring instead:
        class ['a] stack
    Create such a polymorphic stack.
    Write test harness that would then creates two stacks:
      (i) a stack of integer and 
      (ii) another stack of strings.

(3) The current stack is an imperative one with updates.
    Can you design a polymorphic functional stack, call
    it stack_pure, without any updates

class stack_pure_int =
object (self)
  val stk = []
  method push (i:int) = i:stk
  (* make sure modified stack is always returned *)

(4) You are given a module, called Mstack_int, which can be
    used to build an imperative stack of integers; by using
     the command Mstack_int.make.

    Can you re-design a more general polymorphic module,
    calling it Mstack module?

module Mstack_int =
struct
  type t = ('a list) ref
  let make () : t = ref []
  let push (stk:t) (i:'a) : unit = 
    stk := i::!stk


    Contrast the pros and cons of OOP versus modules.

(5) Can you also re-design a functional version of the
    stack module that works for arbitrary types?


module Mstack_int =
struct
  type t = ('a list) 
  let make () : t = []
  let push (stk:t) (i:'a) : t = i::stk

(6) For polymorphic stack, one of the problem we encounter
    is obtaining the string representation of an element
    The show method requires an extra to_string function-type
    parameter.

    We can avoid this problem by using a functor, called Sstack,
    to structure our code. Complete this functor, so that we
    can use it to build stacks which have the show method that
    is imported for another module.


You may compile the code: 
     ocamlc tut11.ml
(which generates a.out bytecode)

To run the code, use:    
     ./a.out

*)

class stack_int =
object (self)
  val mutable stk = []
  method push (i:int) =
    (* push an element into the stack *)
    begin
      stk <- i::stk
    end
  method pop  = 
    (* pop an element from the stack *)
    begin
      match stk with
        | [] -> failwith "empty stack" (* throws an exception *)
        | i::is -> stk <- is
    end
  method top  = 
    (* return an element from the top of the stack *)
    begin
      match stk with
        | [] -> failwith "empty stack"  (* throws an exception *)
        | i::is -> i
    end
  method is_empty  = 
    (* check if the stk is empty or otherwise *)
    stk == []
  method size  = (List.length stk)
  method show  = 
    (* return a string representation of stack *)
    "["^(String.concat "," (List.map string_of_int stk))^"]"
end

let test_prog () = 
  let test_stk = new stack_int in
  test_stk # push 5;
  test_stk # push 2;
  test_stk # push 6;
  print_endline (test_stk # show);
  test_stk # pop;
  print_endline (test_stk # show);
  test_stk # push 1;
  test_stk # push 8;
  print_endline (test_stk # show);
  print_endline ("size of stack is "^(string_of_int (test_stk # size)));;

test_prog();;

module Mstack_int =
struct
  type t = (int list) ref
  let make () : t = ref []
  let push (stk:t) (i:int) : unit = 
    stk := i::!stk
  let pop (stk:t) : unit = 
    begin
      match !stk with
        | [] -> failwith "stack is empty"
        | _::s -> stk := s
    end
  let top (stk:t) : int = 
    match !stk with
      | [] -> failwith "stack is empty"
      | i::s -> i
  let is_empty (stk:t): bool =
    !stk==[]
  let size (stk:t) : int  = (List.length !stk)
  let show (stk:t) : string = 
    "["^(String.concat "," (List.map string_of_int !stk))^"]"
end

module type Mstack_int_ADT =
sig
  type t
  val make : unit -> t
  val push : t -> int -> unit
  val pop : t -> unit
end;;


module M2 = (Mstack_int : Mstack_int_ADT) ;;

module M = Mstack_int;;

let test_prog2 () = 
  let test_stk =  M.make () in
  M.push test_stk 5;
  M.push test_stk 2;
  M.push test_stk 6;
  print_endline (M.show test_stk);
  M.pop test_stk;
  print_endline (M.show test_stk);
  M.push test_stk 1;
  M.push test_stk 8;
  print_endline (M.show test_stk);
  print_endline ("size of stack is "^(string_of_int (M.size test_stk)));;

test_prog2();;

module type SHOW_TYPE =
sig
  type t
  val show : t -> string
end;;

module INT_Elem : SHOW_TYPE =
struct
  type t = int
  let show (i:t) = string_of_int i
end;;

(* functor that takes a module of the show type *)
module FStack =
    functor (Elt: SHOW_TYPE) ->
struct
  type elem = Elt.t
  type t = elem list ref
  let show (stk:t) : string = 
    "["^(String.concat "," (List.map Elt.show !stk))^"]"
end;;
