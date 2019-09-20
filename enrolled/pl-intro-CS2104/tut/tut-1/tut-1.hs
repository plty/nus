-- Tutorial 1 : Introduction to Haskell
-- Use GHCi (ghci in command line); this is not intended to be compiled by ghc.
-- Execute the lines of expressions one by one
-- The purpose of this tutorial is to familiarized yourself with Haskell

-- "Hello to Lab1/Tutorial1"
-- Goto: 
--   https://repl.it/languages/haskell
-- put this code in main.hs, and press 
--   run
--------------------------------------------
-- Q1 (Scope). Consider the expression below

import Prelude hiding (even,odd,pi)

x = 2

y = let x=3 in x+1

z = x+5

-- What are the values of x, y and z after the definition of z?
-- How many let definitions are there, and what are their scopes?

-- > x is 2, y is 4, z is 7.
-- > one, x is local.

-- let x = x * 4 in x + y

-- Which x is being referenced in the last line?
-- What is the behavior of the last execution? Why?

-- > The one on the same line.
-- > Infinite loop.


---------------------------------------------
-- Q2 (Type). Consider the functions below

goo1 x y = x
goo2 x = \ y -> x
goo3 (x,y) = x

-- What are the types of these three functions?
-- You can check the type of expression in ghci, say of goo1, by :type goo1 or :t goo1.
-- Are these types general?
-- Give some examples of calls to these functions?
-- Describe in words what are the intentions of these functions?
-- How are these functions related to each other?

-- > goo1 and goo2 are p1 -> p2 -> p1, goo3 accepts tuple of (a, b) and returns a
-- > yes.
-- > consumes, a and b, and it returns a
-- > serves the same purpose

---------------------------------------------
-- Q3 (Type). Consider the function below

foo1 x = x + 1
foo2 x = x ++ " there"

-- What are the types of these two function?
-- Give some examples of valid arguments that can
--  be used by foo1 and foo2 methods?
--  e.g foo1 2.5, foo1 4, foo1 "hello"
-- What is [Char]? Is this the same as String?
-- You may get same information about type t using :info t
-- > foo1 is Num a => a -> a
-- > foo2 is [Char] -> [Char]
-- > nothing that can be consumed byy foo1 can be consumed by foo2
-- > Yes, [Char] is the same as String

---------------------------------------------
-- Q4 (Type). Consider this set of mutual recursive functions
-- Take note that multi-line commands are entered in ghci with
-- the help of commands :{ and  :}

even :: Int -> Bool
even n = if n==0 then True else odd (n - 1)
odd :: Int -> Bool
odd n = if n==0 then False else even (n - 1)


-- Illustrate some valid calls to these functions, such as odd 3, even 3
-- Explain what happens when you have negative inputs to these functions?
-- What are the types of these functions?
-- Explain what happens when type declarations of form e::t 
--   are not provided. 

-- > odd(3) -> even(2) -> odd(1) -> even(0) -> True
-- > Int -> Bool
-- > odd became (Eq a, Num a) => a -> Bool

------------------------------------------
-- Q5. Consider the maxlist function below for a non-empty list of numbers.
-- take note that

maxlist [] = error "empty list";
maxlist (x:[]) = x;
maxlist (x:ys) = max x (maxlist ys)

--  maxlist [3,16,1]

-- (i) What is the type of this function?
-- (ii) Explain what happen when an empty list is supplied as the input?
-- (iii) Is it possible for recursive call, maxlist ys, in the third line
-- to throw an exception? Is it possible for ys to be []?

-- > the type is Ord a => [a] -> a
-- > it threw an error
-- > yes.
-- > no, as the matching tries to match from top down, if ys is [], it will be catched by maxlist(x:[]) instead of maxlist(x:ys)

------------------------------------------
-- Q6. Rewrite maxlist function to use an auxiliary recursive
--     function, as follows:


maxlist2 xs =
    let aux [] sofar = sofar
        aux (x:y) sofar = aux y (max sofar x)
    in
    case xs of
    [] -> error "empty list"
    (x:ys) -> aux ys x


-- maxlist2 [3,16,1]

-- How is this function different from the version in Q7
-- Is it better?

-- > apart from maxlist2 right-recursive and maxlist is left-recursive, maxlist2 is more memory eff.
-- > the compiler could optimize it with TCO and using constant stack memory
-------------------------------------------
-- Q7 : The maxlist function currently has type: [Int] -> Int.
--      Rewrite it to a version that is based on the following Maybe type.
--      data Maybe a = Nothing | Just a

maxlist3 xs =
     case xs of
         [] -> Nothing
         (x:[]) -> Just x
         (x:ys) -> max (Just x) (maxlist3 ys)

-- The function 'show' converts a number into a string or [Char]
-- The function 'print' outputs any thing (almost) to the standard output
-- The function (++) concatenates two strings

-- print ("Q7a Ans (maxlist3 [3,16,1] = " ++ (show (maxlist3 [3,16,1])))
-- > "Q7a Ans (maxlist3 [3,16,1] = Just 16"
-- print ("Q7a Ans (maxlist3 [] = " ++ (show (maxlist3 [])))
-- > "Q7a Ans (maxlist3 [] = Nothing"

maxlist4 xs =
    let aux [] sofar = sofar
        aux (x:y) sofar = aux y (max sofar x)
    in
    case xs of
    [] -> Nothing
    (x:ys) -> Just (aux ys x)


-- print ("Q7b Ans (maxlist4 [3,16,1] = " ++ (show (maxlist4 [3,16,1])))
-- > "Q7b Ans (maxlist4 [3,16,1] = Just 16"
-- print ("Q7b Ans (maxlist4 [] = " ++ (show (maxlist4 [])))
-- > "Q7b Ans (maxlist4 [] = Nothing"

-------------------------------------------
-- Q8 : Using function for abstraction.
-- Consider a simple code to compute the sum of 
-- the areas for three circles.
--
-- How would could you generalize the code 
-- to make it more abstract
-- so that it could compute the sum for an
-- arbitrary number of circules instead.
-- Hint : Use a list & use functions!

pi = 3.14159
r1 = 1.2 
r2 = 3.888
r3 = 10.0
three_circle = pi*r1*r1 + pi*r2*r2 * pi*r3*r3

-- > solution:
circ_area rs =
    let aux [] sum = sum
        aux (x:y) sum = aux y (sum + pi * y * y)
    in aux rs 0


-------------------------------------------
-- Q9 : Implement the Babylonian method for square root in
-- Haskell without using loops! Please use a recursive method
-- instead. What would be its expected type?

-- > solution:
eps = 0.0000001
sqrt v =
    let aux x v =
            case abs (x - v * v) < eps of
                True -> v
                False -> (v + x / v) / 2
    in aux v v


main = putStrLn "Tut1 exercise as main"
