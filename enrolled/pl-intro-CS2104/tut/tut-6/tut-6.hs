{-


Q1. Test the code below. Describe how precedence operator
    of + and * were determined by the recursive parser.

    Explain what changes will be needed to change the
    precedence of these operators.

    Write your answer below.

ex1 = "1+2*3"
ex2 = "1*2+3"
q1_test1 = calcE ex1
q1_test2 = calcE ex2


Q2. Test the code below . Describe how left and right-associativity
    of + and * are being supported.

    Explain what changes will be needed to make these
    operators right associative?

ex3 = "1+2+3"
ex4 = "1*2*3"
q2_test3 = calcE ex3
q2_test4 = calcE ex4

Q3. Implement a power operator ^ that that is right associative
    and with lower precedence than +/-.
    For example, 2^3+1^2  = 2^((3+1)^2))
                        = 2^(4^2)
                        = ..
    Add a constructor:
         Power Expr Expr  to Expr type
    Add a function:
         ePow  x y = Power x y

    Change both the parser (parse) and evaluator (eval)
    to support this new operator.


Q4.

Add a lexical phase to the parser by using say:

 data Word = WInt Integer | WId String | WPlus | WMinus
          | WTimes | WDiv | WPower | WOpen | WClose
          deriving (Show)

Write a lexical analyser (as a recursive function or otherwise)
which would convert an input string into [Word].

To motivate the need for lexer, consider parsing:

ex6a = "1-2"
ex6b = "1- 2"
ex6c = "1 - 2"
e6a = calcE ex6a
e6b = calcE ex6b
e6c = calcE ex6c

(Optional - More Challenging)

Q5. Integrate this lexical analyser as a front-end of our parser.
    Add a non-recursive let-binding to our calculator of the
    form:  let x=1+3 in 3*x+2.

Some initial code for constructing lexical analyser is given below.
Please read it, and try to complete it.


-}


module SParsec where

import Text.Parsec
import Text.ParserCombinators.Parsec.Language
import qualified Text.ParserCombinators.Parsec.Token as Token

data Expr = Const Int | Plus Expr Expr
          | Minus Expr Expr | Mult Expr Expr
          | Div Expr Expr   deriving (Show)


eAdd  x y = Plus x y
eSub  x y = Minus x y
eMult x y = Mult x y
eDiv  x y = Div x y
sToC  s   = Const (read s)

type SParsec = Parsec String ()

expr :: SParsec Expr
expr = chainl1 term addop            -- x+y-z+...

term :: SParsec Expr
term = chainl1 factor mulop          -- x*y/z*...

factor :: SParsec Expr
factor = (parens expr) <|> constants -- 12 | (...)

parens :: SParsec Expr -> SParsec Expr
parens ex = do char '('
               x <- ex
               char ')'
               return x

constants :: SParsec Expr
constants = parsecMap sToC $ many1 digits

digits :: SParsec Char
digits =
    char '0' <|> char '1' <|> char '2' <|>
    char '3' <|> char '4' <|> char '5' <|>
    char '6' <|> char '7' <|> char '8' <|> char '9'

mulop :: SParsec (Expr -> Expr -> Expr)
mulop = do { char '*'; return eMult }
    <|> do { char '/'; return eDiv}

addop :: SParsec (Expr -> Expr -> Expr)
addop = do { char '+'; return eAdd }
    <|> do { char '-'; return eSub }

calcE ::  String -> Either ParseError Expr
calcE x = parse expr "" x


eval :: Either ParseError Expr -> Either ParseError Int
eval x =
      let eval' :: Expr -> Int
          eval' (Const  x) = x
          eval' (Plus  x y) = eval' x + eval' y
          eval' (Minus  x y) = eval' x - eval' y
          eval' (Mult x y) = eval' x * eval' y
          eval' (Div  x y) = div (eval' x) (eval' y)
      in
          fmap eval' x


ex1 = "1+2*3"
ex2 = "1*2+3"
q1_test1 = calcE ex1
q1_test2 = calcE ex2

ex3 = "1+2+3"
ex4 = "1*2*3"
q2_test3 = calcE ex3
q2_test4 = calcE ex4

ex5 = "2^3"
ex6 = "2^3+1^2"
q3_test5 = calcE ex5
q3_test6 = calcE ex6

e3_test5 = eval (calcE ex5)
e3_test6 = eval (calcE ex6)

ex6a = "1-2"
ex6b = "1- 2"
ex6c = "1 - 2"
e6a = calcE ex6a
e6b = calcE ex6b
e6c = calcE ex6c


languageDef =
  emptyDef { Token.commentStart    = "/*"
           , Token.commentEnd      = "*/"
           , Token.commentLine     = "//"
           , Token.identStart      = letter
           , Token.identLetter     = alphaNum
           , Token.reservedNames   = [ "let"
                                     , "in"
                                     ]
           , Token.reservedOpNames = ["+", "-", "*", "/", "^"
                                     ,"(",")","="
                                     ]
           }

lexer = Token.makeTokenParser languageDef

identifier = Token.identifier lexer -- parses an identifier
reserved   = Token.reserved   lexer -- parses a reserved name
reservedOp = Token.reservedOp lexer -- parses an operator
integer    = Token.integer    lexer -- parses an integer
semi       = Token.semi       lexer -- parses a semicolon
whiteSpace = Token.whiteSpace lexer -- parses whitespace

data Word = WInt Integer | WId String | WPlus | WMinus
          | WTimes | WDiv | WPower | WLet | WIn | WOpen | WClose
          | WEq
          deriving (Show)

onetoken =
       (parsecMap (\x -> WId x) $ identifier)
       <|> (parsecMap (\x -> WPlus) $ reservedOp "+")
       <|> (parsecMap (\x -> WMinus) $ reservedOp "-")
       <|> (parsecMap (\x -> WTimes) $ reservedOp "*")
       <|> (parsecMap (\x -> WDiv) $ reservedOp "/")
       <|> (parsecMap (\x -> WInt x) $ integer)

mtokens = many1 (error "TBI")

ex7 = "let x=1+3 in 3* x + 2"
q5 = parse mtokens "" ex7
q6 = parse mtokens "" "1- 2"
q6a = parse mtokens "" "1- 2"
q6c = parse expr "" " 1 - 2"

