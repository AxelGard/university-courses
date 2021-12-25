data <- read.csv("pima-indians-diabetes.csv")

colnames(data) <- c(
  "npregnant", "pgc", "dbp",
  "tst","insulin","bmi",
  "dpf","age","diabetes"
)

par(mfrow= c(1,1))

#Plot the data
plot(x = data$pgc, y = data$age,
     pch = 16, col = data$diabetes+1,
     xlab = "Plasma glucose concentration",
     ylab = "Age", main = "Pgc vs Age")
legend("topleft",
       inset = 0.05, title="Diabetes",
       c("Diabetes", "Not Diabetes"),
       pch = 16, col = c(2,1))

set.seed(12345)
n=dim(data)[1]
id=sample(1:n, floor(n*0.7))
train=data[id,]
test=data[-id,]

# create model 
model = glm(diabetes ~ age + pgc, data=train, family = "binomial")
probabilities <- predict(model,test, type = "response");
r <- 0.5
predictions = ifelse(probabilities > r, 1, 0)

missclassificationerror = 1-sum(predictions == test$diabetes)/length(test$diabetes)
sprintf("Missclassification error: %f", missclassificationerror)

x1 = test$pgc
x2 = test$age

plot(x = x1,
     y = x2,
     pch = ifelse(predictions == test$diabetes, 16,4),
     col = predictions + 1, xlab = "Plasma glucose concentration",
     ylab = "Age", main = "Prediction Diabetes ~ Pgc + Age")

legend("topleft",
       inset = 0.05,
       title="Prediction",
       c("Diabetes correctly","Not diabetes correctly", 
         "Diabetes incorrectly", "Not diabetes incorrectly"),
       pch = c(16,16,4,4),
       col = c(2,1,2,1))

bound = coef(model)

slope = -bound[3]/bound[2]
intercept = -bound[1]/bound[2]

plot(x = x1,
     y = x2,
     pch = ifelse(predictions == test$diabetes, 16,4),
     col = predictions + 1,
     xlab = "Plasma glucose concentration",
     ylab = "Age",
     main = "Prediction Diabetes ~ Pgc + Age")

legend("topleft",
       inset = 0.05,
       title="Prediction",
       c("Diabetes correctly","Not diabetes correctly", 
         "Diabetes incorrectly", "Not diabetes incorrectly"),
       pch = c(16,16,4,4), col = c(2,1,2,1))

lines(x = 0:300,
      y = intercept + (0:300 * slope),
      type = "l", lty = 1, col = 4, lwd = 3)

r <- 0.2
predictions.2 = ifelse(probabilities > r, 1, 0)

# Threshold 0.8
r <- 0.8
predictions.8 = ifelse(probabilities > r, 1, 0)

# Plot the graphs
par(mfrow= c(1,2))
plot(x = x1,
     y = x2,
     pch = ifelse(predictions.2 == test$diabetes, 16,4),
     col = predictions.2 + 1,
     xlab = "Plasma glucose concentration",
     ylab = "Age", main = "Threshold 0.2")
plot(x = x1, y = x2,
     pch = ifelse(predictions.8 == test$diabetes, 16,4),
     col = predictions.8 + 1,
     xlab = "Plasma glucose concentration",
     ylab = "Age", main = "Threshold 0.8")

new.train = data.frame(y = train$diabetes,
                       x1 = train$pgc,
                       x2 = train$age)

new.train$z1 = new.train$x1 ^ 4
new.train$z2 = new.train$x1 ^ 3 * new.train$x2
new.train$z3 = new.train$x1 ^ 2 * new.train$x2 ^ 2
new.train$z4 = new.train$x1 * new.train$x2 ^ 3
new.train$z4 = new.train$x2 ^ 4

# Train the model
new.model = glm(y ~ ., data=new.train, family = "binomial")

# Prepare new testing data
new.test= data.frame(y  = test$diabetes,
                     x1 = test$pgc, x2 = test$age)
new.test$z1 = new.test$x1 ^ 4
new.test$z2 = new.test$x1 ^ 3 * new.test$x2
new.test$z3 = new.test$x1 ^ 2 * new.test$x2 ^ 2
new.test$z4 = new.test$x1 * new.test$x2 ^ 3
new.test$z4 = new.test$x2 ^ 4

new.probabilities <- predict(new.model,new.test, type = "response");
r <- 0.5
new.predictions = ifelse(new.probabilities > r, 1, 0)

par(mfrow= c(1,1))
plot(x = new.test$x1,
     y = new.test$x2,
     pch = ifelse(new.predictions == new.test$y, 16,4),
     col = new.predictions + 1,
     xlab = "Plasma glucose concentration",
     ylab = "Age",
     main = "Prediction Diabetes ~ Pgc + Age, r =.2")

legend("topleft", inset = 0.05, title="Prediction",
       c("Diabetes correctly","Not diabetes correctly", 
         "Diabetes incorrectly", "Not diabetes incorrectly"),
       pch = c(16,16,4,4),
       col = c(2,1,2,1))

# Print missclassifaicationerror
new.missclassificationerror = 1-sum(new.predictions == new.test$y)/length(new.test$y)
sprintf("New missclassification error: %f", new.missclassificationerror)
