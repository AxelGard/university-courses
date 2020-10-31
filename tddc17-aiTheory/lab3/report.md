# Bayesian network
## Part II
### 5.
**a) What is the risk of melt-down in the power plant during a day if no observations have been made? What if there is icy weather?**
Answer: P(meltdown) = 0.02578, P(meltdown|icy weather) = 0.03472


**b) Suppose that both warning sensors indicate failure. What is the risk of a meltdown in that case? Compare this result with the risk of a melt-down when there is an actual pump failure and water leak. What is the difference? The answers must be expressed as conditional probabilities of the observed variables, P(Meltdown|...).**
Answer: P(Meltdown=T| PumpFailureWarning=T, WaterLeakWarning=T) = 0.14535
        P(Meltdown=T| PumpFailure=T, WaterLeak=T) = 0.2
        P(Meltdown=T| PumpFailure=T, WaterLeak=T, PumpFailureWarning=F, WaterLeakWarning=F) = 0.2

**c) The conditional probabilities for the stochastic variables are often estimated by repeated experiments or observations. Why is it sometimes very difficult to get accurate numbers for these? What conditional probabilites in the model of the plant do you think are difficult or impossible to estimate?**
Answer: They may be difficult to accurately measure due to hidden or unknown external factors that may contribute to the outcome
of the experiments. Meltdown is a good example of a difficult thing to measure as there may be several
other factors that go into if a meltdown happens or not.


**d) Assume that the "IcyWeather" variable is changed to a more accurate "Temperature" variable instead (don't change your model). What are the different alternatives for the domain of this variable? What will happen with the probability distribution of P(WaterLeak | Temperature) in each alternative?**
Answer: The different domains include a boolean variable that is Hot or Cold.
        We could also use more of a sliding scale where the domain is {0, 10, 20} (not continuous) where the
        exact temperature outside maps to the closest value in the domain.
        Given this we would get a new probability table for Waterleak where ex.
        P(WaterLeak|Temperature = 0) =〈x1,y1〉
        P(WaterLeak|Temperature = 10) =〈x2,y2〉
        P(WaterLeak|Temperature = 20) =〈x3,y3〉

### 6.
**a) What does a probability table in a Bayesian network represent?**
Answer: The probability of the event happening given the outcome of the parent nodes.

**b) What is a joint probability distribution? Using the chain rule on the structure of the Bayesian network to rewrite the joint distribution as a product of P(child|parent) expressions, calculate manually the particular entry in the joint distribution of P(Meltdown=F, PumpFailureWarning=F, PumpFailure=F, WaterLeakWaring=F, WaterLeak=F, IcyWeather=F). Is this a common state for the nuclear plant to be in?**
Answer: Given n random variables assign a probability to each combination of value, this is a joint probability distribution.
P(Meltdown=F, PumpFailureWarning=F, PumpFailure=F, WaterLeakWaring=F, WaterLeak=F, IcyWeather=F) =
P(IcyWeather=F) * P(PumpFailure=F) * P(WaterLeak=F|IcyWeather=F) * P(WaterLeakWaring=F|WaterLeak=F) *
P(PumpFailureWarning=F|PumpFailure=F) * P(Meltdown=F|WaterLeak=F,PumpFailure=F) =
0.95 * 0.9 * 0.9 * 0.95 * 0.95 * 0.999 = 0.693779276249
This is a common state for the plant to be due to it being in it for nearly 70% of the time.


**c) What is the probability of a meltdown if you know that there is both a water leak and a pump failure? Would knowing the state of any other variable matter? Explain your reasoning!**
Answer: P(Meltdown=T|WaterLeak=T, PumpFailure=T) = 0.2
Knowing any other state would not affect the outcome as water leak and pump failure are the only
states that affect meltdown. for example PumpFailureWarning = T does not help as we
already know that PumpFailure = T.

**d) Calculate manually the probability of a meltdown when you happen to know that PumpFailureWarning=F, WaterLeak=F, WaterLeakWarning=F and IcyWeather=F but you are not really sure about a pump failure.**
*Hint: Use the Exact Inference formula near the end of the slides, or in sec. 14.4.1 (3rd edition, sec. 13.3.1 p. 427 in 4th edition) in the book. This formula includes both conditioning on the variables you know (evidence) and marginalizing (summing) over the variable(s) you do not know (often called unobserved or hidden). You need to calculate this both for P(Meltdown=T|...) and P(Meltdown=F|..) and normalize them so that they sum to 1. This normalization factor is the alpha symbol in the equation. With this formula you could answer any query in the network, though it will be impractical for cases with many unobserved variables. A suggestion is to move the terms that do not involve the pump failure variable out of the sum over the two states pump failure can be in (T/F). You may use inference in the applet for verification purposes, but small differences is expected due to rounding errors.*

Answer:

Because IcyWeather and WaterLeakWaring has no impact on the results on meltdown these
can be excluded for simplicity.

So the formula becomes:
P(Meltdown=T | PumpFailureWarning=F, WaterLeak=F, PumpFailure) =
P(Meltdown=T | PumpFailure=T, WaterLeak=F) * P(PumpFailure=T | PumpFailureWarning=F) +
P(Meltdown=T | PumpFailure=F, WaterLeak=F) * P(PumpFailure=F | PumpFailureWarning=F)

P(PumpFailure=T | PumpFailureWarning=F) = P(PumpFailureWarning=F | PumpFailure=T) * P(PumpFailure=T) / P(PumpFailureWarning=F) =
= 0.1 * 0.1 / 0.865 = 0.0115606936416185

P(PumpFailure=F | PumpFailureWarning=F) = P(PumpFailureWarning=F | PumpFailure=F) * P(PumpFailure=F) / P(PumpFailureWarning=F) =
= 0.95 * 0.9 / 0.865 = 0.98843930635

P(PumpFailureWarning=F) =
= P(PumpFailureWarning=F | PumpFailure=F) * P(PumpFailure=F) + P(PumpFailureWarning=F | PumpFailure=T) * P(PumpFailure=T) =
= 0.95 * 0.9 + 0.1 * 0.1 = 0.865
P(PumpFailureWarning=T) = 1 - 0.865 = 0.135


P(Meltdown=T | PumpFailure, WaterLeak=F, PumpFailureWarning=F) =
= P(Meltdown=T | PumpFailure=T, WaterLeak=F) * P(PumpFailure=T | PumpFailureWarning=F) +
  P(Meltdown=T | PumpFailure=F, WaterLeak=F) * P(PumpFailure=F | PumpFailureWarning=F) =
= 0.15 * 0.0115606936416185 + 0.001 * 0.98843930635 = 0.0027225433525927753

P(Meltdown=F | PumpFailure, WaterLeak=F, PumpFailureWarning=F) =
= P(Meltdown=F | PumpFailure=T, WaterLeak=F) * P(PumpFailure=T | PumpFailureWarning=F) +
  P(Meltdown=F | PumpFailure=F, WaterLeak=F) * P(PumpFailure=F | PumpFailureWarning=F) =
= 0.85 * 0.0115606936416185 + 0.999 * 0.98843930635 = 0.9972774566390258


## Part III

P(battery | icyWeather) = 0.8
P(battery | ¬icyWeather) = 0.95
P(radio | battery) = 0.95
P(ignition | battery) = 0.95
P(gas) = 0.95
P(starts | gas ∧ ignition) = 0.95
P(moves | starts) = 0.95
P(survives | moves ∧ melt-down) = 0.8
P(survives | moves ∧ ¬melt-down) = P(survives | ¬moves ∧ ¬melt-down) = 1.0
P(survives | ¬moves ∧ melt-down) = 0.0

**During the lunch break, the owner tries to show off for his employees by demonstrating the many features of his car stereo. To everyone's disappointment, it doesn't work. How did the owner's chances of surviving the day change after this observation?**
Answer: P(survives=T) = 0.99001
        P(survives=T|radio=F) = 0.98116

**The owner buys a new bicycle that he brings to work every day. The bicycle has the following properties: (How does the bicycle change the owner's chances of survival?)**
  - P(bicycle_works) = 0.9
  - P(survives | ¬moves ∧ melt-down ∧ bicycle_works) = 0.6
  - P(survives | moves ∧ melt-down ∧ bicycle_works) = 0.9
Answer: P(survives=T) = 0.99505

**It is possible to model any function in propositional logic with Bayesian Networks. What does this fact say about the complexity of exact inference in Bayesian Networks? What alternatives are there to exact inference?**
Answer:["Bayesian Networks are data structures that represent dependencies among variables and give precise specifications of any full joint probability distribution in a concise manner." s.29](https://www.ida.liu.se/~TDDC17/info/slides/TDDC17_Fo8_4sl.pdf#page=8) This definition does not exclude any functions in propositional logic and therefore
any function in propositional logic can be modeled using Bayesian Networks.
This would then mean that the complexity of exact inference could be infinite.
The alternative to exact inference is approximate inference where
we trade away full accuracy for an improvement in time complexity.



## Part IV

**The owner had an idea that instead of employing a safety person, to replace the pump with a better one. Is it possible, in your model, to compensate for the lack of Mr H.S.'s expertise with a better pump?**
Answer: Yes. as making the pump never fail results in the probability of meltdown decreasing.
P(Meltdown = T | Mr H.S. Has solution = T) = 0.02133
P(Meltdown = T) = 0.02517
P(Meltdown = T | PumpFailure = F) = 0.01116

**Mr H.S. fell asleep on one of the plant's couches. When he wakes up he hears someone scream: "There is one or more warning signals beeping in your control room!". Mr H.S. realizes that he does not have time to fix the error before it is to late (we can assume that he wasn't in the control room at all). What is the chance of survival for Mr H.S. if he has a car with the same properties as the owner? Hint: This question involves a disjunction (A or B) which can not be answered by querying the network as is. How could you answer such questions? Maybe something could be added or modified in the network.**
Answer:
Because PumpFailureWarning = F and WaterLeakWaring = F is not a possible scenario
we need to calculate the normalized probability for the others by multiplying with alpha.

P(PumpFailureWarning = T, WaterLeakWaring = F) = alpha * 0.135 * 0.86075 = alpha * 0.11620125 = 3.9146412475961663 * 0.11620125 = 0.45488620627
P(PumpFailureWarning = F, WaterLeakWaring = T) = alpha * 0.865 * 0.13925 = alpha * 0.12045125 = 3.9146412475961663 * 0.12045125 = 0.47152343157
P(PumpFailureWarning = T, WaterLeakWaring = T) = alpha * 0.135 * 0.13925 = alpha * 0.01879875 = 3.9146412475961663 * 0.01879875 = 0.07359036215

alpha = 1 / (0.11620125 + 0.12045125 + 0.01879875) = 3.9146412475961663

 P(survives = T | PumpFailureWarning = T or WaterLeakWaring = T) =
 = P(survives = T | PumpFailureWarning = T, WaterLeakWaring = F, Mr. H.S Sleeping = F) * P(PumpFailureWarning = T, WaterLeakWaring = F) +
   P(survives = T | PumpFailureWarning = F, WaterLeakWaring = T, Mr. H.S Sleeping = F) * P(PumpFailureWarning = F, WaterLeakWaring = T) +
   P(survives = T | PumpFailureWarning = T, WaterLeakWaring = T, Mr. H.S Sleeping = F) * P(PumpFailureWarning = T, WaterLeakWaring = T) =

 = 0.98528 * 0.45488620627 +
   0.98954 * 0.47152343157 +
   0.97843 * 0.07359036215 =

 = 0.9867845958279079

This can also be seen in the Bayesian network by adding a node OneOrMoreWarnings
and then setting that to True.
P(survives = T| OneOrMoreWarnings = T) = 0.98679


**What unrealistic assumptions do you make when creating a Bayesian Network model of a person?**
Answer: human attributes can not properly be defined in terms of binary values.
Values such as emotion and hunger can not easily be defined in values.
Another unrealistic assumption is that humans are irrational thus ones behaviour becomes
unpredictable to a certain extent.

**Describe how you would model a more dynamic world where for example the "IcyWeather" is more likely to be true the next day if it was true the day before. You only have to consider a limited sequence of days.**
Answer:
In order to do this we would use some kind of lookup table.
Either by using a database or by encoding it into the Bayesian network
by creating a node for each day.
Then we would be able to look at the IcyWeather(day = n).
So then the probability for "icyWeather" could be dependent
on the weather at day n as such:
P(IcyWeather = T) = 0.05 + sum(0.5^n * IcyWeather(day = n) * 0.05) = // (IcyWeather(day = n) returns a binary 1 or 0)
= 0.05 + (0.5^1 * IcyWeather(day = 1) * 0.05 + 0.5^2 * IcyWeather(day = 2) * 0.05 + ...)
