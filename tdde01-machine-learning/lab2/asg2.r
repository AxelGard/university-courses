library(glue)
library(gtools)
library(tree)
printf <- defmacro(str, expr = {print(glue(str))})

# helper functions 
plot_tree <- function(tree, title, ...) {
  plot(tree, ...)
  title(title)
}

print_misclass <- function(lst, title) {
  printf("\n\n -- {title} tree settings --")
  printf("training misclass => {lst$train_misclass}")
  printf("validation misclass => {lst$valid_misclass}")
}

TPR <- function(confusion_matrix) {
  # Calculate true positive rate
  return(confusion_matrix["yes", "yes"] / sum(confusion_matrix["yes", ]))
}

FPR <- function(confusion_matrix) {
  # Calculate false positive rate
  return(confusion_matrix["no", "yes"] / sum(confusion_matrix["no", ]))
}

model_evaluation_metrics <- function(confusion_matrix) {
  # Uses a confusion matrix to calculate evaluation metrics
  lst = list()
  lst$accuracy = sum(diag(confusion_matrix)) / sum(confusion_matrix)
  lst$TPR = TPR(confusion_matrix)
  lst$recall = lst$TPR
  lst$FPR = FPR(confusion_matrix)
  lst$precision = confusion_matrix["yes", "yes"] / sum(confusion_matrix[, "yes"])
  lst$f1_score = (2 * lst$precision * lst$recall) / (lst$precision + lst$recall)
  return(lst)
}

misclass <- function(x, x1){
  return(1 - sum(diag(table(x, x1))) / length(x))
}
###

# import data and remove not relevant data column 
data = read.csv("bank-full.csv", sep=";", stringsAsFactors=TRUE)
data = data[, -match(c("duration"), names(data))] # Remove duration column

# split the data 40 30 30 for train validation test 
n = dim(data)[1]
set.seed(12345)
id = sample(1:n, floor(n*0.4))
train = data[id, ]
id1 = setdiff(1:n, id)
set.seed(12345)
id2 = sample(id1, floor(n*0.3))
valid = data[id2, ]
id3 = setdiff(id1, id2)
test = data[id3, ]

# Train and fit decision trees to taring data 
train_decision_tree <- function(train, valid, ...) {
  tree = tree(y~., train, ...) # ... == args/kwargs in python
  train_fit = predict(tree, train, type="class")
  valid_fit = predict(tree, valid, type="class")
  train_misclass = misclass(train_fit, train$y)
  valid_misclass = misclass(valid_fit, valid$y)
  return(list(tree=tree, train_fit=train_fit, valid_fit=valid_fit,
              train_misclass=train_misclass, valid_misclass=valid_misclass))
}

# train three trees with different given parameters
default = train_decision_tree(train, valid)
minsize = train_decision_tree(train, valid, minsize=7000)
mindev = train_decision_tree(train, valid, mindev=0.0005)

print_misclass(default, title="default")
print_misclass(minsize, title="minsize = 7000")
print_misclass(mindev, title="mindev = 0.0005")

# plot trees for visual comparison
par(mfrow = c(1, 3))
plot_tree(default$tree, title="default")
plot_tree(minsize$tree, title="minsize = 7000")
plot_tree(mindev$tree, title="mindev = 0.0005")

# Prune tree to different number of leaves and calculate deviance
# for both training and test data then plot a curve.
range = 2:50 # Range of leaves to test
score = list()
for(i in range) {
  pruned = prune.tree(mindev$tree, best=i) # Prune tree to i leaves
  predicted = predict(pruned, valid, type="tree") # type="tree" => return a tree object
  score$train[i] = deviance(pruned) / length(train$y) # divide by length to scale deviance
  score$valid[i] = deviance(predicted) / length(valid$y)
}
par(mfrow = c(1, 1))
plot(range, score$train[range], type="b", col="red", ylim=c(0.55, 0.7),
     ylab="average deviance per datapoint", xlab="number of leaves")
points(range, score$valid[range], type="b", col="blue")
legend("topright", legend = c("train", "valid"), col = c("red","blue"), pch = c(1, 1))

# Choose tree with smallest validation deviance
optimal = list(tree = prune.tree(mindev$tree, best=which.min(score$valid)))
print(optimal$tree)

# perform classification using the optimal tree
optimal$fit = predict(optimal$tree, test, type="class")
optimal$confusion_matrix = table(test$y, optimal$fit)
# list concatenation => add evaluation metrics to list named optimal
optimal = c(optimal, model_evaluation_metrics(optimal$confusion_matrix))

printf("\n\n -- optimal number of leaves --")
printf("accuracy => {optimal$accuracy}")
printf("F1 score => {optimal$f1_score}") # F1 score better as we have imbalanced classes
print(optimal$confusion_matrix)

# Use formula from slides to add a loss matrix
# if p(y="no"|x) / p(y="yes"|x) > 5 => classify as "no"
loss = list(probs = predict(optimal$tree, test))
loss$fit = ifelse(loss$probs[,"no"] / loss$probs[,"yes"] > 5, "no", "yes")
loss$confusion_matrix = table(test$y, loss$fit)
loss = c(loss, model_evaluation_metrics(loss$confusion_matrix))

printf("\n\n -- custom loss matrix --")
printf("accuracy => {loss$accuracy}")
printf("F1 score => {loss$f1_score}")
print(loss$confusion_matrix)

logistic_regression_model = glm(y~., train, family="binomial")
# Get the probabilities from the logistic regression model
log = list(probs = predict(logistic_regression_model, test, type="response"))
# Take probability of yes classification from prediction
optimal$probs <- predict(optimal$tree, test)[,"yes"]
for (pi in seq(0.05, 0.95, 0.05)) {
  # Classify using => class = "yes" if p(y="yes"|x) > pi else "no"
  # Optimal tree
  classification = factor(ifelse(optimal$probs > pi, "yes", "no"), levels = c("no", "yes"))
  confusion_matrix = table(test$y, classification)
  optimal$TPR_values = append(optimal$TPR_values, TPR(confusion_matrix))
  optimal$FPR_values = append(optimal$FPR_values, FPR(confusion_matrix))
  # Logistic regression
  classification = factor(ifelse(log$probs > pi, "yes", "no"), levels = c("no", "yes"))
  confusion_matrix = table(test$y, classification)
  log$TPR_values = append(log$TPR_values, TPR(confusion_matrix))
  log$FPR_values = append(log$FPR_values, FPR(confusion_matrix))
}
# ROC curve 
plot(optimal$FPR_values, optimal$TPR_values, type="b", col="red", ylim=c(0, 1),
     xlim=c(0, 1), ylab="True positive rate", xlab="False positive rate")
lines(log$FPR_values, log$TPR_values, type="b", col="blue")
abline(a=0, b=1, col="green", lty=2)
legend("bottomright", legend = c("Optimal tree", "Logistic regression", "Random guess"),
       col = c("red","blue","green"), pch = c(1, 1, 1))






































