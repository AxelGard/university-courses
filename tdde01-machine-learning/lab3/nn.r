# Assignment 3

library(neuralnet)
library(gtools)
library(glue)
printf <- defmacro(str, expr = {print(glue(str))})

# Creating output variable
output = list()

# Part 1
set.seed(1234567890)
Var <- runif(500, 0, 10) 
mydata <- data.frame(Var, Sin=sin(Var)) 
tr <- mydata[1:25,] # Training 
te <- mydata[26:500,] # Test

# Random initialization of the weights in the interval [-1, 1] 
winit <- runif(n = 10, min = -1, max = 1)
nn <- neuralnet(Sin ~ Var, data=tr, hidden = c(10), startweights=winit)
output$o3.1 = printf("start weights: {nn$startweights}")

plot(tr, cex=2) 
points(te, col = "blue", cex=1)
points(te[,1],predict(nn,te), col="red", cex=1)
title("Prediction neural network 0 to 10")
legend("bottomleft", 
       inset=.02, 
       title="Datapoints",
       c("Training","Testing","Predicted"),
       col=c("black","blue","red"), 
       lwd=1, 
       lty=c(0,0), 
       pch=c(1,1,1))

#part 2 

h1 <- function(x) x
h2 <- function(x) ifelse(x > 0, x, 0)
h3 <- function(x) log(1 + exp(x))

nn.h1 = neuralnet(Sin ~ Var, data=tr, hidden = c(10), startweights = winit, act.fct = h1)
nn.h2 = neuralnet(Sin ~ Var, data=tr, hidden = c(10), startweights = winit, act.fct = h2)
nn.h3 = neuralnet(Sin ~ Var, data=tr, hidden = c(10), startweights = winit, act.fct = h3)


par(mfrow=c(2,2))
plot(te, col = "blue", cex=1)
points(te[,1],predict(nn.h1,te), col="red", cex=1)
title("Linear")
plot(te, col = "blue", cex=1)
points(te[,1],predict(nn.h2,te), col="red", cex=1)
title("ReLU")
plot(te, col = "blue", cex=1)
points(te[,1],predict(nn.h3,te), col="red", cex=1)
title("Softplus")
plot(NULL,xaxt='n',yaxt='n',bty='n',ylab='',xlab='', xlim=0:1, ylim=0:1)
legend("topleft", 
       legend =c("Sample", "Predicted"),
       title="Datapoints",
       pch=1, pt.cex=2, cex=1, 
       col = c("blue", "red"))
par(mfrow=c(1,1))


# part 3

Var2 <- runif(500, 0, 50) 
mydata2 <- data.frame(Var=Var2, Sin=sin(Var2))

plot(mydata2[,1],predict(nn,mydata2), 
     col="red", cex=1,
     xlab="Var", ylab="Sin")
points(mydata2, col = "blue", cex=1)
legend("bottomleft",
       y=-6,
       legend =c("Sample", "Predicted"),
       title="Datapoints",
       pch=1, pt.cex=2, cex=1, 
       col = c("blue", "red"))


# part 4
predict(nn,mydata2)
plot(nn)

conv.prediction = predict(nn,data.frame(Var=1000))

conv.w.L1 = nn$weights[[1]][[1]][2,]
conv.mask <- 0 < c(1, conv.w.L1)
conv.w.L2 = nn$weights[[1]][[2]]

conv.calculation = sum(conv.mask * conv.w.L2)
output$o3.4 = printf("Convergence of nn
        Calculation : {conv.calculation}
        Prediction  : {conv.prediction}
        ")


# part 5

set.seed(1234567890)
Var3 <- runif(500, 0, 10) 
mydata3 <- data.frame(Var=Var3, Sin=sin(Var3))

nn.SinPredictVar <- neuralnet(Var ~ Sin, 
                              data=mydata3, 
                              hidden = c(10), 
                              startweights = winit, 
                              threshold = 0.1)

plot(predict(nn.SinPredictVar,mydata3), mydata3[,2],
     col="red", cex=1,
     xlab = "Var", ylab = "Sin")
points(mydata3, col = "blue", cex=1)












