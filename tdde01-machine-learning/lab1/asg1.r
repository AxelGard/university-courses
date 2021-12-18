
library(glue)
library(gtools)
printf <- defmacro(str, expr = {print(glue(str))})

misclass <- function(x, x_){
  n=length(x)
  return(1-sum(diag(table(x,x_)))/n)
}

cross_entropy_loss <- function(y,y_hat){
  return((-1/length(y))*sum(y*log(y_hat)))
}

data = read.csv("optdigits.csv")
colnames(data) <- c(1:64, "y")

n = dim(data)[1]
set.seed(12345)
id = sample(1:n, floor(n*0.5))
train = data[id,]

id1 = setdiff(1:n, id)
set.seed(12345)
id2 = sample(id1, floor(n*0.25))
valid = data[id2,]

id3 = setdiff(id1,id2)
test = data[id3,]



# Finding the optimal K for the KKNN model over K=1:30
misclass.train = c()
misclass.valid = c()
cross_entropy = c()

for (K in 1:30){
  
  # train
  kknn.train = kknn::kknn(as.factor(y)~., train, train, k=K, kernel="rectangular")
  fit.train = fitted(kknn.train)
  misclass.train[K] <- misclass(train$y, fit.train)
  
  #valid
  kknn.valid = kknn::kknn(as.factor(y)~., train, valid, k=K, kernel="rectangular")
  fit.valid = fitted(kknn.valid)
  misclass.valid[K] <- misclass(valid$y, fit.valid)
  
  logic_matrix <- (col(kknn.valid$prob) == (valid$y+1))
  cross_entropy[K] <- (-1/ length(valid$y))*sum(log(1e-15 + as.numeric(kknn.valid$prob[logic_matrix])))
}

plot.default(1:30, misclass.valid, ylim=c(0,0.07), col="red", xlab = "K")
points.default(1:30, misclass.train, col="blue")
legend("topright", c("valid ", "train "), fill=c("red", "blue"))

printf("best min classifecation of validation set is K={which.min(misclass.valid)}")
plot(1:length(cross_entropy), cross_entropy, type = "b")
printf("Cross entropy: K={which.min(cross_entropy)}")


kknn = kknn::kknn(as.factor(y)~., train, test, k=which.min(cross_entropy), kernel="rectangular")
fit.cross = kknn$fitted.values
misclass.cross = misclass(test$y, fit.cross)


# Using optimal K for KKNN
kknn = kknn::kknn(as.factor(y)~., train, test, k=which.min(misclass.valid), kernel="rectangular")
fit = kknn$fitted.values
print(table(test$y, fit))

error.test <- misclass(test$y, fit)
error.valid <- misclass.valid[which.min(misclass.valid)]
printf("test error for optimal k, error:{error.test}")
printf(" differences from validation and test, error delta:{error.test - error.valid}")

kknn = kknn::kknn(as.factor(y)~., train, train, k=which.min(misclass.valid), kernel="rectangular")

# Grab all cases where the model thinks there is a 8.
all_eights <- which(train$y == 8)
our_guesses = c()
for(i in 1:length(all_eights)){
  our_guesses[i] = kknn$prob[all_eights[i], "8"]
}
hardest_train_cases <- order(our_guesses, decreasing = FALSE)[1:3] 
easiest_train_cases <- order(our_guesses , decreasing = TRUE)[1:2]
all_eights[hardest_train_cases]
all_eights[easiest_train_cases]


# create a heatmap for the hardest and easiest case.
for (i in 1:3){
  hard <- t(matrix(unlist(train[all_eights[hardest_train_cases][i], 1:64]), nrow=8, ncol=8))
  heatmap(hard, Colv=NA, Rowv=NA, main="hard")
}
for (i in 1:2){
  easy <- t(matrix(unlist(train[all_eights[easiest_train_cases][i], 1:64]), nrow=8, ncol=8))
  heatmap(easy, Colv=NA, Rowv=NA, main="easy")
}
