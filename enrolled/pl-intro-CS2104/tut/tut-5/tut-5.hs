import System.IO.Unsafe
{-
    Tutorial 5 : Monads and IO

    We will discuss properties of the class Monad m and some useful monads.
    We will use the monads [], Maybe, IO, and (Either e) in this discussion.

   Try complete this tutorial and submit in LumiNUS for participation
   mark by around 7 Oct 2019.
   
   It will NOT be graded.
-}

{-
    'Monad m' is a class with at least the following members.
    (>>=)  :: m a -> (a -> m b)    -> m b
    return ::   a                  -> m a

    In addition to implementing these class functions, 
    all instance of Monad are expected to obey the following
    properties or 'Monad Law'

    return a >>= k                 = k a               (Left Identity)
    m        >>= return            = m
    m        >>= (\x -> k x >>= h) = (m >>= k) >>= h
-}

{-
    TRY creating some monadic values in ghci.
-}

x1 = return 42 :: Maybe Int
x2 = return 42 :: [Int]
x3 = return 42 :: IO Int

{-
x1
x2
x3
:t x3
-}

{-
    Q2. Let us compose these monads which contain a number 42
        with an increment operation.
-}

increment x = return (succ x)

y1 = x1 >>= increment
y2 = x2 >>= increment
y3 = x3 >>= increment

{-
    We may also compose other monads with increment, 
    as shown below. Guess their effects, especially y7

-}

y4 = (Nothing >>= increment) :: Maybe Int
y5 = ([] >>= increment) :: [Int]
y6 = ([1,2,3] >>= increment) :: [Int]
y7 = (getChar >>= increment) :: IO Char

{-

Type y4,y5,y6,y7 in ghci
Explain the effect of each of the monadic operation.

-}

{-
    Q3. Let us write a monad to perform an operation twice.
    What is its type?

-}
perform_twice m = 
  do
    x1 <- m
    x2 <- m
    return (x1,x2)

{-
Execute this method with

> perform_twice x1  -- x1=(Just 42)
> perform_twice x2  -- x2=[42]
> perform_twice y7  -- y7::IO Char
> perform_twice [1,9]

Explain the effect of each of these executions.
What is the type of the last operation?

-}


{-
   Q5

    Re-implement perform_twice using >>=
    instead of the do notation.

-}

perform_twice' :: Monad m => m t -> m (t, t)
perform_twice' m = fail "to be implemented using >>="

{-
   Q6

    Implement a more general method performN
    which will perform a given monadic computation
    a fixed number of times.

    Test this method with (performN 5 getChar)

-}

performN :: Monad m => Int -> m t -> m [t]
performN 0 m = return []
performN n m = 
  do x1<-m
     x2<-error "to be completed"
     return (x1:x2)

{-
   Q7

   Consider another method which is able to 
   repeatedly perform the same monadic computation
   as long as some filter condition is true .

  (i) use perform_filter to test the method getDigits
  (ii) Can you rewrite perform_filter method to use an aux
      method instead?


-}

perform_filter :: Monad m => (t->Bool) -> m t -> m [t]
perform_filter f m = 
  do x1<-m
     if not(f x1) then return []
     else do x2<-perform_filter f m
             return (x1:x2)

perform_filter' f m =
  let aux = error "same functionality as perform_filter.."
  in aux

getDigits :: IO String
getDigits = 
    do
      xs <- perform_filter' (\n -> elem n ['0'..'9']) getChar
      putStrLn "\nThe sequence of digits you just typed is:"
      print xs
      return xs

{-
   Q8

   Using perform_filter, write three IO operations
   which would read and print
     (i) binary number
     (ii) octal number
     (iii) hexadecimal number
   from the terminal. 

   For hexadecimal, take note that "A" denotes 11
   and "E" denotes 15. The conv_base method
   would have to be suitably modified to
   compute the correct values for A..Z.

-}


-- this method takes a base and converts a list of digits
-- into its corresponding number of the given base
--   base=2 gives binary number
--   base=8 gives octal number
conv_base :: Enum a => Int -> [a] -> Int
conv_base base lst =
  let aux xs n = case xs of 
        [] -> 0
        y:ys -> ((fromEnum y)-(fromEnum '0'))*n + (aux ys (n*base))
  in aux (reverse lst) 1

-- Try use >>= and return only
-- To read a binary number, and return its decimal equivalent
getBin :: IO Int
getBin = 
  error "to be completed using perform_filter and conv_base"

-- Try use do notation in your implementation
-- To read an octal number, and return its decimal equivalent
getOct :: IO Int
getOct = 
  error "to be completed using perform_filter and conv_base"


-- Choose either do notation or >>= 
-- To read a hexadecimal number, and return its decimal equivalent
getHex :: IO Int
getHex = 
  error "to be completed using perform_filter and conv_base"

{-

  Q9: DISCUSSION

 Monadic IO as a pure value

 Consider foo1 and foo2.
  (i) Is "IO" a pure value?
  (ii) Are foo1 and foo2 equivalent?
       Is this due to the fact that "io" is a pure value.
  (iii) Pure values has the "referential transparency" property
  which states that you can always replace a (pure) expression
  by its equivalent expression. This "referential transparency"
  property does not hold for impure codes.
 Alternatively, "referential transparency" aslo
 means an expression always evaluates to the same result in any context.

-}

foo1 =
      let io = print "hello" 
      in io >> io

foo2 =
      (print "hello") >> (print "hello")



{-

  Q10: Let us compare to the corresponding pseudo 
  C program of the followng:
  foo3 () {
    io = println "hello"
    return (io,io)
  }
  foo4 () {
    return (println "hello",println "hello")
  }
  Are these two methods equivalent?
  

The corresponding impure IO operation in Haskell
  is implemented as:
      unsafePerformIO (print "hello")
  as illustrated below.
  However, you are discouraged from
  using these since it introduces side-effects and impurity.
  In case you wish to use it, please do so with care!

-}

foo3 =
      let io = unsafePerformIO (print "hello")
      in io `seq` io `seq` ()

foo4 =
      (unsafePerformIO (print "hello")) `seq` 
      (unsafePerformIO (print "hello")) `seq`
      ()

{-

  Q11: What are the corresponding Haskell counterpart
  in pure functions for foo3 and foo4? 

-}

foo3pure :: IO ()
foo3pure = error "to be completed as a pure function"

foo4pure :: IO ()
foo4pure = error "to be completed as a pure function"
