library(glue)
library(gtools)
library(glmnet)
printf <- defmacro(str, expr = {print(glue(str))})

data = read.csv("tecator.csv", fileEncoding="UTF-8-BOM")
data = as.data.frame(scale(data))
data = data[,-match(c("Sample", "Protein","Moisture"), names(data))]
# Split data in to 50/50 train test sets
n = dim(data)[1]
set.seed(12345)
id = sample(1:n, floor(n*0.5))
data.train = data[id,]

id2 = setdiff(1:n,id)
data.test = data[id2,]

# Linear regression
# Linear regression on Fat, with channels as features
model.regression = lm(Fat ~. ,data = data.train)
summary(model.regression) # to get the quality of the model

data.test.chanels = data.test[,-match(c("Fat"), names(data))]

# Prediction using Linear regression model
prediction.regression <- predict(model.regression, data.test.chanels)

#plot prediction vs test data
plot(prediction.regression, col="blue", ylab = "Fat content", xlab = "sample")
points(data.test$Fat , col="red")
legend("topright", c("predciction ", "test "), fill=c("blue", "red"))
title("prediction for fat using linear regression")

printf("train mean square error for linear regression = 
        {mean((model.regression$residuals)^2)}")
printf("test mean square error for linear regression = 
        {mean((prediction.regression - data.test$Fat)^2)}")

# GET data for lasso and ridge regressions
y <- data.train$Fat
x <- as.matrix(data.train[,-match(c("Fat"), names(data.train))])

# Lasso Regression
# creating a model using Lasso Regression
model.lasso <- glmnet(x, y, alpha=1)
coef(model.lasso)
plot(model.lasso, xvar="lambda") # larger log lambda -> coefficients go to 0
title("LASSO")
summary(model.lasso)

# find were model only has 3 features
print(predict(model.lasso, type="nonzero"))
printf("log lambda for 3 features = {log(model.lasso$lambda[21])}")

# Ridge Regression
# creating a model using Ridge Regression
model.ridge <- glmnet(x, y, alpha=0)
coef(model.ridge)
plot(model.ridge, xvar="lambda")
title("Ridge")
summary(model.ridge)

# try to find were model only has 3 features > some small value
coef.prediction = predict(model.ridge, type="coef")[-1,] # remove intercept
print(apply(coef.prediction, 2, function(x) {
      100 - length(which(abs(x) < 1e-2)) }))
printf("log lambda for 4 features = {log(model.ridge$lambda[39])}")

# Cross Validation LASSO
# creating a model using cross validation and LASSO
model.lasso.cv <- cv.glmnet(x, y, alpha=1)
plot(model.lasso.cv)
title("LASSO dependace of CV socre on log lambda")
printf("cross validation LASSO, lambda min={model.lasso.cv$lambda.min}")
coef(model.lasso.cv, s="lambda.min")

# GET test data for compare of prediction
y.new <- data.test$Fat
x.new <- as.matrix(data.test[,-match(c("Fat"), names(data.test))])

# Prediction
# using cross validation lasso for prediction on test data
prediction.lasso <- predict(model.lasso.cv, newx = x.new)
preciction.lasso.train <- predict(model.lasso.cv, newx = x)

plot(prediction.lasso, col="blue", ylab = "Fat content", xlab = "sample")
points(data.test$Fat , col="red")
legend("topright", c("predciction ", "test "), fill=c("blue", "red"))
title("prediction for fat using CV.LASSO")

# Mean square error for CV LASSO, how good the model is
printf("train mean square error for CV LASSO = {mean((preciction.lasso.train - y)^2)}")
printf("test mean square error for CV LASSO = {mean((prediction.lasso - y.new)^2)}")

