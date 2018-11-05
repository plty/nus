# CS3230 Homework 1

#### Jerrell Ezralemuel A0181002B

## K-Sorted Array

### 1.1

Let $G_i$ be the value on index $i$ when $A$ is sorted

At $j = k + i$ the value extracted from the heap is $i$-th smallest element among $A_1, A_2, \dots A_{k + i}$ and we have removed $G_1, G_2, \dots G_i$

### 1.2

The statement holds for $j = k + 1$ as the min heap currently holds $A_1, A_2, \dots, A_{k + 1}$  and the value at the root of the heap is the min of $ A_1, A_2, \dots, A_{k + 1}$, and we have removed $G_1$ the smallest element among those values, as $G_1$ lies between $ A_1, A_2, \dots, A_{k + 1}$, $G_1$ is removed from the heap.

Assume it's true for $j = i + k - 1$

for $j = i + k$:
From our $j= i + k - 1$ we have removed $i - 1$ smallest element among $A_1, A_2, \dots A_{k + i -1}$ , and  hence adding $A_{k + i }$ as now, $G_i$ should be on the heap, as $G_i$ haven't been removed because our previous steps we have removed $G_1, G_2, \dots G_{i - 1}$.  and $G_i$ is in position between $1$ and $k + i$ because of $G_i$ is on the heap and no values smaller than $G_i$ existent on the heap because all of them have been removed on the previous step, $G_i$ will be removed on this step. hence satisfied the condition as we have removed all $G_1, G_2, \dots, G_i$ and removing $i$-th smallest element among $A_1, A_2, \dots, A_{k + i}$

From this induction the statement is correct $\forall j > k$

### 1.3

As we have proven, in 1.2 on step $j = i + k$ we set $B_i = G_i$ Hence, $B = G$ therefore $B$ contains sorted $A$

## Inversions

### 2.1

$\{(1, 5), (2, 5), (3, 4), (3, 5), (4, 5)\}$

### 2.2

$A = \big\langle n, n-1, \dots, 1\big\rangle $ is the configuration. with $\dfrac{n(n - 1)}{2}$ inversion pairs

### Proof:

The number of inversion pairs for any array is bounded from the top by $n \choose 2$ as this is the number of pairs can be created out of $n$ elements. 

let $I$ be the set containing all inversions pairs in $A$

hence in this configuration,  
$$
\forall j < i, A_i < Aj
$$

therefore, 
$$
\forall i \in [1, n], |\{(x, i) \in I\}| = i - 1
$$

thus,
$$
|I| = \sum_{i = 1}^n i - 1 = \frac{n(n - 1)}{2}  = \binom{n}{2}
$$

QED.

### 2.3
### **Algorithm 1: InversionCount(A)**
$$
\begin{align*}
&\text{if } |A| < 1 \text{ then return } 0;\\
&\text{Copy the first half of array $A$ to $B$};\\
&\text{Copy the rest of the array $A$ to $C$};\\
&ans\leftarrow InversionCount(B) + inversionCount(C);\\
&ans\leftarrow ans + MergeCount(A, B, C);\\
&\text{return ans};\\
\end{align*}
$$

### **Algorithm 2: MergeCount(A, B, C)**
$$
\begin{align*}
&n \leftarrow |B|\\
&m \leftarrow |C|\\
&l \leftarrow n + m;\\
&inversion \leftarrow 0;\\
&\text{append $\infty$ to B};\\
&\text{append $\infty$ to C};\\
&\text{for $k \leftarrow 1$ to $l$};\\
&i \leftarrow 1; j \leftarrow 1;\\
&\qquad \text{if $B[i] < C[j]$ then}\\
&\qquad \qquad A[k] \leftarrow B[i];\\
&\qquad \qquad i \leftarrow i + 1;\\
&\qquad \text{else}\\
&\qquad \qquad A[k] \leftarrow C[j];\\
&\qquad \qquad j \leftarrow j + 1;\\
&\qquad \qquad inversion \leftarrow inversion + n - i + 1;\\
&\text{end of for-loop}\\
&\text{return } inversion;
\end{align*}
$$
