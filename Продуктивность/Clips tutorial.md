## CLIPS Tutorial 1

## Working with CLIPS

![CLIPS icon](https://kcir.pwr.edu.pl/~witold/ai/CLIPS_tutorial/CLIPS%20tutorial%201_files/clips.gif)To start CLIPS, double-click on the CLIPSWin.exe file.  You’ll get a window with nothing in it but the command prompt CLIPS>. For the time being, this is where you type in your commands and programs. To exit CLIPS, type **(exit)** or shut down the program like any other windows application. Note that CLIPS commands are always encased in brackets thus: **(assert (foo))**. Here is a list of some important commands:

**(exit)**

Shuts down CLIPS

**(clear)**

Removes all rules and facts from memory. Equivalent to shutting down and restarting CLIPS.

**(reset)**

Removes facts information from memory (but not rules) and resets the agenda.

**(run)**

Starts executing a CLIPS program.

The above commands can also be executed from the CLIPS menu bar.

## Facts and rules

At its most basic, CLIPS operates by maintaining a list of _facts_ and a set of _rules_ which operate on them. A fact is a piece of information such as **(colour green)** or **(parent_of John Susan)**. Facts are created by _asserting_ them onto the fact database using the assert command. Here’s an example, complete with the response from CLIPS:

CLIPS>**(assert (colour green))  
**<Fact-0>

The <Fact-0> part is the response from CLIPS to say that a new fact (fact number 0) has been placed on the fact database. The **(facts)** command will list all current facts. Try it, and you’ll get the following:

CLIPS>**(facts)**  
f-0 (colour green)  
For a total of 1 fact.

Facts may also be _retracted_ (removed) from the fact database by using the **retract** command. As an example, assert two facts as shown:

CLIPS>**(assert (colour green))  
**<Fact-0>  
CLIPS>**(assert (colour red))  
**<Fact-1>

Then retract the first fact and display the fact list:

CLIPS>**(retract 0)  
**CLIPS>**(facts)  
**f-1 (colour red)  
For a total of 1 fact.

There are two things to note here: firstly, to retract a fact you must specify a number (the _fact-index_), not the fact itself, and secondly, fact-indices are not reused. Once fact 0 has been retracted, the next fact asserted will have the index 2, not 0.

Facts on their own are of only limited use. The application of rules is necessary to develop a program capable of some useful function. In general, a rule is expressed in the form ‘IF something is true THEN do some action’. This kind of rule is known as a _production_. For this reason, rule-based expert systems are often known as _production systems_ (CLIPS actually stands for C Language Integrated Production System). In CLIPS, a typical rule looks like this:

**(defrule duck
  (animal-is duck)
  =>
  (assert (sound-is quack)))**

The rule consists of three parts. The first part, **(defrule duck**, simply gives the rule a unique name. The second part, **(animal-is duck)**, is the pattern (the IF part) of the rule and the last part, **(assert (sound-is quack))**, is the action (the THEN part). In plain language, this rule means ‘if there is a fact **(animal-is duck)** on the fact database, then assert another fact, **(sound-is quack)**, onto the fact database’. Try it. Clear the system, then type in the rule exactly as printed above. Typing **(rules)** will give you a list of rules (just the one, in this case) present in the system. At this point, there are no facts present. Now, type **(assert (animal-is duck))**. Check the fact list - there’s one fact. To trigger your rule, type **(run)**. Although nothing appears to happen, if you check the fact list again you’ll see that there is a new fact, **(sound-is quack)**, which has been inferred by the rule. This is the power of rule-based programming - the ability to make inferences from data, particularly as the results of one rule can be used as the pattern for another. Add the rule

**(defrule is-it-a-duck
  (animal-has webbed-feet)
  (animal-has feathers)
  =>
  (assert (animal-is duck)))**

Then type **(reset)** to clear the facts (the rules will be untouched). Note that this rule has two patterns. Both must be satisfied for the action to be taken. This translates to ‘IF the animal has webbed feet AND the animal has feathers THEN the animal is a duck’ (taxonomists and pedants may disagree with this rule). If you now assert the facts **(animal-has webbed-feet)** and **(animal-has feathers)** there will be two facts present. **(run)** the rules, and suddenly there are four. Firstly, rule **is-it-a-duck** has fired, asserting the fact **(animal-is duck)**. This fact has then triggered rule duck, which has asserted the fact **(sound-is quack)**. Very powerful systems can be built using this ability to _chain_ rules.

Asserting facts is a rather unsatisfactory way of presenting results. Type in the first rule again, this time with the multiple actions as shown below:

**(defrule duck
  (animal-is duck)
  =>
  (assert (sound-is quack))
  (printout t "it’s a duck" crlf))**

Next time you run the rules, you'll get a message on screen as well as the asserted quack fact.

![CLIPS Editor icon](https://kcir.pwr.edu.pl/~witold/ai/CLIPS_tutorial/CLIPS%20tutorial%201_files/clpedit.gif)It’s rather inefficient having to type all your rules in each time you run CLIPS. Fortunately, you can load them from a file using the ‘Load Constructs..’ command on the file menu. CLIPS will expect a file with the extension .CLP, and there’s a handy editor to help you create them. You can’t put facts in a .CLP file in the same way as you can from the command prompt, so for now you’ll still enter them as before.

![Decision tree](https://kcir.pwr.edu.pl/~witold/ai/CLIPS_tutorial/CLIPS%20tutorial%201_files/tut03.gif)Here’s a more complex example of rules and facts. The decision tree opposite represents a small section of the diagnosis of a car’s failure to start. Each rounded box is a recommended remedy. Each rectangular box is piece of evidence, which might be represented by a fact such as **(lights-working no)** or **(petrol yes)**. Each connecting path to a remedy represents a rule, for example ‘IF starter is turning AND there is no petrol THEN buy some petrol’.  

---

## CLIPS Tutorial 2 - Patterns and actions

## Persistent facts

For the following exercises, you will need to use the same set of facts several times. Rather than type them in repeatedly, you should use the deffacts structure. This is a way of specifying _facts which are recreated every time a (reset) is executed_. For example, the code

(deffacts startup (animal dog) (animal duck) (animal haddock))

will assert three facts onto the database every time the system is reset. Once they are asserted, the facts are the same as any others - they can be retracted or used in rule patterns - but even if they are retracted they will reappear after a (reset). Below is the list of facts you will need to use - use the CLIPS editor to enter them in a deffacts structure then reset CLIPS and look at the fact list to check that they are present.

(animal dog) (animal cat) (animal duck) (animal turtle) (warm-blooded dog) (warm-blooded cat) (warm-blooded duck) (lays-eggs duck) (lays-eggs turtle) (child-of dog puppy) (child-of cat kitten) (child-of turtle hatchling)

## Matching things

So far, the patterns used to match rules against facts have been very simple and rather restrictive. Each pattern has matched one specific fact. By using _wildcards_, it is possible to make rules match multiple facts, executing their actions repeatedly. For instance, the rule:

(defrule animal
  (animal ?)
  =>
  (printout t "animal found" crlf))

Produces the following results when run:

CLIPS>**(run)  
**Animal found  
Animal found  
Animal found  
Animal found  
CLIPS>

Which shows that it has triggered four times, once for each fact matching the (animal ?) pattern. In this pattern, the ? symbol is a wildcard. It will match any symbol. You can use as many wildcards as you like in a pattern, but the first symbol may not be one. So (child-of ? ?) is legal and will match four facts, but (? ? hatchling) is illegal.

### Variables in patterns

Simple wildcards are only mildly useful. Variables make them indispensable. If we use something like ?var instead of ? on its own, we can use the value of ?var each time the rule is fired. Try this example:

(defrule list-animals  
(animal ?name)  
=>  
(printout t ?name " found" crlf))

This will produce the following results:

CLIPS>**(run)  
**turtle found  
duck found  
cat found  
dog found  
CLIPS>

The rule has matched four facts, and each time the variable ?name has taken the value of the symbol it represents in the pattern, so that in the action part of the rule it can be printed. The real power of this feature is apparent when two or more patterns are used, as in the next example:

**(defrule mammal
  (animal ?name)
  (warm-blooded ?name)
  (not (lays-eggs ?name))
  =>
  (assert (mammal ?name))
  (printout t ?name " is a mammal" crlf))**

You may notice the not function sneaked in there. The purpose of this should be self-evident. This rule gives the results

CLIPS>**(run)  
**cat is a mammal  
dog is a mammal  
CLIPS>

When you are satisfied that you understand how this works, try the next step:

**(defrule mammal2
  (mammal ?name)
  (child-of ?name ?young)
  =>
  (assert (mammal ?young))
  (printout t ?young " is a mammal" crlf))**

After you have run this rule, look at the fact list

CLIPS>**(run)  
**kitten is a mammal  
puppy is a mammal  
CLIPS>

## Facts and rules

In order to retract a fact, you need to know its fact-index. You can retract facts from within rules by binding them to variables, like this:

**(defrule remove-mammals
  ?fact <- (mammal ?)
  =>
  (printout t "retracting " ?fact crlf)
  (retract ?fact))**

In the pattern part of this rule, the variable ?fact is given the fact-index of each fact matching the pattern (mammal ?) in turn. That's what the leftwards arrow (<-) symbol means. When you run it, this is what happens (the fact numbers may be different):

CLIPS>**(run)  
**retracting <Fact-13>  
retracting <Fact-14>  
retracting <Fact-15>  
retracting <Fact-16>  
CLIPS>

All the mammal facts have been retracted.

## Logic and Math Operators

You've already seen that two or more patterns in a rule are automatically connected with a logical AND, which means that both must be true for the rule to fire. You've also seen the **not** function, which leaves OR as the only boolean function missing for pattern specification. CLIPS has an **or** function, which is used as shown:

(defrule take-umbrella
  (or (weather raining)
      (weather snowing))
  =>
  (assert (umbrella required)))

Which means "if it is raining or it is snowing, then take an umbrella". Notice the way the or comes before the two arguments, rather than between them. this is known as _prefix notation_, and all CLIPS operators work this way. For example, to express a sum of two numbers in most computer languages, you would use something like 5 + 7 (this is known as _infix notation_). In CLIPS, the expression would be written (+ 5 7). Examine the following examples which show the addition, subtraction, multiplication and division operators:

CLIPS>**(+ 5 7)** 12
CLIPS>**(- 5 7)** -2
CLIPS>**(* 5 7)** 35
CLIPS>**(/ 5 7)** 0.7142857142857143
CLIPS>

Rewrite the expression 10+4*19-35/12 in CLIPS notation and verify that you get the result 83.0833. (Answer at bottom of page).

## **Getting data from the user**

CLIPS gets information from the user by means of the (read) function. Wherever (read) is encountered, the program waits for the user to type something in, then substitutes that response. To demonstrate this, type (assert (user-input (read))) at the CLIPS prompt. After you press return, you'll need to type something else (anything) before the command will complete. When you look at the facts, you'll see a new fact with your input as the second item. Try the rule

**(defrule what-is-child
        (animal ?name)
        (not (child-of ?name ?))
=>
        (printout t "What do you call the child of a " ?name "?")
        (assert (child-of ?name (read))))**

When you run it, the system will now prompt you for the name of the young of any animal it doesn't know about. It will use the data you enter to assert a fact, which could then be used by other rules. In the car diagnostic example of the previous tutorial, you could have used a rule such as

**(defrule are-lights-working
        (not (lights-working ?))
=>
        (printout t "Are the car's lights working (yes or no)?")
        (assert (lights-working (read))))**

---

The correct CLIPS representation for the expression 10+4*19-35/12 is (+ 10 (- (* 4 19) (/ 35 12))).

---