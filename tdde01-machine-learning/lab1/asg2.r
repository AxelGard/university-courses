library(glue)
library(gtools)
printf <- defmacro(str, expr = {print(glue(str))})

# import data, rm not needed data, split data
data = read.csv("parkinsons.csv")
data = as.data.frame(scale(data)) # scale data 
data = data[,-match(c("subject.", "age", "sex", 
                      "test_time", "total_UPDRS"), names(data))]
n = dim(data)[1]
set.seed(12345)
id = sample(1:n, floor(n*0.6))
train = data[id,]
test = data[-id,]

fit = lm(motor_UPDRS~., train) # create model 
fitted = predict(fit, test)

print(summary(fit))
printf("train MSE = {mean((fit$residual)^2)}") # mean square error 
printf("test MSE = {mean((fitted - test$motor_UPDRS)^2)}")
N = dim(data)[2]

loglikelihood <- function(theta, sigma, x, y) {
  n = dim(x)[1]
  thetaTx = apply(x, 1, function(xi) (xi %*% theta))
  return(-((n/2)*log(2*pi*sigma^2)) - ((1/(2*sigma^2))*sum((thetaTx - y)^2)))
}

ridge <- function(theta, sigma, lambda, x, y) {
  return(-loglikelihood(theta, sigma, x, y) + (lambda*sum(theta^2)))
}

ridgeHelper <- function(param, lambda, x, y) {
  theta = matrix(param[-N], nrow=N-1, ncol=1)
  sigma = param[N]
  return(ridge(theta, sigma, lambda, x, y))
}

ridgeOpt <- function(lambda, x, y) {
  return(optim(par=rep(1,N), fn=ridgeHelper, method="BFGS", lambda=lambda, x=x, y=y)$par)
}

df <- function(lambda, x_) {
  x = as.matrix(x_)
  hat = x %*% (solve((t(x) %*% x) + lambda*diag(dim(x)[2])) %*% t(x))
  return(sum(diag(hat)))
}

ridgeOptRun <- function(lambda, train, test)  {
  xtrain = train[,names(train) != "motor_UPDRS"]
  xtest = test[,names(test) != "motor_UPDRS"]
  ytrain = train$motor_UPDRS
  ytest = test$motor_UPDRS
  
  param = ridgeOpt(lambda, xtrain, ytrain)
  theta = matrix(param[-N], nrow=N-1, ncol=1)
  sigma = param[N]
  printf("\n\n Ridge regression: lambda = {lambda}")
  printf("degrees of freedom => {df(lambda, xtrain)}")
  thetaTx = apply(xtrain, 1, function(xi) (xi %*% theta))
  printf("train MSE => {mean((thetaTx - ytrain)^2)}")
  thetaTx = apply(xtest, 1, function(xi) (xi %*% theta))
  printf("test MSE => {mean((thetaTx - ytest)^2)}")
}

ridgeOptRun(1, train, test)
ridgeOptRun(100, train, test)
ridgeOptRun(1000, train, test)
