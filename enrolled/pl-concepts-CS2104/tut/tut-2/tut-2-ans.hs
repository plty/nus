{-  Tutorial 2 
.
 We will discuss this for the week of 3 Sept 2018
 Please prepare in advance before the tutorial.
 You do not need to submit this tutorial.
 
-}

-- In this section we will focus on simple data type, type of function, algebraic data type, and some application of type.

import Debug.Trace (trace)

-- Q1. Haskell supports various basic data type. For example:
capA :: Char
capA = 'A'

zero :: Int
zero = 0

e :: Float
e = 2.71828

-- Haskell also has types to support very large numbers.
-- The range of Integer is only determined by the machine's available memory

largeN :: Integer
largeN = 1234567890112374756646636363883

-- Q2. Haskell supports polymorphic type

-- Let us define the function 'the3rd' that picks the 3rd element from a 3-tuple data.
-- An example is given below.
-- "(a,b,c) -> c" denotes "forall a,b,c. (a,b,c) -> c"

the3rd :: (a, b, c) -> c
the3rd (_, _, z) = z

test1 = the3rd (26, 2.3, 'z')

-- Q. What is the type of 'test1'?
-- ANS: Type of 'test1': Char

-- Now let us define the following function:

the2ndInList = head . tail

-- Q. Can you infer the type of the above function?
-- ANS: Type of 'the2ndInList': [a] -> a

-- Q. Take a look as the type of (.) (composition):
--   Prelude> :t (.)
--   (.) :: (b -> c) -> (a -> b) -> a -> c
-- Does it remind you of any function you have used before?
-- ANS: It is the function composition operator that is often denoted by a small circle in mathematics.

-- Q. Can you fix the following function using 'the3rd' and 'the2ndInList'?

the3rdOf2nd :: [(a, b, c)] -> c
the3rdOf2nd = the3rd . the2ndInList -- <- ANS

-- Q3. Algebraic Data Type
-- Let us define a fairly complex data type with a number of possible values

data T = AAA | BBB Int | CCC [String] | DDD {name::String, age::Int}
     deriving Show

-- Let us built some simpl

v1 = AAA
v2 = CCC ["hello","cs2104"]
v3 = DDD {name="Ali", age=12}

-- Apply access function name to v1 and v2
-- Explain their different outcomes
e1 = name v2
e2 = name v3

-- Can you write a similar function with the same effect as name?
-- ANS:
name' x = case x of
          DDD {name=y} -> y
          otherwise -> error "the data does not have a field named 'name'"

-- Let us try to build a new record from an existing record, as follows
v4 = v3 {name="Mohd"}

-- What happens if no such record name exist in the said data
v5 = v2 {name="Mohd"}

-- The record update operator v {..} is a syntactic sugar.
-- Can you write a similar function with the same effect as such
-- the record update but without using v {..}? 
-- Is this a pure function?
--Ans : v {name=s}
update v s = case v of 
   DDD {name _, age = a} -> DDD{name=s,age=a}
   _ -> error "the data does not have a field named 'name'"

{- Q4: Consider the append operator 

[] ++ ys     = ys
(x:xs) ++ ys = x:(xs++ys)

Trace through the execution of the following concatenation
and determine how many recursive calls were invoked.

[1,2,3] ++ [6,7,8,9]
=> 1:([2,3] ++ [6,7,8,9])
=> 1:2:([3] ++ [6,7,8,9])
=> 1:2:3:([] ++ [6,7,8,9])
=> 1:2:3:[6,7,8,9]
=> 1:2:[3,6,7,8,9]
=> 1:[2,3,6,7,8,9]
=> [1,2,3,6,7,8,9]

"hello"  ++ "there"
=> 'h':("ello" ++ "there")
=>

-}


{- Q5: Consider the two variants of reverse method -}

reverse' [] = []
reverse' (x:xs) = reverse' xs ++ [x]


revit xs = 
  let 
   aux [] acc = acc
   aux (x:xs) acc = aux xs (x:acc)
  in aux xs []

{-
  Trace through the execution of the following two
  reverse and revit calls to determine how the
  linear recursive and tail-recursive methods work 
  
  
  reverse [1,2,3,4]
  => reverse [2,3,4]++[1]
  => (reverse [3,4]++[2])++[1]
  => ((reverse [4]++[3])++[2])++[1]
  => (((reverse []++[4])++[3])++[2])++[1]
  => ((([]++[4])++[3])++[2])++[1]
  => (([4]++[3])++[2])++[1]
  => ([4,3]++[2])++[1]
  => [4,3,2]++[1]
  => [4,3,2,1]

  
  revit [1,2,3,4]
  => aux [1,2,3,4] []
  => aux [2,3,4] (1:[])
  => aux [3,4] (2:1:[])
  => aux [4] (3:2:1:[])
  => aux [] (4:3:2:1:[])
  => [4,3,2,1]
  
  Which of these two methods have better time-complexity?
  ANS: revit O(n) compared to O(n^2) of reverse.
 -}

{- Q6: Define the naive fibonacci for a series of fibonacci.
At which method call did the interpreter paused for some time?
   ANS: fib1 25 (it will vary based on machine's performance)
 -}

fib1 0 = 0
fib1 1 = 1
fib1 n = fib1 (n-1) + fib1 (n-2)

-- We define a general function to help print in this tutorial
-- What is the type of this printer?
-- ANS: (Show t1, Show t, Enum t) => (t -> t1) -> t -> t -> IO ()
--      Enum is typeclass and data of an instance of it can be enumerated

printl f start end =
    let aux [] = putStr ""
        aux (x:xs) =
            let s = "Fib " ++ (show x) ++ " : " ++ (show (f x))
            in do {
                putStrLn s;
                aux xs
            }
    in  aux [start..end]

-- We define the actual function to print fibonaccis using fib1
print_fib1 = printl fib1 1 40

{- Q7: Let us define a more efficient version of fib2 below together with its
    print method. Which call first has an integer overflow? 
   ANS: fib2 93 gives first integer overflow in a 64 bit machine if fib2 :: Int -> Int
    (fib2 92 = 7540113804746346429, but fib2 93 = 12200160415121876738)
-}

fib2 n =
    let aux 0 = (1, 0)
        aux n =
            let (a, b) = aux (n-1)
            in
                (a + b, a)
    in
        snd (aux n)

print_fib2 = printl fib2 1 60


{-
  Q8: Implement fibonacci as a tail-recursive method with two
      parameters, initially denoting fib(0) and fib(1).
      Is it's computational behaviour different from a Loop?
-}

fib3 :: Int -> Integer
fib3 n =
    let aux n r1 r0 =
         if n == 0 then
            r0
         else
            aux (n-1) (r1+r0) r1
    in
        aux n 1 0

-- Q9. Debugging can be facilitated by trace function in
--  https://hackage.haskell.org/package/base-4.10.0.0/docs/Debug-Trace.html

-- Define the following functions
mult' x = trace "mult' invoked earlier" (x * x)
add' x = trace "add' invoked only once" (x + x)
multOfAdd' x = mult' (add' x)

-- Q. What is output you expect? Why?
-- By beta-reduction, multOfAdd' x = trace "..." (add' x * add' x)
-- ANS:   Invoke order should be mult' -> add'.
--        Reason: lazy evaluation. (\x -> x * x) ((\y -> y + y) z) ----> ((\y -> y + y) z) * ((\y -> y + y) z)

-- Q. How many times add' is evaluated? Why?
--        add' is invoked once.
--        Reason: result of one expression is stored and used later for a differenct instance of the same expression.

-- However, these methods are not pure and have side-effects.
-- Discuss the effects of the following
--   trace :: String -> a -> a
--   traceId :: String -> String
--   traceShow :: Show a => a -> b -> b
--   traceStack :: String -> a -> a
--   traceIO :: String -> IO ()
{-
    trace: prints the given message and then evaluates the given expression.
    traceId: prints the given message and returns it.
    traceShow: prints the value of type a and then it evaluates and return the given expression.
    traceStack: additionally prints a call-stack
    traceIO: it just print the message and sequences to other IO action.
-}
