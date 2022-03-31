
|         Title         |                                         Formula                                         |                                     Markdown                                     |
|-----------------------|-----------------------------------------------------------------------------------------|----------------------------------------------------------------------------------|
|       Variables       |                               $x_1$, $y_1$, $z_{3,4}$                                |                            `$x_1$, $y_1$, $z_{3,4}$`                             |
|        Square         |                                 $a^2$, $x^y$, $2^{n-1}$                                 |                            `$a^2$, $$x^y$, $2^{n-1}$`                            |
|      Square Root      |                        $\sqrt{9}$, $\sqrt{x}$, $\sqrt[n]{x}$                         |                     `$\sqrt{9}$, $\sqrt{x}$, $\sqrt[n]{x}$`                      |
|       Logarithm       |                                $\log{}x$, $\log_{2}x$                                |                             `$\log{}x$, $\log_{2}x$`                             |
|       Fraction        |                    $\frac{1}{2}$, $\left(-\frac{1}{2}\right)^n$                     |                  `$\frac{1}{2}$, $\left(-\frac{1}{2}\right)^n$`                  |
|       Infinity        |                                        $\infty$                                        |                                    `$\infty$`                                    |
|    Absolute Value     |$\vert{x}\vert$, $\vert\frac{x}{2}\vert$, $\lfloor{x}\rfloor$, $\lceil{x}\rceil$|`$\vert{x}\vert$, $\vert\frac{x}{2}\vert$, $\lfloor{x}\rfloor$, $\lceil{x}\rceil$`|
| Arithmetic Operation  |                                $2\times 3$, $6\div 3$                                 |                             `$2\times 3$, $6\div 3$`                             |
|       Factorial       |                                          $n!$                                           |                                      `$n!$`                                      |
|Trigonometric Functions|                     $\sin\theta$, $\cos\theta$, $\tan\theta$                      |                   `$\sin\theta$, $$\cos\theta$, $\tan\theta$`                    |
|    Greater or Less    |                      $a\gt b$, $a\geq b$, $a\lt b$, $a\leq b$                       |                    `$a\gt b$, $a\geq b$, $a\lt b$, $a\leq b$`                    |
|       Equation        |                            $a=b$, $a\neq b$, $a\approx b$                             |                        `$a=b$, $a\neq b$, $a\approx b$ `                         |
|       Times Dot       |                                     $a\cdot b=ab$                                      |                                 `$a\cdot b=ab$`                                  |
|    Divide Fraction    |                                   $a/b=\frac{a}{b}$                                    |                               `$a/b=\frac{a}{b}$`                                |
|  Trinomial Equation   |                                    $a^2 + b^2 = c^2$                                    |                               `$a^2 + b^2 = c^2$`                                |
|  Matrix Parentheses   |                 $\begin{pmatrix} a & b \\ c & d \end{pmatrix}$                  |                `$$\begin{pmatrix} a & b \\ c & d \end{pmatrix}$$`                |
|    Matrix Brackets    |                 $\begin{bmatrix} a & b \\ c & d \end{bmatrix}$                  |                `$$\begin{bmatrix} a & b \\ c & d \end{bmatrix}$$`                |
|    Matrix Equation    |              $\begin{vmatrix} a & b \\ c & d \end{vmatrix}=ad-bc$               |             `$$\begin{vmatrix} a & b \\ c & d \end{vmatrix}=ad-bc$$`             |
|          Set          |                           $x\in A$, $A\ni x$, $x\notin A$                            |                        `$x\in A$, $A\ni x$, $x\notin A$`                         |
|        Subset         |                  $A\subset B$, $A\subseteq B$, $A \not \subset B$                   |                `$A\subset B$, $A\subseteq B$, $A \not \subset B$`                |
| Intersection & Union  |                         $A\cap B$, $A\cup B$, $\overline{A}$                         |                      `$A\cap B$, $A\cup B$, $\overline{A}$`                      |
|   Quadratic formula   |                       $x = {-b\pm\sqrt{b^2-4ac}\over2a}$                       |                    `$x = {-b \pm \sqrt{b^2-4ac} \over 2a}$`                    |
|       Binomial        |                                 $\sqrt{3x-1}+(1+x)^2$                                  |                             `$\sqrt{3x-1}+(1+x)^2$`                              |
|    Differentiation    |                                $f’$, $f^{(n)}$, $D_x f$                                |                            `$f'$, $f^{(n)}$, $D_x f$`                            |
|       Integral        |                                  $\int_0^1 f(x) dx$                                   |                               `$\int_0^1 f(x) dx`                               |
|    Integral Large     |                   $\displaystyle \int_{-\infty }^{\infty}f(x)dx$                   |                 `$\displaystyle \int_{-\infty }^{\infty}f(x)dx$`                 |
|      Max Sample       |         $\max(a,b)=\begin{cases}a&(a\geqq b)\\b&(a\lt b)\end{cases}$         |         `$$\max(a,b)=\begin{cases}a&(a\geqq b)\b&(a\lt b)\end{cases}$$`         |

3. Big O and related notations
----------

|      Name      |      Notation       |       Markdown       |
|----------------|---------------------|----------------------|
| Big O [micron] |$\mathcal{O}$ or $O$|`$\mathcal{O}$ or $O$`|
|   Big Omega    |      $\Omega$      |      `$\Omega$`      |
|   Big Theta    |      $\Theta$      |      `$\Theta$`      |
|Small O [micron]|         $o$         |        `$o$`         |
|  Small Omega   |      $\omega$      |      `$\omega$`      |
|On the order of |       $\sim$       |       `$\sim$`       |

4. Common Time and Space Complexity
----------

|      Name      | Running Time |   Markdown    |
|----------------|--------------|---------------|
| Constant Time  |    $O(1)$    |   `$O(1)$`    |
|Logarithmic Time|$O(\log{}n)$ |`$O(\log{}n)$` |
|  Linear Time   |    $O(n)$    |   `$O(n)$`    |
|Quasilinear Time|$O(n\log{}n)$|`$O(n\log{}n)$`|
| Quadratic Time |   $O(n^2)$   |  `$O(n^2)$`   |
|   Cubic Time   |   $O(n^3)$   |  `$O(n^3)$`   |
| Factorial Time |   $O(n!)$    |   `$O(n!)$`   |
