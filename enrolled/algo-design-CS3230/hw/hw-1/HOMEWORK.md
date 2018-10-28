# CS3230 Homework 1

#### Jerrell Ezralemuel

## K-Sorted Array

## Inversions

### 2.1

$\{(1, 5), (2, 5), (3, 4), (3, 5), (4, 5)\}$

### 2.2

$\big\langle n, n-1, \dots, 1\big\rangle $ is the configuration. with $\dfrac{n(n - 1)}{2}$ inversion pairs

### Proof:

The number of inversion pairs for any array is bounded from the top by $n \choose 2$ as this is the number of pairs can be created out of $n​$ elements. 

let $I$ be the set of inversions pairs in $A$

hence in this configuration,  
$$
\forall j < i, A_i < Aj
$$

therefore, 
$$
|(x, i) \in I| = i - 1
$$

thus,
$$
\sum_{i = 1}^n i - 1 = \frac{n(n - 1)}{2}  = \binom{n}{2}
$$

QED.

### 2.3

